#include <litrix/portio.h>

void outw(short port, short val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

short inw(short port) {
    short ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void outb(short port, char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

char inb(short port) {
    char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outl(short port, int value) {
    asm("outl %%eax, %%dx" :: "d" (port), "a" (value));
}

static inline int inl(short port) {
    int ret;
    asm("inl %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}
