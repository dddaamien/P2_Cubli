/*
------------------------------------------------------------
Copyright 2003-2010 Haute école ARC Ingéniérie, Switzerland. 
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
	kPwm1,
	kPwm2
}PwmEnum;

// Pwm channels enum
typedef enum
{
	kPwmCh0,
	kPwmCh1
}PwmChannelEnum;


//------------------------------------------------------------
// Flextimer setup
//------------------------------------------------------------
void iFlextimer_Config(void);

//------------------------------------------------------------
// Set PWM duty
//------------------------------------------------------------
void iFlextimer_UpdatePWM(PwmEnum aPwm, PwmChannelEnum aChannel, float aDuty);

#endif
