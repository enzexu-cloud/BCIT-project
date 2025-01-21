/*
 * P2Int_C2new.c
 *
 *  Created on: 2024Äê11ÔÂ19ÈÕ
 *      Author: Enze3
 */

#include <msp430.h>


/*
 * main.c
 */

#define LED1 BIT0
#define LED2 BIT7
#define PBS1  BIT1
#define DELAY_1000 1000

volatile int counter = 0;
int button_press = 0;
int button_release = 0;

int main(void){


    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer. Never remove.

    P4DIR |= LED2;              //  set direction for LED2
    P1DIR |= LED1;              //  set direction for LED1
    P1OUT &= ~LED1;
    //Input Port Configuration
    P2OUT |= PBS1;              // set P2OUT.1 = 1, so that pullup is selected. See Table 12-1 in User Manual
    P2REN |= PBS1;              // enable pull up resistor for push button switch

    // configure the interrupts
    P2IES |= PBS1;             // set edge detection to H->L
    P2IFG = 0;                  // clear interrupt flags by READING the register.
    P2IE |= PBS1;               // enable P2.1 interrupt. Always do this last.  QUESTION.  Why do you do this last?

    // enable the global interrupts with intrinsic function _bis_SR_register
    _bis_SR_register(GIE);

    while (1) {

        if(button_press){
            button_press = 0;
            _delay_cycles((long int) 5*DELAY_1000); //debounce the press

            P2IE |= PBS1;

            while (!(PBS1 & P2IN)){
            }
        }

        if(button_release){
            button_release = 0;
            _delay_cycles((long int) 5*DELAY_1000); //debounce the release

            P2IE |= PBS1;

            P1OUT ^= LED1;

            // QUESTION: what other ways besides as a global variable can counter be declared so it can be used to keep track of ISR entry
            counter++;
        }

     }

    return 0;
}


// Here is where the ISR address is assigned to the PORT2_VECTOR address
#pragma vector = PORT2_VECTOR // this statement must be left as is. PORT2_VECTOR is defined in msp430.h as are the other interrupt vector addresses.
 __interrupt void Port2_ISR1 (void) // you can choose any name for the ISR. Port2_ISR1
    {
         P2IE &= ~PBS1;

         // if detect H->L
         if(P2IES & PBS1){
          button_press = 1;
         }

         // if detect L->H
         if(!(P2IES & PBS1)){
           button_release = 1;
         }


        P2IES ^= PBS1;
        P2IFG &= ~PBS1;
    }




