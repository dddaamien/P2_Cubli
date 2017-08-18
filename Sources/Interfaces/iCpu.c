/*
------------------------------------------------------------
Copyright 2003-2016 Haute école ARC Ingénierie, Switzerland.
All rights reserved.
------------------------------------------------------------
File name :	iCpu.c
Author and date :	Monnerat Serge 11.06.2015

Description in the header file iCpu.h

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

 - 16.08.2017: Adaptation pour MK10D5

-----------------------------------------------------------------------------
*/

#include "iCpu.h"
#include "MK10D5.h"

// Peripherals clock init
static void iCpu_PeriphClkInit(void);
	
// Core and peripheral clock init
static void iCpu_PllInit(void);

// K10 pin functions configuration
static void iCpu_PinFctCfg(void);

// Internal voltage reference init (VREF0)
static void iCpu_VREFInit(void);

//------------------------------------------------------------
// Core and peripheral clock init
// External clock = 8MHz
// Core clock = 50MHz
// Peripheral bus = 25MHz
// Flash = 25MHz
//------------------------------------------------------------
static void iCpu_PllInit(void)
{
	// If the internal load capacitors are being used, they should be selected
	// before enabling the oscillator. Application specific. 16pF and 8pF selected
	// in this example
	//OSC_CR = OSC_CR_SC16P_MASK | OSC_CR_SC8P_MASK;
 
	// Enable external oscillator, RANGE=2(>=8MHz), HGO=1, EREFS=1, LP=0, IRCS=0
  MCG->C2 = MCG_C2_RANGE0(2) | MCG_C2_HGO0_MASK | MCG_C2_EREFS0_MASK;
  // Select external oscilator and Reference Divider and clear IREFS to start ext osc
	// The clock should be between 31.25kHz & 39.0625kHz
	// --> 8MHz / 256 = 31.25kHz
  // CLKS=2(ext osc), FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0
  MCG->C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(3);
  
  // Wait for oscillator to initialize
  while (!(MCG->S & MCG_S_OSCINIT0_MASK));
  
  // Wait for Reference clock Status bit to clear
  while (MCG->S & MCG_S_IREFST_MASK);
  
  // Wait for clock status bits to show clock source is ext ref clk
  while (((MCG->S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2);

  // Now in FBE

  // Configure PLL Ref Divider, PLLCLKEN=0, PLLSTEN=0, PRDIV=5
  // The crystal frequency is used to select the PRDIV value. Only even frequency crystals are supported
  // that will produce a 2MHz reference clock to the PLL.
  // 3 = divide factor -> 4 --> 8MHz/4 = 2MHz (doit être entre 2 et 4 MHz)
  MCG->C5 = MCG_C5_PRDIV0(3);

  // Ensure MCG_C6 is at the reset default of 0. LOLIE disabled, PLL disabled, clk monitor disabled, PLL VCO divider is clear
  //MCG->C6 = 0x0;
  
  // Set the VCO divider and enable the PLL for 50MHz, LOLIE=0, PLLS=1, CME=0, VDIV=26 (2MHz * 50)
  	  //2MHz*25
  MCG->C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV0(1);
 
  // Wait for PLL status bit to set
  while (!(MCG->S & MCG_S_PLLST_MASK));
  
  // Wait for LOCK bit to set
  while (!(MCG->S & MCG_S_LOCK0_MASK));
  
  // Set up the SIM clock dividers BEFORE switching to the PLL to ensure the
  // system clock speeds are in spec.
  // core = PLL (50MHz), bus = PLL/2 (25MHz), flash = PLL/4 (25MHz)
  SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(1) | SIM_CLKDIV1_OUTDIV4(1);
  
  // Now running PBE Mode

  // Transition into PEE by setting CLKS to 0
  // CLKS=0, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0
  MCG->C1 &= ~MCG_C1_CLKS_MASK;

  // Wait for clock status bits to update
  while (((MCG->S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3);

  // Now running PEE Mode
  
	// Set the trace clock to the core clock frequency 
	SIM->SOPT2 |= SIM_SOPT2_TRACECLKSEL_MASK;
	
	// Enable the clock to the FlexBus module
//	SIM->SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;
}

//------------------------------------------------------------
// K10 pin functions configuration
//------------------------------------------------------------
static void iCpu_PinFctCfg(void)
{
	// ---------------------------------------------------------
	// K10 pin functions configuration
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011
	// Pin Control Register n (PORTx_PCRn) page 244
	// By default:
	// Bit 0: PS --> Internal pull-down resistor is enabled on the corresponding pin, if the corresponding Port Pull Enable Register bit is set.
	// Bit 1: PE --> Internal pull-up or pull-down resistor is not enabled on the corresponding pin.
	// Bit 2: SRE --> Fast slew rate is configured on the corresponding pin, if pin is configured as a digital output.
	// Bit 3: Reserved 
	// Bit 4: PFE --> Passive Input Filter is disabled on the corresponding pin.
	// Bit 5: ODE --> Open Drain output is disabled on the corresponding pin.
	// Bit 6: DSE --> Low drive strength is configured on the corresponding pin, if pin is configured as a digital output.
	// Bit 7: Reserved 
	// Bit 8,9,10: MUX --> Pin function, see below
	// Bit 11,12,13,14: Reserved
	// Bit 15: LK --> Pin Control Register bits [15:0] are not locked.
	// Bit 16,17,18,19: IRQC --> Interrupt/DMA Request disabled.
	// Bit 20,21,22,23: Reserved
	// Bit 24: ISF --> Interrupt Status Flag
	// Bit 25,26,27,28,29,30,31: Reserved
	// ---------------------------------------------------------
	
	//GPIO out:
		//Leds capteurs
	PORTB->PCR[0] = PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(1);
	PORTB->PCR[1] = PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(1);
		//Mot break
	PORTC->PCR[3] = PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(1);
		//MotFwRv
	PORTD->PCR[4] = PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(1);
		//Leds
	PORTD->PCR[2] = PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(1);
	PORTD->PCR[3] = PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(1);
	PORTE->PCR[16] = PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(1);
	PORTE->PCR[17] = PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(1);
	//GPIO in:
		//Int capteurs
	PORTC->PCR[4] = PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(1);
	PORTC->PCR[5] = PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(1);
		//Sw2, Sw3, Sw0, Sw1
	PORTC->PCR[6] = PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(1);
	PORTC->PCR[7] = PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(1);
	PORTD->PCR[0] = PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(1);
	PORTD->PCR[1] = PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(1);
		//TachoHall3
	PORTD->PCR[5] = PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(1);
	//I2C0:
	PORTB->PCR[2] = PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(2);
	PORTB->PCR[3] = PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(2);
	//UART0
	PORTB->PCR[16] = PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(3);
	PORTB->PCR[17] = PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(3);
	//FTM0:
		//ServoPwm, MotTacho, MotVref
	PORTC->PCR[1] = PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(4);
	PORTC->PCR[2] = PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(4);
	PORTD->PCR[6] = PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(4);
}

//------------------------------------------------------------
// Peripherals clock init
//------------------------------------------------------------
void iCpu_PeriphClkInit(void)
{
	//I2C0, UART0
	SIM->SCGC4 = SIM_SCGC4_I2C0_MASK | SIM_SCGC4_UART0_MASK; //VREF
	//GPIO: ports B,C,D,E
	SIM->SCGC5 = SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;
	//FTFL, PIT, FTM0
	SIM->SCGC6 = SIM_SCGC6_FTFL_MASK | SIM_SCGC6_PIT_MASK | SIM_SCGC6_FTM0_MASK;
}

//------------------------------------------------------------
// Internal voltage reference init (VREF0)
//------------------------------------------------------------
static void iCpu_VREFInit(void)
{
	UInt8 *aVREFTRM=(UInt8*)(0x40074000);
	
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 851
	// VREF Status and Control Register (VREF_SC)
	// Voltage Reference enabled,VREF bandgap and tight regulation buffer on
	// VREF_OUT available for internal and external use
  VREF->SC |= (VREF_SC_VREFEN_MASK|(VREF_SC_MODE_LV(2)));
  
	*aVREFTRM|=0x3f;
	
	// Wait till the VREFSC is stable
	while (!(VREF->SC & VREF_SC_VREFST_MASK));
}


//------------------------------------------------------------
// CPU configuration
//------------------------------------------------------------
void iCpu_SysInit(void)
{
	// Peripherals clock init
	iCpu_PeriphClkInit();
	
	// Disable MPU TODO: voir si c'est nécessaire..
//	MPU->CESR &= (~MPU_CESR_VLD_MASK);
	
	// Core and peripheral clock init
	iCpu_PllInit();
	
	// K10 pin functions configuration
	iCpu_PinFctCfg();
	
	// Internal voltage reference init (VREF0)
//	iCpu_VREFInit();
}
