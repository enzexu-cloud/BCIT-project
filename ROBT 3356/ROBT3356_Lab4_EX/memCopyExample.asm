;-------------------------------------------------------------------------------
; memCopyExample.asm
;
; Example assembler source file for ROBT 3356
; Demonstates word copy from two locations in memory
; 
; Author:  Greg Scutt, Oct 2018
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
ramS0Base	.equ 0x2400				; base addresses
ramS1Base 	.equ 0x2C00
OneKB		.equ 1024				; 1KB definition
			.define R4, ramS0ptr	; pointer defn
			.define R5, ramS1ptr
			.define R7, index	

MAIN_LOOP:
			mov.w #ramS0Base, ramS0ptr	; set up the pointers to RAM
			mov.w #ramS1Base, ramS1ptr	; true this one may not be used ...
			clr.w index					; clear index counter.  emulated with mov.w #0, dst


CPY1:		mov.w @ramS0ptr+, ramS1Base(index)		; 4 copy a word from ramS0 to ramS1

			incd index								; 2 incd increments by 2.  emulated with   add.w #2, dst
			cmp.w #OneKB, index						; 2 yes, 1KB words or 2KBytes
EndCpy: 	jne CPY1								; 2
													; cycles = 5 + (2^11)*10 = 20485
													; time = cycles * (1/frequency) = 20485 * 1/1048000 = 19.55 ms
DONE:
			NOP
MAIN_LOOP2:
			mov.w #ramS0Base, ramS0ptr	; set up the pointers to RAM
			mov.w #ramS1Base, ramS1ptr	; true this one may not be used ...
			mov.w #numBytes, index					; clear index counter.  emulated with mov.w #0, dst

CPY2:		mov.w @ramS0ptr+, ramS1Base(index)		; 4 do it again but try to make CPY1 more efficient by avoiding a cmp statement
			dec index								; 1
EndCpy:		jne CPY2								; 2
													; cycles = 5 + (2^11)*7 = 14341
DONE:
			NOP

CPY3:		; do it again but try to do it with a while-do loop using indexed mode in both src and dst of the mov statement

MAIN_LOOP3:
			mov.w #ramS0Base, ramS0ptr	; set up the pointers to RAM
			mov.w #ramS1Base, ramS1ptr	; true this one may not be used ...
			mov.w #numBytes, index		; clear index counter.  emulated with mov.w #0, dst

WHILE		dec index								; 1
			jnz CPY3								; 2
CPY3:		mov.w ramS0ptr(index), ramS1Base(index)	; 5
			jmp WHILE								; 2



;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            
