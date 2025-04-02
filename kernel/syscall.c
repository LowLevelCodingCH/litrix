#include <litrix/syscall.h>
#include <litrix/stdout.h>
#include <litrix/lxpi.h>
#include <litrix/fs/lifs.h>
#include <litrix/execve.h>
#include <litrix/typedef.h>
#include <litrix/stack.h>
#include <litrix/scheduler.h>
#include <litrix/fs/wrap_inc.h>

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
        case SYS_WRITE: write(ebx, ecx, (char*)edx); break;
        case SYS_READ: read(ebx, (char*)edx); break;
        case SYS_OPEN: eax = open((char*)ebx); break;
        case SYS_CLOSE: close(ebx); break;
        case SYS_CREAT: creat((char*)ebx); break;

        case SYS_SHUTDOWN: shutdown(); break;
        case SYS_PRINT: print_len((char*)ebx, ecx); break;
//        case SYS_GETC: keyboard_handler(); eax = cchar; break;
        default: printf("NONE\n"); break;
    }

    asm ("movl %0, %%eax" : : "a" (eax));

    return;
}
