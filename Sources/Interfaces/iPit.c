/*
------------------------------------------------------------
Copyright 2003-2012 Haute �cole ARC Ing�ni�rie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	iPit.c
Author and date :	Monnerat Serge 16 f�vr. 2012

Description in the header (.h)

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $

-----------------------------------------------------------------------------
*/

#include "iPit.h"
#include "MK10D5.h"
#include "core_cm4.h"


// Maximum delay number to use together
#define kCounterNumber 10

// Delay struct
typedef struct
{
	UInt16 Counter;
	bool isFree;
	bool DelayDone;
}CounterStruct;

typedef struct
{
	CounterStruct CounterTab[kCounterNumber];
}DlyStruct;

static DlyStruct sDly[3];

//------------------------------------------------------------
// PIT setup
// aPit					: which PIT (0 to 3)
// aPeriodeMs  	: PIT period in ms
//------------------------------------------------------------
void iPit_Config(PitEnum aPit,UInt16 aPeriodeMs)
{
	static UInt32 aTmp=0;
		UInt8 i=0;
		
		// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1016
		// PIT Module Control Register (PIT_MCR)
		// --> Timers are stopped in debug mode
		PIT->MCR|=PIT_MCR_FRZ_MASK;
		
		// PIT interrupts setup
		if(aPit==kPit0)
			{
				// Counting value to do the delay
				// Counting Value=Delay Time/(1/Core clock)
				aTmp=aPeriodeMs/(1/kBusClock)*1000;
				// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1017
				// Timer Load Value Register (PIT_LDVALn)
				PIT->CHANNEL[0].LDVAL=aTmp;
				// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1018
				// Timer Control Register (PIT_TCTRLn)
				// Enable PIT interrupt
				PIT->CHANNEL[0].TCTRL|=PIT_TCTRL_TIE_MASK;
				// DUI0553A_cortex_m4_dgug.pdf page page 222 
				// Interrupt Clear-pending Registers
				// Clear prending interrupt
				NVIC_ClearPendingIRQ(PIT0_IRQn);
				// DUI0553A_cortex_m4_dgug.pdf page page 220
				// Interrupt Set-enable Registers
				// Interrupt enable
				NVIC_EnableIRQ(PIT0_IRQn);
				// DUI0553A_cortex_m4_dgug.pdf page page 223
				// Interrupt Priority Registers
				// Set interrupt priority
				NVIC_SetPriority(PIT0_IRQn,kPIT_IntPriority);
			}
		else if(aPit==kPit1) 
			{
				// Counting value to do the delay
				// Counting Value=Delay Time/(1/Core clock)
				aTmp=aPeriodeMs/(1/kBusClock)*1000;
				// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1017
				// Timer Load Value Register (PIT_LDVALn)
				PIT->CHANNEL[1].LDVAL=aTmp;
				// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1018
				// Timer Control Register (PIT_TCTRLn)
				// Enable PIT interrupt
				PIT->CHANNEL[1].TCTRL|=PIT_TCTRL_TIE_MASK;
				// DUI0553A_cortex_m4_dgug.pdf page page 222 
				// Interrupt Clear-pending Registers
				// Clear prending interrupt
				NVIC_ClearPendingIRQ(PIT1_IRQn);
				// DUI0553A_cortex_m4_dgug.pdf page page 220
				// Interrupt Set-enable Registers
				// Interrupt enable
				NVIC_EnableIRQ(PIT1_IRQn);
				// DUI0553A_cortex_m4_dgug.pdf page page 223
				// Interrupt Priority Registers
				// Set interrupt priority
				NVIC_SetPriority(PIT1_IRQn,kPIT_IntPriority);
			}
		else if(aPit==kPit2)
			{
				// Counting value to do the delay
				// Counting Value=Delay Time/(1/Core clock)
				aTmp=aPeriodeMs/(1/kBusClock)*1000;
				// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1017
				// Timer Load Value Register (PIT_LDVALn)
				PIT->CHANNEL[2].LDVAL=aTmp;
				// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1018
				// Timer Control Register (PIT_TCTRLn)
				// Enable PIT interrupt
				PIT->CHANNEL[2].TCTRL|=PIT_TCTRL_TIE_MASK;
				// DUI0553A_cortex_m4_dgug.pdf page page 222 
				// Interrupt Clear-pending Registers
				// Clear prending interrupt
				NVIC_ClearPendingIRQ(PIT2_IRQn);
				// DUI0553A_cortex_m4_dgug.pdf page page 220
				// Interrupt Set-enable Registers
				// Interrupt enable
				NVIC_EnableIRQ(PIT2_IRQn);
				// DUI0553A_cortex_m4_dgug.pdf page page 223
				// Interrupt Priority Registers
				// Set interrupt priority
				NVIC_SetPriority(PIT2_IRQn,kPIT_IntPriority);
			}
		else if(aPit==kPit3)
			{
				// Counting value to do the delay
				// Counting Value=Delay Time/(1/Core clock)
				aTmp=aPeriodeMs/(1/kBusClock)*1000;
				// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1017
				// Timer Load Value Register (PIT_LDVALn)
				PIT->CHANNEL[3].LDVAL=aTmp;
				// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1018
				// Timer Control Register (PIT_TCTRLn)
				// Enable PIT interrupt
				PIT->CHANNEL[3].TCTRL|=PIT_TCTRL_TIE_MASK;
				// DUI0553A_cortex_m4_dgug.pdf page page 222 
				// Interrupt Clear-pending Registers
				// Clear prending interrupt
				NVIC_ClearPendingIRQ(PIT3_IRQn);
				// DUI0553A_cortex_m4_dgug.pdf page page 220
				// Interrupt Set-enable Registers
				// Interrupt enable
				NVIC_EnableIRQ(PIT3_IRQn);
				// DUI0553A_cortex_m4_dgug.pdf page page 223
				// Interrupt Priority Registers
				// Set interrupt priority
				NVIC_SetPriority(PIT3_IRQn,kPIT_IntPriority);
			}
		
		// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1016
		// PIT Module Control Register (PIT_MCR)
		// Clock for PIT Timers is enabled
		PIT->MCR&=(~PIT_MCR_MDIS_MASK);
}


//------------------------------------------------------------
// Start Pit
// aPit		: which PIT
//------------------------------------------------------------
void iPit_StartPit(PitEnum aPit)
{
	// Start PIT
	if(aPit==kPit0)
		{
			// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1018
			// Timer Control Register (PIT_TCTRLn)
			// Timer is active
			PIT->CHANNEL[0].TCTRL|=PIT_TCTRL_TEN_MASK;
		}
	else if(aPit==kPit1)
		{
			// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1018
			// Timer Control Register (PIT_TCTRLn)
			// Timer is active
			PIT->CHANNEL[1].TCTRL|=PIT_TCTRL_TEN_MASK;
		}
	else if(aPit==kPit2)
		{
			// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1018
			// Timer Control Register (PIT_TCTRLn)
			// Timer is active
			PIT->CHANNEL[2].TCTRL|=PIT_TCTRL_TEN_MASK;
		}
	else if(aPit==kPit3)
		{
			// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1018
			// Timer Control Register (PIT_TCTRLn)
			// Timer is active
			PIT->CHANNEL[3].TCTRL|=PIT_TCTRL_TEN_MASK;
		}
}

//------------------------------------------------------------
// Stop Pit
// aPit		: which PIT
//------------------------------------------------------------
void iPit_StopPit(PitEnum aPit)
{
	// Start PIT
	if(aPit==kPit0)
		{
			// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1018
			// Timer Control Register (PIT_TCTRLn)
			// Timer is active
			PIT->CHANNEL[0].TCTRL&=(~PIT_TCTRL_TEN_MASK);
		}
	else if(aPit==kPit1)
		{
			// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1018
			// Timer Control Register (PIT_TCTRLn)
			// Timer is active
			PIT->CHANNEL[1].TCTRL&=(~PIT_TCTRL_TEN_MASK);
		}
	else if(aPit==kPit2)
		{
			// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1018
			// Timer Control Register (PIT_TCTRLn)
			// Timer is active
			PIT->CHANNEL[2].TCTRL&=(~PIT_TCTRL_TEN_MASK);
		}
	else if(aPit==kPit3)
		{
			// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1018
			// Timer Control Register (PIT_TCTRLn)
			// Timer is active
			PIT->CHANNEL[3].TCTRL&=(~PIT_TCTRL_TEN_MASK);
		}
}
//------------------------------------------------------------
// Setup the delay structure
// aPit		: which PIT
//------------------------------------------------------------
void iPit_InitDelay(PitEnum aPit)
{
	UInt16 i=0;
	
	for(i=0;i<kCounterNumber;i++)
		{
			sDly[aPit].CounterTab[i].DelayDone=false;
			sDly[aPit].CounterTab[i].Counter=0;
			sDly[aPit].CounterTab[i].isFree=true;
		}
}
//------------------------------------------------------------
// Get and setup a delay
// aPit		: which PIT
// aDelay	: the delay (ms)
// Retour	: the delay number
//------------------------------------------------------------
Int16 iPit_GetDelay(PitEnum aPit,UInt16 aDelay)
{
	Int16 aDelayNb=0;
	UInt16 i=0;
	
	// By default
	aDelayNb=0;
	
	// Find and confegure a free delay
	for(i=0;(i<kCounterNumber)&&(false==sDly[aPit].CounterTab[i].isFree);i++,aDelayNb=i)
		;
	
	// Check if a delay was found
	if((i==kCounterNumber)&&(false==sDly[aPit].CounterTab[i-1].isFree))
		aDelayNb=-1;
	else
		{
			// Delay setup
			sDly[aPit].CounterTab[aDelayNb].isFree=false;
			sDly[aPit].CounterTab[aDelayNb].Counter=aDelay;
			sDly[aPit].CounterTab[aDelayNb].DelayDone=false;
		}
			
	return aDelayNb;
} 

//------------------------------------------------------------
// Check if the delay is done
// aPit			: which PIT
// aDelayNb	: the delay number
// Retour		: state of the delay
//------------------------------------------------------------
bool iPit_IsDelayDone(PitEnum aPit,UInt16 aDelayNb)
{
	return sDly[aPit].CounterTab[aDelayNb].DelayDone;
}

//------------------------------------------------------------
// Release a delay
// aPit			: which PIT
// aDelayNb	: the delay number
//------------------------------------------------------------
void iPit_DelayRelease(PitEnum aPit,UInt16 aDelayNb)
{
	sDly[aPit].CounterTab[aDelayNb].DelayDone=false;
	sDly[aPit].CounterTab[aDelayNb].Counter=0;
	sDly[aPit].CounterTab[aDelayNb].isFree=true;
}

//------------------------------------------------------------
// Restart un delay
// aPit			: which PIT
// aDelayNb	: the delay number
// aDelay		: the delay time
//------------------------------------------------------------
void iPit_ReStart(PitEnum aPit,UInt16 aDelayNb,UInt16 aDelay)
{
	sDly[aPit].CounterTab[aDelayNb].DelayDone=false;
	sDly[aPit].CounterTab[aDelayNb].Counter=aDelay;
	sDly[aPit].CounterTab[aDelayNb].isFree=false;
}

//------------------------------------------------------------
// PIT 0 interrupt routine
//------------------------------------------------------------
void PIT0_IRQHandler(void)
{
	UInt16 i=0;
	UInt32 aTmp;
	
	// PIT TIF flag clear
	PIT->CHANNEL[0].TFLG|=PIT_TFLG_TIF_MASK;
	// ! errata e2682 !
	// Workaround: In order to enable the use of subsequent interrupts from the PIT, the user must access any PIT
	// register after clearing the interrupt flag in the ISR.?
	aTmp=PIT->CHANNEL[0].CVAL;
 	
 	// Counter update
	for(i=0;i<kCounterNumber;i++)
		{
			if((false==sDly[kPit0].CounterTab[i].isFree)&&(sDly[kPit0].CounterTab[i].Counter>0))
				sDly[kPit0].CounterTab[i].Counter--;
			
			if((false==sDly[kPit0].CounterTab[i].isFree)&&(sDly[kPit0].CounterTab[i].Counter==0))
				sDly[kPit0].CounterTab[i].DelayDone=true;
		}
}

////------------------------------------------------------------
//// PIT 1 interrupt routine
////------------------------------------------------------------
//void PIT1_IRQHandler(void)
//{
//	UInt16 i=0;
//	UInt32 aTmp;
//
//	// PIT TIF flag clear
//	PIT->CHANNEL[1].TFLG|=PIT_TFLG_TIF_MASK;
//	// ! errata e2682 !
//	// Workaround: In order to enable the use of subsequent interrupts from the PIT, the user must access any PIT
//	// register after clearing the interrupt flag in the ISR.?
//	aTmp=PIT->CHANNEL[1].CVAL;
//
// 	// Counter update
//	for(i=0;i<kCounterNumber;i++)
//		{
//			if((false==sDly[kPit1].CounterTab[i].isFree)&&(sDly[kPit1].CounterTab[i].Counter>0))
//				sDly[kPit1].CounterTab[i].Counter--;
//
//			if((false==sDly[kPit1].CounterTab[i].isFree)&&(sDly[kPit1].CounterTab[i].Counter==0))
//				sDly[kPit1].CounterTab[i].DelayDone=true;
//		}
//}

//------------------------------------------------------------
// PIT 2 interrupt routine
//------------------------------------------------------------
void PIT2_IRQHandler(void)
{
	UInt16 i=0;
	UInt32 aTmp;
	
	// PIT TIF flag clear
	PIT->CHANNEL[2].TFLG|=PIT_TFLG_TIF_MASK;
	// ! errata e2682 !
	// Workaround: In order to enable the use of subsequent interrupts from the PIT, the user must access any PIT
	// register after clearing the interrupt flag in the ISR.?
	aTmp=PIT->CHANNEL[2].CVAL;
 	
 	// Counter update
	for(i=0;i<kCounterNumber;i++)
		{
			if((false==sDly[kPit2].CounterTab[i].isFree)&&(sDly[kPit2].CounterTab[i].Counter>0))
				sDly[kPit2].CounterTab[i].Counter--;
			
			if((false==sDly[kPit2].CounterTab[i].isFree)&&(sDly[kPit2].CounterTab[i].Counter==0))
				sDly[kPit2].CounterTab[i].DelayDone=true;
		}
}

//------------------------------------------------------------
// PIT 3 interrupt routine
//------------------------------------------------------------
void PIT3_IRQHandler(void)
{
	UInt16 i=0;
	UInt32 aTmp;
	
	// PIT TIF flag clear
	PIT->CHANNEL[3].TFLG|=PIT_TFLG_TIF_MASK;
	// ! errata e2682 !
	// Workaround: In order to enable the use of subsequent interrupts from the PIT, the user must access any PIT
	// register after clearing the interrupt flag in the ISR.?
	aTmp=PIT->CHANNEL[3].CVAL;
 	
 	// Counter update
	for(i=0;i<kCounterNumber;i++)
		{
			if((false==sDly[kPit3].CounterTab[i].isFree)&&(sDly[kPit3].CounterTab[i].Counter>0))
				sDly[kPit3].CounterTab[i].Counter--;
			
			if((false==sDly[kPit3].CounterTab[i].isFree)&&(sDly[kPit3].CounterTab[i].Counter==0))
				sDly[kPit3].CounterTab[i].DelayDone=true;
		}
}
