[bits 32]
[extern kmain]
kernel_entry_point:
	call kmain
	jmp $
    ret
