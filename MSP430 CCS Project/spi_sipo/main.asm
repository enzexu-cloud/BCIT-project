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
receivedByteAddr .equ 0x2500                ; Memory address to store received bytes
            .define BIT0, SIN               ; P3IN<0> (Serial input)
            .define BIT1, SCLK              ; P3IN<1> (Clock input)

;-------------------------------------------------------------------------------
PORT_INIT:                                  ; Initialize the required ports
            bic.b   #BIT0|BIT1, &P3DIR      ; Set P3.0 (SIN) and P3.1 (SCLK) as inputs

;-------------------------------------------------------------------------------
MAIN:
            mov.w   #receivedByteAddr, R12  ; Address to store received bytes
            mov.w   #2, R6                  ; Byte counter for two bytes
            call    #SPI_SIPO               ; Call SPI_SIPO to receive bytes
            jmp     MAIN                    ; Loop to keep receiving bytes

;-------------------------------------------------------------------------------
SPI_SIPO:
            clr.b   R7                      ; Clear R7 to store received byte
            mov.b   #8, R5                  ; Bit counter (8 bits per byte)

RECEIVE_BIT:
            bit.b   #SCLK, &P3IN            ; Check if SCLK is high
            jz      RECEIVE_BIT             ; Wait for rising edge of SCLK

            bit.b   #SIN, &P3IN             ; Read SIN (P3.0)
            rlc.b   R7                      ; Rotate SIN bit into R7

WAIT_CLK_LOW:
            bit.b   #SCLK, &P3IN            ; Check if SCLK is low
            jnz     WAIT_CLK_LOW            ; Wait for falling edge of SCLK

            dec.b   R5                      ; Decrement bit counter
            jnz     RECEIVE_BIT             ; Repeat until all 8 bits are received

            mov.b   R7, 0(R12)                ; Store the received byte in RAM
            inc.w   R12                     ; Move to the next memory location
            dec.w   R6                      ; Decrement byte counter
            jnz     SPI_SIPO                ; Repeat for the next byte
            ret                             ; Return to MAIN loop

                                            

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
            
