/*
------------------------------------------------------------
Copyright 2003-2017 Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	mMotor.h
Author and date :	damien 19 Aug 2017

Goal

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/
#ifndef SOURCES_MODULES_MMOTOR_H_
#define SOURCES_MODULES_MMOTOR_H_

enum DirectionEnum
{
	kForward,
	kBackward
};

void mMotor_Setup(void);

void mMotor_Set(enum DirectionEnum aDirection, uint16_t aSpeed);


#endif /* SOURCES_MODULES_MMOTOR_H_ */
