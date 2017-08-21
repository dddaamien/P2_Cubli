/*
------------------------------------------------------------
Copyright 2003-2016 Haute école ARC Ingénierie, Switzerland.
All rights reserved.
------------------------------------------------------------
File name :	iFlextimer.c
Author and date :	Monnerat Serge 11.06.2015

Description in the header (.h)

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

16.08.2017: Adaptation pour MK10D5

-----------------------------------------------------------------------------
*/

#include "iFlextimer.h"
#include "MK10D5.h"

#define FTM1_CLK_PRESCALE 1
#define FTM1_OVERFLOW_FREQUENCY (12500)
#define PERIPHERAL_BUS_CLOCK (25000000)

#define FTM1_CLK_PRESCALE 2
#define FTM1_OVERFLOW_FREQUENCY (2500)

//------------------------------------------------------------
// Flextimer setup
//------------------------------------------------------------
void iFlextimer_Config(void)
{
	// Gestion du clock & pin faite dans iCpu

	//------------------------------------------------------------
	// FTM0
	//------------------------------------------------------------

	FTM0->MODE |= FTM_MODE_WPDIS_MASK; // Desactive la protection en ecriture

	FTM0->CNTIN = 0; // Valeur d'initialisation du compteur
	FTM0->CNT = 0;
	FTM0->MOD = FTM_MOD_MOD(0xFFFF); // Modulo (p689)
	// PWM ch0->servo, ch6->motVref
	FTM0->CONTROLS[0].CnSC = 0x28; // Edge-Aligned PWM (p691)
	FTM0->CONTROLS[6].CnSC = 0x28; // Edge-Aligned PWM (p691)
	// Capture ch1->motTacho
	FTM0->CONTROLS[1].CnSC = 0x01; //Input capture, rising edge (p691)


	FTM0->CONF |= FTM_CONF_BDMMODE(3); // PWM actif meme en mode debug
	FTM0->SC |= FTM_SC_CLKS(0x01) | FTM_SC_PS(3); // system clock (p688), prescaler 8x
}

void iFlextimer_UpdateDuty(FtmEnum aFtm, FtmChannelEnum aChannel, uint16_t aDuty)
{
	switch(aFtm)
	{
	case kFtm0:
		FTM0->CONTROLS[aChannel].CnV = aDuty;
		break;
	case kFtm1:
		FTM1->CONTROLS[aChannel].CnV = aDuty;
		break;
	}
}





