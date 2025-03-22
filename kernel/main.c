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

#define XKFS_BASE 60816
#define VERSION 7
#define NAME "Litrix"
#define KERNEL "xk"

void main(unsigned int stack_top) {
    clear();

    printf("%s %s%d booting...\n", NAME, KERNEL, VERSION);

    stack_init(stack_top, 1);
    struct vm_map vr_mmap;
    struct heap_t heap;
    char heap_adr[MEMAMOUNT*512];
    heap_init(&heap, heap_adr, 1);
    vm_init_map(&vr_mmap, 1);
    set_color(0x07);
    init_ata(1);
    init_keyboard(1);

    printf("Initializing done...\n");

    clear();

    while(1) {
        keyboard_handler();
        putc(cchar);
        if(cursor >= (79*25)*2)
            clear();
    }

    printf(LITRIX_ERR "[io::kernel] Kernel Stopped\n");


    while(1)
        asm("hlt");
}
