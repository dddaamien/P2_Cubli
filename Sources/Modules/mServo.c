/*
------------------------------------------------------------
Copyright 2003-2017 Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	mServo.c
Author and date :	damien 19 Aug 2017

Description in the header file (.h)

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/
#include "def.h"
#include "mServo.h"
#include "iFlextimer.h"

void mServo_Setup()
{
	iFlextimer_Config();
}

// aAngle:angle en degrés
// Le FTM doit avoir une période de 20ms
void mServo_SetAngle(uint8_t aAngle)
{
	//0°->1ms, 180°->2ms
	iFlextimer_UpdateDuty(kFtm0,kFtmCh0,((uint32_t)aAngle<<12)/225);
}
