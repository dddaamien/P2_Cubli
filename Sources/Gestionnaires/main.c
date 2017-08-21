/*
 ------------------------------------------------------------
 Copyright 2003-2017 Haute école ARC Ingéniérie, Switzerland.
 All rights reserved.
 ------------------------------------------------------------
 File name :	main.c
 Author and date :	damien 16 Aug 2017

 Description in the header file (.h)

 -----------------------------------------------------------------------------
 History:
 -----------------------------------------------------------------------------
 TODO:
	-FTM
		-Capture
	-mMotor
	-mCentral
 -----------------------------------------------------------------------------
 */

#include "mCpu.h"
#include "mDelay.h"
#include "gInput.h"
#include "gCompute.h"
#include "gOutput.h"
#include "gTerminal.h"
#include "def.h"


//-----------------------------------------------------------------------
// Entry point
//-----------------------------------------------------------------------
static Int16 sDlyInpNb;

void main(void)
{
	//-----------------------------------------------------------------------
	// Peripherals setup
	//-----------------------------------------------------------------------
	// Low level CPU setup
	mCpu_Setup();
	// Delay module setup (PIT)
	mDelay_Setup();

	// Enable all interrupts
	EnableInterrupts;

	//-----------------------------------------------------------------------
	// Peripherals start
	//-----------------------------------------------------------------------
	// Delay module open
	mDelay_Open();

	// Input handle setup
	gInput_Setup();
	// Compute handle setup
	gCompute_Setup();
	// Output handle setup
	gOutput_Setup();
	// Terminal handle setup
	gTerminal_Setup();

	// Get one delay
	sDlyInpNb = mDelay_GetDelay(kPit0, 1000 / kPit0Period);

	//-----------------------------------------------------------------------
	// Inifinite loop
	//-----------------------------------------------------------------------
	while(1)
	{
		//-----------------------------------------------------------------------
		// Input execute (ms)
		//-----------------------------------------------------------------------
		if(mDelay_IsDelayDone(kPit0, sDlyInpNb))
		{
			// Delay restart
			mDelay_ReStart(kPit0, sDlyInpNb, 100 / kPit0Period);

			// Input handle execute
			gInput_Execute();
			// Compute handle execute
			gCompute_Execute();
			// Output handle execute
			gOutput_Execute();
			// Terminal handle execute
			gTerminal_Execute();
		}
	}
}
