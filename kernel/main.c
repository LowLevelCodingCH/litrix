#include <litrix/preverror.h>

#include <litrix/typedef.h>

#include <litrix/stdarg.h>

#include <litrix/portio.h>
#include <litrix/keyboard.h>
#include <litrix/disk.h>
#include <litrix/pit.h>

#include <litrix/cpu.h>
#include <litrix/pc.h>

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

struct vm_map    vr_mmap = {0};
struct heap_t    heap    = {0};

struct dev_t     tty     = {0};
struct dev_t     ide     = {0};

struct process_t root    = {0};
struct process_t dev_hnd = {0};

int root_pid = 0;

char heap_adr[MEMAMOUNT*512];
char *stack = NULL;

char c = 0;

void root_func(unsigned int *esp, pid_t pid) {
    if(pid != 0) panic("WHAT FUCKER PUT ANOTHER PROCESS BEFORE ROOT???\n");

    sysc_fde();

    keyboard_handler();
    putc(cchar);

    if(cursor >= (79*25)*2) clear();
}

int m(unsigned int stack_top) {
    set_color(0xf0);
    clear();
    puts("xk7 is booting..\n");

    stack = (char*)(stack_top - STACK_SIZE);
    stack_init(stack);
    heap_init(&heap, heap_adr);
    vm_init_map(&vr_mmap);
    init_ata();
    init_keyboard();

    sysc_set(0, 0, 0, 0);
    sysc_clrf();
    sysc_clret();

    w_gs(0);
    w_fs(0);
    w_eax(0);

    lifs_ctl();

    root = create_process(root_func, "root");
    root_pid = attach_process(&root);

    lifs_creat("dev.tty0");
    lifs_creat("dev.ide0");

    init_dev(&tty, "dev.tty0" , OUTPUT, READ_WRITE);
    init_dev(&ide, "dev.ide", BLOCK_ATA, READ_WRITE);

    lifs_creat("kbd.lay::DE_ch@lower");
    lifs_creat("kbd.lay::DE_ch@upper");

    lifs_write("kbd.lay::DE_ch@lower", 256, (char*)lowercase);
    lifs_write("kbd.lay::DE_ch@upper", 256, (char*)uppercase);

    scheduler();

    while(true)
        asm("hlt");

    return 0;
}
