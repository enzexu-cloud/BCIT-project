#include <msp430.h>


/*
 * main.c
 */

#define LED1 BIT0
#define LED2 BIT7
#define PBS1  BIT1
#define DELAY_1000 1000

volatile int counter = 0;
volatile int button_press = 0;

int main(void){


    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer. Never remove.

    P4DIR |= LED2;              //  set direction for LED2
    P1DIR |= LED1;              //  set direction for LED1

    //Input Port Configuration
    P2OUT |= PBS1;              // set P2OUT.1 = 1, so that pullup is selected. See Table 12-1 in User Manual
    P2REN |= PBS1;              // enable pull up resistor for push button switch

    // configure the interrupts
    P2IES |= PBS1;             // set edge detection to L->H
    P2IFG = 0;                  // clear interrupt flags by READING the register.
    P2IE |= PBS1;              // enable P2.1 interrupt. Always do this last.  QUESTION.  Why do you do this last?

    // enable the global interrupts with intrinsic function _bis_SR_register
    _bis_SR_register(GIE);

    while (1) {
        P2IE |= PBS1;
        if(button_press){
        _delay_cycles(1);


        while (!(PBS1 & P2IN)){

        }
                      // enable P2.1 interrupt. Always do this last.  QUESTION.  Why do you do this last?
        // ok,  now do something in the ISR ...
        P1OUT ^= LED1;
        button_press = 0;
        }


    }
}


// Here is where the ISR address is assigned to the PORT2_VECTOR address
#pragma vector = PORT2_VECTOR // this statement must be left as is. PORT2_VECTOR is defined in msp430.h as are the other interrupt vector addresses.
 __interrupt void Port2_ISR1 (void) // you can choose any name for the ISR. Port2_ISR1
    {

         P2IE &= ~PBS1;
         P2IES ^= PBS1;
         button_press = 1;

        /* clear interrupt flag at end of ISR
        QUESTION: what would happen if the flag wasn't cleared*/
        P2IFG &= ~PBS1;


    }

