/***************************************************************************
* uart.c                                                    Version 0.0.1
*
* Routines to run UART function.
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
#include <AVRdef.h>
#include "def.h"
#include "dbg.h"
#include "uart.h"

/* This is not a accurate timer, just for delay a while */
void delay_1ms(UINT16 i)
{  
    UINT16 j;
    
    for(;i>0;i--)
        for(j=667;j>0;j--);
}

/*ISR for receive */
#pragma interrupt_handler USART_R:12
void USART_R(void)
{  

}

/* ISR for transmit */
#pragma interrupt_handler USART_T:14
void USART_T(void)
{ 

}

void UART_PutChar(BYTE fmt)
{
    /* Wait for empty transmit buffer */
    while ( !( UCSRA & (1<<UDRE)) )
    ;
    /* Put data into buffer, sends the data */
    UDR = fmt;
}

/*print the string through UART
and the length of the string must be less than 255*/
void UART_Print(PBYTE fmt)
{
    BYTE i;    
    
    for(i = 0; fmt[i] != '\0'; i++)
    {
        UART_PutChar(fmt[i]);
    }
    
    UART_PutChar('\r');
    UART_PutChar('\n');
}

void UART_PrintUInt(UINT16 num)
{
    BYTE num_str[6] = {'\0','\0','\0','\0','\0','\0'};
    BYTE num0;
    
    if((num>=10000))
    {
        num0 = num/10000;
        num_str[0] = num0 + '0';

        num  = num - num0*10000; 
        num0 = num/1000 ;
        num_str[1] = num0 + '0';
        
        num  = num - num0*1000; 
        num0 = num/100 ;
        num_str[2] = num0 + '0';
        
        num  = num - num0*100; 
        num0 = num/10 ;
        num_str[3] = num0 + '0';
        
        num  = num - num0*10; 
        num_str[4] = num + '0';
    }        
    else if ((num>=1000))
    {
        num0 = num/1000 ;
        num_str[0] = num0 + '0';
        
        num  = num - num0*1000; 
        num0 = num/100 ;
        num_str[1] = num0 + '0';
        
        num  = num - num0*100; 
        num0 = num/10 ;
        num_str[2] = num0 + '0';
        
        num  = num - num0*10; 
        num_str[3] = num + '0';
    }        
    else if ((num>=100))
    {
        num0 = num/100 ;
        num_str[0] = num0 + '0';
        
        num  = num - num0*100; 
        num0 = num/10 ;
        num_str[1] = num0 + '0';
        
        num  = num - num0*10; 
        num_str[2] = num + '0';
    }        
    else if ((num>=10))
    {
        num0 = num/10 ;
        num_str[0] = num0 + '0';
        
        num  = num - num0*10; 
        num_str[1] = num + '0';
    }        
    else
    {
        num_str[0] = num + '0';
    }    
    
    for(num0 = 0; num_str[num0] != '\0'; num0++)
    {
        UART_PutChar(num_str[num0]);
    }
    
    UART_PutChar('\r');
    UART_PutChar('\n');
}

int UART_Printf(const char *fmt, ...)
{
    return 0;
}
int UART_Scanf(const char *fmt, ...)
{
    return 0;
}

void UART_Init(void)
{   
    /* disable all interrupt */
    CLI();
    
    UCSRB=(0<<RXCIE)|(0<<TXCIE)|(1<<RXEN)|(1<<TXEN);
    /*RXCIE(bit 7): RX Complete Interrupt Enable
    TXCIE(bit 6): TX Complete Interrupt Enable
    RXEN(bit 4): Receiver Enable          
    TXEN(bit 3): Transmitter Enable
    UCSZ2(bit 2): Character Size, combined with the UCSZ1:0 in UCSRC sets the number of data bits (Character Size) in the frame the Receiver and Transmitter use
    */
    
    UCSRC=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);//|(1<<UPM1)|(1<<UPM0)
    /*URSEL(bit 7): UCSRC shares the same I/O location as the UBRRH, set this bit (Register Select) when writing the UCSRC
    UMSEL(bit 6): USART Mode Select, 0: Asynchronous, 1: Synchronous
    UPM[1:0](bit 5:4): Parity Mode, 00: Disable, 10:Even Parity, 11: Odd Parity
    USBS(bit 3): Stop Bit Select
    UCSZ[1:0](bit 2:1): Character Size
    */
    
    /* set baud rate */
    UBRR=(CLOCK/(16*BAUD)-1);
    
    /* enable all interrupt, the same function with SREG|=0x80 */
    SEI();
}

