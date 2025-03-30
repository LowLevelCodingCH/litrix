#include <litrix/preverror.h>
#include <litrix/typedef.h>
#include <litrix/stdarg.h>
#include <litrix/portio.h>
#include <litrix/keyboard.h>
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
#include <litrix/fs/smfs.h>
#include <litrix/fs/lifs.h>
#include <litrix/lix8/ivt.h>

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


void root_func(unsigned int *esp, pid_t pid) {
    if(pid != 0) panic("WHAT FUCKER PUT ANOTHER PROCESS BEFORE ROOT???\n");

    keyboard_handler();
    putc(cchar);

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

int m(unsigned int stack_top) {
    set_color(0xf0);
    clear();
    puts("xk7 is booting..\n");

    i386_gdt_initialize();
    i386_idt_initialize();

    stack = (char*)(stack_top - STACK_SIZE);
    stack_init(stack);
    heap_init(&heap, heap_adr);
    vm_init_map(&vr_mmap);
    init_ata();
    init_pit(1000);
    init_keyboard();

    w_gs(0);
    w_fs(0);
    w_eax(0);

    lifs_ctl();

    root = create_process(root_func, "root");
    root_pid = attach_process(&root);

    printf("Do you wanna initialize this System or is it already initialized (1 or 2): ");
    keyboard_handler();
    putc(cchar);
    putln();
    clear();
    if(cchar == '1') {
        lifs_creat("dev.ide");
        lifs_creat("dev.vgacrd");
        lifs_creat("kbd.lay::DE_ch@lower");
        lifs_creat("kbd.lay::DE_ch@upper");
        lifs_creat("testexe.xke");
        lifs_write("kbd.lay::DE_ch@lower", 256, (char*)lowercase);
        lifs_write("kbd.lay::DE_ch@upper", 256, (char*)uppercase);
        lifs_write("testexe.xke", 30, (char*)testprog);
        printf("Reinitialized \"vgacrd\", \"ide\", \"kbd.lay::DE_ch@[upper|lower]\", testexe.xke\n");
    } else {}

    init_dev(&ide,      "dev.ide",    BLOCK_ATA,  READ_WRITE);
    init_dev(&vga_card, "dev.vgacrd", VIDEO_CARD, WRITE);
    
    asm volatile("int $64");

    execve("testexe.xke", NULL, NULL);

    scheduler();

    shutdown();

    return 0;
}
