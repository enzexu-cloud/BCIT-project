#include <msp430.h>
#include "swEncDec.h"

#define TEST 1
Q_ENCODER qEncoder1;// declare a quadrature encoder variable

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    swEncDecInit(&qEncoder1); // Initialize the encoder

    __bis_SR_register(GIE); // Enable global interrupts

#if TEST
    P1DIR |= CHA;
    P1DIR |= CHB;
#endif

    while (1) {

    #if TEST
    test(&qEncoder1);
    #endif
    swEncDecUpdate(&qEncoder1);
    // Perform computations outside ISR
          int angle = computeFlywheelAngle(&qEncoder1);
    }
}


// ISR for PORT1
#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR(void) {
  swEncDecUpdate(&qEncoder1); // Handle encoder state change
  P1IFG &= ~(CHA | CHB);     // Clear interrupt flags
}
