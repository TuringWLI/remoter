/*******************************************************************
*
*         Copyright (c) 2015 by Whileloop, Inc. (WLI)
*
*  All rights are reserved. Reproduction or in part is prohibited
*  without the written consent of the copyright owner.
*
*  Dbg.H
*  This file contains the definitions for data type.
*
*******************************************************************/
#ifndef _DEF_H_
#define _DEF_H_

#define ICCAVR

#ifdef ICCAVR
typedef unsigned char BYTE;            /* SIZE: 1 byte, RANGE: 0..255 */
typedef signed char SBYTE;             /* SIZE: 1 byte, RANGE: -128..127 */
typedef char * PBYTE;                  /* SIZE: 1 byte, RANGE: 0..255 */
typedef unsigned short USHORT16;       /* SIZE: 2 bytes, RANGE: 0..65535 */
typedef signed short SHORT16;          /* SIZE: 2 bytes, RANGE: -32768..32767 */
typedef unsigned int UINT16;           /* SIZE: 2 bytes, RANGE: 0..65535 */
typedef signed int INT16;              /* SIZE: 2 bytes, RANGE: -32768..32767 */
typedef unsigned long ULONG;           /* SIZE: 4 bytes, RANGE: 0..4294967295 */
typedef signed long LONG;              /* SIZE: 4 bytes, RANGE: -2147483648..2147483647 */
typedef unsigned long UINT32;          /* SIZE: 4 bytes, RANGE: 0..4294967295 */
typedef signed long INT32;             /* SIZE: 4 bytes, RANGE: -2147483648..2147483647 */
typedef float FLOAT;                   /* SIZE: 4 bytes, RANGE: +/-1.175e-38..3.40e+38 */
typedef double DOUBLE;                 /* SIZE: 4 bytes, RANGE: +/-1.175e-38..3.40e+38 */
#endif

#endif /*_DEF_H_*/

