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


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
ramS0Base	.equ 0x2400                ; Base address of data to serialize
            .define BIT0, SOUT          ; P3OUT<0> (SOUT)
            .define BIT1, SCLK          ; P3OUT<1> (SCLK)

;-------------------------------------------------------------------------------
PORT_INIT:                                  ; Initialize the required ports
            bic.b   #BIT1, &P1DIR           ; Set P1.3 as input (button)
            bis.b   #BIT0|BIT1, &P3DIR      ; Set P3.0 (SOUT) and P3.1 (SCLK) as outputs
            bic.b   #BIT0|BIT1, &P3OUT      ; Clear SOUT and SCLK initially

            mov.b   #0x95, &ramS0Base       ; Store byte 0x95 at ramS0Base
            mov.b   #0x55, &ramS0Base+1     ; Store byte 0x55 at ramS0Base+1

;-------------------------------------------------------------------------------
MAIN:
            mov.w   #ramS0Base, R12         ; Load starting address of data
            mov.w   #2, R6                  ; Byte counter for two bytes

WAIT_FOR_BUTTON:
            bit.b   #BIT1, &P1IN            ; Check if button is pressed (P1.3 high)
            jnz     WAIT_FOR_BUTTON         ; Wait here if button not pressed
            call    #SPI_PISO               ; Transmit bytes on button press
            jmp     WAIT_FOR_BUTTON         ; Return to wait for button press

;-------------------------------------------------------------------------------
SPI_PISO:
            mov.b   @R12, R7                ; Load byte to transmit into R7
            mov.b   #8, R5                  ; Initialize bit counter for 8 bits

BIT_LOOP:
            rlc.b   R7                      ; Rotate left through carry to send MSB first
            jc      SET_SOUT                ; If carry is 1, set SOUT
            bic.b   #SOUT, &P3OUT           ; Clear SOUT (send 0)
            jmp     CLOCK_PULSE             ; Generate clock pulse

SET_SOUT:
            bis.b   #SOUT, &P3OUT           ; Set SOUT (send 1)

CLOCK_PULSE:
            bic.b   #SCLK, &P3OUT           ; Set SCLK low (start of clock pulse)
            nop                             ; (50% duty cycle)
            nop                             ; (50% duty cycle)
            bis.b   #SCLK, &P3OUT           ; Set SCLK high (end of clock pulse)
            nop                             ; (50% duty cycle)
            nop                             ; (50% duty cycle)

            dec.b   R5                      ; Decrement bit counter
            jnz     BIT_LOOP                ; Repeat for all 8 bits

            inc.w   R12                     ; Move to next byte in memory
            dec.w   R6                      ; Decrement byte counter
            jnz     SPI_PISO                ; Repeat for next byte if any remain
            ret                             ; Return to wait for button press

                                            

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
            
