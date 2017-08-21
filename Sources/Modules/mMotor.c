/*
------------------------------------------------------------
Copyright 2003-2017 Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	mMotor.c
Author and date :	damien 19 Aug 2017

Description in the header file (.h)

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/
#include "def.h"
#include "mMotor.h"
#include "iFlextimer.h"
#include "iDio.h"


void mMotor_Setup(void)
{
	iFlextimer_Config();
	iDio_SetPortDirection(kPortC,kMaskIo3,kIoOutput); //break
	iDio_SetPortDirection(kPortD,kMaskIo4,kIoOutput); //Fw/Rv
	iDio_SetPortDirection(kPortD,kMaskIo5,kIoOutput); //tachoHall3
}

void mMotor_Set(enum DirectionEnum aDirection, uint16_t aSpeed)
{
	//break(C3) (actif bas)
	iDio_SetPort(kPortC,kMaskIo3,kIoOn);
	//direction
	((aDirection==kForward) ? (iDio_SetPort(kPortD,kMaskIo4,kIoOn)) : (iDio_SetPort(kPortD,kMaskIo4,kIoOff)));
	//control du courant
	iFlextimer_UpdateDuty(kFtm0,kFtmCh6,aSpeed);

}
