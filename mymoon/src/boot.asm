[org 0x7c00]

jmp main

; Prints string pointed by bx
print_string:
	pusha
	mov ah, 0x0e
	print_string_loop:
	mov al, [bx]
	cmp al, 0
	je print_string_end
	int 0x10
	add bx, 1
	jmp print_string_loop
	print_string_end:
	popa
	ret

; Converts al value to its ascii
to_ascii:
	cmp al, 9
	jle to_ascii_digit
	add al, 'a'
	sub al, 0xa
	jmp to_ascii_end
	to_ascii_digit:
	add al, '0'

	to_ascii_end:
	ret

; Prints value at dx as hex
print_hex:
	pusha

	mov bx, HEX_OUT
	add bx, 2

	mov al, dh
	shr al, 4
	and al, 0x0f
	call to_ascii
	mov [bx], al
	add bx, 1

	mov al, dh
	and al, 0x0f
	call to_ascii
	mov [bx], al
	add bx, 1

	mov al, dl
	shr al, 4
	and al, 0x0f
	call to_ascii
	mov [bx], al
	add bx, 1

	mov al, dl
	and al, 0x0f
	call to_ascii
	mov [bx], al
	add bx, 1


	mov bx, HEX_OUT
	call print_string
	popa
	ret

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

main:
	mov [BOOT_DRIVE], dl
	mov bp, 0x8000
	mov sp, bp
	mov bx, 0x9000
	mov dh, 2
	mov dl, [BOOT_DRIVE]
	call disk_load
	mov dx, [0x9000] 
	call print_hex

	mov dx, [0x9000 + 512]
	call print_hex 

	jmp $

DISK_ERROR_MSG:
db "Disk read error!", 10, 13, 0

DISK_READ_ERROR:
db "Disk read error!", 10, 13, 0

BOOT_DRIVE: 
db 0

HEX_OUT:
db "0x0000", 10, 13, 0

times 510 - ($ - $$) db 0x00

dw 0xaa55

times 256 dw 0xdada
times 256 dw 0xface