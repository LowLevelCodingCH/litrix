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

#define XKFS_BASE 60816
#define VERSION 7
#define NAME "Litrix"
#define KERNEL "xk"

char *stack = NULL;
struct vm_map vr_mmap = {0};
struct heap_t heap = {0};
struct process_t root = {0};
char heap_adr[MEMAMOUNT*512];
char sec1[512] = {0};
char oldtext[512-32] = "Hallo Welt!\0";
char newtext[512-32] = "Tschuess welt!\0";

void rootfunc(unsigned int *esp) {
    char c = read();
    putc(c);
    sysc_fde();

    if(cursor >= (79*25)*2) clear();
}

void main(unsigned int stack_top) {
    set_color(0x07);

    clear();

    printf("%s %s%d booting...\n", NAME, KERNEL, VERSION);


    stack = (void*)(stack_top - STACK_SIZE);

    if(stack == NULL) {
        printf("Stack is nil from beginning. Try to reboot!\n");
        asm("hlt");
    }

    printf("1 : SMFS [Simple File System]\n2 : XKFS [X Kernel File System]\nWhat Filesystem do you use [default: 1]: ");

    char choice = read();

    switch(choice) {
    case '1': {smfs_ctl(); break;}
    case '2': {printf("\nXKFS not implemented yet\n"); asm("hlt"); break;}
    default: {smfs_ctl(); break;}
    }


    stack_init(stack, 0);
    heap_init(&heap, heap_adr, 0);
    vm_init_map(&vr_mmap, 0);
    
    init_ata(0);

    init_keyboard(0);

    sysc_set(0, 0);

    root = create_process(0, 0, rootfunc, "root");
    pid_t root_pid = attach_process(&root);

    printf("Initializing done...\n");

    clear();

    if(root_pid == -1) {
        printf(LITRIX_ERR "[io::kernel] Root Process Died Before It Could Start\n");
        asm("hlt");
    }

    while(plist[root_pid]->running == 1) {
        scheduler();
    }

    printf(LITRIX_ERR "[io::kernel] Critical Process Died\n");

    printf("Stack dump kernel:       %s\n", stack);
    printf("Stack dump root process: %s\n", (char*)(plist[root_pid]->stack));


    while(1)
        asm("hlt");
}
