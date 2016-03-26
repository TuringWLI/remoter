/*******************************************************************
*
*         Copyright (c) 2015 by Whileloop, Inc. (WLI)
*
*  All rights are reserved. Reproduction or in part is prohibited
*  without the written consent of the copyright owner.
*
*  16bits_timer.h
*  This file contains the interface of 16 bits timer.
*
*******************************************************************/
#ifndef _16_BITS_TIMER_H_
#define _16_BITS_TIMER_H_

#define TIMER1_INPUT_CAPTURE_EDGE_SELECE_RISING 1
#define TIMER1_INPUT_CAPTURE_EDGE_SELECE_FALLING 0
#define TIMER1_CLOCK_SELECE_NO_SOURCE 0
#define TIMER1_CLOCK_SELECE_NO_PRESCALING 1
#define TIMER1_CLOCK_SELECE_256_PRESCALING 4

#define WAVE_CHANGE_NUM	100

#define INPUT_CAPTURE_ENABLE 1
#define INPUT_CAPTURE_DISABLE 0

extern INT16 capture_pnt;
extern INT16 wave_time[FUNC_KEY_NUM][WAVE_CHANGE_NUM];

void Timer1_CaptureStart(void);
void Timer1_CaptureStop(void);
void Timer1_CntStart(UINT16 start_num);
void Timer1_CntStop(void);
void External_Int0_Init(void);

#endif /*_16_BITS_TIMER_H_*/
