/*
------------------------------------------------------------
Copyright 2003-2016 Haute �cole ARC Ing�ni�rie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	iFlextimer.c
Author and date :	Monnerat Serge 11.06.2015

Description in the header (.h)

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $

-----------------------------------------------------------------------------
*/

#include "iFlextimer.h"
#include "MK10D5.h"

#define FTM1_CLK_PRESCALE 1
#define FTM1_OVERFLOW_FREQUENCY (12500)
#define PERIPHERAL_BUS_CLOCK (50000000)

#define FTM2_CLK_PRESCALE 2
#define FTM2_OVERFLOW_FREQUENCY (2500)

//------------------------------------------------------------
// Flextimer setup --> Quadrature measures
//------------------------------------------------------------
void iFlextimer_Config(void)
{
	//------------------------------------------------------------
	// FTM1
	//------------------------------------------------------------
	// Features Mode Selection (FTMx_MODE)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 900
	// Disable Write Protection - enables changes to QUADEN, DECAPEN, etc.  
	FTM1->MODE |= FTM_MODE_WPDIS_MASK;
	//FTMEN is bit 0, need to set to zero so DECAPEN can be set to 0
	FTM1->MODE &= (~FTM_MODE_FTMEN_MASK);
	
	// Quadrature Decoder Control and Status (FTMx_QDCTRL)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 923
	// Enable de quadrature mode
	FTM1->QDCTRL |= FTM_QDCTRL_QUADEN_MASK;
	
	// Counter (FTMx_CNT)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 892
	// FTM Counter Value - reset counter to zero
	FTM1->CNT = 0x0; //FTM Counter Value - reset counter to zero
	
	// Modulo (FTMx_MOD)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 892
	//Set the overflow rate
	FTM1->MOD=(PERIPHERAL_BUS_CLOCK/(1<<FTM1_CLK_PRESCALE))/FTM1_OVERFLOW_FREQUENCY;
	
	// Counter Initial Value (FTMx_CNTIN)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 897
	//Set the Counter Initial Value to 0
	FTM1->CNTIN = 0;
	
	// Channel (n) Status and Control (FTMx_CSC)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 893
	// CHIE=0, Disable channel interrupts. Use software polling
	// MSB/MSA/ELSB/ELSA --> Edge-aligned PWM
	// DMA=0, Disable DMA transfers
	FTM1->CONTROLS[0].CnSC=0;
	FTM1->CONTROLS[0].CnSC|=(FTM_CnSC_MSB_MASK|FTM_CnSC_ELSB_MASK);
	FTM1->CONTROLS[1].CnSC=0;
	FTM1->CONTROLS[1].CnSC|=(FTM_CnSC_MSB_MASK|FTM_CnSC_ELSB_MASK);
	
	// Status and Control (FTMx_SC)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 890
	// Start the timer clock, source is the external clock
	FTM1->SC=FTM_SC_CLKS(3);
	
	// Channel (n) Value (FTMx_CV)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 896
	// FTMx_CnV contains the captured FTM counter value, this value determines the pulse width
	// Duty at 50%
	FTM1->CONTROLS[0].CnV = FTM1->MOD/2;
	FTM1->CONTROLS[0].CnV = FTM1->MOD/2;
	
	// Status and Control (FTMx_SC)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 890
	// CPWMS=0,Center-aligned PWM Select -> FTM counter operates in up counting mode
	// CLKS=1, Clock Source Selection -> System clock -> 50Mhz
	// PS:3,Prescale Factor Selection -> Divide by 8 
	FTM1->SC|=(FTM_SC_CLKS(1)|FTM_SC_PS(FTM1_CLK_PRESCALE));
	
	// FTM PWM Load (FTMx_PWMLOAD)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 933
	FTM1->PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;
	
	//------------------------------------------------------------
	// FTM2
	//------------------------------------------------------------
	// Features Mode Selection (FTMx_MODE)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 900
	// Disable Write Protection - enables changes to QUADEN, DECAPEN, etc.  
	FTM2->MODE |= FTM_MODE_WPDIS_MASK;
	//FTMEN is bit 0, need to set to zero so DECAPEN can be set to 0
	FTM2->MODE &= (~FTM_MODE_FTMEN_MASK);
	
	// Quadrature Decoder Control and Status (FTMx_QDCTRL)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 923
	// Set Edge Aligned PWM
	FTM2->QDCTRL &=~FTM_QDCTRL_QUADEN_MASK;
	
	// Counter (FTMx_CNT)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 892
	// FTM Counter Value - reset counter to zero
	FTM2->CNT = 0x0; //FTM Counter Value - reset counter to zero
	
	// Modulo (FTMx_MOD)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 892
	//Set the overflow rate
	FTM2->MOD=(PERIPHERAL_BUS_CLOCK/(1<<FTM2_CLK_PRESCALE))/FTM2_OVERFLOW_FREQUENCY;
	
	// Counter Initial Value (FTMx_CNTIN)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 897
	//Set the Counter Initial Value to 0
	FTM2->CNTIN = 0;
	
	// Channel (n) Status and Control (FTMx_CSC)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 893
	// CHIE=0, Disable channel interrupts. Use software polling
	// MSB/MSA/ELSB/ELSA --> Edge-aligned PWM
	// DMA=0, Disable DMA transfers
	FTM2->CONTROLS[0].CnSC=0;
	FTM2->CONTROLS[0].CnSC|=(FTM_CnSC_MSB_MASK|FTM_CnSC_ELSB_MASK);
	FTM2->CONTROLS[1].CnSC=0;
	FTM2->CONTROLS[1].CnSC|=(FTM_CnSC_MSB_MASK|FTM_CnSC_ELSB_MASK);
	
	// Status and Control (FTMx_SC)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 890
	// Edit registers when no clock is fed to timer so the MOD value, gets pushed in immediately
	FTM2->SC=0;
	
	// Channel (n) Value (FTMx_CV)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 896
	// FTMx_CnV contains the captured FTM counter value, this value determines the pulse width
	// Duty at 50%
	FTM2->CONTROLS[0].CnV = FTM2->MOD/2;
	FTM2->CONTROLS[0].CnV = FTM2->MOD/2;
	
	// Status and Control (FTMx_SC)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 890
	// CPWMS=0,Center-aligned PWM Select -> FTM counter operates in up counting mode
	// CLKS=1, Clock Source Selection -> System clock -> 50Mhz
	// PS:3,Prescale Factor Selection -> Divide by 8 
	FTM2->SC|=(FTM_SC_CLKS(1)|FTM_SC_PS(FTM2_CLK_PRESCALE));
	
	// FTM PWM Load (FTMx_PWMLOAD)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 933
	FTM2->PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;
}







