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
//	FTM0_SC = 0; // Desactivation de l'entrée du clock le temps de la configuration

//	GPIOD_PDDR |= (1 << 6); // Sortie

	FTM0->MODE |= FTM_MODE_WPDIS_MASK; // Desactive la protection en ecriture
	//FTM0_MODE |= FTM_MODE_FTMEN_MASK; // FTM enable

	FTM0->CNTIN = 0; // Valeur d'initialisation du compteur
	FTM0->CNT = 0;

	FTM0->CONTROLS[0].CnSC = 0x28; // Edge-Aligned PWM (p691)
	FTM0->MOD = FTM_MOD_MOD(0xFFFF); // Modulo (p689)
	FTM0->CONTROLS[0].CnV = 0x0FFF; //Servo - quand CNT atteint cette valeur -> passe à l'état bas

	FTM0->CONF |= FTM_CONF_BDMMODE(3); // PWM actif meme en mode debug


	FTM0->SC |= FTM_SC_CLKS(0x01); // system clock (p688)
/*	// Features Mode Selection (FTMx_MODE)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 900
	// Disable Write Protection - enables changes to QUADEN, DECAPEN, etc.  
	FTM0->MODE |= FTM_MODE_WPDIS_MASK;
	//FTMEN is bit 0, need to set to zero so DECAPEN can be set to 0
	FTM0->MODE &= (~FTM_MODE_FTMEN_MASK);
	
	// Quadrature Decoder Control and Status (FTMx_QDCTRL)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 923
	// Enable de quadrature mode
	FTM0->QDCTRL |= FTM_QDCTRL_QUADEN_MASK;
	
	// Counter (FTMx_CNT)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 892
	// FTM Counter Value - reset counter to zero
	FTM0->CNT = 0x0; //FTM Counter Value - reset counter to zero
	
	// Modulo (FTMx_MOD)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 892
	//Set the overflow rate
	FTM0->MOD=(PERIPHERAL_BUS_CLOCK/(1<<FTM1_CLK_PRESCALE))/FTM1_OVERFLOW_FREQUENCY;
	
	// Counter Initial Value (FTMx_CNTIN)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 897
	//Set the Counter Initial Value to 0
	FTM0->CNTIN = 0;
	
	// Channel (n) Status and Control (FTMx_CSC)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 893
	// CHIE=0, Disable channel interrupts. Use software polling
	// MSB/MSA/ELSB/ELSA --> Edge-aligned PWM
	// DMA=0, Disable DMA transfers
	FTM0->CONTROLS[0].CnSC=0;
	FTM0->CONTROLS[0].CnSC|=(FTM_CnSC_MSB_MASK|FTM_CnSC_ELSB_MASK);
	FTM0->CONTROLS[1].CnSC=0;
	FTM0->CONTROLS[1].CnSC|=(FTM_CnSC_MSB_MASK|FTM_CnSC_ELSB_MASK);
	
	// Status and Control (FTMx_SC)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 890
	// Start the timer clock, source is the external clock
	FTM0->SC=FTM_SC_CLKS(3);
	
	// Channel (n) Value (FTMx_CV)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 896
	// FTMx_CnV contains the captured FTM counter value, this value determines the pulse width
	// Duty at 50%
	FTM0->CONTROLS[0].CnV = FTM1->MOD/2;
	FTM0->CONTROLS[0].CnV = FTM1->MOD/2;
	
	// Status and Control (FTMx_SC)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 890
	// CPWMS=0,Center-aligned PWM Select -> FTM counter operates in up counting mode
	// CLKS=1, Clock Source Selection -> System clock -> 50Mhz
	// PS:3,Prescale Factor Selection -> Divide by 8 
	FTM0->SC|=(FTM_SC_CLKS(1)|FTM_SC_PS(FTM1_CLK_PRESCALE));
	
	// FTM PWM Load (FTMx_PWMLOAD)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 933
	FTM0->PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;*/
}

void iFlextimer_UpdatePWM(PwmEnum aPwm, PwmChannelEnum aChannel, float aDuty)
{

}





