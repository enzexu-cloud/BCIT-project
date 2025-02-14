/*
 * keyPad.h
 *
 *  Created on: 2024Äê11ÔÂ27ÈÕ
 *      Author: Enze Xu
 */

#ifndef KEYPAD_H
#define KEYPAD_H

#define KEY_BUFF_SZ 12  // Size of key press history buffer

// Redefine ports for keypad and LED array
#define ROW P2IN
#define COL P6OUT
#define LED_ARRAY P3OUT

typedef struct KEYPAD_4X3 {
    unsigned char currentKeyCoord;  // ROW and COL coordinate of the pressed key
    unsigned char keyPressCnt;      // Count of key presses
    unsigned char keyBuffer[KEY_BUFF_SZ]; // Optional: History of key press coordinates
    unsigned char currentKey;       // The literal alphanumeric key
} KEYPAD_4X3;

// Function prototypes
void keypadInit(KEYPAD_4X3 *keypad);
unsigned char scanKeyPad(KEYPAD_4X3 *keypad);
unsigned char updateKeypad(KEYPAD_4X3 *keypad);
unsigned char decodeKeyCoord(KEYPAD_4X3 *keypad);

#endif

