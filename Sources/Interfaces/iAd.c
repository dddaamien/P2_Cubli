/*
------------------------------------------------------------
Copyright 2003-2016 Haute �cole ARC Ing�ni�rie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	iAd.c
Author and date :	Monnerat Serge 11.06.2015

Description in the header (.h)

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $

-----------------------------------------------------------------------------
*/

#include "iAd.h"
#include "MK10D5.h"



//------------------------------------------------------------
// ADC 0 & 1 setup
//------------------------------------------------------------
void iAd_Config(void)
{
	// ADC status and control registers 1 (ADCx_SC1n)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 754
	// AIEN=0,Conversion complete interrupt disabled
	// DIFF=0,Differential mode enable -> Single-ended conversions and input channels are selected
	// ADCH=0x1f,Input channel select -> Module disabled
	ADC0->SC1[0]=0;
	ADC0->SC1[0]|=(ADC_SC1_ADCH(0x1f));
	
	// ADC configuRtion register 1 (ADCx_CFG1)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 757
	// ADLPC=0, Normal power configuration 
	// ADIV=1,The divide ratio is 2 and the clock rate is (input clock)/8
	// ADLSMP=1,Long sample time
	// MODE=01,It is single-ended 12-bit conversion
	// ADICLK=1,Bus clock divided by 2
	ADC0->CFG1=0;
	ADC0->CFG1|=(ADC_CFG1_ADIV(1)|ADC_CFG1_MODE(1)|ADC_CFG1_ADICLK(1)|ADC_CFG1_ADLSMP_MASK);
	
	// Configuration register 2 (ADCx_CFG2)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 759
	// MUXSEL=0,ADxxa channels are selected
	// ADACKEN=0,Asynchronous clock output disabled
	// ADHSC=0,Normal conversion sequence selected
	// ADLSTS=0,Default longest sample time (20 extra ADCK cycles; 24 ADCK cycles total)
	// AD clock = Bus clock / 4 = 50Mhz/4=12.5 MHz
	// One channel converison time = 1/12.5MHz * 24 = 1.92 us
	ADC0->CFG2=0;
	
	// Status and control register 2 (ADCx_SC2)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 762
	// ADTRG=0,Software trigger selected
	// ACFE=0,Compare function disabled
	// ACFGT=0,Configures less than threshold, outside range not inclusive and inside range not inclusive functionality
	// based on the values placed in the CV1 and CV2 registers
	// ACREN=0,Range function disabled. Only the compare value 1 register (CV1) is compared
	// DMAEN=0,DMA is disabled
	// REFSEL=0,Default voltage reference pin pair (external pins VREFH and VREFL) 
	ADC0->SC2=0;
	ADC0->SC2|=ADC_SC2_REFSEL(0);
	
	// Status and control register 2 (ADCx_SC3)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 764
	// ADCO=0, One conversion or one set of conversions if the hardware average function is enabled (AVGE=1) after initiating a conversion.
	// AVGE=0, Hardware average function disabled
	// AVGS=0, 4 samples averaged
	ADC0->SC3=0;
}

//------------------------------------------------------------
// ADC 0 & 1 get conversion
// aAd: which ADC
//------------------------------------------------------------
UInt16 iAd_GetResult(ADCEnum aAd)
{
	UInt16 aVal;
	switch(aAd)
		{
			case kAdc0:
				aVal=ADC0->R[0];
			break;
		}

	return aVal;
}

//------------------------------------------------------------
// ADC 0 & 1 get conversion
// aAd: which ADC
// aCh: which ADC channel
//------------------------------------------------------------
void iAd_SelectChannel(ADCEnum aAd,ADCChannelEnum aCh)
{
	UInt16 aVal;
	switch(aAd)
		{
			case kAdc0:
				// Clear ADCH field
				ADC0->SC1[0]&=(~ADC_SC1_ADCH_MASK);
				// To clear the flag COCO after reseting ADCH 
				aVal=ADC0->R[0];
				ADC0->SC1[0]|=(ADC_SC1_ADCH(aCh));
			break;
		}
}

//------------------------------------------------------------
// ADC 0 & 1 get conversion complete flag
// aAd: which ADC
//------------------------------------------------------------
bool iAd_GetCompleteConvStatus(ADCEnum aAd)
{
	bool aRet;
	switch(aAd)
		{
			case kAdc0:
				aRet=(ADC0->SC1[0]&ADC_SC1_COCO_MASK)==ADC_SC1_COCO_MASK;
			break;
		}
	return aRet;
}

//------------------------------------------------------------
// ADC 0 & 1 calibration
//------------------------------------------------------------
bool iAd_Cal(ADCEnum aAd)
{
  UInt16 aCalVar=0;
  bool aRet;
  
  switch(aAd)
  		{
  			case kAdc0:
  				
  				// Max ADC clock -> 4MHz
  				ADC0->CFG1&=((~ADC_CFG1_ADIV_MASK)&(~ADC_CFG1_ADICLK_MASK));
  				ADC0->CFG1|=(ADC_CFG1_ADIV(3)|ADC_CFG1_MODE(1)|ADC_CFG1_ADICLK(1)|ADC_CFG1_ADLSMP_MASK);
  				
  				// Enable Software Conversion Trigger for Calibration Process
					ADC0->SC2&=(~ADC_SC2_ADTRG_MASK);
					// Set single conversion, clear avgs bitfield for next writing
					ADC0->SC3&= ((~ADC_SC3_ADCO_MASK) & (~ADC_SC3_AVGS_MASK));
					// Turn averaging ON and set at max value ( 32 )
					ADC0->SC3|= (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3) );
					// Start CAL
					ADC0->SC3 |= ADC_SC3_CAL_MASK ;
					// Wait calibration end
					while ( (ADC0->SC1[0] & ADC_SC1_COCO_MASK ) == false );
					
					// Check for Calibration fail error and return
					if ((ADC0->SC3&ADC_SC3_CALF_MASK) == ADC_SC3_CALF_MASK)
						{  
							aRet=false;     
						}
					else
						{
							// Calculate plus-side calibration
							aCalVar = 0x00;
							
							aCalVar =  ADC0->CLP0;
							aCalVar += ADC0->CLP1;
							aCalVar += ADC0->CLP2;
							aCalVar += ADC0->CLP3;
							aCalVar += ADC0->CLP4;
							aCalVar += ADC0->CLPS;
							aCalVar = aCalVar/2;
							aCalVar |= 0x8000; // Set MSB
							ADC0->PG= ADC_PG_PG(aCalVar);
						
							// Calculate minus-side calibration
							aCalVar = 0x00;
						
							aCalVar =  ADC0->CLM0;
							aCalVar += ADC0->CLM1;
							aCalVar += ADC0->CLM2;
							aCalVar += ADC0->CLM3;
							aCalVar += ADC0->CLM4;
							aCalVar += ADC0->CLMS;
							aCalVar = aCalVar/2;
							aCalVar |= 0x8000; // Set MSB
							ADC0->MG = ADC_MG_MG(aCalVar);
							
							// Clear CAL bit
							ADC0->SC3&= (~ADC_SC3_CAL_MASK);
							
							// Return OK
							aRet=true;  
						}
  			break;
  		}
  	
  return aRet;
  
}
