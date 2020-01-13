# nasm -f bin
bits 64
section .header 
DOS:
	dw 0x5a4d		;e_magic
	times 29 dw 0	;unused
	dd 0x40		;e_lfanew

PECOFF:
        dd `PE\0\0`	;Signature
	dw 0x8664		;Machine
	dw 3			;NumberOfSections
	dd 0			;TimeDateStamp
	dd 0			;PointerToSymbolTable
	dd 0			;NumberOfSymbols
	dw 160		;SizeOfOptionalHeader
	dw 0x202		;Characteristics
	dw 0x20b		;Magic
    db 0			;MajorLinkerVersion
    db 0			;MinorLinkerVersion
    dd 0x200		;SizeOfCode
    dd 0x400		;SizeOfInitializedData
    dd 0			;SizeOfUninitializedData
    dd 0x0			;AddressOfEntryPoint
    dd 0x0			;BaseOfCode
    dq 0x0			;ImageBase
    dd 0x1000		;SectionAlignment
    dd 0x200		;FileAlignment
    dw 0			;MajorOperatingSystemVersion
    dw 0			;MinorOperatingSystemVersion
    dw 0			;MajorImageVersion
    dw 0			;MinorImageVersion
    dw 0			;MajorSubsystemVersion
    dw 0			;MinorSubsystemVersion
    dd 0			;Reserved
    dd 0x3000		;SizeOfImage
    dd 0x200		;SizeOfHeaders
    dd 0			;CheckSum
    dw 10			;Subsystem
    dw 0			;DllCharacteristics
    dq 0			;SizeOfStackReserve
    dq 0			;SizeOfStackCommit
    dq 0			;SizeOfHeapReserve
    dq 0			;SizeOfHeapCommit
    dd 0			;LoaderFlags
    dd 6			;NumberOfRvaAndSizes

DIRS:
	times 40 db 0	;unused dirs for this app
	dd 0x1000		;VirtualAddress(.reloc)
	dd 8			;Size(.reloc)

SECTS:
.1:
	dq  `.text`			;Name
	dd  codesize		;VirtualSize
	dd  0x0				;VirtualAddress	
	dd  0x200			;SizeOfRawData
	dd  0x200			;PointerToRawData
	dd  0				;PointerToRelocations
	dd  0				;PointerToLinenumbers
	dw  0				;NumberOfRelocations
	dw  0				;NumberOfLinenumbers
	dd  0x60500020		;Characteristics

.2:
	dq  `.reloc`
	dd  0x8		
	dd  0x1000
	dd  0x200
	dd  0x400
	dd  0
	dd  0
	dw  0
	dw  0
	dd  0x42100040

.3:
	dq  `.data`
	dd  datasize
	dd  0x2000
	dd  0x200
	dd  0x600
	dd  0
	dd  0
	dw  0
	dw  0
	dd  0xc0100040

section .text follows=.header align=0x200
	sub rsp, 40

	mov rcx, [rdx+64]         
	lea rdx, [rel hello]
	call     [rcx+8]		

	add rsp, 40 
	ret

codesize equ $ - $$

section .reloc follows=.text align=0x200
	dd 0					;PageRVA
	dd 8					;BlockSize

section .data follows=.reloc align=0x200 vstart=0x2200    ;yeah this is the trick!
hello:
	db __utf16__ `hello world!\n\r\0`

datasize equ $ - $$
align 0x200,db 0
