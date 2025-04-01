extern syscall_hnd
extern div_by_zero
extern invalid_opc
extern kbhnd

global i386_idt_load
global kbd_hnd
global exc0
global exc6
global default_hnd
global syscall_hand

i386_idt_load:
    push ebp
    mov ebp, esp
    
    mov eax, [ebp + 8]
    lidt [eax]

    mov esp, ebp
    pop ebp
    ret

syscall_hand:
    pusha
    cli

    call syscall_hnd

    sti
    popa
    iret

exc0:
    pusha
    cli

    call div_by_zero

    sti
    popa
    iret

exc6:
    pusha
    cli

    call invalid_opc

    sti
    popa
    iret

kbd_hnd:
    pusha
    cli

    call kbhnd

    sti
    popa
    iret

default_hnd:
    pusha
    cli

    sti
    popa
    iret
