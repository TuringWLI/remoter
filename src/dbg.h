/*******************************************************************
*
*         Copyright (c) 2015 by Whileloop, Inc. (WLI)
*
*  All rights are reserved. Reproduction or in part is prohibited
*  without the written consent of the copyright owner.
*
*  Dbg.H
*  This file contains the definitions for debugging control and print selection.
*
*******************************************************************/
#ifndef _DBG_H_
#define _DBG_H_

/*#define RELEASE*/

#ifndef RELEASE
/* #define ERROR_PRINT */
/* #define WARNING_PRINT */
 #define INFO_PRINT
#else
/* #define ERROR_PRINT */
/* #define WARNING_PRINT */
/* #define INFO_PRINT */
#endif

#if defined(ERROR_PRINT) 
void ERR_Print(const char *fmt, ...);
#define WAR_Print(format,...)
#define INFO_Print(format,...)
#elif defined(WARNING_PRINT) 
void ERR_Print(const char *fmt, ...);
void WAR_Print(const char *fmt, ...);
#define INFO_Print(format,...)
#elif defined(INFO_PRINT) 
void ERR_Print(const char *fmt, ...);
void WAR_Print(const char *fmt, ...);
void INFO_Print(const char *fmt, ...);
void INFO_Value_Print(UINT16);
#else
#define ERR_Print(format,...)
#define WAR_Print(format,...)
#define INFO_Print(format,...)
#define INFO_Value_Print(value)
#endif

 #define UART_PRINT
/* #define IDE_PRINT */

#if defined(UART_PRINT) 
#define APP_Print UART_Print
#elif defined(IDE_PRINT) 
#define APP_Print printf
#else
#define APP_Print(format,...)
#endif


#endif /*_DBG_H_*/

