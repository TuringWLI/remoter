/***************************************************************************
* remoter.c                                                    Version 0.0.1
*
* Routines to run remoter main function.
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
#include "8bits_timer.h"

/*these functions just for debug*/
#if defined(ERROR_PRINT)

void ERR_Print(const char *fmt, ...)
{
    APP_Print("ERR:");
    APP_Print(fmt);
}

#elif defined(WARNING_PRINT)

void ERR_Print(const char *fmt, ...)
{
    APP_Print("ERR:");
    APP_Print(fmt);
}
void WAR_Print(const char *fmt, ...)
{
    APP_Print("WAR:");
    APP_Print(fmt);
}

#elif defined(INFO_PRINT)

void ERR_Print(const char *fmt, ...)
{
    APP_Print("ERR:");
    APP_Print(fmt);
}
void WAR_Print(const char *fmt, ...)
{
    APP_Print("WAR:");
    APP_Print(fmt);
}
void INFO_Print(const char *fmt, ...)
{
    APP_Print(fmt);
}
#endif


/* Before the download the code by ISP and run it
 It is necessary to set the Fuse Bits to config some parameters 
 Now the code running with 12M Hz external crystal 
 the Fuse Bits like this
 CKSEL0 = 1
 CKSEL1 = 1
 CKSEL2 = 1
 CKSEL3 = 1
 SUT1 = 1
 SUT2 = 1
 CKOPT = 0

 The default settings of Fuse Bits like this
 The low byte of Fuse Bits is 11100001(0xE1)
 The high byte of Fuse Bits is 11011001(0xD9)
 This settings is for the 1M Hz internal RC resonator
 */
void main(void)
{    
    BYTE i;

    DDRB=0xfb;PORTB=0xff;/* all pins are for output, except INT0*/
    DDRC=0xfe;PORTC=0xff;/* all pins are for output, except ICP1*/
    DDRD=0xff;PORTD=0xff;

    UART_Init();
    UART_Print(APPLICATION_NAME);
    UART_Print(PLATFORM);
    UART_Print(VENDOR);
    UART_Print(APPLICATION_VERSITON);   

    External_Int0_Init();
    Timer1_Init();
    /* To generate the carrier waveform */     
    Timer2_Init();

    while(1)
    {  
    }
}
