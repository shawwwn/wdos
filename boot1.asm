;; boot1.asm
;; 1st-stage bootloader
;; in real mode
;;

extern _boot2				; gcc symbol always starts with '_'
global start				; program entry

bits 16
start:
	jmp boot1				; 1st-stage bootloader
	times 3-($-$$) db 0x90	; 3-byte jmp

	; https://stackoverflow.com/questions/47277702
	; Dos 4.0 EBPB 1.44MB floppy
	OEMname:           db    "WDOSBOOT"
	bytesPerSector:    dw    512
	sectPerCluster:    db    1
	reservedSectors:   dw    1
	numFAT:            db    2
	numRootDirEntries: dw    224
	numSectors:        dw    2880
	mediaType:         db    0xf0
	numFATsectors:     dw    9
	sectorsPerTrack:   dw    18
	numHeads:          dw    2
	numHiddenSectors:  dd    0
	numSectorsHuge:    dd    0
	driveNum:          db    0
	reserved:          db    0
	signature:         db    0x29
	volumeID:          dd    0x2d7e5a1a
	volumeLabel:       db    "NO NAME    "
	fileSysType:       db    "FAT12   "

bits 16
boot1:
	cli
	cld

	; turn off all keyboard locks
	mov ax, 0x40
	mov ds, ax
	mov bx, 0x17			; 0040:0017, keyboard shift status
	mov ax, 0				; 0b0xxx0000, cap/num/scroll locks
	mov [bx], ax

	; setup data segment
	mov ax, 0
	mov ds, ax

	; setup stack
	mov ax, 0x07E0			; stack starts at 0x7E00
	mov ss, ax
	mov sp, 0x0200			; 512 bytes of stack space, stack ends at 0x8000

	; welcome message (real mode)
	mov si, msg
	call print16

	; use BIOS to read sector from disk
	call readsector16
	; jc .fail

	; load GDT
	lgdt [toc]

	; switch on protected mode
	mov eax, cr0
	or al, 1				; bit 0 in cr0
	mov cr0, eax

	; enter 32-bit protected mode
 .jump_protected:
	jmp 08h:protected

; read 2nd-7th sectors from disk
bits 16
readsector16:
	; real mode addr = (segment << 1) + offset
	mov bx, 0x0800			; segment to read into
	mov es, bx
	mov bx, 0				; offset to read into, starts at 0x8000

	mov ah, 02				; BIOS read sector function
	mov al, 07				; read 7 sector, a total of 3.5kb, ends at 0x8E00
	mov ch, 00				; track to read
	mov cl, 02				; sector to read from
	mov dh, 00				; head to read
	; mov dl, 00			; bios will set %dl to boot drive
	int 0x13				; BIOS call
	ret

; print text to screen, using BIOS interrupts
bits 16
print16:
	lodsb					; load a byte from SI to AL
	or al, al				; al=0?
	jz .done				; if yes, then done
	mov ah, 0x0E			; if no, print the byte
	int 0x10				; BIOS call
	jmp print16
 .done:
	ret

msg:
	db "-> real mode", 0xA, 0xD, 0

; Global Descriptor Table
bits 16
gdt_data:
	dd 0 					; null descriptor
	dd 0 

; gdt code:					; code descriptor
	dw 0FFFFh 				; limit low
	dw 0 					; base low
	db 0 					; base middle
	db 10011010b 			; access
	db 11001111b 			; granularity
	db 0 					; base high

; gdt data:					; data descriptor
	dw 0FFFFh 				; limit low (Same as code)
	dw 0 					; base low
	db 0 					; base middle
	db 10010010b 			; access
	db 11001111b 			; granularity
	db 0					; base high
end_of_gdt:
toc: 
	dw end_of_gdt - gdt_data - 1 	; limit (Size of GDT)
	dd gdt_data 			; base of GDT

	dw 0


; ------------------------------------------------------------------------------
bits 32
protected:
	; for some CPU/BIOS, this is important!
	cli

	; setup mem descriptor
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	; set stack top to 2MB
	mov esp, 0x200000

	call _boot2				; 2nd-stage bootloader
	hlt

; placeholder:
; 	times 510-($-$$) db 0
; 	dw 0xAA55				; sector's boot flag

