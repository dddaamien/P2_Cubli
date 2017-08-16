/*
------------------------------------------------------------
Copyright 2003-2012 Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	mDelay.c
Author and date :	Monnerat Serge 17 févr. 2012

Description in the header (.h)

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $

-----------------------------------------------------------------------------
*/

#include "mDelay.h"
#include "iPit.h"

//-----------------------------------------------------------------------------
// Delay module setup
//-----------------------------------------------------------------------------
void mDelay_Setup(void)
{
	// --> PIT0 used for delays
	// --> PIT1 unused
	// --> PIT2 unused 
	// --> PIT3 unused 
	
	// PIT0 delays structure setup
	iPit_InitDelay(kPit0);
	// PIT0 setup
 	iPit_Config(kPit0,kPit0Period);
}

//-----------------------------------------------------------------------------
// Delay module open   
//-----------------------------------------------------------------------------
void mDelay_Open(void)
{
	iPit_StartPit(kPit0);
}

//-----------------------------------------------------------------------------
// Delay module close 
//-----------------------------------------------------------------------------
void mDelay_Close(void)
{
}

//------------------------------------------------------------
// Get one delay
// aPit		: Wich PIT
// aDelay	: delay time in ms
// Retour	: delay number or -1 if no more free delay
// 
//------------------------------------------------------------
Int16 mDelay_GetDelay(PitEnum aPit,UInt16 aDelay)
{
	return iPit_GetDelay(aPit,aDelay);
}

//------------------------------------------------------------
// Delay release
// aPit		: which PIT
// aDelayNb	: delay number
//------------------------------------------------------------
void mDelay_DelayRelease(PitEnum aPit,UInt16 aDelayNb)
{
	iPit_DelayRelease(aPit,aDelayNb);
}

//------------------------------------------------------------
// Delay restart
// aPit			: which PIT
// aDelay		: delay time
// aDelayNb	: delay number
//------------------------------------------------------------
void mDelay_ReStart(PitEnum aPit,UInt16 aDelayNb,UInt16 aDelay)
{
	iPit_ReStart(aPit,aDelayNb,aDelay);
}


//------------------------------------------------------------
// Check if the delay is done
// aPit			: which PIT
// aDelayNb	: delay number
// Retour		: flag state
//------------------------------------------------------------
bool mDelay_IsDelayDone(PitEnum aPit,UInt16 aDelayNb)
{
	return iPit_IsDelayDone(aPit,aDelayNb);
}
