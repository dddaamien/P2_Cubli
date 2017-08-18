/*
------------------------------------------------------------
Copyright 2003-2016 Haute �cole ARC Ing�ni�rie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	iDio.c
Author and date :	Monnerat Serge 11.06.2015

Description in the header (.h)

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $

-----------------------------------------------------------------------------
*/

#include "iDio.h"
#include "MK10D5.h"
#include "core_cm4.h"

//-----------------------------------------------------------------------------
// IO ports direction setup
// aPort: which IO port
// aMask: which port pin(s) to configure
// aDir:  input or output
//-----------------------------------------------------------------------------
void iDio_SetPortDirection(PortIOEnum aPort,IoMaskEnum aMask,IoDirectionEnum aDir)
{
	UInt32 *aPortPtr;
  
  // Port Data Direction Register (GPIOx_PDDR)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1528
  switch(aPort)
    {
      // GPIOA_PDDR
      case kPortA:
        aPortPtr=(UInt32 *)&PTA->PDDR;
      break;
      
      // GPIOB_PDDR
      case kPortB:
        aPortPtr=(UInt32 *)&PTB->PDDR;
      break;
      
      // GPIOC_PDDR
			case kPortC:
				aPortPtr=(UInt32 *)&PTC->PDDR;
			break;
			
			// GPIOD_PDDR
			case kPortD:
				aPortPtr=(UInt32 *)&PTD->PDDR;
			break;
      // GPIOE_PDDR
      case kPortE:
        aPortPtr=(UInt32 *)&PTE->PDDR;
      break;
    }
    
  // IO direction setup 
  if(kIoOutput==aDir)
    {
      *aPortPtr|=aMask;
    }
  else if(kIoInput==aDir)
    {
      *aPortPtr &=~aMask;
    }
}

//-----------------------------------------------------------------------------
// Output pin set state
// aPort: which IO port
// aMask: which port pin(s) to configure
// aState:  output pin state
//-----------------------------------------------------------------------------
void iDio_SetPort(PortIOEnum aPort,IoMaskEnum aMask,IoStateEnum aState)
{
	UInt32 *aPortSetPtr;
	UInt32 *aPortClrPtr;
  
	// Port Data Output Register (GPIOx_PDOR)
	// Port Clear Output Register (GPIOx_PCOR)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1528
	switch(aPort)
		{
			// GPIOA_PDOR & GPIOA_PCOR
			case kPortA:
				aPortSetPtr=(UInt32 *)&PTA->PSOR;
				aPortClrPtr=(UInt32 *)&PTA->PCOR;
			break;
			
			// GPIOB_PDOR & GPIOB_PCOR
			case kPortB:
				aPortSetPtr=(UInt32 *)&PTB->PSOR;
				aPortClrPtr=(UInt32 *)&PTB->PCOR;
			break;
			
			// GPIOC_PDOR & GPIOC_PCOR
			case kPortC:
				aPortSetPtr=(UInt32 *)&PTC->PSOR;
				aPortClrPtr=(UInt32 *)&PTC->PCOR;
			break;
			
			// GPIOD_PDOR & GPIOD_PCOR
			case kPortD:
				aPortSetPtr=(UInt32 *)&PTD->PSOR;
				aPortClrPtr=(UInt32 *)&PTD->PCOR;
			break;
			// GPIOE_PDOR & GPIOE_PCOR
			case kPortE:
				aPortSetPtr=(UInt32 *)&PTE->PSOR;
				aPortClrPtr=(UInt32 *)&PTE->PCOR;
			break;
		}
    
  // Set or reset the output
  if(kIoOff==aState)
    {
      *aPortClrPtr |= aMask;
    }
  else
    {
      *aPortSetPtr |= aMask;
    }
}

//-----------------------------------------------------------------------------
// Read input pin state
// aPort: which IO port
// aMask: which port pin(s) to configure 
// return:  input pin state
//-----------------------------------------------------------------------------
bool iDio_GetPort(PortIOEnum aPort, IoMaskEnum aMask)
{
  bool  aRet=false;
  UInt32 *aPortPtr;
  
  // Port Data Input Register (GPIOx_PDIR)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1528
	switch(aPort)
		{
			// GPIOA_PDIR
			case kPortA:
				aPortPtr=(UInt32 *)&PTA->PDIR;
			break;
			
			// GPIOB_PDIR
			case kPortB:
				aPortPtr=(UInt32 *)&PTB->PDIR;
			break;
			
			// GPIOC_PDIR
			case kPortC:
				aPortPtr=(UInt32 *)&PTC->PDIR;
			break;
			
			// GPIOD_PDIR
			case kPortD:
				aPortPtr=(UInt32 *)&PTD->PDIR;
			break;
			// GPIOE_PDIR
			case kPortE:
				aPortPtr=(UInt32 *)&PTE->PDIR;
			break;
		}
    
  // Input pin state
  aRet=(bool)((*aPortPtr&aMask)==aMask);
  
  return aRet;
}

//-----------------------------------------------------------------------------
// Set PORTA interrupt
// aPinNumber: portA pin number 
// aPriority: interrupt priority
// aMode: state, rising or falling edge 
//-----------------------------------------------------------------------------
void iDio_SetPortAInt(UInt16 aPinNumber, UInt16 aPriority,IOIrqModeEnum aMode)
{
	UInt32 aTmp;
	
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1018
	// Pin Control Register n (PORTx_PCRn)
	// Enable & setup PTA0 interrupt
	PORTA->PCR[aPinNumber]&=(~PORT_PCR_IRQC_MASK);
	PORTA->PCR[aPinNumber]|=PORT_PCR_IRQC(aMode);

	// DUI0553A_cortex_m4_dgug.pdf page page 222 
	// Interrupt Clear-pending Registers
	// Clear prending interrupt
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	// DUI0553A_cortex_m4_dgug.pdf page page 220
	// Interrupt Set-enable Registers
	// Interrupt enable
	NVIC_EnableIRQ(PORTA_IRQn);
	// DUI0553A_cortex_m4_dgug.pdf page page 223
	// Interrupt Priority Registers
	// Set interrupt priority
	NVIC_SetPriority(PORTA_IRQn,aPriority);
}
