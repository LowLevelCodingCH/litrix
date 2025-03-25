#include <litrix/preverror.h>

#include <litrix/stdarg.h>

#include <litrix/portio.h>
#include <litrix/keyboard.h>
#include <litrix/disk.h>
#include <litrix/pit.h>

#include <litrix/cpu.h>

#include <litrix/memory.h>
#include <litrix/stack.h>
#include <litrix/virtmem.h>

#include <litrix/stdout.h>

#include <litrix/syscall.h>
#include <litrix/syscall_wrapper.h>

#include <litrix/scheduler.h>

#include <litrix/device.h>

#include <litrix/fs/smfs.h>
#include <litrix/fs/lifs.h>

#define VERSION 7
#define NAME "Litrix"
#define DESCEND "Nitrix"
#define KERNEL "xk"

struct vm_map vr_mmap = {0};
struct heap_t heap = {0};
struct process_t root = {0};
struct process_t dev_hnd = {0};

struct dev_t hdd = {0};
struct dev_t kbd = {0};
struct dev_t tty = {0};
struct dev_t pit = {0};
struct dev_t *devs[32];

char heap_adr[MEMAMOUNT*512];

char bf[1024*1000] = {0};
char pit_bf[1024] = {0};
char term[1024] = {0};

char *stack = NULL;

char c = 0;

int i = 0;

void root_func(unsigned int *esp, pid_t pid) {
    if(pid != 0) return;

    c = read();

    putc(c);
    sysc_fde();

    if(c != 0) {
        bf[i] = c;
        i++;
    }

    if(cursor >= (79*25)*2) clear();
}

void device_step(unsigned int *esp, pid_t pid) {
    if(i >= 1023) {
        i = 0;
        memset(bf, 0, 1024);
        write_dev(&kbd, bf);
    }

    read_dev(&pit, pit_bf);
    pit_wait(pit_bf[0] * (pit_bf[1] + pit_bf[2] + pit_bf[3]));

    write_dev(&kbd, bf);

    write_dev(&tty, term);
}

void main(unsigned int stack_top) {
    set_color(0x07);
    clear();
    printf("xk7 is booting..\n");

    printf("Setting the Stack\n");
    stack = (char*)(stack_top - STACK_SIZE);

    printf("Initializing the Stack\n");
    stack_init(stack);

    printf("Initializing the Heap\n");
    heap_init(&heap, heap_adr);

    printf("Initializing Virtual Memory\n");
    vm_init_map(&vr_mmap);

    printf("Initializing ATA device\n");
    init_ata();

    printf("Initializing keyboard\n");
    init_keyboard();

    printf("Clearing syscall\n");
    sysc_set(0, 0);

    printf("Clearing registers: fs, gs, eax\n");
    w_gs(0);
    w_fs(0);
    w_eax(0);

    printf("\nXKFS : 1\nLiFS : 2\nSMFS : 3\n\nWhat filesystem do you want to use? [default: 3] ");
    char c = read();
    switch(c) {
        case '1': printf("Not yet implemented\n"); asm("hlt");
	case '2': lifs_ctl(); printf("Working on it:\n\tRead: half-done\n\tWrite: not even begun\n"); asm("hlt");
        case '3': smfs_ctl();
        default: smfs_ctl();
    }

    clear();

    printf("Initializing smfs\n");
    smfs_ctl();

    printf("Creating device file: /dev/hdd0\n");
    smfs_creat("/dev/hdd0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
    printf("Creating device file: /dev/kbd0\n");
    smfs_creat("/dev/kbd0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
    printf("Creating device file: /dev/tty0\n");
    smfs_creat("/dev/tty0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
    printf("Creating device file: /dev/pit0\n");
    smfs_creat("/dev/pit0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");

    printf("Initializing device: /dev/hdd0\n");
    init_dev(&hdd, "/dev/hdd0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", BLOCK_ATA, READ_WRITE);
    printf("Initializing device: /dev/kbd0\n");
    init_dev(&kbd, "/dev/kbd0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", KEYBOARD,  READ_WRITE);
    printf("Initializing device: /dev/tty0\n");
    init_dev(&tty, "/dev/tty0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", OUTPUT,    WRITE);
    printf("Initializing device: /dev/pit0\n");
    init_dev(&pit, "/dev/pit0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", OUTPUT,    READ_WRITE);

    printf("Assigning device: /dev/hdd0\n");
    devs[0] = &hdd;
    printf("Assigning device: /dev/kbd0\n");
    devs[1] = &kbd;
    printf("Assigning device: /dev/tty0\n");
    devs[2] = &tty;
    printf("Assigning device: /dev/pit0\n");
    devs[3] = &pit;

    printf("Creating process: root\n");
    root = create_process(root_func, "root\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
    printf("Creating process: device\n");
    dev_hnd = create_process(device_step, "device\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");

    printf("Assigning process: root\n");
    pid_t root_pid = attach_process(&root);
    printf("Assigning process: device\n");
    pid_t dev_hnd_pid = attach_process(&dev_hnd);

    scheduler();
}
