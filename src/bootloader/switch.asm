[bits 16]

switch_to_pm:
	cli                     ; 1. disable interrupts
	; call enable_A20
    lgdt [gdt_descriptor]   ; 2. load GDT descriptor
    mov eax, cr0
    or eax, 0x1             ; 3. enable protected mode
    mov cr0, eax
    jmp CODE_SEG:init_pm ; 4. far jump

[bits 32]
init_pm:
	mov ax, DATA_SEG        ; 5. update segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000        ; 6. setup stack
    mov esp, ebp

    call start_protected_mode ; 7. move back to mbr.asm

[bits 32]

enable_A20:
        call    a20wait
        mov     al, 0xAD
        out     0x64, al
 
        call    a20wait
        mov     al, 0xD0
        out     0x64, al
 
        call    a20wait2
        in      al, 0x60
        push    eax
 
        call    a20wait
        mov     al, 0xD1
        out     0x64, al
 
        call    a20wait
        pop     eax
        or      al, 2
        out     0x60, al
 
        call    a20wait
        mov     al, 0xAE
        out     0x64, al
 
        call    a20wait
        sti
        ret
 
a20wait:
        in      al,0x64
        test    al, 2
        jnz     a20wait
        ret
 
 
a20wait2:
        in      al,0x64
        test    al, 1
        jz      a20wait2
        ret