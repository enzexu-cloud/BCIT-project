;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
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

LED1 	.equ BIT0
LED2 	.equ BIT7
SW1		.equ BIT1			;  COMPLETE this BIT defn

		.asg R4, counter
ramS0Base	.equ 0x2400                ; Base address of data to serialize

        .define BIT0, SOUT          ; P3OUT<0> (SOUT)
        .define BIT1, SCLK          ; P3OUT<1> (SCLK)


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------

INIT:
	bic.b #LED1, &P1OUT		; set output to 0
	bis.b #LED1, &P1DIR
	bic.b #LED2, &P4OUT		; set output to 0
	bis.b #LED2, &P4DIR
	bic.b #SW1, &P2DIR
	bis.b #SW1, &P2REN
	bis.b #SW1, &P2OUT


    bis.b   #BIT0|BIT1, &P3DIR      ; Set P3.0 (SOUT) and P3.1 (SCLK) as outputs
    bic.b   #BIT0|BIT1, &P3OUT      ; Clear SOUT and SCLK initially

    mov.b   #0x95, &ramS0Base       ; Store byte 0x95 at ramS0Base
    mov.b   #0x55, &ramS0Base+1     ; Store byte 0x55 at ramS0Base+1

	; COMPLETE: configure the PORT so that SW1 can be read as INPUT. 3 registers. Use SW1 defn above.


	clr counter


MAIN_LOOP:

  	mov.w   #ramS0Base, R9         ; Load starting address of data
    mov.w   #2, R6                  ; Byte counter for two bytes

SW_HIGH:
	bit.b #SW1, &P2IN 			;  COMPLETE: fill in correct &PxIN
	jnz SW_HIGH

SW_LOW:
	bit.b #SW1, &P2IN			;  COMPLETE fill in correct &PxIN
	jz SW_LOW

	push R12
	mov.b #2, R12
	call #DELAY_10m
	pop R12

LED_T0GGGLE:
	xor #LED1,&P1OUT

	push R5
	push R7

	mov.b   @R9+, R7                ; Load byte to transmit into R7
    mov.b   #8, R5                  ; Initialize bit counter for 8 bits
	call #SPI_PISO

	pop R7
	pop R5

	inc.b counter


	jmp MAIN_LOOP



;--------------------------------------------------------------------------------------------------------
;Subroutine Delay_10m: provides a scalable N*10ms delay.N is an integer passed to the subroutine
;
; Input Parameters:
; R12 - Input parameter used to multiply 10ms. Total delay = (R12)*10ms.
;
; Registers Used In Subroutine: R10
; Registers Destroyed: R12. User is reqd to save R12 before passing parameter with it
;
; Local Variables:
; R10 - inner loop counter.

; Output Parameters: none.
;---------------------------------------------------------------------------------------------------------
DELAY_10m:
			; local variable declaration
			.asg R10, innerLoopCtr			;  the .asn statement is like .define except it can be re-defined at any time. So it's used in subroutines to name the local vars.
			.asg R12, outerLoopCtr

			; constants
LOOP_10ms 	.equ 3333
											; the outer loop
			push innerLoopCtr				; save R10 on the stack. [3 MCLK cycles]
			jmp LOOP_TEST					; jump to test innerLoopCtr in case it is zero [2 MCLK cycles]
DELAY_LOOP1:
			mov.w #LOOP_10ms, innerLoopCtr		; load the inner delay count 3333 into register innerLoopCtr.   Set to 1 for single stepping. Be sure to change it back!!! [2 MCLK cycles]
DELAY_LOOP2:								; the inner loop
			dec.w innerLoopCtr				; dec is emulated with a subtraction sub #1,R5 !!  so it is not a single operand instruction.  [1 MCLK cycles]
			jnz DELAY_LOOP2					; all jumps have [2 MCLK cycles]

			dec.w outerLoopCtr				; [1 MCLK cycles]
LOOP_TEST:	cmp #0, outerLoopCtr			; check if R8 is zero
			jnz DELAY_LOOP1					; [2 MCLK cycles]

			pop innerLoopCtr				; restore R10 [2 MCLK cycles]
			ret								; return to calling subroutine [2 MCLK cycles]
;-----------------------------------------------------------------------------------------------------------

;  fill in subroutine header
;--------------------------------------------------------------------------------------------------------
;Subroutine piso1:
SPI_PISO:


BIT_LOOP:
            rlc.b   R7                      ; Rotate left through carry to send MSB first
            jc      SET_SOUT_LOW                ; If carry is 1, set SOUT

            jmp     SET_SOUT_HIGH             ; Generate clock pulse

SET_SOUT_HIGH:

            clr.w &P3OUT           				; 3 Set P3.0 (SOUT) low
            NOP									; 1
            jmp CLOCK_PULSE						; 2

SET_SOUT_LOW:

			NOP									; 1
			NOP									; 1
			mov.w #SOUT, &P3OUT					; 3 h
			NOP									; 1
			NOP									; 1
			NOP									; 1

CLOCK_PULSE:
			NOP									;1
            NOP									;1
            bis.b #SCLK, &P3OUT					;3 h

CHECK:
            dec.b   R5                      ; Decrement bit counter
            jnz     BIT_LOOP                ; Repeat for all 8 bits

            inc.w   R9                     	; Move to next byte in memory
            dec.w   R6                      ; Decrement byte counter
            bic.b	#SCLK, &P3OUT
            ret                             ; Return to wait for button press

; Input Parameters:

;
; Registers Used In Subroutine:
; Registers Destroyed:
;
; Local Variables:
;

; Output Parameters: none.
;---------------------------------------------------------------------------------------------------------

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
            
