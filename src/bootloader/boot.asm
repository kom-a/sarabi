[org 0x7c00]
KERNEL_OFFSET equ 0x1000

[bits 16]
main:
	mov [BOOT_DRIVE], dl

	mov bp, 0x9000
	mov sp, bp

	mov bx, REAL_MODE_MSG
	call print_string

	call load_kernel
	call switch_to_pm

	jmp $

%include "print.asm"
%include "disk.asm"
%include "gdt.asm"
%include "switch.asm"

[bits 16]
load_kernel:
	mov bx, LOAD_KERNEL_MSG
	call print_string

	mov bx, KERNEL_OFFSET
	mov dh, 30
	mov dl, [BOOT_DRIVE]
	call disk_load
	ret

[bits 32]
start_protected_mode:
	call KERNEL_OFFSET

	jmp $

BOOT_DRIVE: db 0
REAL_MODE_MSG db "Started in 16 - bit Real Mode", 10, 13, 0
PTCD_MODE_MSG db "Successfully landed in 32 - bit Protected Mode", 0
LOAD_KERNEL_MSG db "Loading kernel...", 10, 13, 0

times 510 - ($ - $$) db 0x00

dw 0xaa55
