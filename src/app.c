/***************************************************************************
* app.c                                                    Version 0.0.1
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
#include "sst25vf.h"
#include "spi.h"

BYTE key_value = NO_KEY_;
BYTE key_value_index = NO_KEY_;
BYTE remoter_status = REMOTER_IDLE;/* !THIS GLOBAL VARIABLE IS VERY IMPORTANT, IT CONTROLS THE REMOTER SYSTEM RUNNING ! */

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
void INFO_Value_Print(UINT16 num)
{
    UART_PrintUInt(num);
}
#endif

#pragma interrupt_handler External_Int1_ISR:3
void External_Int1_ISR(void)
{
    /* tightly associated with hardware, only for REMOTER_EVB key board */      
    BYTE key;
#ifndef REMOTER_EVB_KEY_INTR    
    key_value = PIND;
    key_value = key_value & 0xC0;  /* only remain bit 7 and bit 6 for key s5 and key s4 */  
    key = PINB;
    key = key & 0x07; /* only remain bit 0 and bit 1and bit 2 for key s1 and key s2 and key s3 */  
    key_value = key_value | key;    
    key = PINC;
    key = key & 0x04; /* only remain bit 2for key REC */  
    key = key<<1;
    key_value = key_value | key;    
    /*Now the key_value like this: |PD7|PD6|x|x|PC2|PB2|PB1|PB0|*/  
    /* According to the REMOTER_EVB, it will be 0 when the key is pressed */
    key_value = ~key_value;
#else
    key = PINC;
    key = ~key;
    if(key & 0x04)
    {
        key_value = KEY_REC;
        return;
    }
    
    key = PINB;
    key = ~key;
    if(key & 0x01)
    {
        key_value = KEY_S1;
        return;
    }
    if(key & 0x02)
    {
        key_value = KEY_S2;
        return;
    }
    if(key & 0x04)
    {
        key_value = KEY_S3;
        return;
    }
    
    key = PIND;
    key = ~key;
    if(key & 0x40)
    {
        key_value = KEY_S4;
        return;
    }
    if(key & 0x80)
    {
        key_value = KEY_S5;
        return;
    }
    
    key_value = NO_KEY_;
#endif
}

void External_Int1_DeInit(void)
{
    GICR &= ~(1 << INT1);
}

/* Initialize the external interrupt to occur for low level which indicates a key pressed */
void External_Int1_Init(void)
{
    asm("cli"); /* disable all interrupt */

    /* MCUCR: power mode and external interrupt 0&1 sense control */
    MCUCR=(1<<ISC11);
    /*ISC1[1:0](bit 1:0): Interrupt 1 Sense control
    00: low level
    01: any logical change 
    10: falling edge
    11: rising edge
    */
    
    /* GICR: gerneral interrupt control register */
    GICR |= (1<<INT1);
    /*INT0: External Interrupt Request 0 Enable
    */
    
    asm("sei");/* Global interrupt enable */
}

void LED_TurnOn(void)
{
    PORTD &= ~(1 << PD5);
    /*INFO_Print("ON");*/
}

void LED_TurnOff(void)
{
    PORTD |= (1 << PD5);
    /*INFO_Print("OFF");*/
}

void LED_Flash(BYTE time)
{
    BYTE i;
    for(i = time; i > 0; i--)
    { 
        LED_TurnOn();
        delay_1ms(300);
        LED_TurnOff();
        delay_1ms(300);
    }
}

void Key_Function(void)
{
    BYTE cnt;
    if(remoter_status == REMOTER_IDLE)
    {            
        remoter_status = REMOTER_SENDING;/* The status will be REset in T1 ISR when send over */
        INFO_Print("sending key");
        INFO_Value_Print(key_value_index);
#if USE_FLASH       
        SpiInit(); 
        sst25vf_ReadDevice( key_value_index* FLASH_SECTOR_SIZE, (PBYTE)wave_time, WAVE_BUFFER_SIZE);
        SpiDeInit(); 
#endif        
        /* WAIT FOR THE HARDWARE READY 
         delay_1ms(5000);*/

        /* get REC data from flash
        enable T1 overflow interrupt 
        enable T2 CTC mode
        control T2 according to the REC data in T1 ISR and send the waveform 
        disable T2 in T1 ISR
        disable T1 in T1 ISR*/
        capture_pnt = 0;
#if PRINT_REC_VALUE
        for(cnt = 0; cnt < WAVE_CHANGE_NUM; cnt++)
            INFO_Value_Print(wave_time[cnt]);
#endif
        Timer2_CTCStart();
        Timer1_TimeStart(wave_time[capture_pnt]);
    }
    else if(remoter_status == REMOTER_REC)
    {
        LED_Flash(1);
        LED_TurnOn();
        remoter_status = REMOTER_LEARNING;/* The status will be REset after LEARNING_TIME and save REC data into flash */
        INFO_Print("learning key");
        capture_pnt = 0;
        for(cnt = 0; cnt < WAVE_CHANGE_NUM; cnt++)
            wave_time[cnt] = 0;
        
    #ifdef REMOTER_EVB_INP_CAP
        /* enable INT0 falling edge interrupt 
        enable T1 input capture interrupt in INT0 ISR
        record REC data in T1 ISR
        disable INT0 falling edge interrupt in INT0 ISR
        disable T1 input capture interrupt after LEARNING_TIME*/
        External_Int0_Init();/* Timer1_CaptureStart in INT0's ISR and it disble itself */
    #else
        /* enable INT0 falling edge interrupt 
        enable T1 normal mode in INT0 ISR
        record REC data in INT0 ISR except first INT0 interrupt
        disable INT0 falling edge interrupt after LEARNING_TIME
        disable T1 after LEARNING_TIME*/
        waveform_start = 1;
        External_Int0_Init();/* Timer1 normal mode start count in INT0's ISR */
    #endif
    }
    else
        remoter_status = REMOTER_IDLE;
}

BYTE Flash_Test(void)
{
    BYTE sst_test = 0x78;
    
    /* Flash init and check */
    SpiInit(); 
    sst25vf_DetectChip();
    sst25vf_ReadDeviceStatus();
    sst25vf_WriteDeviceStatus(2);/* According to the Status Register defined in the spec of 016B, '2' means write enable and no block protected */
    sst25vf_ReadDeviceStatus();

    sst25vf_EraseSector(FLASH_SECTOR_INDEX_TEST);
    sst25vf_ProgramDevice(FLASH_SECTOR_INDEX_TEST * FLASH_SECTOR_SIZE, &sst_test, 1);
    sst_test = 0;
    sst25vf_ReadDevice(FLASH_SECTOR_INDEX_TEST * FLASH_SECTOR_SIZE, &sst_test, 1);
    SpiDeInit();
    
    if(sst_test == 0x78)
        return 1;
    else
        return 0;
}

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
#ifdef SUEY_EVB    
    UINT16 key;
    UINT16 key_test = KEY_RELEASED;
    BYTE key_status = KEY_UNPRESSED;
#endif

#ifdef REMOTER_EVB    
    #ifndef REMOTER_EVB_KEY_INTR    
        UINT16 key_test = KEY_RELEASED;
        BYTE key_status = KEY_UNPRESSED;
    #endif
#endif

    UART_Init();

    /* GPIO summary here */
#ifdef SUEY_EVB    
    /* Polling the key, without interrupt */
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
    DDRB = 0xff;/* In fact, the PB6(X1) and PB7(X2) are input when using the external crystal */
    DDRB &= ~(1 << PB0);
    DDRB &= ~(1 << PB4);
    DDRB &= ~(1 << PB5);
    PORTB=0xff;
    
    /* all pins are for output */
    DDRC=0xff;PORTC=0xff;
#endif

#ifdef REMOTER_EVB    /* GPIO config for key board and input waveform record and output waveform and SPI to flash */
    /* all pins are for output, except PD2(INT0) and PD3(INT1(key)) and PD6(key S4) and PD7(key S5) */
    DDRD = 0xff;
    DDRD &= ~(1 << PD2);
    #ifdef REMOTER_EVB_KEY_INTR    
        DDRD &= ~(1 << PD3);
    #endif
    DDRD &= ~(1 << PD6);
    DDRD &= ~(1 << PD7);
    PORTD=0xff;
    #ifndef REMOTER_EVB_KEY_INTR    
        /*all pins are high level, except PD3(INT1) */
        PORTD &= ~(1 << PD3); 
    #endif

    /* all pins are for output, such as PB3(OC2, MOSI) and PB5(SCK), except PB0(key S1)(ICP1), PB1(key S2) and PB2(key S3) and PB4(MISO) */
    DDRB = 0xff;/* In fact, the PB6(X1) and PB7(X2) are input when using the external crystal */
    DDRB &= ~(1 << PB0);
    DDRB &= ~(1 << PB1);
    DDRB &= ~(1 << PB2);
    DDRB &= ~(1 << PB4);
    PORTB=0xff;
    PORTB &= ~(1 << PB5);/* For data mode 0 of SPI */
    
    /* all pins are for output, such as PC1(/SS), except PC2(key REC) */
    DDRC=0xff;
    DDRC &= ~(1 << PC2);
    PORTC=0xff;
#endif

    UART_Print(APPLICATION_NAME);
    UART_Print(PLATFORM);
    UART_Print(VENDOR);
    UART_Print(APPLICATION_VERSITON);   

#ifdef REMOTER_EVB
    LED_Flash(3);
    #ifdef REMOTER_EVB_KEY_INTR    
        External_Int1_Init();/* For key detect */
    #endif
#endif

#if USE_FLASH       
    i = Flash_Test();
    if(i)
        ERR_Print("Flash access OK");
    else
        ERR_Print("Flash access Failed");
#endif

#ifdef REMOTER_EVB
#ifdef REMOTER_EVB_KEY_INTR
    while(1)/* Waiting for the key value set in ISR */
    {
        switch(key_value)
        {
            case KEY_REC:
                    LED_TurnOn();
                    remoter_status = REMOTER_LEARNING;
                    key_value = NO_KEY_;
                    while(remoter_status)
                    {
                        switch(key_value)
                        {
                            case KEY_REC:
                                LED_TurnOff();
                                remoter_status = REMOTER_IDLE;
                                break;
                                
                            case KEY_S1:
                            case KEY_S2:
                            case KEY_S3
                            case KEY_S4:
                            case KEY_S5:
                                LED_TurnOff();
                                delay_1ms(100);
                                LED_TurnOn();
                                /* wait the wave input and record it */
                                /* save the data into SROM */
                                remoter_status = REMOTER_LEARNDONE;
                                break;
                                
                            default:
                                break;
                        }
                    }
                break;
                
            case KEY_S1:
            case KEY_S2:
            case KEY_S3:
            case KEY_S4:
            case KEY_S5:
                /* get data from SROM and send wave */
                key_value = NO_KEY_;
                break;
                
            default:
                ERR_Print("key check err");
                if(remoter_status == REMOTER_ERR)
                    LED_Flash(1);
                break;
        }
    }
#else
    while(1)/* Polling the keyboard */
    {  
        /* Check if there are any keys are pressed */
        External_Int1_ISR();     
        
        if(KEY_PRESSED == key_status)
        {
            if(remoter_status == REMOTER_LEARNING)
            {
                remoter_status = REMOTER_LEARNDONE;
                while(waveform_start); /*wait for the wave form start */
                delay_1ms(LEARNING_TIME);
                LED_TurnOff();
                /* After LEARNING_TIME, remoter_status will be err or not */
            }
            
            if(remoter_status == REMOTER_LEARNDONE)
            {
                #ifdef REMOTER_EVB_INP_CAP
                    Timer1_CaptureStop();
                #else
                    Timer1_CntStop();
                #endif
                    External_Int0_DeInit();
 #if PRINT_REC_VALUE               
                if(capture_pnt)/* Print all data which captured */
                    for(i = 0; i < WAVE_CHANGE_NUM; i++)
                    {
                        INFO_Value_Print(wave_time[i]);
                    }
#endif
                /* save the record data */
                for(i = 0; i < WAVE_CHANGE_NUM && wave_time[i] != 0; i++)
                    wave_time[i] = MAX_TIME_VALUE - wave_time[i];
#if USE_FLASH       
                SpiInit(); 
                sst25vf_EraseSector(key_value_index);
                sst25vf_ProgramDevice(key_value_index* FLASH_SECTOR_SIZE, (PBYTE)wave_time, WAVE_BUFFER_SIZE);
                SpiDeInit(); 
#endif       

 #if PRINT_REC_VALUE               
                if(capture_pnt)/* Print all data which captured */
                    for(i = 0; i < WAVE_CHANGE_NUM; i++)
                    {
                        INFO_Value_Print(wave_time[i]);
                    }
#endif

                remoter_status = REMOTER_SAVEDONE;
            }

            if(remoter_status == REMOTER_SENDDONE)
            {
                LED_TurnOff();
                remoter_status = REMOTER_IDLE;
            }
                
            if((remoter_status == REMOTER_IDLE)||(remoter_status == REMOTER_REC))
            {
                /* There is a weird problem, if using immediate value for AND by bit, I cant get the correct result 
                I debug it by IAR simulator but cant get the root cause
                Maybe the KEY_RELEASED is out fo range and cant put into one instruction
                And the compiler need split KEY_RELEASED to two parts and AND twice and then OR once 
                But the compiler just AND the first part and then OR the last part
                I dont know why but the variable can work */
                if((key_value & key_test) == 0)
                {
                    INFO_Print("key released");
                    key_status = KEY_UNPRESSED;
                }
            }
        }
        else if(KEY_UNPRESSED == key_status)
        {
            if(key_value & KEY_S1_) 
            {
                key_value_index = KEY_S1;
                INFO_Print("key 1 pressed");
                Key_Function();
                key_status = KEY_PRESSED;
            }        
            else if(key_value & KEY_S2_)
            {
                key_value_index = KEY_S2;
                INFO_Print("key 2 pressed");
                Key_Function();
                key_status = KEY_PRESSED;
            }        
            else if(key_value & KEY_S3_)
            {
                key_value_index = KEY_S3;
                INFO_Print("key 3 pressed");
                Key_Function();
                key_status = KEY_PRESSED;
            }        
            else if(key_value & KEY_S4_)
            {
                key_value_index = KEY_S4;
                INFO_Print("key 4 pressed");
                Key_Function();
                key_status = KEY_PRESSED;
            }        
            else if(key_value & KEY_S5_) 
            {
                key_value_index = KEY_S5;
                INFO_Print("key 5 pressed");
                Key_Function();
                key_status = KEY_PRESSED;
            }        
            else if(key_value & KEY_REC_)
            {
                if(remoter_status == REMOTER_REC)
                {
                    LED_TurnOff();
                    remoter_status = REMOTER_IDLE;
                }
                else if(remoter_status == REMOTER_IDLE)
                {
                    LED_TurnOn();
                    remoter_status = REMOTER_REC;
                }
                else
                    remoter_status = REMOTER_IDLE;
                key_status = KEY_PRESSED;
            }
            else
            {                
                /*ERR_Print("key check err");*/
                if(remoter_status == REMOTER_ERR)
                    LED_Flash(1);
            }
        }
        else
            ERR_Print("key status err");
    }
#endif
        /* Enter power down mode */
#endif    
    
#ifdef SUEY_EVB    
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
#endif    
}
