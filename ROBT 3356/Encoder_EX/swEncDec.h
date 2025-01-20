/*
 * swEncDec.h
 *
 *  Created on: Nov 7, 2024
 *      Author: exu10
 */

#ifndef SWENCDEC_H_
#define SWENCDEC_H_

#include <msp430.h> // Include microcontroller-specific definitions

// Define input pins for encoder channels A and B
#define QPORT P1IN      // Encoder port input
#define CHA BIT5        // Channel A input
#define CHB BIT4        // Channel B input

// Define the Q_ENCODER structure
typedef struct {
    unsigned char currentState;  // Current state of encoder
    unsigned char prevState;     // Previous state of encoder
    int posCount;                // Position count
} Q_ENCODER;

// Function prototypes
void swEncDecInit(Q_ENCODER *qEncoder);
char swEncDecUpdate(Q_ENCODER *qEncoder);
void test(Q_ENCODER *qEncoder);
void computeFlywheelAngle(Q_ENCODER *qEncoder);

extern volatile float angle;

#endif /* SWENCDEC_H_ */
