dd 0x1BADB002
dd 3
dd 0xE4524FFB

[extern] main

section .data

stack_bottom:
times 65536 db 0
stack_top:

section .text
    global _start

_start:
    mov esp, stack_top

    push esp
    call main

    hlt
