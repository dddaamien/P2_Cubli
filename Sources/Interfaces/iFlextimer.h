/*
------------------------------------------------------------
Copyright 2003-2010 Haute �cole ARC Ing�ni�rie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name : 	iFlextimer.h	
Author and date :	Monnerat Serge 1 mars 2012

Goal : 

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $


-----------------------------------------------------------------------------
*/
#ifndef __iFlextimer__
#define __iFlextimer__

#include "def.h"

// Pwm enum
typedef enum
{
	kFtm0,
	kFtm1
}FtmEnum;

// Pwm channels enum
typedef enum
{
	kFtmCh0 = 0,
	kFtmCh1 = 1,
	kFtmCh2 = 2,
	kFtmCh3 = 3,
	kFtmCh4 = 4,
	kFtmCh5 = 5,
	kFtmCh6 = 6,
	kFtmCh7 = 7
}FtmChannelEnum;


//------------------------------------------------------------
// Flextimer setup
//------------------------------------------------------------
void iFlextimer_Config(void);

//------------------------------------------------------------
// Set PWM duty
//------------------------------------------------------------
void iFlextimer_UpdateDuty(FtmEnum aFtm, FtmChannelEnum aChannel, uint16_t aDuty);

#endif
