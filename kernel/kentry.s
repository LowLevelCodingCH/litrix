extern kmain
global _start

begin_:
; Was ist dieser müll
dd 464367618
dd 3
dd 3830599675

empty_: times 16384 db 0

sflo_: times 16384 db 0
stop_:

_start:
  mov esp, stop_

  mov ecx, _start

  mov eax, begin_

  jmp kmain
