/*******************************************************************
 * 
 *         Copyright (c) 2010 by Silicon Motion, Inc. (SMI)
 * 
 *      All rights are reserved. Reproduction or in part is prohibited
 *  without the written consent of the copyright owner.
 * 
 *  sst25vf512a.h --- SM750/SM718 DDK 
 *  This file contains the definitions for the MX25L.
 *
 *      This driver is created to support SST25VF512A chip from SST 
 *  ( Silicon Storage Technology, Inc.) 
 * 
 *******************************************************************/

#ifndef _SST25VF_H_
#define _SST25VF_H_

/* SST25VF016B Commands */
#define SST25VF_SET_STATUS                              0x01
#define SST25VF_PROGRAM                                   0x02
#define SST25VF_READ_DATA                               0x03
#define SST25VF_WRITE_DISABLE                       0x04
#define SST25VF_GET_STATUS                              0x05
#define SST25VF_WRITE_ENABLE                        0x06
#define SST25VF_GET_DEVICE_ID                       0x90
#define SST25VF_ERASE_SECTOR                        0x20
#define SST25VF_ERASE_CHIP                              0xC7

#define SST25VF_FAST_READ_DATA                  0x0B
#define SST25VF_BLOCK_ERASE                         0x52
//#define SST25VF_POWER_DOWN                        0xB9
//#define SST25VF_POWER_UP                              0xAB
#define SST25VF_SET_STATUS_ENABLE               0x50
#define SST25VF_ATUO_ADDRESS_INCREMENT    0xAF

/* SST25VF016B Chip ID */
#define SST25VF_MANUFACTURER_ID                 0xBF
#define SST25VF_DEVICE_ID                               0x41

/* Status Register Definition */
#define SST25VF_STATUS_REGISTER_PROTECT       0x80
#define SST25VF_ATUO_ADDRESS_INCREMENT_STATUS 0x40
#define SST25VF_BLOCK_PROTECT_BITS_1          0x08
#define SST25VF_BLOCK_PROTECT_BITS_0          0x04
#define SST25VF_WRITE_ENABLE_LATCH            0x02
#define SST25VF_STATUS_BUSY                             0x01

#define SST25VF_STATUS_REGISTER_PROTECT_MASK  0x80
#define SST25VF_BLOCK_PROTECT_MASK                        0x0C
#define SST25VF_WAIT_TIMER 0xFFFF

#define SST25VF_CS_L() (PORTC &= ~(1 << PC1))
#define SST25VF_CS_H() (PORTC |= (1 << PC1))

void sst25vf_DetectChip(void);
BYTE sst25vf_ReadDeviceStatus(void);
void sst25vf_WriteDeviceStatus(BYTE newStatus);
void sst25vf_ReadDevice(UINT32 memoryAddress, PBYTE pData, UINT16 length);
BYTE sst25vf_ProgramDevice(UINT32 memoryAddress, PBYTE pData, UINT16 length);
void sst25vf_EraseSector(UINT16 sectorIndex);
void sst25vf_EraseChip(void);
#endif  /* _SST25VF_H_ */

