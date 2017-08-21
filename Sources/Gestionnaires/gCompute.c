/*
------------------------------------------------------------
Copyright 2003-2016 Haute �cole ARC Ing�ni�rie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	gCompute.c
Author and date :	Monnerat Serge 29.06.2015

Description in the header (.h)

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $

-----------------------------------------------------------------------------
*/

#include "gCompute.h"
#include "gMBox.h"
#include "PI.h"
#include "mDelay.h"


static uint16_t sDelay;

//-----------------------------------------------------------------------------
// handle setup
//-----------------------------------------------------------------------------
void gCompute_Setup(void)
{
	sDelay = mDelay_GetDelay(kPit0,10/kPit0Period);
	gCompute.regPiConfig.LimTotLow=(float)-0xFFFF;
	gCompute.regPiConfig.LimTotLow=(float)0xFFFF;
	gCompute.regPiConfig.GainP=0.5;
	gCompute.regPiConfig.GainI=0.5;
}


//-----------------------------------------------------------------------------
// Handle execute
//-----------------------------------------------------------------------------
void gCompute_Execute(void)
{
	static float sIntPart=0;
	float calcSpeed;

	switch(gCompute.mode)
	{
	case kModeManuel:
		gOutput.leds = gInput.switchsArray<<2 | gInput.btnTab[1].current<<1 | gInput.btnTab[0].current;
		break;
	case kModeAuto:
		//Régulation toute les 10ms
		if(mDelay_IsDelayDone(kPit0,sDelay))
		{
			mDelay_ReStart(kPit0,sDelay,10/kPit0Period);
			calcSpeed = RegPI(&gCompute.regPiConfig,&sIntPart,gCompute.angle);
			if(calcSpeed>0)
			{
				gOutput.motorDirection=kForward;
				gOutput.motorSpeed=(uint16_t)calcSpeed;
			}
			else
			{
				gOutput.motorDirection=kBackward;
				gOutput.motorSpeed=(uint16_t)-calcSpeed;
			}
		}
		break;
	}
}
