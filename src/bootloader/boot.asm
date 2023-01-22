[org 0x7c00]
KERNEL_OFFSET equ 0x1000

; FAT32 header
jmp boot 			; Boot jump instruction
nop
; db "Sarabi  "		; Oem identifier
; db "MSWIN4.1"
; dw 512				; Bytes per cluster
; db 1				; Sectors per cluster
; dw 32 				; Reserved sectors
; db 2				; Number of copies of FAT
; dw 0 				; Maximum root directory entries (N/A for FAT32)
; dw 2880				; Number of sectors in partition smaller than 32MB (N/A for FAT32)
; db 0xF0				; Media Descriptor (F8h for hard disks)
; dw 0				; Sectors Per FAT in Older FATSystems (N/A for FAT32)
; dw 18				; Sectors per track
; dw 2				; Number of Heads
; dd 0 				; Number of Hidden Sectors inPartition
; dd 0				; Number of Sectors inPartition
; dd 0 				; Number of Sectors Per FAT
; dw 0				; Flags (Bits 0-4 IndicateActive FAT Copy) (Bit 7 Indicates whether FAT Mirroringis Enabled or Disabled) (If FATMirroring is Disabled, the FAT Information is onlywritten to the copy indicated by bits 0-4)
; dw 0 				; Version of FAT32 Drive (HighByte = Major Version, Low Byte = Minor Version)
; dd 0 				; Cluster Number of the Startof the Root Directory
; dw 0 				; Sector Number of the FileSystem Information Sector (See Structure Below)(Referenced from the Start of the Partition)
; dw 0 				; Sector Number of the BackupBoot Sector (Referenced from the Start of the Partition)
; times 12 db 0		; Reserved 12 bytes
; db 0 				; Logical Drive Number ofPartition
; db 0 				; Unused (Could be High Byteof Previous Entry)
; db 0x29				; Extended Signature (29h)
; dd 0 				; Serial Number of Partition
; db "123456789ab"	; Volume Name of Partition
; db "FAT_NAME"		; FAT Name (FAT32) (8 bytes)

;; See https://www.youtube.com/watch?v=HcNjEY-tzo0&t=44s
db "MSWIN5.0"
dw 512				; Bytes per cluster
db 1 				; Sectors per cluster
dw 1 				; Num of relative sectors
db 2 				; Num of FATs
dw 224				; Max num of root enties
dw 2880				; Total number of sectors
db 0xF0				; Media Descriptor (F8h for hard disks, F0h for floppy)
dw 0x09				; Sectors per FAT
dw 18		  		; Secotors per track
dw 2				; Num of heads
dd 0				; Num of hidden secotrs
dd 0 				; Number of Sectors inPartition (0 in FAT12)
db 0 
db 0				; Reserved (Current head)
db 0x29				; Extended boot singnature
dd 0xDA2237A8		; Volume ID 
db "NO NAME    "	
dd "FAT12   "

[bits 16]
boot:
	mov [BOOT_DRIVE], dl

	mov bp, 0x9000
	mov sp, bp

	call load_kernel

	; mov ax, 0x0013
	; int 0x10

	; mov ax, 0xA000
	; mov es, ax
	; mov al, 0x04 ; VGA RED
	; mov cx, 320*200
	; xor di, di
	; loop: 
	; mov [es:di], al
	; add di, 1
	; sub cx, 1
	; jnz loop
	
	; jmp $

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
LOAD_KERNEL_MSG db "Loading kernel...", 10, 13, 0

times 447 - ($ - $$) db 0x00


; FAT32 partition 1
db 0x80 ; Current state of partition (00h = Inactive, 80h = Active)
db 0x00 ; Begginnning of partiion - Head
dw 0x0000 ; Beginning of partition - Cylinder/Sector 
db 0x00 ; Type of partition
db 0x00 ; End of partition - Head
db 0x00 ; End of partition - Cylinder/Sector
dd 0x00000000 ; Number of Sectors Between the MBR and the First Sector in the Partition
dd 0x00000000 ; Number of Sectors in the Partition

; FAT32 partition 2
dd 0x00 ; Boot flag + CHS begin
dd 0x00 ; Type code + CHS end
dd 0x00 ; LBA begin
dd 0x00 ; Number of sectors

; FAT32 partition 3
dd 0x00 ; Boot flag + CHS begin
dd 0x00 ; Type code + CHS end
dd 0x00 ; LBA begin
dd 0x00 ; Number of sectors

; FAT32 partition 4
dd 0x00 ; Boot flag + CHS begin
dd 0x00 ; Type code + CHS end
dd 0x00 ; LBA begin
dd 0x00 ; Number of sectors

dw 0xaa55
