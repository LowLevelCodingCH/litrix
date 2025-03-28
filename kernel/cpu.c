#include <litrix/cpu.h>

unsigned char r_fs(void) {
    unsigned char x;
//    asm volatile("movb %%fs, %0" : "=r" (x));
    return x;
}

unsigned char r_gs(void) {
    unsigned char x;
//    asm volatile("movb %%gs, %0" : "=r" (x));
    return x;
}

unsigned int r_eax(void) {
    unsigned int x;
    asm volatile("mov %%eax, %0" : "=r" (x));
    return x;
}

void w_eax(unsigned int x) {
    asm volatile("mov %0, %%eax" : "=r" (x));
}

void w_fs(unsigned char x) {
//    asm volatile("movb %0, %%fs" : "=r" (x));
}

void w_gs(unsigned char x) {
//    asm volatile("movb %0, %%gs" : "=r" (x));
}

void jmp(unsigned int x) {
    asm volatile inline ("call *%0" : : "r"(x));
}

void push(unsigned int x) {
    asm volatile inline ("push %0" : : "r"(x));
}

unsigned int cpuid(void) {
    return r_fs();
}
