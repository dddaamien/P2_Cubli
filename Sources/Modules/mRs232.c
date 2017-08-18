/*
------------------------------------------------------------
Copyright 2003-2017 Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	mRs232.c
Author and date :	damien 18 Aug 2017

Description in the header file (.h)

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/
#include "iUart.h"
#include "mRs232.h"

//-------------------------------------------------------------------------
// Configuration du module SCI
//-------------------------------------------------------------------------
void mRs232_Setup(void)
{
	// Configuration du port RS232
	iUart_Config();
}

//-------------------------------------------------------------------------
// Ouverture des interfaces
//-------------------------------------------------------------------------
void mRs232_Open(void)
{
}

//-------------------------------------------------------------------------
// Envoi d'un caractère par RS232
// aChar : la caractère à transmettre
//-------------------------------------------------------------------------
void mRs232_WriteChar(unsigned char aChar)
{
	// Attente fin d'envoi
	while (!iUart_GetStatus(kUart0,kSciTransmitComplete));
	// Ecriture du registre de données
	iUart_SetData(kUart0,aChar);
}

//-------------------------------------------------------------------------
// Envoi d'une chaîne de caractères par RS232
// La tramsmission s'arrête au caractère NULL
// *aDataPtr : pointeur sur la chaîne de caractères à transmettre, la
// chaîne doit-être terminée par le caractère NULL
// ("ma chaîne")
//-------------------------------------------------------------------------
void mRs232_WriteString(unsigned char *aDataPtr)
{
	// Tant que toutes les données ne sont pas émises
	while (*aDataPtr != 0)
	{
		// Attente fin d'envoi
		while (!iUart_GetStatus(kUart0,kSciTransmitComplete));
		// Ecriture du registre de données
		iUart_SetData(kUart0,*aDataPtr);
		// prochain byte du buffer
		aDataPtr++;
	}
}

// Retourne TRUE si le buffer est vide sinon enregistre le
// byte lu du buffer dans le tableau passé en paramètre
// !! Attention au débordement possible !!
bool mRs232_ReadDataFromBuffer(uint8_t *aBufferPtr)
{
	uint8_t i=0;
	if(iUart_IsBufferEmpty(kUart0)) return true;
	else
	{
		while(!iUart_IsBufferEmpty(kUart0))
		{
			aBufferPtr[i++] = iUart_GetCharFromBuffer(kUart0);
		}
		aBufferPtr[i] = '\0';
		return false;
	}
}

uint8_t mRs232_ReadCharFromBuffer(void)
{
	if(!iUart_IsBufferEmpty(kUart0))
	{
		return iUart_GetCharFromBuffer(kUart0);
	}
	else
	{
		return 0;
	}
}
