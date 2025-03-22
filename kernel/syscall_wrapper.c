#include <litrix/syscall_wrapper.h>
#include <litrix/syscall.h>

void write(char *str) {
    sysc_set(PRINT, (unsigned int)str);
    sysc_fde();
}

char read(void) {
    char c = 0;
    sysc_set(READKBD, (unsigned int)(&c));
    sysc_fde();
    return c;
}
