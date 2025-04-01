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

struct process_t root     = {0};
struct process_t dev_hnd  = {0};

int root_pid = 0;

char heap_adr[MEMAMOUNT * 512];
char *stack = NULL;

char c = 0;
char file_ds[128][16] = {0};
int newest_fd = 0;

void root_func(unsigned int *esp, pid_t pid) {
    if(pid != 0) panic("WHAT FUCKER PUT ANOTHER PROCESS BEFORE ROOT???\n");

    if(cursor >= (79*25)*2) clear();
}

// Testing program
// Syscalls, Prints "Hi\n"
// Gets loaded at address 0,
// May get offset when I
// Change that to be something else
char testprog[30] = {
    // SIGNATURE (MAGIC "XKE.")
    0x58, 0x4b, 0x45, 0x2e,
    // More metadata maybe
    //     Where code starts
    //     Where data starts
    // To load more efficiently I guess
    // CODE
    0xb8, 0x05, 0x00, 0x00, 0x00,  // mov eax, 1  // Syscall number
    0xbb, 0x17, 0x00, 0x00, 0x00,  // mov ebx, 23 // Arg0
    0xb9, 0x03, 0x00, 0x00, 0x00,  // mov ecx, 3  // Arg1
    0xba, 0x00, 0x00, 0x00, 0x00,  // mov edx, 0  // Arg2
    0xcd, 0x80,                    // int 0x80
    0xc3,                          // ret
    // DATA
    'H', 'i', 0xa,
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
    init_pit(1000);

    w_gs(0);
    w_fs(0);
    w_eax(0);

    fsctl();

    root = create_process(root_func, "root");
    root_pid = attach_process(&root);

    creat("dev.ide");
    creat("dev.vgacrd");
    creat("swap");

    init_dev(&ide,      "dev.ide",    BLOCK_ATA,  READ_WRITE, (void*)NULL);
    init_dev(&vga_card, "dev.vgacrd", VIDEO_CARD, WRITE, (void*)NULL);
    
    asm volatile("int $64");

    scheduler();

    shutdown();

    return 0;
}
