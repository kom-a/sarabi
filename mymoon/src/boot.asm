[org 0x7c00]

jmp main

%include "print.asm"
%include "gdt.asm"
%include "switch.asm"

[bits 16]
main:
	mov bp, 0x9000
	mov sp, bp

	mov bx, REAL_MODE_MSG
	call print_string

	; switch to protected mode
	call switch_to_pm 

	jmp $

[bits 32]
ptcd:
	mov ebx, PTCD_MODE_MSG
	call print_string_pm
	jmp $

REAL_MODE_MSG db "Started in 16 - bit Real Mode", 10, 13, 0
PTCD_MODE_MSG db "Successfully landed in 32 - bit Protected Mode", 10, 13, 0

times 510 - ($ - $$) db 0x00

dw 0xaa55
