/*
 * main.c
 *
 *  Created on: 2024Äê11ÔÂ27ÈÕ
 *      Author: Enze Xu
 */
#include <msp430.h>
#include "keypad.h"
#define DELAY_1000 1000

KEYPAD_4X3 myKeypad;  // Declare a global keypad structure

int button_press = 0;
int button_release = 0;
// Function prototypes
void updateLedArray(KEYPAD_4X3 *keypad);

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;  // Stop the watchdog timer

    // Initialize keypad and LED array
    keypadInit(&myKeypad);
    //P3DIR |= 0xFF;     // Configure PORT3 as output for LED array
   //P3OUT = 0x00;      // Turn off all LEDs initially

    // Enable interrupts
    __bis_SR_register(GIE);

    while (1) { // Super-loop
        if (button_press) {  // Check if a key event occurred
            button_press = 0;
            _delay_cycles(1); //debounce the press
            P2IE |= 0xF0;
            updateKeypad(&myKeypad);
            while((P2IN & 0xF0)){
            }

        }

        if(button_release){
            button_release = 0;
            _delay_cycles(1); //debounce the press
            P2IE |= 0xF0;
        }

    }
}

// Interrupt Service Routine for key events
#pragma vector = PORT2_VECTOR
__interrupt void Port_2_ISR(void) {

    P2IE &= 0x00;

            // if detect H->L
            if(P2IES & 0xF0){
             button_release = 1;
            }

            // if detect L->H
            if(!(P2IES & 0xF0)){
              button_press = 1;
            }


    P2IES ^= 0xF0;
    scanKeyPad(&myKeypad);   // Scan the keypad to update currentKeyCoord
    P2IFG &= ~0xF0;          // Clear the interrupt flag for rows (P2.4 - P2.7)
}




