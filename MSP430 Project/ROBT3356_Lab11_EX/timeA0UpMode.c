/*
 * timeA0UpMode.c
 *
 *  Created on: 2024Äê12ÔÂ4ÈÕ
 *      Author: Enze3
 */
#include <msp430.h>
#include "timerA0UpMode.h"

// Initialize Timer A0 for Output Compare, Up Mode
void timerA0UpModeInit(int ta0ccr0Val) {
    TA0CTL = TASSEL_2 | ID_0 | MC_1;  // SMCLK, Up Mode, Divider /1
    TA0CCR0 = ta0ccr0Val - 1;         // Set CCR0 value for rollover interval
    TA0CTL &= ~TAIFG;                 // Clear TAIFG interrupt flag
}

// Configure a Timer A0 channel
void timerA0UpModeChannelConfig(unsigned char channel, unsigned char outMode, unsigned char intEn) {
    switch (channel) {
        case 3:
            TA0CCTL3 = outMode | (intEn ? CCIE : 0);  // Set output mode and interrupt enable
            P1SEL |= BIT3;                            // Select Timer function for P1.3
            P1DIR |= BIT3;                            // Set P1.3 as output
            break;
        case 4:
            TA0CCTL4 = outMode | (intEn ? CCIE : 0);
            P1SEL |= BIT4;
            P1DIR |= BIT4;
            break;
        default:
            // Invalid channel
            break;
    }
}

// ISR for Timer A0 (Multi-source) - Adjusted for TAIFG clearing
#pragma vector=TIMER0_A1_VECTOR
__interrupt void timerA0MultiISR(void) {
    static int count = 0;

    count++;  // Simple counter to demonstrate timing

    if (count == 31250) {  // Adjust to match TOGGLE_CNT behavior
        P1OUT ^= BIT0;  // Toggle LED (or other debug output)
        count = 0;
    }

    TA0CTL &= ~TAIFG;  // Clear interrupt flag
}

