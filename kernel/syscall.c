#include <litrix/syscall.h>
#include <litrix/stdout.h>
#include <litrix/keyboard.h>
#include <litrix/fs/lifs.h>
#include <litrix/stack.h>
#include <litrix/scheduler.h>

void sysc_setf(void) {
    *SYSCALL_STR = 1;
}

void sysc_clrf(void) {
    *SYSCALL_STR = 0;
}

void sysc_set(char call, unsigned int arg0, unsigned int arg1) {
    if(call == 0) sysc_clrf();
    else sysc_setf();

    *SYSCALL_REGISTER = call;
    *(unsigned int*)(SYSCALL_ADDRESS+sizeof(char)) = arg0;
    *(unsigned int*)(SYSCALL_ADDRESS+sizeof(char)+sizeof(unsigned int)) = arg1;
}

unsigned char sysc_read(void) {
    return *SYSCALL_REGISTER;
}

unsigned int sysc_rarg0(void) {
    return *(unsigned int*)(SYSCALL_ADDRESS+sizeof(char));
}

unsigned int sysc_rarg1(void) {
    return *(unsigned int*)(SYSCALL_ADDRESS+sizeof(char)+sizeof(unsigned int));
}

void sysc_process(char call) {
    if(call == 0) return;

    unsigned int sarg0 = sysc_rarg0();
    unsigned int sarg1 = sysc_rarg1();

    switch(call) {
    case PRINT:   print_len((char*)sarg0, (unsigned int)sarg1); break;
    case READFIL: lifs_read((char*)sarg0, (char*)sarg1); break;
    case SETCOL:  set_color(sarg0); break;
    case CLRSCR:  clear(); break;
    case LIFSCTL: lifs_ctl(); break;
    case CREPROC: *SYSRET_REGISTER = attach_process((struct process_t*)sarg0); break;
    case READKBD:
        keyboard_handler();
        *SYSRET_REGISTER = cchar;
        break;
    default:      break;
    }
}

void sysc_fde(void) {
    if(!(*SYSCALL_STR)) return;

    sysc_process(sysc_read());
    sysc_set(0, 0, 0);
}
