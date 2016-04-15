/***************************************************************************
* spi.c                                                    Version 0.0.1
*
* Driver for SPI bus.
* In remoter project, using SPI to connect a SROM for storing data.
* Currently supported:  Atmega8
*
* Copyright (c) 2005-2016 Whileloop Inc.
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
#include "spi.h"

/*
**  SpiInit() initializes the SPI as a master, enable it
**  and set it up as requested by parameter Controlregister.
**  See "spi.h" for availible parameter values, they could
**  be ORed together. See AVR SPI documentation for more 
**  detailed information.
*/
void SpiInit(void)
{
    /*
    SPI interrupt disable 
    SPI enable
    the MSB of the data word is transmitted first
    Master
    Data mode 0
    Clock: fosc/16
    */
    INFO_Print(" SPI in");
    SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);    // Write to control register.
}

/*
**  SpiWriteByte() writes a byte to the SPI and waits until
**  it has been transmitted. This function doesn't
**  return any value back.
*/
void SpiWriteByte(BYTE byte)
{
    SPDR = byte;
    while (!(SPSR & (1 << SPIF)));
}

/*
**  SpiReadByte() first writes a byte (a dummy, since
**  that byte is to generate clock signals to "poll" home
**  the byte from the slave. The function returns the
**  received byte.
*/
BYTE SpiReadByte(void)
{
    SPDR = 0x00;
    while (!(SPSR & (1 << SPIF)));
    return SPDR;
}

/*
**  SpiReadDataReg() reads the last byte in the SPI register
**  without any clock signals generated. Could be used
**  as reading the last byte received.
*/
BYTE SpiReadDataReg(void)
{
    return SPDR;
}

/**/
void SpiDeInit(void)
{
    /*
    SPI interrupt disable 
    SPI enable
    the MSB of the data word is transmitted first
    Master
    Data mode 0
    Clock: fosc/16
    */
    SPCR =  (1<<MSTR) | (1<<SPR0);    // Write to control register.
    INFO_Print(" SPI de");
#if 0    
    /* all pins are for output, such as PB3(OC2, MOSI) and PB5(SCK), except PB0(key S1)(ICP1), PB1(key S2) and PB2(key S3) and PB4(MISO) */
    DDRB = 0xff;/* In fact, the PB6(X1) and PB7(X2) are input when using the external crystal */
    DDRB &= ~(1 << PB0);
    DDRB &= ~(1 << PB1);
    DDRB &= ~(1 << PB2);
    DDRB &= ~(1 << PB4);
    PORTB=0xff;
    PORTB &= ~(1 << PB5);/* For data mode 0 of SPI */
#endif
}

