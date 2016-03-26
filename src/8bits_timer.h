/*******************************************************************
*
*         Copyright (c) 2015 by Whileloop, Inc. (WLI)
*
*  All rights are reserved. Reproduction or in part is prohibited
*  without the written consent of the copyright owner.
*
*  8bits_timer.h
*  This file contains the interface of 8 bits timer0 and timer1.
*
*******************************************************************/
#ifndef _8_BITS_TIMER_H_
#define _8_BITS_TIMER_H_

#define TIMER2_OPERATION_MODE_NORMAL 0
#define TIMER2_OPERATION_MODE_PWM_PHASE_CORRECT_20 1
#define TIMER2_OPERATION_MODE_PWM_PHASE_CORRECT_21 0
#define TIMER2_OPERATION_MODE_CTC_20 0
#define TIMER2_OPERATION_MODE_CTC_21 1
#define TIMER2_OPERATION_MODE_PWM_FAST_20 1
#define TIMER2_OPERATION_MODE_PWM_FAST_21 1
#define TIMER2_CLOCK_SELECE_NO_SOURCE 0
#define TIMER2_CLOCK_SELECE_NO_PRESCALING 1
#define TIMER2_CLOCK_SELECE_1024_FROM_PRESCALING 7
#define TIMER2_COM_NON_PWM_NORMAL 0
#define TIMER2_COM_NON_PWM_TOGGLE 1
#define TIMER2_COM_NON_PWM_CLEAR 2
#define TIMER2_COM_NON_PWM_SET 3

/* The formula to calculate the frequency from the datasheet */
#define CARRY_WAVE_WITH_12MCRYSTAL_WITHOUT_PRESCALING 0x9D /*38KhZ*/
#define CARRY_WAVE_WITH_8MCRYSTAL_WITHOUT_PRESCALING 0x68 /*38KhZ*/

void Timer2_Init(void);


#endif /*_8_BITS_TIMER_H_*/
