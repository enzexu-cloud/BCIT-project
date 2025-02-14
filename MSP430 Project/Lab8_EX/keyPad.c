/*
 * keyPad.c
 *
 *  Created on: 2024Äê11ÔÂ27ÈÕ
 *      Author: Enze Xu
 */
#include "keypad.h"
#include <msp430.h>

// Initialize the keypad and associated hardware
void keypadInit(KEYPAD_4X3 *keypad) {
    // Configure COL port as output
    P6DIR |= 0x07;      // Set P6.0 - P6.2 as output (COL<3:1>)
    P6OUT = 0x07;       // Initialize COL to all high (0x7)

    // Configure ROW port as input with pull-down resistors
    P2DIR &= ~0xF0;     // Set P2.4 - P2.7 as input (ROW<4:1>)
    P2REN |= 0xF0;      // Enable pull-down resistors for P2.4 - P2.7
    P2OUT &= ~0xF0;     // Configure pull-down resistors

    P2IE |= 0xF0;
    P2IES &= ~0xF0;
    P2IFG &= ~0xF0;

    // Initialize keypad structure
    keypad->currentKeyCoord = 0x00;
    keypad->keyPressCnt = 0;
    int i;
    /*
    for (i = 0; i < KEY_BUFF_SZ; i++) {
        keypad->keyBuffer[i] = 0x00;
    }
    */
    keypad->currentKey = 0x00;
}

// Scan the keypad to detect the row and column of a key press
unsigned char scanKeyPad(KEYPAD_4X3 *keypad) {
    unsigned char rowValue, colValue;

    for (colValue = 1; colValue <= 0x04; colValue <<= 1) {  // Iterate through 3 columns
        P6OUT = colValue;       // Activate one column at a time

        rowValue = P2IN & 0xF0;  // Read row inputs (high nibble)

        if (rowValue) {
            keypad->currentKeyCoord = (rowValue | colValue);
            return 0; // Key detected
        }
    }

    keypad->currentKeyCoord = 0x00; // No key press
    return -1; // No key detected
}

// Update the keypad state after a key press is detected
unsigned char updateKeypad(KEYPAD_4X3 *keypad) {
    if (keypad->currentKeyCoord == 0x00) {
        return -1; // No valid key coordinate to update
    }

    // Update keyPressCnt
    keypad->keyPressCnt++;
    if (keypad->keyPressCnt >= KEY_BUFF_SZ) {
        keypad->keyPressCnt = 0; // Reset count if buffer overflows
    }

    // Update keyBuffer (if implemented)
    keypad->keyBuffer[keypad->keyPressCnt - 1] = keypad->currentKeyCoord;

    // Decode key coordinate into the actual key
    decodeKeyCoord(keypad);

    return 0; // Update successful
}

// Decode the current key coordinate into the actual key
unsigned char decodeKeyCoord(KEYPAD_4X3 *keypad) {
    unsigned char row = (keypad->currentKeyCoord & 0xF0) >> 4;
    unsigned char col = (keypad->currentKeyCoord & 0x0F);

    static const char keyMap[4][3] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'},
        {'*', '0', '#'}
    };

    if (row >= 1 && row <= 4 && col >= 1 && col <= 3) {
        keypad->currentKey = keyMap[row - 1][col - 1];
        return 0; // Decoding successful
    }

    keypad->currentKey = 0x00; // Invalid key
    return -1; // Decoding failed
}




