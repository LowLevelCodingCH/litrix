#include <litrix/cpu.h>

unsigned int r_fs(void) {
    unsigned int x;
    asm volatile("mov %%fs:0, %0" : "=r" (x));
    return x;
}

unsigned int r_gs(void) {
    unsigned int x;
    asm volatile("mov %%gs:0, %0" : "=r" (x));
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

void w_fs(unsigned int x) {
    asm volatile("mov %0, %%fs:0" : "=r" (x));
}

void w_gs(unsigned int x) {
    asm volatile("mov %0, %%gs:0" : "=r" (x));
}

unsigned int cpuid(void) {
    return r_fs();
}
