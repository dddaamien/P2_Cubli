/*
------------------------------------------------------------
Copyright 2003-2016 Haute �cole ARC Ing�ni�rie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	gInput.c
Author and date :	Monnerat Serge 29.06.2015

Description in the header (.h)

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $

-----------------------------------------------------------------------------
*/
#include "gInput.h"
#include "gMBox.h"
#include "mSwitchs.h"
#include "mRs232.h"

//-----------------------------------------------------------------------------
// handle setup
//-----------------------------------------------------------------------------
void gInput_Setup(void)
{
	mSwitchs_Setup();
	mRs232_Setup();
	mRs232_Open();
	//Initialisation des variables d'entrée
	for(uint8_t i = 0;i<kNbOfBtn;++i)
	{
		gInput.btnTab[i].current = false;
		gInput.btnTab[i].old = false;
	}
	gInput.switchsArray = 0;
	gInput.uartRx = 0;
}

//-----------------------------------------------------------------------------
// Handle execute
//-----------------------------------------------------------------------------
void gInput_Execute(void)
{
	gInput.switchsArray = mSwitchs_Read(kSwAll);
	for(uint8_t i = 0;i<kNbOfBtn;++i)
	{

		gInput.btnTab[i].old = gInput.btnTab[i].current;
		gInput.btnTab[i].current = gInput.switchsArray & 0x01;
		gInput.switchsArray >>= 1;
	}
	gInput.uartRx = mRs232_ReadCharFromBuffer();
}

