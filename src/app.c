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
    APP_Print((PBYTE)fmt);
}

#elif defined(WARNING_PRINT)

void ERR_Print(const char *fmt, ...)
{
    APP_Print("ERR:");
    APP_Print((PBYTE)fmt);
}
void WAR_Print(const char *fmt, ...)
{
    APP_Print("WAR:");
    APP_Print((PBYTE)fmt);
}

#elif defined(INFO_PRINT)

void ERR_Print(const char *fmt, ...)
{
    APP_Print("ERR:");
    APP_Print((PBYTE)fmt);
}
void WAR_Print(const char *fmt, ...)
{
    APP_Print("WAR:");
    APP_Print((PBYTE)fmt);
}
void INFO_Print(const char *fmt, ...)
{
    APP_Print((PBYTE)fmt);
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
    UINT16 key;
    UINT16 key_test = KEY_RELEASED;
    BYTE key_status = KEY_UNPRESSED;
    UART_Init();
    
    /* all pins are for output, except PD2(INT0)(key1) and PD3(key2) and PD4(key3) and PD5(key4)
    DDRD=0xc3;*/
    DDRD = 0xff;
    DDRD &= ~(1 << PD2);
    DDRD &= ~(1 << PD3);
    DDRD &= ~(1 << PD4);
    DDRD &= ~(1 << PD5);
    PORTD=0xff;

    /* all pins are for output, except PB0(ICP1), PB4(key5) and PB5(key6)
    DDRB=0xce;*/
    DDRB = 0xff;/* In face, the PB6(X1) and PB7(X2) are input when using the external crystal */
    DDRB &= ~(1 << PB0);
    DDRB &= ~(1 << PB4);
    DDRB &= ~(1 << PB5);
    PORTB=0xff;
    
    /* all pins are for output */
    DDRC=0xff;PORTC=0xff;

    UART_Print(APPLICATION_NAME);
    UART_Print(PLATFORM);
    UART_Print(VENDOR);
    UART_Print(APPLICATION_VERSITON);   
                               
    External_Int0_Init();
    
    Timer1_Init(INPUT_CAPTURE_ENABLE);
    
    /* To generate the carrier waveform */     
    Timer2_Init();
    
    /*Flash init */

    /*Wait for the input wave and save the elapsed time into Flash*/
    while(1)
    {  
        /* Check if there are any keys are pressed */
        key = PIND;
        key = key<<8;
        key = key | PINB;
        /* According to the SUEY EVB, it will be 0 when the key is pressed */
        key = ~key;
        
        if(KEY_PRESSED == key_status)
        {
            /* There is a weird problem, if using immediate value for AND by bit, I cant get the correct result 
            I debug it by IAR simulator but cant get the root cause
            Maybe the KEY_RELEASED is out fo range and cant put into one instruction
            And the compiler need split KEY_RELEASED to two parts and AND twice and then OR once 
            But the compiler just AND the first part and then OR the last part
            I dont know why but the variable can work */
            if((key & key_test) == 0)
            {
                INFO_Print("key released");
                key_status = KEY_UNPRESSED;
            }
        }
        else if(KEY_UNPRESSED == key_status)
        {
            if(key & KEY_1_)
            {
                INFO_Print("key 1 pressed");
                key_status = KEY_PRESSED;
            }
            else if(key & KEY_2_)
            {
                INFO_Print("key 2 pressed");
                key_status = KEY_PRESSED;
            }
            else if(key & KEY_3_)
            {
                INFO_Print("key 3 pressed");
                key_status = KEY_PRESSED;
            }
            else if(key & KEY_4_)
            {
                INFO_Print("key 4 pressed");
                key_status = KEY_PRESSED;
            }
            else if(key & KEY_5_)
            {
                INFO_Print("key 5 pressed");
                key_status = KEY_PRESSED;
            }
            else if(key & KEY_6_)
            {
                INFO_Print("key 6 pressed");
                key_status = KEY_PRESSED;
            }
            /*else
                ERR_Print("key check err");*/
        }
        else
            ERR_Print("key status err");
    }
}
