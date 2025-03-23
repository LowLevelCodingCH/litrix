#include <litrix/preverror.h>

#include <litrix/stdarg.h>

#include <litrix/portio.h>
#include <litrix/keyboard.h>
#include <litrix/disk.h>
#include <litrix/pit.h>

#include <litrix/memory.h>
#include <litrix/stack.h>
#include <litrix/virtmem.h>

#include <litrix/stdout.h>

#include <litrix/syscall.h>
#include <litrix/syscall_wrapper.h>

#include <litrix/scheduler.h>

#include <litrix/fs/smfs.h>

#include <litrix/device.h>

#define VERSION 7
#define NAME "Litrix"
#define DESCEND "Nitrix"
#define KERNEL "xk"

struct vm_map vr_mmap = {0};
struct process_t root = {0};

struct dev_t kbd = {0};
struct dev_t hdd = {0};
struct dev_t tty = {0};

char heap_adr[MEMAMOUNT*512];

char bf[1024*1000] = {0};
char cb[512] = {0};
char clean[512] = {0};

int i = 0;

// If you wanna add a function call, optimize it right away
// Yes, the external function
// This also counts for functions inside func calls
// Such as smfs_creat which calls memset, smfs_test, smfs_write and smfs_rename

void rootfunc(unsigned int *esp, pid_t pid) {
    if(pid != 0) return;

    char c = read();

    putc(c);
    sysc_fde();

    if(c != 0) {
        bf[i] = c;
        i++;
    }
    if(i >= 511) {
        i = 0;
        memset(bf, 0, 512);
        write_dev(&kbd, bf);
    }
    if(c == '\n')
        write_dev(&kbd, bf);
    if(c == 'c') {
        read_dev(&kbd, cb);
        print_len(cb, i);
    }

    if(cursor >= (79*25)*2) clear();
}

void main(unsigned int stack_top) {
    set_color(0x07);

    clear();

    printf("xk7 kernel is booting...\n");

    vm_init_map(&vr_mmap);

    init_ata();
    init_keyboard();
    sysc_set(0, 0);

    smfs_ctl();
    smfs_creat("/dev/hdd0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
    smfs_creat("/dev/kbd0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
    smfs_creat("/dev/tty0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");

    smfs_write("/dev/hdd0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", clean);
    smfs_write("/dev/kbd0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", clean);
    smfs_write("/dev/tty0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", clean);

    init_dev(&kbd, "/dev/kbd0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", KEYBOARD,  READ_WRITE);
    init_dev(&hdd, "/dev/hdd0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", BLOCK_ATA, READ_WRITE);
    init_dev(&tty, "/dev/tty0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", OUTPUT,    READ_WRITE);

    root = create_process(rootfunc, "root\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
    pid_t root_pid = attach_process(&root);

    scheduler();
}
