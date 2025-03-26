test_inode:
    blocks: dd 102, 20, 4, 0, 0;, 0, 0, 0, 0
    size: dd 0
    name: db "panicscr"

times 512-($-$$) db 0

times 512*100 db 0


panic_screen:
    db "Litrix failed.", 0xa, "                Error code: %x", 0

times 512-($-panic_screen) db 0

times 512*500 db 0

;SMFS: times 512*100 db 0
