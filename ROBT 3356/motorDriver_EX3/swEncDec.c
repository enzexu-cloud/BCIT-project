/*
 * swEncDec.c
 *
 *  Created on: Nov 7, 2024
 *      Author: btan34
 */
#include "swEncDec.h"

// Initialize encoder interface
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
       P1IES |=
       P1IFG &= ~(CHA | CHB);  // Clear interrupt flags

       __bis_SR_register(GIE); // Enable global interrupts
}

void test(Q_ENCODER *qEncoder) {
    int testArray[] = {0x1, 0x3, 0x2, 0x0};
    int i = 0;

    // Set Port 6 pins as outputs to simulate quadrature signals
    P1DIR |= (CHA | CHB);

    while (1) {  // Test loop
        // Update encoder states
        qEncoder->prevState = qEncoder->currentState;
        qEncoder->currentState = testArray[i];

        // Clear previous state bits, set to the new state
        P1OUT = (P1OUT & ~(CHA | CHB)) | qEncoder->currentState;

        //swEncDecUpdate(qEncoder); ISR

        // Go to next array element, loop through using mod 4
        i = (i + 1) % 4;

    }
}



// Update encoder state and position count
char swEncDecUpdate(Q_ENCODER *qEncoder) {
    qEncoder->currentState = QPORT & (CHA | CHB);
    P1IES &= ~(CHA | CHB);
    switch (qEncoder->currentState) {
                   case 0x00:
                       P1IES |= CHA; // Trigger on rising edge of CHA
                       break;
                   case CHA:
                       P1IES |= CHB; // Trigger on rising edge of CHB
                       break;
                   case (CHA | CHB):
                       P1IES |= ~CHA; // Trigger on falling edge of CHA
                       break;
                   case CHB:
                       P1IES |= ~CHB; // Trigger on falling edge of CHB
                       break;
               }

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

// Function to compute the angle of rotation
void computeFlywheelAngle(Q_ENCODER *qEncoder) {

    float angle = (qEncoder1->posCount%400) * (360.0 / 400.0); // 400 counts per revolution

}

