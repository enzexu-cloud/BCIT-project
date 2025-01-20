#ifndef SWENCDEC_H_
#define SWENCDEC_H_

#include <msp430.h>

#define QPORT P6IN    // Port 6 Input for encoder
#define CHA BIT1      // Channel A connected to P6.1
#define CHB BIT0      // Channel B connected to P6.0

// Encoder structure to store state and position
typedef struct {
    unsigned char currentState;// CHA = BIT1, CHB = BIT0
    unsigned char prevState;// CHA = BIT1, CHB = BIT0
    int posCount;// signed 16 bit quadrature state counter (pulse counter)
} Q_ENCODER;

// Function prototypes

/* Initializes swEncDec interface. Initialize the required I/O ports (CHA_IN, CH_B_IN),
states and counter. Note the current and previous state need to be assigned to whatever
is read from CHA_IN, CHB_IN at the time this function is called. The counter is set to
the 0.
*/

void swEncDecInit(Q_ENCODER *qEncoder); 

/*
Updates the current state, previous state and position count of the quadrature encoder.
Returns 0 if the encoder state changed, -1 if there was no change.
*/
char swEncDecUpdate(Q_ENCODER *qEncoder);

#endif // SWENCDEC_H_
