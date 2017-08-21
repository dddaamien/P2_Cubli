/*
------------------------------------------------------------
Copyright 2003-2017 Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	mEm7180.h
Author and date :	damien 20 Aug 2017

Goal

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/
#ifndef SOURCES_MODULES_MEM7180_H_
#define SOURCES_MODULES_MEM7180_H_

//adresses des capteurs (write)
typedef enum Em7180DeviceEnum
{
	kEm7180D0=0b01010010,
	kEm7180D1=0b01010000
};

typedef union Em7180DataUnion
{
	uint32_t int32Value;
	float float32Value;
};

typedef struct Em7180DataStruct
{
	union Em7180DataUnion qX;
	union Em7180DataUnion qY;
	union Em7180DataUnion qZ;
	union Em7180DataUnion qW;
	uint16_t qTime;
};


void mEm7180_Setup(enum Em7180DeviceEnum);


#endif /* SOURCES_MODULES_MEM7180_H_ */
