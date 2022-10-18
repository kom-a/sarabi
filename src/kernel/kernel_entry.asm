[bits 32]
[extern kmain]

global kernel_entry_point

global AsmHandler
[extern DefaultInterruptHandler]

global KeyboardHandlerAsm
[extern KeyboardHandler]

kernel_entry_point:
	call kmain
	jmp $
    ret

AsmHandler:
	call DefaultInterruptHandler
	iret

KeyboardHandlerAsm:
	call KeyboardHandler
	iret