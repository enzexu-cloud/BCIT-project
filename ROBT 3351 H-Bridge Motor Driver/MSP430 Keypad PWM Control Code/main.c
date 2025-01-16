
#include <msp430.h>
#include "keypad.h"
#include "timerA0UpMode.h"

#define DELAY_1000 1000
#define PWM_FREQUENCY 1000  // PWM frequency in Hz
int button_press = 0;
int button_release = 0;

KEYPAD_4X3 myKeypad;  // Declare a global keypad structure

// Function prototypes
void updateDutyCycle(unsigned char key);

int main(void) {
     WDTCTL = WDTPW | WDTHOLD;  // Stop the watchdog timer

    // Initialize keypad
    keypadInit(&myKeypad);

    // Initialize Timer A0 for PWM
    timerA0PWMConfig(PWM_FREQUENCY);  // Set frequency to 1kHz

    // Enable global interrupts
    __bis_SR_register(GIE);

    while (1) {  // Super-loop
        if (button_press) {  // Key event detected
            button_press = 0;
            __delay_cycles(5 * DELAY_1000);  // Debounce the press
            P2IE |= 0xF0;  // Re-enable keypad interrupts
            while ((P2IN & 0xF0)) {}  // Wait for button release
        }

        if (button_release) {  // Key release detected
            button_release = 0;
            __delay_cycles(5 * DELAY_1000);  // Debounce the release
            P2IE |= 0xF0;  // Re-enable keypad interrupts
            updateKeypad(&myKeypad);  // Update keypad state
            updateDutyCycle(myKeypad.currentKey);  // Adjust duty cycle
        }
    }
}

// Adjust PWM duty cycle based on keypad input
void updateDutyCycle(unsigned char key) {
    unsigned int dutyCycle;

    switch (key) {
        case '0': dutyCycle = 0; break;    // 0% (squelch)
        case '1': dutyCycle = 10; break;   // 10%
        case '2': dutyCycle = 20; break;   // 20%
        case '3': dutyCycle = 30; break;   // 30%
        case '4': dutyCycle = 40; break;   // 40%
        case '5': dutyCycle = 50; break;   // 50%
        case '6': dutyCycle = 60; break;   // 60%
        case '7': dutyCycle = 70; break;   // 70%
        case '8': dutyCycle = 80; break;   // 80%
        case '9': dutyCycle = 90; break;   // 90%
        default: return;  // Invalid key, do nothing
    }

    // Update CCR3 for new duty cycle
    TA0CCR3 = (TA0CCR0 + 1) * dutyCycle / 100;
}
