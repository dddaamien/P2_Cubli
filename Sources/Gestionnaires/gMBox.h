/*
------------------------------------------------------------
Copyright 2003-2016 Haute �cole ARC Ing�ni�rie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name : 	gMBox.h	
Author and date :	Monnerat Serge 29.06.2015

Goal : 

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $


-----------------------------------------------------------------------------
*/
#ifndef __gMBox__
#define __gMBox__

#include "def.h"
#include "mEm7180.h" //Pour Em7180DataStruct
#include "mMotor.h"
#include "PI.h"

#define kNbOfBtn 2

typedef struct BtnStateStruct
{
	bool current;
	bool old;
};

typedef enum ModeEnum
{
	kModeAuto,
	kModeManuel
};

//-----------------------------------------------------------------------------
// Input handle structure
//-----------------------------------------------------------------------------
typedef struct
{
	struct BtnStateStruct btnTab[kNbOfBtn];
	uint8_t switchsArray;
	struct Em7180DataStruct sentralData;
}InputStruct;
extern InputStruct gInput;

typedef struct
{
	float angle;
	struct RegPIStruct regPiConfig;
	enum ModeEnum mode;
}ComputeStruct;
extern ComputeStruct gCompute;

//-----------------------------------------------------------------------------
// Output handle structure
//-----------------------------------------------------------------------------
typedef struct
{
	uint8_t leds;
	uint8_t servoAngle;
	uint16_t motorSpeed;
	enum DirectionEnum motorDirection;
}OutputStruct;
extern OutputStruct gOutput;


#endif
