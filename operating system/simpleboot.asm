; Infinite loop (e9 fd ff)
loop:
	jmp loop
;FIll with 510 zeros minus the size of previous code
times 510-($-$$) db 0
;Magic number
dw 0xaa55