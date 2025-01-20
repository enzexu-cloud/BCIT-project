
#include "swEncDec.h"
// Initialize encoder
void swEncDecInit(Q_ENCODER *qEncoder) {
    qEncoder->currentState = QPORT & (CHA | CHB);
    qEncoder->prevState = qEncoder->currentState;
    qEncoder->posCount = 0;

    // Configure PORT1 pins for input
    P1DIR &= ~(CHA | CHB);  // Set CHA and CHB as inputs
    P1REN |= (CHA | CHB);   // Enable pull-up/pull-down resistors
    P1OUT |= (CHA | CHB);   // Select pull-up resistors

    // Configure interrupts
    P1IE |= (CHA | CHB);    // Enable interrupts for CHA and CHB
    P1IES |= (CHA | CHB);   // Trigger on high-to-low transitions
    P1IFG &= ~(CHA | CHB);  // Clear interrupt flags
}

void test(Q_ENCODER *qEncoder) {
    int testArray[] = {0x0, 0x2, 0x3, 0x1};
    int i = 0;

    // Set Port 6 pins as outputs to simulate quadrature signals
    P1DIR |= (CHA | CHB);  // Set P6.1 and P6.0 as outputs

    while (1) {  // Infinite loop for testing
        // Update encoder states based on testArray
        qEncoder->prevState = qEncoder->currentState;
        qEncoder->currentState = testArray[i];

        // Clear previous state bits on P6OUT, then set to the new state
        P1OUT = (P1OUT & ~(CHA | CHB)) | qEncoder->currentState;

        computeFlywheelAngle(qEncoder);
        // Move to the next state in testArray, wrapping around
        i = (i + 1) % 4;
    }
}



// Update encoder state and position count
char swEncDecUpdate(Q_ENCODER *qEncoder) {
    if (qEncoder->currentState != qEncoder->prevState) {
        // Update dynamic edge selection
        switch (qEncoder->currentState) {
            case 0x00:
                P1IES = (P1IES & ~(CHA | CHB)) | CHA; // Trigger on rising edge of CHA
                break;
            case CHA:
                P1IES = (P1IES & ~(CHA | CHB)) | CHB; // Trigger on rising edge of CHB
                break;
            case (CHA | CHB):
                P1IES = (P1IES & ~(CHA | CHB)) | ~CHA; // Trigger on falling edge of CHA
                break;
            case CHB:
                P1IES = (P1IES & ~(CHA | CHB)) | ~CHB; // Trigger on falling edge of CHB
                break;
        }

        // Clockwise or counterclockwise logic
        if ((qEncoder->prevState == 0x00 && qEncoder->currentState == CHA) ||
            (qEncoder->prevState == CHA && qEncoder->currentState == (CHA | CHB)) ||
            (qEncoder->prevState == (CHA | CHB) && qEncoder->currentState == CHB) ||
            (qEncoder->prevState == CHB && qEncoder->currentState == 0x00)) {
            qEncoder->posCount++; // Clockwise
        } else {
            qEncoder->posCount--; // Counterclockwise
        }

        qEncoder->prevState = qEncoder->currentState; // Update the previous state
        return 0; // State changed
    }

    return -1; // No state change


}


// Function to calculate the angle based on position count
int computeFlywheelAngle(Q_ENCODER *qEncoder) {
    int angle;
    angle = ((qEncoder->posCount) % 400) * (360 / (400 * 11.5));

    return angle;
}




