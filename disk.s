; LIFS: test_inode:
; LIFS:     blocks: dd 1, 300, 0, 0, 0, 0, 0, 0
; LIFS:     size: dd 51
; LIFS:     name: db "hallo.txt"
; LIFS: 
; LIFS: times 512-($-$$) db 0
; LIFS: 
; LIFS: test_block:
; LIFS:     db "Hallo welt", 0xa, 0
; LIFS: times 512-($-test_block) db 0
; LIFS: 
; LIFS: times 512*298 db 0
; LIFS: 
; LIFS: block:
; LIFS:     db "HBB"

SMFS: times 512*100 db 0
