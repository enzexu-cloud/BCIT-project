;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
; Lab 9 ROBT 3356
; Modified Nov 2019.
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

LED1 .equ BIT0
LED2 .equ BIT7
PBS1 .equ BIT1			; push button switch S1
			.define R8, counter

;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------


			; configure I/O ports
			mov.b #LED1 ,&P1OUT 	;
			bis.b #LED1 ,&P1DIR 	;
			bis.b #LED2 ,&P4DIR		;

			bis.b #PBS1, &P2OUT		; set up PBS1 as input with internal pullup
			bis.b #PBS1, &P2REN

			clr.b counter			; clear button press counter

			; ***********************  interrupt configuration for PORT2 *************************
			bic.b #PBS1, &P2IES		; L->H edge detection (button release!)
			clr.b &P2IFG			; writes to the P2IFG register to clear any pending interrupts.
			bis.b #PBS1, &P2IE		; enables interrupt on P2S1.

			NOP;					; assembler recommends NOP before and after GIE set.  No idea why yet.
			bis.b #GIE, SR			; enable ALL maskable interrupts.
			NOP




MAIN:

	add.w #0x4573, R7	; dummy instructions while we wait for the phone to ring.
	sub.w #0x38, R7
	NOP
	
	push R12
	mov.b #1000, R12
	call #DELAY_10m		; control flash rate at 1 sec = 1000 * 1ms
	pop R12
	
	xor.b #LED1, &P1OUT		; toggle LED1

	jmp MAIN





;--------------------------------------------------------------------------------------------------------
;Port2_ISR1: ISR1 for PORT2
;
; debounces the initial H->L interrupt detection on P2.1 using a 50ms delay
; waits for button to be released and then debounces the release for 50ms.
;
; toggles LED1
; increments counter (R8). This is a effectively a global variable

;---------------------------------------------------------------------------------------------------------
Port2_ISR1:	; ring ring



S1PRESS:	
			; ok this ISR assumes that the push button ONLY bounces when it is released. So the ISR is dead simple.
			

			; Press (L --> H) detected.  debounce the button release
			push R12
			mov.b #1, R12
			call #DELAY_10m			;  debounce
			pop R12

			; OK, now it is time to service the interrupt
			xor.b #LED2, &P4OUT		; toggle LED2
			inc.b counter			; increment the counter. notice the counter is like a global variable

			; clear the interrupt flags before exiting the ISR. It is crucial to clear them at the end of debouncing, so they don't get triggered again by spurious bouncing.
			; remember that although GIE is 0 during ISR that doesn't mean the P2IFG won't register interrupts.  It will set it's
			; flags anytime it sees a L->H transition. And then when ISR is complete after rti is executed, GIE will be set, and the CPU
			; will immediatey respond to the pending P2.1 interrupt, which was a "false trigger".
			bic.b #PBS1, &P2IFG

			reti
; ----------------------------------------------------------------------------------------------------------

;--------------------------------------------------------------------------------------------------------
;Subroutine Delay_10m: provides a scalable M*10ms delay. M is a integer passed through a CPU register
;
; Input Parameters:
; R12 - Input parameter used to multiply 10ms. Total delay = (R12)*10ms.
;
; Registers Used In Subroutine: R10
; Registers Destroyed: R12. User to save R12 before passing parameter with it
;
; Local Variables:
; R10 - inner loop counter.

; Output Parameters: none.
;---------------------------------------------------------------------------------------------------------
DELAY_10m:
											; the outer loop
			push R10						; save R10 on the stack. [3 MCLK cycles]
			jmp LOOP_TEST					; jump to test R8 in case it is zero [2 MCLK cycles]
DELAY_LOOP1:
			mov.w #1, R10				; load the inner delay count 3333 into register R10. Set to 1 for single stepping [2 MCLK cycles]
DELAY_LOOP2:								; the inner loop
			dec.w R10						; dec is emulated with a subtraction sub #1,R5 !!  so it is not a single operand instruction.  [1 MCLK cycles]
			jnz DELAY_LOOP2					; all jumps have [2 MCLK cycles]

			dec.w R12						; [1 MCLK cycles]
LOOP_TEST:	cmp #0, R12						; check if R8 is zero
			jnz DELAY_LOOP1					; [2 MCLK cycles]

			pop R10							; restore R10 [2 MCLK cycles]
			ret								; return to calling subroutine [2 MCLK cycles]
;-----------------------------------------------------------------------------------------------------------


;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END				; .global is indicating that __STACK_END, the bottom of the stack, is defined in another file
            .sect   .stack					; current section is the stack. Beneath this user could assign values to the stack like global vars etc ...
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            

;--------------------------- I needed to add this here sepecifically to initialize the PORT2_VECTOR address to be vector pointing to the ISR-----------------------------------------------------
      		.intvec PORT2_VECTOR, Port2_ISR1	;  Port2_ISR assigned to be the vector at vector address PORT2_VECTOR
      											; intvec is an assembler directive that assigns the vector Port2_ISR1  address PORT2_VECTOR
      											; PORT2_VECTOR is defined in msp430.h as 0xFFD4. which corresponds to the vector address in the msp430 vector table
      											; consider its equivalent in C:
      											; *(PORT2_VECTOR) = Port2_ISR1.
      											; Or if we knew before linking the code what Port2_ISR1 was going to be we could write in assembler:
      											; mov.w #Port2_ISR1, &PORT2_VECTOR
      											; but the bottom line is that we do not know what the address of the ISR is, so we need  the assembler linker to make that
      											; assigment once it figures it out. That is what the .intvec will do.
;-----------------------------------------------------------------------------------------------------------------------------------------
