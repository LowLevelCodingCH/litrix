; This modifies the "program counter" (jmp) for a scheduler
; it also means personal computer and register manipulation

global w_pc
global r_pc
global asm_switch_um
global dostf

w_pc:
    jmp eax
    ret

r_pc:
    mov eax, [esp]
    ret

dostf:
    ; So the compiled code is this:
    ; push   ebp
    ; mov    ebp,esp
    ; push   esi
    ; push   ebx
    ; call   __x86.get_pc_thunk.ax
    ; add    eax,0x3d5f
    ; So, that function gets us IP (PC) into eax.
    ; I guess
    ; lets look at it:
    ; mov    eax,DWORD PTR [esp]
    ; ret
    ; So IP is in *esp (deref).
    ; now IP is in eax
    ; Then we add 0x3d5f to eax
    sub eax, 0x3d5f
    ret

asm_switch_um:
    ;mov cs, 0b0000000000000011
    sti
    ret

testexec:
    mov [0xb8000], byte 'a'
    ret
