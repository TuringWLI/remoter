/***************************************************************************
* 16bits_timer.c                                                    Version 0.0.1
*
* Routines to operate external interrupt 0 for starting the counting.
* and routines to run 16-bit timer1 in M8 for measure the elapsed time of input wave.
* and routines to run 16-bit timer1 in M8 for send the wave form
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
#include "16bits_timer.h"

INT16 capture_pnt = 0;
INT16 wave_time[FUNC_KEY_NUM][WAVE_CHANGE_NUM];

#pragma interrupt_handler Timer1_Overflow_ISR:9
void Timer1_Overflow_ISR(void)
{
    UART_Print("T1 overflow");
    
    /* Enable or disable the carry wave */

    
    /* Get next record and start count again 
    Timer1_CntStart();*/
}

#pragma interrupt_handler Timer1_CaptureEvent_ISR:6
void Timer1_CaptureEvent_ISR(void)
{  
    INT16 tcc1b;
    
    /* Saving wave time finished and stop timer 1 */
    if( capture_pnt >= WAVE_CHANGE_NUM )
    {
        TCCR1B &= 0xF8;/* Disable the input clock */
        return;
    }
    
    wave_time[0][capture_pnt] = ICR1;
    capture_pnt++;

    TCNT1=0;/* Clear counter */
    ICR1=0;/* Clear input capture register */

    /* change the Input Capture Edge Select */
    tcc1b = TCCR1B;
    TCCR1B &= 0xBF;/* clear ICES1 */
    tcc1b = ~tcc1b;
    tcc1b &= 0x40;/* only ICES1 */
    TCCR1B |= tcc1b;

    TIFR|=(1<<ICF1);/* Clear flag of input capture */
}

#pragma interrupt_handler External_Int0_ISR:2
void External_Int0_ISR(void)
{
    /* External Interrupt Flag GIFR[7] is cleared when the interrupt routine is executed.
    Alternatively, the flag can be cleared by writing a logical one to it */

    /* Start to record the time */
    Timer1_CaptureStart();

    /* Disable interrupt 0, prevent it from disturbing the learning */
    GICR&=~(1<<INT0);
}

void Timer1_CntStart(UINT16 start_num)
{
    asm("cli");    /* disable all interrupt */
    
    TIMSK |= (1<<TOIE1);
    /*TOIE1(bit 2): Overflow Interrupt Enable */

    TIFR |= (1<<TOV1); 
	
    TCNT1=start_num;/* Clear counter */

    /* Setup Normal Mode */
    TCCR1A=0x00;/* For Output Compare unit */
    TCCR1B=(TIMER1_CLOCK_SELECE_256_PRESCALING<<CS10);
    /*
    CS12~CS10(bit 2:0): Clock Select 
    For 12M crystal, the clock is 46,875kHz after 256 prescaling, one tick is 0.0213ms, and T1 can totally measue about 1.4s.
    For 12M crystal, the clock is 31,25kHz after 256 prescaling, one tick is 0.032ms, and T1 can totally measue about 2.0s.
    */
    
    /* TCCR1A and TCCR1B can define Mode of Operations, now is Normal Mode 
    in normal mode, the counting direction is always up and no counter clear is performed */
    
    asm("sei");
}

void Timer1_CntStop(void)
{
    TCCR1B &= 0xF8;/* Disable the input clock and Timer 1 stop*/

}

void Timer1_CaptureStop(void)
{
    TCCR1B &= 0xF8;/* Disable the input clock and Timer 1 stop*/
}


/* Initialize the timer1 to operate the input capture for saving elapsed time */
void Timer1_CaptureStart(void)
{
    asm("cli");    /* disable all interrupt */
	
    TCNT1=0;/* Clear counter */
    ICR1=0;/* Clear input capture register */

    /* Setup Input Captue Unit */
    TCCR1A=0x00;/* For Output Compare unit */
    TCCR1B=(1<<ICNC1)|(TIMER1_INPUT_CAPTURE_EDGE_SELECE_RISING<<ICES1)|(TIMER1_CLOCK_SELECE_256_PRESCALING<<CS10);
    /*ICNC1(bit 7): Input capture noise canceler
    ICES1(bit 6): Input Capture Edge Select  -** start record from falling edge and the first capture should be for the rising edge **-
    CS12~CS10(bit 2:0): Clock Select 
    For 12M crystal, the clock is 46,875kHz after 256 prescaling, one tick is 0.0213ms, and T1 can totally measue about 1.4s.
    For 12M crystal, the clock is 31,25kHz after 256 prescaling, one tick is 0.032ms, and T1 can totally measue about 2.0s.
    */
    
    /* TCCR1A and TCCR1B can define Mode of Operations, now is Normal Mode 
    in normal mode, the counting direction is always up and no counter clear is performed */

    TIMSK |= (1<<TICIE1);
    /*TICIE1(bit 5): Input Capture Interrupt Enable */

    TIFR |= (1<<ICF1); 
    /*ICF1(bit 5): clear the flag which will be set when a capture event occurs on the ICP1 pin 
    ICF1 is automatically cleared when the Input Capture Interrupt Vector is executed.
    Alternatively, the flag can be cleared by writing a logical one to its bit location */
    
    asm("sei");
}

/* Initialize the external interrupt to occur for falling edge which indicates the start of wave */
void External_Int0_Init(void)
{
    asm("cli"); /* disable all interrupt */

    /* MCUCR: power mode and external interrupt 0&1 sense control */
    MCUCR=(1<<ISC01);
    /*ISC0[1:0](bit 1:0): Interrupt 0 Sense control
    00: low level
    01: any logical change 
    10: falling edge
    11: rising edge
    */
    
    /* GICR: gerneral interrupt control register */
    GICR=(1<<INT0);
    /*INT0: External Interrupt Request 0 Enable
    */
    
    asm("sei");/* Global interrupt enable */
}
