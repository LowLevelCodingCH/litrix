#include <litrix/syscall.h>
#include <litrix/stdout.h>
#include <litrix/keyboard.h>

void sysc_set(char call, unsigned int arg0) {
    if(call == 0) *SYSCALL_STR = 0;
    else *SYSCALL_STR = 1;

    *SYSCALL_REGISTER = call;
    *(unsigned int*)(SYSCALL_ADDRESS+sizeof(char)) = arg0;
}

unsigned char sysc_read(void) {
    return *SYSCALL_REGISTER;
}

unsigned int sysc_rarg0(void) {
    return *(unsigned int*)(SYSCALL_ADDRESS+sizeof(char));
}

void sysc_process(char call) {
    if(call == 0) return;

    unsigned int sarg0 = sysc_rarg0();

    switch(call) {
    case PRINT:   print((char*)sarg0); break;
    case READKBD: {
        keyboard_handler();
        *(char*)(sarg0) = cchar;
        break;
    }
    default:      break;
    }
}

void sysc_test(void) {
    char *to_print = "Hallo";
    sysc_set(PRINT, (unsigned int)to_print);
}

void sysc_fde(void) {
    if(!(*SYSCALL_STR)) return;

    sysc_process(sysc_read());
    sysc_set(0, 0);
}
