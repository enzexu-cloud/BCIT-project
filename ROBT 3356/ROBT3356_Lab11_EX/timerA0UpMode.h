/*
 * timerA0UpMode.h
 *
 *  Created on: 2024��12��4��
 *      Author: Enze3
 */

#ifndef TIMERA0UPMODE_H
#define TIMERA0UPMODE_H

void timerA0UpModeInit(int ta0ccr0Val);
void timerA0UpModeChannelConfig(unsigned char channel, unsigned char outMode, unsigned char intEn);
__interrupt void timerA0MultiISR(void);

#endif

