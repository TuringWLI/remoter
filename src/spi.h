/*******************************************************************
*
*         Copyright (c) 2016 by Whileloop, Inc. (WLI)
*
*  All rights are reserved. Reproduction or in part is prohibited
*  without the written consent of the copyright owner.
*
*  spi.h
*  This file contains the interface of SPI.
*
*******************************************************************/
#ifndef _SPI_H_
#define _SPI_H_

/*
**  Constants for SpiInit()
*/
#define     SPI_DATA_ORDER_MSB_FIRST    0x00
#define     SPI_DATA_ORDER_LSB_FIRST    0x40
#define     SPI_CLOCK_POLARITY          0x08     
#define     SPI_CLOCK_PHASE             0x04     
#define     SPI_CLOCK_RATE_DIV4         0x00
#define     SPI_CLOCK_RATE_DIV16        0x01
#define     SPI_CLOCK_RATE_DIV64        0x02
#define     SPI_CLOCK_RATE_DIV128       0x03

void SpiInit(void);
void SpiDeInit(void);
void SpiWriteByte(BYTE);
BYTE SpiReadByte(void);
BYTE SpiReadDataReg(void);
#endif /*_SPI_H_*/
