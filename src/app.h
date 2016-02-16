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


#endif /*_APP_H_*/

