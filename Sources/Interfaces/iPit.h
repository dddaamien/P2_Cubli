/*
------------------------------------------------------------
Copyright 2003-2010 Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name : 	iPit.h	
Author and date :	Monnerat Serge 16 févr. 2012

Goal : 

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $


-----------------------------------------------------------------------------
*/
#ifndef __iPit__
#define __iPit__

#include "def.h"

//------------------------------------------------------------
// PIT setup
// aPit					: which PIT (0 to 3)
// aPeriodeMs  	: PIT period in ms
//------------------------------------------------------------
void iPit_Config(PitEnum aPit,UInt16 aPeriodeMs);

//------------------------------------------------------------
// Start Pit
// aPit		: which PIT
//------------------------------------------------------------
void iPit_StartPit(PitEnum aPit);

//------------------------------------------------------------
// Stop Pit
// aPit		: which PIT
//------------------------------------------------------------
void iPit_StopPit(PitEnum aPit);

//------------------------------------------------------------
// Setup the delay structure
// aPit		: which PIT
//------------------------------------------------------------
void iPit_InitDelay(PitEnum aPit);

//------------------------------------------------------------
// Get and setup a delay
// aPit		: which PIT
// aDelay	: the delay (ms)
// Retour	: the delay number
//------------------------------------------------------------
Int16 iPit_GetDelay(PitEnum aPit,UInt16 aDelay);

//------------------------------------------------------------
// Check if the delay is done
// aPit			: which PIT
// aDelayNb	: the delay number
// Retour		: state of the delay
//------------------------------------------------------------
bool iPit_IsDelayDone(PitEnum aPit,UInt16 aDelayNb);

//------------------------------------------------------------
// Release a delay
// aPit			: which PIT
// aDelayNb	: the delay number
//------------------------------------------------------------
void iPit_DelayRelease(PitEnum aPit,UInt16 aDelayNb);

//------------------------------------------------------------
// Restart un delay
// aPit			: which PIT
// aDelayNb	: the delay number
// aDelay		: the delay time
//------------------------------------------------------------
void iPit_ReStart(PitEnum aPit,UInt16 aDelayNb,UInt16 aDelay);


#endif
