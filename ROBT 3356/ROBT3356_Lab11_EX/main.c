
/**
 * main.c
 */
#include <msp430.h>
#include "timerA0UpMode.h"

#define LED BIT0

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;       // Stop watchdog timer
    P1DIR |= LED;                   // Set P1.0 as output
    P1OUT &= ~LED;                  // Turn off LED initially

    // Initialize Timer A0 with CCR0 for 10ms delay
    timerA0UpModeInit(62500);  // 10ms interval assuming 1MHz clock
    timerA0UpModeChannelConfig(3,OUTMOD_4,1);

    // Enable global interrupts
    __bis_SR_register(GIE);

    while (1) {
        // Nothing else to do, waiting for ISR to toggle LED
    }
}
