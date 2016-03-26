/*******************************************************************
*
*         Copyright (c) 2015 by Whileloop, Inc. (WLI)
*
*  All rights are reserved. Reproduction or in part is prohibited
*  without the written consent of the copyright owner.
*
*  Dbg.H
*  This file contains the definitions for uart.
*
*******************************************************************/
#ifndef _UART_H_
#define _UART_H_

/*this clock is the system clock
if it is 1000000, the workable baud is 4800
if use the 12MHz external crystal, the workable baud is from 4800~57600 */
#define CLOCK 12000000 /* System clock */
#define BAUD 57600 /* Baud rate */
void delay_1ms(UINT16 i);

void UART_Init(void);
void UART_Print(char *fmt);
void UART_PrintUInt(UINT16 num);

int UART_Printf(const char *fmt, ...);
int UART_Scanf(const char *fmt, ...);

#endif /*_UART_H_*/

