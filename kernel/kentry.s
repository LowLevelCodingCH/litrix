dd 0x1BADB002
dd 3
dd 0xE4524FFB

extern main

stack_bottom:
times 32768 db 0
stack_top:

section .text
global _start

_start:
    mov esp, stack_top

    push esp
    call main

    jmp _end

_end:
    cli
    hlt
    jmp _end
