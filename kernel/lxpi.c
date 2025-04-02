#include <litrix/portio.h>
#include <litrix/stdout.h>
#include <litrix/fs/lifs.h>
#include <litrix/stack.h>
#include <litrix/memory.h>
#include <litrix/syscall.h>
#include <litrix/typedef.h>
#include <litrix/scheduler.h>
#include <litrix/virtmem.h>

extern char *stack;
extern struct vm_map vr_mmap;
extern struct heap_t heap;
extern char *heap_adr;

// Litrix power interface
void shutdown(void) {
    clear();

    lifs_ctl();

    stack_init(stack);
    vm_init_map(&vr_mmap);
    heap_init(&heap, heap_adr);

    outw(0x604, 0 | 0x2000);
end:
    panic("[lxpi] Shutdown did not work! Halting..\n");
brz:
    asm("hlt");
    goto brz;
}
