/*
------------------------------------------------------------
Copyright 2003-2017 Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	mLeds.c
Author and date :	damien 18 Aug 2017

Description in the header file (.h)

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/
#include "mLeds.h"

/**
 *  Paramétrage des sorties PCR, PDDR et SCGC5
 */
void mLeds_Setup(void)
{
	//La config du clock et des pins est faite dans iCpu

	/**
	 * Appel des fonctions de configuration de la direction des outputs PDDR
	 */
	iDio_SetPortDirection(kPortB, kMaskIo0 | kMaskIo1, kIoOutput);
	iDio_SetPortDirection(kPortD, kMaskIo2 | kMaskIo3, kIoOutput);
	iDio_SetPortDirection(kPortE, kMaskIo16 | kMaskIo17, kIoOutput);
}

/**
 * Fonction d'activation d'une ou plusieurs leds
 */
void mLeds_Write(enum LedEnum aLed, enum LedEnum aState)
{
	//Port D
	if(aLed & kLed1)
	{
		iDio_SetPort(kPortD,kMaskIo2,kLed1 & aState);
	}
	if(aLed & kLed2)
	{
		iDio_SetPort(kPortD,kMaskIo3,kLed2 & aState);
	}
	//Port E
	if(aLed & kLed3)
	{
		iDio_SetPort(kPortE,kMaskIo16,kLed3 & aState);
	}
	if(aLed & kLed4)
	{
		iDio_SetPort(kPortE,kMaskIo17,kLed4 & aState);
	}
	//Port B
	if(aLed & kLed5)
	{
		iDio_SetPort(kPortB,kMaskIo1,kLed5 & aState);
	}
	if(aLed & kLed6)
	{
		iDio_SetPort(kPortB,kMaskIo0,kLed6 & aState);
	}
}
