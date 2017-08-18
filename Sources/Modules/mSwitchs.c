/*
------------------------------------------------------------
Copyright 2003-2017 Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	mSwitchs.c
Author and date :	damien 18 Aug 2017

Description in the header file (.h)

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/
#include "mSwitchs.h"

void mSwitchs_Setup(void)
{
	//La config du clock et des pins est faite dans iCpu

	/**
	 * Appel des fonctions de configuration de la direction des outputs PDDR
	 */
	iDio_SetPortDirection(kPortC, kMaskIo6 | kMaskIo7, kIoInput);
	iDio_SetPortDirection(kPortD, kMaskIo0 | kMaskIo1, kIoInput);
}

uint8_t mSwitchs_Read(enum SwitchEnum aSwitch)
{
	return (uint8_t)iDio_GetPort(kPortD,kMaskIo0) | ((uint8_t)iDio_GetPort(kPortD,kMaskIo1)<<1) | ((uint8_t)iDio_GetPort(kPortC,kMaskIo6)<<2) | ((uint8_t)iDio_GetPort(kPortC,kMaskIo7)<<3);
}
