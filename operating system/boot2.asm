mov ah, 0x0e ;ttymode
mov ql, 'H'
int 0x10
mov al, 'e'
int 0x10
int 0x10
mov al,'o'
int 0x10

jmp$ ; jump to current address = infinite loop
;padding and magic number
time 510 - ($-$$) db 0
dw 0xaa55