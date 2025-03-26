#include <litrix/syscall_wrapper.h>
#include <litrix/syscall.h>

void write(char *str, unsigned int len) {
    sysc_set(PRINT, (unsigned int)str, len);
    //sysc_fde();
}

char read(void) {
    char c = 0;
    sysc_set(READKBD, 0, 0);
    //sysc_fde();
    return *SYSRET_REGISTER;
}

void fread(char *name, char *buf) {
    sysc_set(READFIL, (unsigned int)name, (unsigned int)buf);
    //sysc_fde();
}

void fwrite(char *name, char *buf) {}