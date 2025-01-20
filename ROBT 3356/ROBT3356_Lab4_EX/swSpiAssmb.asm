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

;-------------------------------------------------------------------------------
; Define constants and labels
;-------------------------------------------------------------------------------
ramS0Base   .equ 0x2400                     ; Base address of data to serialize
OneKB       .equ 0x1024                     ; Define 1KB

            .define BIT0, SOUT              ; P3OUT<0> for serial data out-----
            .define BIT1, SCLK              ; P3OUT<1> for clock signal

            .define R12, ramS0ptr           ; R12 points to the data array
            .define R5, bitCtr              ; R5 for bit counter
            .define R6, byteCtr             ; R6 for byte counter

;-------------------------------------------------------------------------------
PORT_INIT:                                    ; Initialize ports
            bis.b   #SOUT|SCLK, &P3DIR       ; Set BIT0 (SOUT) and BIT1 (SCLK) as output
            bic.b   #SOUT|SCLK, &P3OUT       ; Clear the SOUT and SCLK lines

;-------------------------------------------------------------------------------
MAIN:                                         ; Main loop
            mov.w   #ramS0Base, ramS0ptr     ; Load base address of data into R12
            mov.w   #2, byteCtr        ; Copy number of bytes into byte counter (R6)

BYTE_LOOP:                                    ; Outer loop to serialize each byte
            mov.b   @ramS0ptr+, R7           ; Load byte into R7, increment pointer
            mov.w   #8, bitCtr               ; Load bit counter with 8 (since each byte has 8 bits)

BIT_LOOP:                                     ; Inner loop to serialize each bit
            rlc.b   R7                       ;1 ; Rotate left through carry to get MSB into carry flag
            jc      SET_SOUT                 ;2 ; If carry is set, set SOUT high
            bic.b   #BIT0, &P3OUT            ;3 ; Clear SOUT (BIT0)

            jmp     CLK_LOW                  ;2 Jump to toggle clock low

SET_SOUT:                                     ; Set SOUT high
            bis.b   #BIT0, &P3OUT             ;3 ; Set SOUT (BIT0)

CLK_LOW:                                      ; Toggle SCLK (BIT1) low, start of clock pulse
            bic.b   #BIT1, &P3OUT             ;3 ; Clear SCLK
            nop								  ;1
            nop								  ;1
            nop
            nop
            nop
			nop								  ;1
            nop								  ;1
            nop
            nop
			nop

CLK_HIGH:                                     ; Toggle SCLK high, end of clock pulse
            bis.b   #BIT1, &P3OUT            ;3 ; Set SCLK

            dec.w   bitCtr                   ;1 ; Decrement bit counter
            jnz     BIT_LOOP                 ;2 ; If not zero, repeat BIT_LOOP

            dec.w   byteCtr                  ;1 ; Decrement byte counter
            jnz     BYTE_LOOP                ;2 ; If not zero, repeat BYTE_LOOP

            								 ;CYCLES HIGH =

DONE:
            jmp MAIN                         ; Repeat the super loop

            nop
			nop
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
