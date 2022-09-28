
[bits 16]
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
;print_hex:
;	pusha
;
;	mov bx, HEX_OUT
;	add bx, 2
;
;	mov al, dh
;	shr al, 4
;	and al, 0x0f
;	call to_ascii
;	mov [bx], al
;	add bx, 1
;
;	mov al, dh
;	and al, 0x0f
;	call to_ascii
;	mov [bx], al
;	add bx, 1
;
;	mov al, dl
;	shr al, 4
;	and al, 0x0f
;	call to_ascii
;	mov [bx], al
;	add bx, 1
;
;	mov al, dl
;	and al, 0x0f
;	call to_ascii
;	mov [bx], al
;	add bx, 1
;
;
;	mov bx, HEX_OUT
;	call print_string
;	popa
;	ret

HEX_OUT:
db "0x0000", 10, 13, 0

[bits 32]

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

; prints a null - terminated string pointed to by EDX
print_string_pm:
	pusha
	mov edx, VIDEO_MEMORY		; Set edx to the start of vid mem.
	print_string_pm_loop:
	mov al, [ebx]				; Store the char at EBX in AL
	mov ah, WHITE_ON_BLACK		; Store the attributes in AH
	cmp al, 0					; if (al == 0) , at end of string , so
	je print_string_pm_done		; jump to done
	mov [edx], ax				; Store char and attributes at current
								; character cell.
	add ebx , 1					; Increment EBX to the next char in string.
	add edx , 2					; Move to next character cell in vid mem.
	jmp print_string_pm_loop	; loop around to print the next char.
	print_string_pm_done:
	popa
	ret
