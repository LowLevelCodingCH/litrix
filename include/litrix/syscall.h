#define SYSCALL_ADDRESS  0x1fffff8
#define SYSCALL_REGISTER (unsigned char*)SYSCALL_ADDRESS
#define SYSCALL_STR      (unsigned int*)0x1000008

enum Syscall {
    PRINT   = 1,
    READKBD = 2,
};

void sysc_test(void);
unsigned int sysc_rarg0(void);
void sysc_set(char call, unsigned int arg0);
unsigned char sysc_read(void);
void sysc_process(char call);
void sysc_fde(void);
