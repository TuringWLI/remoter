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

/*About the key board*/
#define KEY_PRESSED 0
#define KEY_UNPRESSED 1

#define KEY_RELEASED (((1 << PD2) |(1 << PD3) | (1 << PD4) | (1 << PD5) ) <<8 ) | ((1 << PB4) | (1 << PB5) )
#define KEY_1_ ((1 << PD2) <<8 )
#define KEY_2_ ((1 << PD3) <<8 )
#define KEY_3_ ((1 << PD4) <<8 )
#define KEY_4_ ((1 << PD5) <<8 )
#define KEY_5_ (1 << PB4)
#define KEY_6_ (1 << PB5)


#endif /*_APP_H_*/

