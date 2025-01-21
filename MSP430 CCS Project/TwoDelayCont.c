#include <msp430.h> 

/*
 * main.c
 */

#define T1Interval 62500 	// SMCLK/8 half second delay.  assume 1MHz SMCLK.
#define T2Interval (T1Interval/2)   // SMCLK/8 .25 second delay
#define LED1 BIT0


int main(void) {
	WDTCTL = WDTPW | WDTHOLD;// quiet dog


	P1DIR |= BIT0;
	P4DIR |= BIT7;

	// best way to set CTRL registers ...  SMCLK/8 !!!!
	TA0CTL = TASSEL_2 | ID_3 | MC_2;


	TA0CCR1 = TA0R + T1Interval;		// load CCRn registers. Notice the count is exact !!  no need to sub 1
	TA0CCR2 = TA0R + T2Interval;

	// enable interrupts
	TA0CCTL1 = CCIE;
	TA0CCTL2 = CCIE;

	// clear the flags ...
	TA0CCR1 &= ~CCIFG;
	TA0CCR2 &= ~CCIFG;

	_enable_interrupts();			// intrinsic function provided by MSP C compiler


while(1);
}


	#pragma vector = TIMER0_A1_VECTOR  		// yes this vector !!  since we are using CCR1 and CCR2 flags
	interrupt void timerA0_A1Isr() {



		if (TA0CCTL1 & CCIFG) {				// poll flags to check interval
			TA0CCR1 += T1Interval;			// add on next interval
			TA0CCTL1 &= ~CCIFG;				// clear the flag
			P1OUT ^= BIT0;					// toggle red LED
		}



		if (TA0CCTL2 & CCIFG) {				// poll flags to check interval
			TA0CCR2 += T2Interval;			// add on next interval
			TA0CCTL2 &= ~CCIFG;				// clear the flag
			P4OUT ^= BIT7;					// toggle green LED
		}
	}
