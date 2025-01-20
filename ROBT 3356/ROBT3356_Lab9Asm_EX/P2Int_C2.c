#include <msp430.h> 


/*
 * main.c
 */

#define LED1 BIT0
#define LED2 BIT7
#define PBS1  BIT1
#define DELAY_1000 1000

volatile int counter = 0;
volatile int button_pressed = 0;  // Flag to indicate if button press event occurred

int main(void){


    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer. Never remove.

    P4DIR |= LED2;              //  set direction for LED2
    P1DIR |= LED1;              //  set direction for LED1

    //Input Port Configuration
    P2OUT |= PBS1;              // set P2OUT.1 = 1, so that pullup is selected. See Table 12-1 in User Manual
    P2REN |= PBS1;              // enable pull up resistor for push button switch

    // configure the interrupts
    P2IES &= ~PBS1;             // set edge detection to L->H
    P2IFG = 0;                  // clear interrupt flags by READING the register.
    P2IE |= PBS1;               // enable P2.1 interrupt. Always do this last.  QUESTION.  Why do you do this last?

    // enable the global interrupts with intrinsic function _bis_SR_register
    _bis_SR_register(GIE);

    while (1) {

        // wait for the phone to ring.  Here is where you could put the CPU to sleep with its low power modes and just wait for an interrupt
        P4OUT ^= LED2; // just toggle the led
        _delay_cycles(1000000);
    }

    return 0;
}


// Here is where the ISR address is assigned to the PORT2_VECTOR address
#pragma vector = PORT2_VECTOR // this statement must be left as is. PORT2_VECTOR is defined in msp430.h as are the other interrupt vector addresses.
 __interrupt void Port2_ISR1 (void) // you can choose any name for the ISR. Port2_ISR1
    {
         button_pressed = 1;      // Signal that the button has been pressed


        // QUESTION: Is the CPU still listening to interrupts when it has entered the ISR?


        /* Debounce the press.  This is assuming bouncing happens during a press.
         * QUESTION:  Is it a good idea to debounce in an ISR?  Why or why not?
         * */
         _delay_cycles((long int) 5*DELAY_1000);


        //QUESTION:  Explain what this statement below is doing and what it is for. Use a diagram of a switch changing from H --> L --> H with bouncing at press and release
            while (!(PBS1 & P2IN)){
            }


      /* debounce the released switch.
       * QUESTION:  Is this _delay_cycles really necessary at this location in the ISR?  Can you explain why or why not using a diagram of a switch that bounces both when pressed and released ?  */
        _delay_cycles((long int) 5*DELAY_1000);


        // ok,  now do something in the ISR ...
        P1OUT ^= LED1;

        // QUESTION: what other ways besides as a global variable can counter be declared so it can be used to keep track of ISR entry
        counter++;

        /* clear interrupt flag at end of ISR
        QUESTION: what would happen if the flag wasn't cleared*/
        P2IFG &= ~PBS1;
    }

/*
#include <msp430.h>

#define LED1 BIT0
#define LED2 BIT7
#define PBS1 BIT1
#define DEBOUNCE_DELAY 5000  // Adjust as needed for debounce timing

volatile int button_released = 0;  // Flag to indicate if button release event occurred

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    P4DIR |= LED2;              // Set direction for LED2
    P1DIR |= LED1;              // Set direction for LED1

    // Input Port Configuration
    P2OUT |= PBS1;              // Enable pull-up resistor on P2.1 (PBS1)
    P2REN |= PBS1;              // Enable resistor on push button

    // Configure interrupts (initially disabled)
    P2IES &= ~PBS1;             // Set edge detection to L->H (button release)
    P2IFG = 0;                  // Clear interrupt flags
    P2IE &= ~PBS1;              // Keep interrupt disabled initially

    while (1) {
        // Step 1: Detect and debounce button press in main loop
        if (!(P2IN & PBS1)) {  // Button press detected
            __delay_cycles(DEBOUNCE_DELAY);  // Debounce press in main loop

            // Check if button is still pressed after debounce delay
            if (!(P2IN & PBS1)) {
                // Enable interrupt for button release after debounce
                P2IE |= PBS1;
            }

            // Step 4: Wait until ISR signals release has occurred
            while (!button_released) {
                // Do nothing, wait for ISR to set button_released flag
            }

            // Step 5: Debounce button release in main loop
            __delay_cycles(DEBOUNCE_DELAY);  // Debounce release

            // Check if button is still released after debounce delay
            if (P2IN & PBS1) {
                button_released = 0;  // Reset the release flag for the next press
            }
        }

        P4OUT ^= LED2;  // Toggle LED2 to indicate main loop is running
        _delay_cycles(1000000);
    }

    return 0;
}

// ISR for Port 2
#pragma vector = PORT2_VECTOR
__interrupt void Port2_ISR(void) {
    P1OUT ^= LED1;           // Toggle LED1
    button_released = 1;     // Signal that the button has been released
    P2IE &= ~PBS1;           // Disable interrupt to prevent further triggering
    P2IFG &= ~PBS1;          // Clear interrupt flag
}
*/
