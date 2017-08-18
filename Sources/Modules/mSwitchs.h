/*
------------------------------------------------------------
Copyright 2003-2017 Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	mSwitchs.h
Author and date :	damien 18 Aug 2017

Goal

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/
#ifndef SOURCES_MODULES_MSWITCHS_H_
#define SOURCES_MODULES_MSWITCHS_H_
#include "iDio.h"

enum SwitchEnum
{
	kSw1 = 0x1,
	kSw2 = 0x2,
	kSw3 = 0x4,
	kSw4 = 0x8,
	kSwAll = 0x0F
};

void mSwitchs_Setup(void);
uint8_t mSwitchs_Read(enum SwitchEnum aSwitch);

#endif /* SOURCES_MODULES_MSWITCHS_H_ */
