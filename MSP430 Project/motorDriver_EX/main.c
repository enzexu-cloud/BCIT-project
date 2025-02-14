#include <msp430.h>
#include "swEncDec.h"

#define TEST 0

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
#if TEST
    P6DIR |= CHA;
    P6DIR |= CHB;
#endif
    Q_ENCODER qEncoder1;// declare a quadrature encoder variable

    swEncDecInit(&qEncoder1); // Initialize the encoder

    while (1) {

    #if TEST
    test(&qEncoder1);
    #endif
    swEncDecUpdate(&qEncoder1);
        //if (!swEncDecUpdate(&qEncoder1)) { // Poll for state changes
           // computeFlywheelAngle(&qEncoder1);
        //}
    }
}
