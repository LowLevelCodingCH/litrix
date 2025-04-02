extern syscall_hnd
extern div_by_zero
extern gen_prot_fault
extern debug_exc
extern invalid_opc
extern machine_check
extern double_fault

global exc0
global exc1
global exc6
global exc8
global exc13
global exc18

global default_hnd
global syscall_hand
global i386_idt_load

i386_idt_load:
    push ebp
    mov ebp, esp
    
    mov eax, [ebp + 8]
    lidt [eax]

    mov esp, ebp
    pop ebp
    ret

syscall_hand:
    pushad
    cli

    call syscall_hnd

    sti
    popad
    iret

exc0:
    pushad
    cli

    call div_by_zero

    sti
    popad
    iret

exc1:
    pushad
    cli

    call debug_exc

    sti
    popad
    iret

exc6:
    pop eax
    pushad
    cli

    call invalid_opc

    sti
    popad
    iret

exc8:
    pop eax
    pushad
    cli

    call double_fault

    sti
    popad
    iret

exc13:
    pop eax
    pushad
    cli

    call gen_prot_fault

    sti
    popad
    iret

exc18:
    pop eax
    pushad
    cli

    call machine_check

    sti
    popad
    iret

default_hnd:
    pushad
    cli

    sti
    popad
    iret
