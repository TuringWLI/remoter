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

INT16 cnt = 0;
INT16 wave_time[WAVE_CHANGE_NUM];

#pragma interrupt_handler Timer1_Overflow_ISR:9
void Timer1_Overflow_ISR(void)
{
}

#pragma interrupt_handler Timer1_CaptureEvent_ISR:6
void Timer1_CaptureEvent_ISR(void)
{  
#if 0
    uchar i,j,k;
    uchar key_d[4]={0};//红外按键码：用户码1、用户码2、按键码、按键码反码
    
    TCNT1=0;/* Clear counter */
    ICR1=0;/* Clear input capture register */
    TIFR|=(1<<ICF1);/* Clear flag of input capture */
    
    while((TIFR&(1<<ICF1))==0)/* waiting for input capture */
    {
        asm("wdr");
        if(TIFR&(1<<TOV1))
                return;/* If the counter overflow, the input is noise, return */
    }
	
    if((ICR1>13000)&&(ICR1<15000))/*if getting the introduct code (9ms low Level + 4.5ms High Level) */
    {  
        for(i=0;i<4;i++)//共有四个8位码组成一个红外按键码
        	for(j=0;j<8;j++)
	        {  
	            key_d[i]=key_d[i]<<1;
	            TCNT1=0;/* Clear counter */
				ICR1=0;/* Clear input capture register */
	            TIFR|=(1<<ICF1);/* Clear flag of input capture */
	            while((TIFR&(1<<ICF1))==0)/* waiting for input capture */
	            { 
	                asm("wdr");
	                if(TIFR&(1<<TOV1))
	                    return;/* If the counter overflow, the input is noise, return */
	            }
	            if((ICR1>2100)&&(ICR1<2400))key_d[i]=key_d[i]|0x01;//信号1：0.565ms低+1.685ms高=2.25ms
	            //else if((ICR1>1100)&&(ICR1<1150))datt[i]=datt[i]&0xfe;//信号0：0.565ms低+0.65ms高=1.125ms
	        }
    }  
    cnt++;
    UART_PrintUInt(cnt);
#else
    INT16 tcc1b;

    wave_time[cnt] = ICR1;
    UART_PrintUInt(wave_time[cnt]);

    cnt++;

    /* Saving wave time finished and stop timer 1 */
    if( cnt >= WAVE_CHANGE_NUM )
    {
        TCCR1B &= 0xF8;/* Disable the input clock */
        cnt = 0;
    }

    TCNT1=0;/* Clear counter */
    ICR1=0;/* Clear input capture register */
    TIFR|=(1<<ICF1);/* Clear flag of input capture */

    /* change the Input Capture Edge Select */
    tcc1b = TCCR1B;
    TCCR1B &= 0xBF;/* clear ICES1 */
    tcc1b = ~tcc1b;
    tcc1b &= 0x40;/* only ICES1 */
    TCCR1B |= tcc1b;
#endif
}

#pragma interrupt_handler External_Int0_ISR:2
void External_Int0_ISR(void)
{
    /* External Interrupt Flag GIFR[7] is cleared when the interrupt routine is executed.
    Alternatively, the flag can be cleared by writing a logical one to it */
    cnt++;
    UART_PrintUInt(cnt);
    /* Enable Input Capture of timer 1 and start to count */
    /*Timer1_CaptureEvent_ISR();*/
}

/* Initialize the timer1 to operate the input capture for saving elapsed time or just counting for sending*/
void Timer1_Init(BYTE ic)
{
    asm("cli");    /* disable all interrupt */
	
    TCNT1=0;/* Clear counter */
    ICR1=0;/* Clear input capture register */

    /* Setup Input Captue Unit */
    TCCR1A=0x00;/* For Output Compare unit */
    TCCR1B=(1<<ICNC1)|(TIMER1_INPUT_CAPTURE_EDGE_SELECE_FALLING<<ICES1)|(TIMER1_CLOCK_SELECE_NO_PRESCALING<<CS10);
    /*ICNC1(bit 7): Input capture noise canceler
    ICES1(bit 6): Input Capture Edge Select
    CS12~CS10(bit 2:0): Clock Select 
    */
    
    /* TCCR1A and TCCR1B can define Mode of Operations, now is Normal Mode 
    in normal mode, the counting direction is always up and no counter clear is performed */
    
    if(INPUT_CAPTURE_ENABLE == ic)
        TIMSK=(1<<TICIE1);
        /*TICIE1(bit 5): Input Capture Interrupt Enable */

    TIFR=(1<<ICF1); 
    /*ICF1(bit 5): clear the flag which will be set when a capture event occurs on the ICP1 pin 
    ICF1 is automatically cleared when the Input Capture Interrupt Vector is executed.
    Alternatively, the flag can be cleared by writing a logical one to its bit location */
    
    asm("sei");
}

/* Initialize the external interrupt to occur for falling edge */
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
