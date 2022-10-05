
[bits 16]
; Loads dh sectors to es:bx from drive dl
disk_load:
	push dx
	mov ah, 0x02
	mov al, dh
	mov ch, 0x00
	mov dh, 0x00
	mov cl, 0x02
	int 0x13
	jc disk_error
	pop dx
	cmp dh, al
	jne read_error
	ret

	disk_error:
	mov bx, DISK_ERROR_MSG
	call print_string
	jmp $

	read_error:
	mov bx, DISK_READ_ERROR
	call print_string
	jmp $

DISK_ERROR_MSG:
db "Disk read error!", 10, 13, 0

DISK_READ_ERROR:
db "Disk read error!", 10, 13, 0