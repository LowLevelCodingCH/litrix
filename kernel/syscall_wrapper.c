#include <litrix/syscall_wrapper.h>
#include <litrix/syscall.h>

void write(char *str, unsigned int len) {
    sysc_set(PRINT, (unsigned int)str, (unsigned int)len, 0);
    //sysc_fde();
}

char read(void) {
    char c = 0;
    sysc_set(READKBD, 0, 0, 0);
    //sysc_fde();
    return *SYSRET_REGISTER;
}

void fread(char *name, char *buf) {
    sysc_set(READFIL, (unsigned int)name, (unsigned int)buf, 0);
    //sysc_fde();
}

void fwrite(char *name, unsigned int amount, char *buf) {
    sysc_set(WRITEFIL, (unsigned int)name, (unsigned int)amount, (unsigned int)buf);
    //sysc_fde();
}

void fcreat(char *name) {
    sysc_set(CREATFIL, (unsigned int)name, 0, 0);
    //sysc_fde();
}

void fsctl(void) {
    sysc_set(FSCTL, 0, 0, 0);
    //sysc_fde
}
