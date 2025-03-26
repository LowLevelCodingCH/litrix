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

#define LIX_EIGHT

#ifdef LIX_EIGHT
#   include <litrix/lix8/ivt.h>
#endif

#define VERSION 7
#define NAME "Litrix"
#define DESCEND "Nitrix"
#define KERNEL "xk"

struct vm_map vr_mmap = {0};
struct heap_t heap = {0};
char heap_adr[MEMAMOUNT*512];
char *stack = NULL;

struct process_t root = {0};
struct process_t dev_hnd = {0};

char c = 0;

void root_func(unsigned int *esp, pid_t pid) {
    if(pid != 0) return;

    sysc_fde();

    c = read();

    putc(c);

    if(cursor >= (79*25)*2) clear();
}

void device_step(unsigned int *esp, pid_t pid) {
}

int main(unsigned int stack_top) {
    set_color(0xf0);
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
    sysc_set(0, 0, 0);
    sysc_clrf();

    printf("Clearing registers: fs, gs, eax\n");
    w_gs(0);
    w_fs(0);
    w_eax(0);

    printf("Initializing lifs\n");
    lifs_ctl();

    printf("Creating process: root\n");
    root = create_process(root_func, "root\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");

    printf("Assigning process: root\n");
    pid_t root_pid = attach_process(&root);

    char nbuf[512*LIFS_BLOCKS] = {0};

    char *ok = "Hallo welt\n";

    lifs_creat("Hallo.text");
    lifs_creat("Tschu.text");
    lifs_ctl();
    lifs_write("Hallo.text", ok );
    lifs_read ("Hallo.text", nbuf);


    printf(nbuf);
    asm("hlt");

    scheduler();

    return 0;
}
