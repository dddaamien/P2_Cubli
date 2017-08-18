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

//-----------------------------------------------------------------------------
// handle setup
//-----------------------------------------------------------------------------
void gCompute_Setup(void)
{

}


//-----------------------------------------------------------------------------
// Handle execute
//-----------------------------------------------------------------------------
void gCompute_Execute(void)
{
	gOutput.leds = gInput.switchsArray<<2 | gInput.btnTab[1].current<<1 | gInput.btnTab[0].current;
	if(gInput.uartRx != 0)
	{
		gOutput.uartTx = gInput.uartRx+1;
	}
	else
	{
		gOutput.uartTx = 0;
	}
}
