/*
------------------------------------------------------------
Copyright 2003-2017 Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	mLeds.h
Author and date :	damien 18 Aug 2017

Goal

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/
#ifndef SOURCES_MODULES_MLEDS_H_
#define SOURCES_MODULES_MLEDS_H_
#include "iDio.h"

enum LedEnum
{
	kLed1 = 0x1,
	kLed2 = 0x2,
	kLed3 = 0x4,
	kLed4 = 0x8,
	kLed5 = 0x10,
	kLed6 = 0x20,
	kLedAll = 0x3F
};

void mLeds_Setup(void);
void mLeds_Write(enum LedEnum aLed, enum LedEnum aState);

#endif /* SOURCES_MODULES_MLEDS_H_ */
