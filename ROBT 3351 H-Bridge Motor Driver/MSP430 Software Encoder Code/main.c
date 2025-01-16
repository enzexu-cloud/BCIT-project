#include <msp430.h>
#include "swEncDec.h"
#define TEST 1
Q_ENCODER qEncoder1;         // Declare a quadrature encoder variable

int main(void){

WDTCTL = WDTPW | WDTHOLD;// stop watchdog timer

swEncDecInit(&qEncoder1);    // Initialize encoder

    while (1) {

        #if TEST
        test(&qEncoder1);
        computeFlywheelAngle(&qEncoder1);
        #endif
        // Check for state change
        if (!swEncDecUpdate(&qEncoder1)) {
            computeFlywheelAngle(qEncoder1.posCount); // Compute angle
        }
        swEncDecUpdate(&qEncoder1);
    }

    // ISR for PORT1
}

#pragma vector=PORT1_VECTOR
__interrupt void Port2_ISR(void) {
  swEncDecUpdate(&qEncoder1); // Handle encoder state change
  P1IFG &= ~(CHA | CHB);     // Clear interrupt flags
}
