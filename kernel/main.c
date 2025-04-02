#include <litrix/preverror.h>
#include <litrix/typedef.h>
#include <litrix/stdarg.h>
#include <litrix/portio.h>
#include <litrix/disk.h>
#include <litrix/lxpi.h>
#include <litrix/pit.h>
#include <litrix/i386/gdt.h>
#include <litrix/i386/idt.h>
#include <litrix/cpu.h>
#include <litrix/pc.h>
#include <litrix/memory.h>
#include <litrix/stack.h>
#include <litrix/execve.h>
#include <litrix/virtmem.h>
#include <litrix/stdout.h>
#include <litrix/syscall.h>
#include <litrix/syscall_wrapper.h>
#include <litrix/scheduler.h>
#include <litrix/shell.h>
#include <litrix/device.h>
#include <litrix/device/ps2kbd.h>
#include <litrix/fs/smfs.h>
#include <litrix/fs/lifs.h>
#include <litrix/lix8/ivt.h>
#include <litrix/typedef.h>
#include <litrix/fs/wrap_inc.h>
#include <litrix/fs/wrap.h>

#define VERSION 7
#define NAME "Litrix"
#define DESCEND "Nitrix"
#define KERNEL "xk"

struct vm_map    vr_mmap  = {0};
struct heap_t    heap     = {0};

struct dev_t     vga_card = {0};
struct dev_t     ide      = {0};

int root_pid = 0;

char heap_adr[MEMAMOUNT * 512];
char *stack = NULL;

char c = 0;
char file_ds[128][16] = {0};
int newest_fd = 0;

// Testing program
// Syscalls, Prints "Hi\n"
// Gets loaded at address 0,
// May get offset when I
// Change that to be something else
char testprog[6] = {
    // SIGNATURE (MAGIC "XKE.")
    0x58, 0x4b, 0x45, 0x2e,
    // More metadata maybe
    //     Where code starts
    //     Where data starts
    // To load more efficiently I guess
    // CODE
    0xf4,
    0xc3,                          // ret
};

int kmain(void) {
    int stop = 0;
    asm("mov %%esp, %0" : "=S"(stop));

    set_color(0x07);
    clear();
    puts("xk7 is booting..\n");

    i386_gdt_initialize();
    i386_idt_initialize();
    print_log_OK("Initialized descriptor tables");

    stack = (char*)(stop - STACK_SIZE);
    stack_init(stack);
    heap_init(&heap, heap_adr);
    vm_init_map(&vr_mmap);
    print_log_OK("Initialized memory");

    init_ata();
    print_log_OK("Initialized ata device");

    w_gs(0);
    w_fs(0);
    w_eax(0);

    fsctl();

    creat("dev.ide");
    creat("dev.vgacrd");
    creat("swap");
    creat("a.xke");

    //int fd = open("a.xke");
    //write(fd, 11, testprog);
    //close(fd);

    init_dev(&ide,      "dev.ide",    BLOCK_ATA,  READ_WRITE, (void*)NULL);
    init_dev(&vga_card, "dev.vgacrd", VIDEO_CARD, WRITE, (void*)NULL); 
    switch_um();

    asm volatile("int $0x64");
    execve("a.xke", NULL, NULL);
    context c;

    save_context(&c);
    printf("b\n");
    load_context(&c);

    while(1);
    shutdown();

    return 0;
}
