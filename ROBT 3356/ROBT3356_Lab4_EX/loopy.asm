;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
; ROBT3356 Lab3. Use this file for
; Greg Scutt
; Oct 1, 2016
;
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

;			.define R5, COUNTER
COUNT1 		.equ 50000

;-------------------------------------------------------------------------------
; Main loop here



MAIN_LOOP:

			bis.b #BIT0, &P1DIR			; set up LED1 for output
			bis.b #BIT7, &P4DIR			; set up LED2 for output

			bic.b #BIT0, &P1OUT			; turn off LED's
			bic.b #BIT7, &P4OUT

			clr.w R2						; clear all flags
;    start single stepping here to observe registers and flow of program
; --------------------IF/THEN/ELSE ----------------------------------------

			; let's test see if two values are equivalent. A basic IF/ELSE statement.  IF A==B then turn on led1 else turn on led2
			mov.b #0x78,R7				; after this instruction executes you can manually modify the contents of R7 to affect the result of the cmp.
			cmp.b #0x78,R7				; cmp will compute R7 - 0x78 and set the flags. Check Zero flag in SR register. Is it set ? What set it?
			jeq	EQUAL					; now the flag is checked and a jump results if it is set (Z = 1) then jump to EQUAL.  jz is the same as jeq
NOT_EQUAL	bis.b #BIT7, &P4OUT			; ELSE turn on LED2 if not equal
			bic.b #BIT0, &P1OUT			; turn off LED1 if not equal
			jmp DONE

EQUAL		bis.b #BIT0, &P1OUT			; turn on LED1 if equal
			bic.b #BIT7, &P4OUT			; turn off LED2 if equal



; -------------------- DO-WHILE ----------------------------------------

			mov.w #COUNT1, R4				;
			xor #BIT0, &P1OUT				; toggle  LED for fun
			clr.w &0x2400					; arbitrary clear of mem contents

DO			; loop instructions. whatever you want to DO in the loop
			; output to a port if you want
			; move memory contents
			inc.w &0x2400					; instructions inside the loop. always executed at least once with a DO-WHILE !!

			dec.w R4						; Test instrunction: decrement R4.  This will set flags
WHILE1		jnz DO							; WHILE
			xor #BIT0, &P1OUT				; toggle  LED again when loop is done.


; -------------------- WHILE-DO ----------------------------------------
; we can execute a traditional While loop too as long as we test for the condition at the top of the loop
			clr.w &0x2400				; arbitrary clear of mem contents
			clr.w R4


WHILE2		cmp #COUNT1, R4				; Test instrunction: compare R4 to COUNT.  R4 - #COUNT1
			jeq CONT					; test condition. jeq is same as jz

			inc.w &0x2400				; execute loop instructions. Here just arbitrarily  increment contents of memory

			inc.w R4					; increment the loop counter
			jmp WHILE2

CONT		xor #BIT0, &P1OUT			; toggle  LED again when loop is done.




DONE		jmp MAIN_LOOP				; jump back to main loop
			NOP

;-------------------------------------------------------------------------------

                                            

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
            
