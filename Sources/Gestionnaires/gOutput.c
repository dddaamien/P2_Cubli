/*
------------------------------------------------------------
Copyright 2003-2017 Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	gOutput.c
Author and date :	damien 18 Aug 2017

Description in the header file (.h)

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/
#include "gOutput.h"
#include "gMBox.h"
#include "mLeds.h"
#include "mMotor.h"


//-------------------------------------------------------------------------
// Configuration du gestionnaire
//-------------------------------------------------------------------------
void gOutput_Setup(void)
{
	mLeds_Setup();
	// Pas nécessaire de faire l'init de l'uart, déjà fait dans gInput
	mServo_Setup();
	mMotor_Setup();

	mLeds_Write(kLedAll,kLedAll);
}

//-------------------------------------------------------------------------
// Exécution du gestionnaire
//-------------------------------------------------------------------------
void gOutput_Execute(void)
{
	mLeds_Write(kLedAll,gOutput.leds);
	mServo_SetAngle(gOutput.servoAngle);
	mMotor_Set(gOutput.motorDirection,gOutput.motorSpeed);
}
