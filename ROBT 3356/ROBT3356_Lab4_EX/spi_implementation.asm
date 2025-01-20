
; MSP430 Assembly Code for SPI with 50% Duty Cycle on SCLK
; SOUT on BIT0 (P3.0), SCLK on BIT1 (P3.1)
;-------------------------------------------------------------------------------
; swSpiAsmb.asm
;
; Template file for Lab 4 to implement SPI in assembler
;
; Author:  Enze Xu, Oct 2024
;-------------------------------------------------------------------------------

            .cdecls C,LIST,"msp430.h"       ; Include device header file

;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Retain current section in memory.
            .retainrefs                     ; Retain sections with references to current section.

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer

_start:
    ; Set P3.0 (SOUT) and P3.1 (SCLK) as output
    bis.b   #0x03, &P3DIR       ; Set bits 0 and 1 of P3 to output (SOUT and SCLK)

send_spi:
    ; Load the byte to be transmitted into register R5
    mov.b   #0x95, R5           ; Load byte (0x95 for example)

    ; Start bit transmission loop for 8 bits
    mov.b   #8, R6              ; Initialize loop counter (8 bits to transmit)

transmit_bit:
    ; Rotate left R5, placing MSB into carry
    rla.b   R5                  ; MSB of R5 goes to carry flag
    jc      set_sout_high        ; If carry is set, set SOUT high
    bic.b   #0x01, &P3OUT        ; If carry is clear, set SOUT (P3.0) low
    jmp     clock_pulse

set_sout_high:
    bis.b   #0x01, &P3OUT        ; Set SOUT (P3.0) high

clock_pulse:
    ; Set SCLK (P3.1) high to start clock pulse
    bis.b   #0x02, &P3OUT        ; Set SCLK high
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    ; Set SCLK (P3.1) low for the second half of clock pulse
    bic.b   #0x02, &P3OUT        ; Set SCLK low
    nop
    nop

    ; Decrement the bit counter
    dec.b   R6                  ; Decrement R6, which keeps track of bits sent
    jnz     transmit_bit         ; If not zero, send the next bit

    ; Loop back to the start of SPI transmission
    jmp     send_spi

;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack

;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                 ; MSP430 RESET Vector
            .short  RESET

