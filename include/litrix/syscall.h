#define SYSCALL_ADDRESS  0x1ffffff
#define SYSRET_ADDRESS   0x1eeeeee
#define SYSSET_ADDRESS   0x1dddddd

#define SYSRET_REGISTER  (unsigned int*)(SYSRET_ADDRESS)
#define SYSCALL_REGISTER (unsigned char*)(SYSCALL_ADDRESS)
#define SYSCALL_STR      (unsigned int*)(SYSSET_ADDRESS)

enum Syscall {
    PRINT   = 1,
    READFIL,
    SETCOL,
    CLRSCR,
    LIFSCTL,
    CREPROC,
    READKBD,
};

void sysc_test(void);
unsigned int sysc_rarg0(void);
void sysc_setf(void);
void sysc_clrf(void);
void sysc_set(char call, unsigned int arg0, unsigned int arg1);
unsigned char sysc_read(void);
void sysc_process(char call);
void sysc_fde(void);
