#include "swEncDec.h"

// Initialize encoder interface
void swEncDecInit(Q_ENCODER *qEncoder) {
    qEncoder->currentState = QPORT & (CHA | CHB);
    qEncoder->prevState = qEncoder->currentState;
    qEncoder->posCount = 0;
}

void test(Q_ENCODER *qEncoder) {
    int testArray[] = {0x00, 0x10, 0x11, 0x01};
    int i = 0;

    while (1) {  // Infinite loop
        qEncoder->prevState = testArray[i];
        qEncoder->currentState = testArray[(i + 1) % 4];  // Wrap around to the start of the array

        // Perform other operations if needed

        i = (i + 1) % 4;  // Increment and wrap `i` to stay within array bounds
    }
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

    if (((CHA & qEncoder->currentState) - (CHA & qEncoder->prevState) == CHA) && 
       ((CHB & qEncoder->currentState) == (CHB & qEncoder->prevState)) ||
       ((CHB & qEncoder->currentState) - (CHB & qEncoder->prevState) == CHB) && 
       ((CHA & qEncoder->currentState) == (CHA & qEncoder->prevState))){
       qEncoder->posCount++; // Clockwise
       }
       else {
        qEncoder->posCount--; // Counterclockwise
    }
    qEncoder->prevState = qEncoder->currentState; // Update previous state
    return 0; // State changed
}
