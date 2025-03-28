; This does NOT mean Personal Computer
; This modifies the "program counter" (jmp) for a scheduler

global w_pc
global r_pc

w_pc:
    pop eax
    jmp eax
    ret

r_pc:               ; doesnt work
    call r_pc_imm
    ret
r_pc_imm:
    pop eax
    ret
