/*******************************************************************
*
*         Copyright (c) 2015 by Whileloop, Inc. (WLI)
*
*  All rights are reserved. Reproduction or in part is prohibited
*  without the written consent of the copyright owner.
*
*  app.h
*  This file contains the definitions for application.
*
*******************************************************************/
#ifndef _APP_H_
#define _APP_H_

/* For hardware version control */
//#define SUEY_EVB
#define REMOTER_EVB
//#define REMOTER_EVB_KEY_INTR
//#define REMOTER_EVB_INP_CAP
#define USE_FLASH 1
#define PRINT_REC_VALUE 1

#define APP_LEARNING_REMOTER
#if defined(APP_LEARNING_REMOTER) 
#define APPLICATION_NAME					"Learning Remoter\n"
#endif
#define PLATFORM								"Atmega8 development platform\n"
#define VENDOR								"Whileloop, Inc.\n"

/*
version x._._._ means the x time release for customer
version _.x._._ means the x time release for re-stucture or big integration
version _._.x._ means the x time release for feature add
version _._._.x means the x time release for bug fix
*/
#if defined(APP_LEARNING_REMOTER) 
#define APPLICATION_VERSITON			        "v 0.0.0.1\n"
#endif

/* remoter status */
#define REMOTER_IDLE 0
#define REMOTER_REC 1
#define REMOTER_LEARNING 2
#define REMOTER_LEARNDONE 3
#define REMOTER_SAVEDONE REMOTER_IDLE
#define REMOTER_SENDING 4
#define REMOTER_SENDDONE 5
#define REMOTER_ERR 255

#define LEARNING_TIME 10000

#ifdef REMOTER_EVB    
#define FUNC_KEY_NUM 1 /* function key number */
#define TTL_KEY_NUM 6 /* total key number, function key and record key */
#define KEY_PRESSED 0
#define KEY_UNPRESSED 1
#define KEY_RELEASED (((1 << PB0) |(1 << PB1) | (1 << PB2) | (1 << PC2) ) <<1 ) | ((1 << PD6) | (1 << PD7) )
#define KEY_S1_ (1 << PB0) 
#define KEY_S2_ (1 << PB1) 
#define KEY_S3_ (1 << PB2)  
#define KEY_S4_ (1 << PD6) 
#define KEY_S5_ (1 << PD7)
#define KEY_REC_ ((1 << PC2)<<1)
#define KEY_S1 0 
#define KEY_S2 1 
#define KEY_S3 2 
#define KEY_S4 3 
#define KEY_S5 4
#define KEY_REC 5
#define NO_KEY_ 0xff

#define FLASH_SECTOR_SIZE 4096
#define FLASH_SECTOR_INDEX_TEST 10
#endif

#ifdef SUEY_EVB    
/*About the key board*/
#define FUNC_KEY_NUM 1 /* function key number */
#define KEY_PRESSED 0
#define KEY_UNPRESSED 1
#define KEY_RELEASED (((1 << PD2) |(1 << PD3) | (1 << PD4) | (1 << PD5) ) <<8 ) | ((1 << PB4) | (1 << PB5) )
#define KEY_1_ ((1 << PD2) <<8 )
#define KEY_2_ ((1 << PD3) <<8 )
#define KEY_3_ ((1 << PD4) <<8 )
#define KEY_4_ ((1 << PD5) <<8 )
#define KEY_5_ (1 << PB4)
#define KEY_6_ (1 << PB5)
#endif

extern BYTE remoter_status;

BYTE Flash_Test(void);
void LED_Flash(BYTE );
void LED_TurnOn(void);
void LED_TurnOff(void);

#endif /*_APP_H_*/

