#include <litrix/syscall.h>
#include <litrix/stdout.h>
#include <litrix/lxpi.h>
#include <litrix/keyboard.h>
#include <litrix/fs/lifs.h>
#include <litrix/execve.h>
#include <litrix/stack.h>
#include <litrix/scheduler.h>

void syscall_hnd(void) {
    int eax;
    int ebx;
    int ecx;
    int edx;

    asm ("movl %%eax, %0" : "=a" (eax));
    asm ("movl %%ebx, %0" : "=b" (ebx));
    asm ("movl %%ecx, %0" : "=c" (ecx));
    asm ("movl %%edx, %0" : "=d" (edx));

    switch(eax) {
        case 0: printf("SYS_WRITE\n"); break;
        case 1: printf("SYS_READ\n"); break;
        case 2: printf("SYS_FORK\n"); break;
        case 3: shutdown(); break;
        case 4: detach_process(ebx); break;
        case 5: print_len((char*)ebx, ecx); break;
        case 6: keyboard_handler(); eax = cchar; break;
        default: printf("NONE\n"); break;
    }

    asm ("mov %0, %%eax" : : "a" (eax));

    return;
}
