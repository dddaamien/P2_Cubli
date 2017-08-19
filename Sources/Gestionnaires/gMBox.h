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

#define kNbOfBtn 2

typedef struct
{
	bool current;
	bool old;
}BtnStateStruct;


//-----------------------------------------------------------------------------
// Input handle structure
//-----------------------------------------------------------------------------
typedef struct
{
	BtnStateStruct btnTab[kNbOfBtn];
	uint8_t switchsArray;
	uint8_t uartRx;
}InputStruct;
extern InputStruct gInput;

//-----------------------------------------------------------------------------
// Output handle structure
//-----------------------------------------------------------------------------
typedef struct
{
	uint8_t leds;
	uint8_t uartTx;
	uint8_t servoAngle;
}OutputStruct;
extern OutputStruct gOutput;


#endif
