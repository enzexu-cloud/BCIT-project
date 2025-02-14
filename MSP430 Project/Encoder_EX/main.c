#include <msp430.h>
#include "swEncDec.h"
#define TEST 0

Q_ENCODER qEncoder1;
volatile float angle;


int main(void){

WDTCTL = WDTPW | WDTHOLD;// stop watchdog timer

swEncDecInit(&qEncoder1);    // Initialize encoder

while (1) {

       #if TEST
       test(&qEncoder1);

       #endif
       // Check for state change
       if (!swEncDecUpdate(&qEncoder1)) {
           computeFlywheelAngle(&qEncoder1); // Compute angle
        }
    }

    // ISR for PORT1
}

#pragma vector=PORT1_VECTOR
__interrupt void Port6_ISR(void) {
  swEncDecUpdate(&qEncoder1); // Handle encoder state change
  P1IFG &= ~(CHA | CHB);     // Clear interrupt flags
}
