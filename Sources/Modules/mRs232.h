/*
------------------------------------------------------------
Copyright 2003-2017 Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	mRs232.h
Author and date :	damien 18 Aug 2017

Goal

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/
#ifndef SOURCES_MODULES_MRS232_H_
#define SOURCES_MODULES_MRS232_H_

#include "def.h"

//-------------------------------------------------------------------------
// Configuration du module SCI
//-------------------------------------------------------------------------
void mRs232_Setup(void);

//-------------------------------------------------------------------------
// Ouverture des interfaces
//-------------------------------------------------------------------------
void mRs232_Open(void);

//-------------------------------------------------------------------------
// Envoi d'un caractère par RS232
// aChar : la caractère à transmettre
//-------------------------------------------------------------------------
void mRs232_WriteChar(unsigned char aChar);

//-------------------------------------------------------------------------
// Envoi d'une chaîne de caractères par RS232
// La tramsmission s'arrête au caractère NULL
// *aDataPtr : pointeur sur la chaîne de caractères à transmettre, la
// chaîne doit-être terminée par le caractère NULL
//-------------------------------------------------------------------------
void mRs232_WriteString(unsigned char *aDataPtr);

bool mRs232_ReadDataFromBuffer(uint8_t *aBufferPtr);

uint8_t mRs232_ReadCharFromBuffer(void);

#endif /* SOURCES_MODULES_MRS232_H_ */
