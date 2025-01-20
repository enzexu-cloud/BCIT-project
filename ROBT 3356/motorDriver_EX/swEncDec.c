/*
 * swEncDec.c
 *
 *  Created on: Nov 7, 2024
 *      Author: exu10
 */
#include "swEncDec.h"
// Initialize encoder interface
void swEncDecInit(Q_ENCODER *qEncoder) {
    qEncoder->currentState = QPORT & (CHA | CHB);
    qEncoder->prevState = qEncoder->currentState;
    qEncoder->posCount = 0;
}

void test(Q_ENCODER *qEncoder) {
    int testArray[] = {0x0, 0x2, 0x3, 0x1};
    int i = 0;

    // Set Port 6 pins as outputs to simulate quadrature signals
    P6DIR |= (CHA | CHB);  // Set P6.1 and P6.0 as outputs

    while (1) {  // Infinite loop for testing
        // Update encoder states based on testArray
        qEncoder->prevState = qEncoder->currentState;
        qEncoder->currentState = testArray[i];

        // Clear previous state bits on P6OUT, then set to the new state
        P6OUT = (P6OUT & ~(CHA | CHB)) | qEncoder->currentState;

        swEncDecUpdate(qEncoder);

        computeFlywheelAngle(qEncoder);
        // Move to the next state in testArray, wrapping around
        i = (i + 1) % 4;
    }
}



// Update encoder state and position count
char swEncDecUpdate(Q_ENCODER *qEncoder) {
    qEncoder->currentState = QPORT & (CHA | CHB);

    if (qEncoder->currentState == qEncoder->prevState) {
        return -1; // No state change
    }

    // Clockwise or counterclockwise transition logic
    if ((qEncoder->prevState == 0x00 && qEncoder->currentState == CHA) ||
        (qEncoder->prevState == CHA && qEncoder->currentState == (CHA | CHB)) ||
        (qEncoder->prevState == (CHA | CHB) && qEncoder->currentState == CHB) ||
        (qEncoder->prevState == CHB && qEncoder->currentState == 0x00)) {
        qEncoder->posCount++; // Clockwise
    } else {
        qEncoder->posCount--; // Counterclockwise
    }

    qEncoder->prevState = qEncoder->currentState; // Update previous state
    return 0; // State changed


}

// Function to calculate the angle based on position count
int computeFlywheelAngle(Q_ENCODER *qEncoder) {
    int angle;
    angle = ((qEncoder->posCount) % 400) * (360 / (400 * 11.5));

    return angle;
}




