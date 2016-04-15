/***************************************************************************
* SST25VF.c                                                    Version 0.0.1
*
* Routines to access sst25vf016b(2MB).
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
#include "app.h"
#include "sst25vf.h"
#include "spi.h"

/*
 * Enable Write
 */
void sst25vfWriteEnable(void)
{
    SST25VF_CS_L();/* pull low the slave select(/SS) for start */
    SpiWriteByte(SST25VF_WRITE_ENABLE);
    SST25VF_CS_H();/* pull high the slave select(/SS) for end */
}

/*
 * Wait for the device to be ready
 * 
 * Output:
 *      0   - Device is ready
 *     1   - Device is not ready
 */
BYTE sst25vfWaitDeviceReady(void)
{
    UINT16 timeout = SST25VF_WAIT_TIMER;
    BYTE status;
    
    /* Check the ready status by sending the read status command */
    while (timeout--)
    {
        status = sst25vf_ReadDeviceStatus();
        if ((status & SST25VF_STATUS_BUSY) == 0)
        {   
            /*INFO_Print("Flash idle");*/
            return 0;
        }
    }

    ERR_Print("Flash busy");
    return (1);
}

/*
 * Read the device status
 *
 * Input:
 *     
 */
BYTE sst25vf_ReadDeviceStatus(void)
{    
    BYTE status;

    SST25VF_CS_L();/* pull low the slave select(/SS) for start */
    SpiWriteByte(SST25VF_GET_STATUS);
    status = SpiReadByte();
    //INFO_Value_Print(status);
    SST25VF_CS_H();/* pull high the slave select(/SS) for end */
    
    return status;
}


/*
 * Writes the device status
 *
 * Input:
 *      newStatus   - status to be written to the device
 */
void sst25vf_WriteDeviceStatus(
    BYTE newStatus
)
{    
    SST25VF_CS_L();/* pull low the slave select(/SS) for start */
    /* Enable Device Write */
    SpiWriteByte(SST25VF_SET_STATUS_ENABLE);/* must do write status register enable first */
    SST25VF_CS_H();/* pull high the slave select(/SS) for end */
    
    SST25VF_CS_L();/* pull low the slave select(/SS) for start */
    /* Send command */
    SpiWriteByte(SST25VF_SET_STATUS);
    SpiWriteByte(newStatus);
    SST25VF_CS_H();/* pull high the slave select(/SS) for end */

    /* Wait device ready */
    sst25vfWaitDeviceReady();
}

/*
 * Reads the device ID
 *
 *      Note:
 *              The first byte is Manufacture ID
 *              The second  byte device ID
 *
 *      This function returns the manufacture ID and the individual Device ID
 */
void sst25vf_DetectChip(void)
{
    BYTE mfgID, deviceID;
        
    SST25VF_CS_L();/* pull low the slave select(/SS) for start */
    
    /* Send Read ID Command and 2 Dummy data.
       The dummy data is used to clock in the 2-bytes ID response from the SST25VF016B */
    SpiWriteByte(SST25VF_GET_DEVICE_ID);
    SpiWriteByte(0x00);/* Address */
    SpiWriteByte(0x00);/* Address */
    SpiWriteByte(0x00);/* Address, get the manufacturer ID first */
    mfgID = SpiReadByte();
    deviceID = SpiReadByte();
    
    SST25VF_CS_H();/* pull high the slave select(/SS) for end */
    
    INFO_Print("MID");
    //INFO_Value_Print(mfgID);
    INFO_Print("DID");
    //INFO_Value_Print(deviceID);
}

/*
 * Reads device data from the given memory address
 *
 * Input:
 *      memoryAddress   - Memory address to be read
 *      pData           - Pointer to a data storage to store the
 *                        data value that have been read.
 *      length          - Length of the data pointer in bytes. 
 *
 * Output:
 *      Total number of data read
 */
void sst25vf_ReadDevice(
    UINT32 memoryAddress,
    PBYTE pData, 
    UINT16 length
)
{       
    INFO_Print("Read");
    INFO_Value_Print(memoryAddress);
    INFO_Value_Print(length);
    
    SST25VF_CS_L();/* pull low the slave select(/SS) for start */
    SpiWriteByte(SST25VF_READ_DATA); /* Send CMD */
    SpiWriteByte((BYTE)(memoryAddress>>16));/* Set the memory address */
    SpiWriteByte((BYTE)(memoryAddress>>8));
    SpiWriteByte((BYTE)(memoryAddress));
    while (length > 0)
    {
        /* Reading the device. Since this driver is using 8-bit data size,
        therefore, the length is not adjusted. For bigger data size,
        please do not forget to adjust the length since the length is
        specified in bytes. */
        *pData = SpiReadByte();
        //INFO_Value_Print(memoryAddress);
        //INFO_Value_Print(*pData);
        pData++;
        /* Adjust the memory address */
        memoryAddress ++;
        /* Adjust the dataLength */
        length --;
    }
    SST25VF_CS_H();/* pull high the slave select(/SS) for end */
}

/*
 * Program the device
 *
 * Input:
 *      memoryAddress   - Memory Starting Address to be programmed
 *      pData           - Pointer to a data to be programmed
 *      length          - Length of the data to be written to memory in byte
 *
 * Output:
 *      0   - Success
 *     1   - Fail
 */
BYTE sst25vf_ProgramDevice(
    UINT32 memoryAddress,
    PBYTE pData, 
    UINT16 length
)
{
    INFO_Print("Program");
    INFO_Value_Print(memoryAddress);
    INFO_Value_Print(*pData);
    INFO_Value_Print(length);
    
    /* Programming the device */
    while (length > 0)
    {
        /* Write Enable */
        sst25vfWriteEnable();
        
        SST25VF_CS_L();/* pull low the slave select(/SS) for start */
        SpiWriteByte(SST25VF_PROGRAM); /* Send CMD *//* Byte program command */
        SpiWriteByte((BYTE)(memoryAddress>>16));/* Set the memory address */
        SpiWriteByte((BYTE)(memoryAddress>>8));
        SpiWriteByte((BYTE)(memoryAddress));
        /* Setup the data. Do not forget to cast the data
        as unsigned short when the ssp data size is 8-bit
        or less. */
        SpiWriteByte((BYTE)(*pData));
        SST25VF_CS_H();/* pull high the slave select(/SS) for end */
        
        //INFO_Value_Print(memoryAddress);
        //INFO_Value_Print(*pData);
        
        pData++;

        /* Wait Device Ready */
        if (sst25vfWaitDeviceReady() != 0)
            return (1);

        /* Adjust the memory address */
        memoryAddress ++;

        /* Adjust the dataLength */
        length --;
    }
    
    return 0;
}

/*
 * Erases a sector
 *
 * Input:
 *      sectorIndex - Sector to be erased
 */
void sst25vf_EraseSector(
    UINT16 sectorIndex
)
{
    BYTE add_h, add_l;
    INFO_Print("Erase Sector");
    /* Write Enable */
    sst25vfWriteEnable();

    /* Calculate the real sector address. */
    add_l = ((BYTE)sectorIndex) << 4;/* (BYTE) will get the lower byte in the word */
    add_h = (BYTE) (sectorIndex >> 4);
    
    SST25VF_CS_L();/* pull low the slave select(/SS) for start */
    SpiWriteByte(SST25VF_ERASE_SECTOR);/* Send CMD */
    SpiWriteByte(add_h);/* 1 sector = 4Kb */
    SpiWriteByte(add_l); /* The lowest 12 bits are 0 */
    SpiWriteByte(0);/* The lowest 12 bits are 0 */
    SST25VF_CS_H();/* pull high the slave select(/SS) for end */
    
    /* Wait Device Ready */
    sst25vfWaitDeviceReady();
}

/*
 * Erases the whole data on the chip
 */
void sst25vf_EraseChip(void)
{
    /* Write Enable */
    INFO_Print("Erase Chip");
    sst25vfWriteEnable();    
    
    SST25VF_CS_L();/* pull low the slave select(/SS) for start */
    SpiWriteByte(SST25VF_ERASE_CHIP);
    SST25VF_CS_H();/* pull high the slave select(/SS) for end */

    /* Wait Device Ready */
    sst25vfWaitDeviceReady();

}
