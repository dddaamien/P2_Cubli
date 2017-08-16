/*
------------------------------------------------------------
Copyright 2003-2010 Haute �cole ARC Ing�ni�rie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name : 	iAd.h	
Author and date :	Monnerat Serge 28 f�vr. 2012

Goal : 

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $


-----------------------------------------------------------------------------
*/
#ifndef __iAd__
#define __iAd__

#include "def.h"

// ADC enum
typedef enum
{
	kAdc0
}ADCEnum;

// ADC channel enum
typedef enum
{
	kAdcCh0=0,
	kAdcCh1,
	kAdcCh2,
	kAdcCh3,
	kAdcCh4,
	kAdcCh5,
	kAdcCh6,
	kAdcCh7,
	kAdcCh8,
	kAdcCh9,
	kAdcCh10,
	kAdcCh11,
	kAdcCh12,
	kAdcCh13,
	kAdcCh14,
	kAdcCh15,
	kAdcCh16,
	kAdcCh17,
	kAdcCh18,
	kAdcCh19,
	kAdcCh20,
	kAdcCh21,
	kAdcCh22,
	kAdcCh23,
	kAdcTempSensor=0x1a,
	kAdcBandgap,
	kAdcVrefSh=0x1d,
	kAdcVrefSL
}ADCChannelEnum;

//------------------------------------------------------------
// ADC 0 & 1 setup
//------------------------------------------------------------
void iAd_Config(void);

//------------------------------------------------------------
// ADC 0 & 1 get conversion
// aAd: which ADC
//------------------------------------------------------------
UInt16 iAd_GetResult(ADCEnum aAd);

//------------------------------------------------------------
// ADC 0 & 1 get conversion
// aAd: which ADC
// aCh: which ADC channel
//------------------------------------------------------------
void iAd_SelectChannel(ADCEnum aAd,ADCChannelEnum aCh);

//------------------------------------------------------------
// ADC 0 & 1 get conversion complete flag
// aAd: which ADC
//------------------------------------------------------------
bool iAd_GetCompleteConvStatus(ADCEnum aAd);

//------------------------------------------------------------
// ADC 0 & 1 calibration
//------------------------------------------------------------
bool iAd_Cal(ADCEnum aAd);

#endif
