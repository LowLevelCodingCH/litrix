enum syscall {
    SYS_WRITE = 0,
    SYS_READ,
    SYS_OPEN,
    SYS_CLOSE,
    SYS_CREAT,

    SYS_FORK,
    SYS_KILL,

    SYS_SHUTDOWN,
    SYS_PRINT,
    SYS_GETC,
};

void syscall_hnd(void);
