/***************************************************************************
* remoter.c                                                    Version 0.0.1
*
* Routines to run 8-bit timer2 in M8 for generating the 38kHz carrier waveform.
* Currently supported:  Atmega8
*
* Copyright (c) 2005-2015 Whileloop Inc.
* All rights reserved. www.whileloop.com
*
* This software is confidential and proprietary to Whileloop Inc.
* It has been furnished under a license and may be used, copied, or
* disclosed only in accordance with the terms of that license and with
* the inclusion of this header. No title to nor ownership of this
* software is hereby transferred.
*
* Author:  Guang Yang
*
*****************************************************************************/
#include<iom8v.h>
#include "def.h"
#include "dbg.h"
#include "uart.h"
#include "app.h"
#include "8bits_timer.h"

INT16 ctc_cnt = 0;

#pragma interrupt_handler Timer2_CTC_ISR:4
void Timer2_CTC_ISR(void)
{  
    ctc_cnt++;
    UART_PrintUInt(ctc_cnt);
}

/* Initialize the timer2 to operate the output compare in CTC mode */
void Timer2_Init(void)
{
    asm("cli");    /* disable all interrupt */
	
    TCNT2=0;/* Clear counter */
    OCR2=0xF0;/* Set the value for output compare */

    /* Setup CTC mode */
    TCCR2=(TIMER2_OPERATION_MODE_CTC_21<<WGM21)|(TIMER2_COM_NON_PWM_TOGGLE<<COM20)|(TIMER2_CLOCK_SELECE_1024_FROM_PRESCALING<<CS20);
    /*WGM21(bit 6, bit 3): Waveform Generator Mode
    COM20(bit 5:4): Compare Match Output Mode
    CS22~CS20(bit 2:0): Clock Select 
    */
    
    /* TCCR2 can define Mode of Operations, now is CTC Mode */

    /* OC2(PB3) is already set as output in main() */
    
#if 0    
    TIMSK=(1<<OCIE2);
    /*OCIE2(bit 7): Output Compare Match Interrupt Enable */
#endif

    TIFR=(1<<OCF2); 
    /*OCF2(bit 7): clear the flag which will be set when a compare match occurs between the Counter2 and the data in OCR2 
    OCF2 is automatically cleared when the Input Capture Interrupt Vector is executed.
    Alternatively, the flag can be cleared by writing a logical one to its bit location */
    
    asm("sei");
}


