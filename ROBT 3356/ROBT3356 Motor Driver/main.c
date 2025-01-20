#include <msp430.h>
#include "swEncDec.h"

// Function to calculate the angle based on position count
void computeFlywheelAngle(int posCount) {
    // Placeholder for angle calculation logic
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    Q_ENCODER qEncoder1;// declare a quadrature encoder variable

    swEncDecInit(&qEncoder1); // Initialize the encoder

    while (1) {
        if (!swEncDecUpdate(&qEncoder1)) { // Poll for state changes
            computeFlywheelAngle(qEncoder1.posCount); // Calculate angle
        }
    }
}
