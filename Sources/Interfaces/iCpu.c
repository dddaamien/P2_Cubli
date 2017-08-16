/*
------------------------------------------------------------
Copyright 2003-2016 Haute �cole ARC Ing�ni�rie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	iCpu.c
Author and date :	Monnerat Serge 11.06.2015

Description in the header file iCpu.h

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $

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
// External clock = 20MHz
// Core clock = 100MHz
// Peripheral bus = 50MHz
// Flexbus = 50MHz
// Flash = 25MHz
//------------------------------------------------------------
static void iCpu_PllInit(void)
{
	// If the internal load capacitors are being used, they should be selected
	// before enabling the oscillator. Application specific. 16pF and 8pF selected
	// in this example
	//OSC_CR = OSC_CR_SC16P_MASK | OSC_CR_SC8P_MASK;
 
	// Enable external oscillator, RANGE=2, HGO=1, EREFS=1, LP=0, IRCS=0
	MCG->C2 = MCG_C2_RANGE(2) | MCG_C2_HGO_MASK | MCG_C2_EREFS_MASK;
  
  // Select external oscilator and Reference Divider and clear IREFS to start ext osc
	// The clock should be between 31.25kHz & 39.0625kHz
	// --> 8MHz / 256 = 31.25kHz
  // CLKS=2, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0
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
  // 3 = divide factor -> 4 --> 8MHz/4 = 2MHz
  MCG->C5 = MCG_C5_PRDIV0(3);

  // Ensure MCG_C6 is at the reset default of 0. LOLIE disabled, PLL disabled, clk monitor disabled, PLL VCO divider is clear
  //MCG->C6 = 0x0;
  
  // Set the VCO divider and enable the PLL for 100MHz, LOLIE=0, PLLS=1, CME=0, VDIV=26 (2MHz * 50)
  MCG->C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV0(26);
 
  // Wait for PLL status bit to set
  while (!(MCG->S & MCG_S_PLLST_MASK));
  
  // Wait for LOCK bit to set
  while (!(MCG->S & MCG_S_LOCK0_MASK));
  
  // Set up the SIM clock dividers BEFORE switching to the PLL to ensure the
  // system clock speeds are in spec.
  // core = PLL (100MHz), bus = PLL/2 (50MHz), flexbus = PLL/2 (50MHz), flash = PLL/4 (25MHz)
  SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(1)| SIM_CLKDIV1_OUTDIV3(1) | SIM_CLKDIV1_OUTDIV4(3);
  
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
	SIM->SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;
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
	
	// ---------------------------------------------------------
	// PORTA --> pin 0 to 5 well configured out of reset --> JTAG
	// Pull-up enable
	// High drive strengh
	// PTA5 --> Diagnostique de la commande du relais de s�curit� de la cuve 1 du corps de chauffe--> Digital IN --> 1 = erreur
	PORTA->PCR[5] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTA6 --> Diagnostique de la commande du relais de s�curit� de la cuve 2 du corps de chauffe--> Digital IN --> 1 = erreur
	PORTA->PCR[6] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTA7 --> Diagnostique de la commande du relais de la pompe BIB (recharge huile: bag in a box) de la cuve 1--> Analog IN --> tension qui est une image du courant
	PORTA->PCR[7] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x0));
	// PTA8 --> Diagnostique de la commande du relais de la pompe BIB (recharge huile: bag in a box) de la cuve 2--> Analog IN --> tension qui est une image du courant
	PORTA->PCR[8] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x0));
	// PTA9 --> Diagnostique de la commande de l'�lectrovanne BIB (recharge huile: bag in a box) de la cuve 1--> Digital IN --> 1 = erreur
	PORTA->PCR[9] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTA10 --> Signal A en quadrature de l'actuateur de levage de la cuve 2--> Input Capture FTM2 channel 0, QD phase A
	PORTA->PCR[10] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x3));
	// PTA11 --> Signal B en quadrature de l'actuateur de levage de la cuve 2--> Input Capture FTM2 channel 1, QD phase B 
	PORTA->PCR[11] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x3));
	// PTA12 --> Signal A en quadrature de l'actuateur de levage de la cuve 1--> Input Capture FTM1 channel 0, QD phase A 
	PORTA->PCR[12] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x3));
	// PTA13 --> Signal B  en quadrature de l'actuateur de levage de la cuve 1--> Input Capture FTM1 channel 0, QD phase B 
	PORTA->PCR[13] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x3));
	
	// PTA14 --> Commande fil noire l'�lectrovanne BIB (recharge huile: bag in a box) de la cuve 1 --> Digital OUT 
	PORTA->PCR[14] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTA15 --> Commande fil rouge de l'�lectrovanne BIB (recharge huile: bag in a box) de la cuve 1 --> Digital OUT 
	PORTA->PCR[15] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTA16 --> Etat fil bleu de l'�lectrovanne BIB (recharge huile: bag in a box) de la cuve 1 --> Digital IN
	PORTA->PCR[16] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTA17 --> Etat fil blanc de l'�lectrovanne BIB (recharge huile: bag in a box) de la cuve 1 --> Digital IN
	PORTA->PCR[17] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTA24 --> Diagnostique de la commande de l'�lectrovanne BIB (recharge huile: bag in a box) de la cuve 2--> Digital IN --> 1 = erreur
	PORTA->PCR[24] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTA25 --> Commande fil noire l'�lectrovanne BIB (recharge huile: bag in a box) de la cuve 2 --> Digital OUT 
	PORTA->PCR[25] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTA26 --> Commande fil rouge de l'�lectrovanne BIB (recharge huile: bag in a box) de la cuve 2 --> Digital OUT 
	PORTA->PCR[26] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTA27 --> Etat fil bleu de l'�lectrovanne BIB (recharge huile: bag in a box) de la cuve 2 --> Digital IN
	PORTA->PCR[27] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTA28 --> Etat fil blanc de l'�lectrovanne BIB (recharge huile: bag in a box) de la cuve 2 --> Digital IN
	PORTA->PCR[28] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTA29 --> Interrupt du circuit I2C IO suppl�mentaires --> Digital IN --> interrupt si changement sur les entr�es
	PORTA->PCR[29] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	
	// ---------------------------------------------------------
	// PORTB
	// Pull-up enable
	// High drive strengh
		
	// PTB0 --> Diagnostique de la commande du relais de la pompe remplissage (recharge huile: bag in a box) de la cuve 1--> Analog IN --> tension qui est une image du courant
	PORTB->PCR[0] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x0));
	// PTB1 --> Diagnostique de la commande du relais de la pompe remplissage (recharge huile: bag in a box) de la cuve 2--> Analog IN --> tension qui est une image du courant
	PORTB->PCR[1] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x0));
	// PTB2 --> Test point --> Digital OUT 
	PORTB->PCR[2] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTB3 --> Mesure de temp�rature s�lectionn�e par le switch analogique--> Analog IN
	PORTB->PCR[3] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x0));
	// PTB4 --> Mesure du courant de la phase R du corps de chauffe de la cuve 1 --> Analog IN
	PORTB->PCR[4] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x0));
	// PTB5 --> Mesure du courant de la phase S du corps de chauffe de la cuve 1 --> Analog IN
	PORTB->PCR[5] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x0));
	// PTB6 --> Mesure du courant de la phase T du corps de chauffe de la cuve 1 --> Analog IN
	PORTB->PCR[6] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x0));
	// PTB7 --> Mesure du courant de la phase R du corps de chauffe de la cuve 2 --> Analog IN
	PORTB->PCR[7] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x0));
	
	// PTB8 --> Diagnostique de la commande du relais de travail de la cuve 1 du corps de chauffe--> Digital IN --> 1 = erreur
	PORTB->PCR[8] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTB9 --> Diagnostique de la commande du relais de travail de la cuve 2 du corps de chauffe--> Digital IN --> 1 = erreur
	PORTB->PCR[9] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	
	// PTB10 --> Mesure du courant de la phase S du corps de chauffe de la cuve 2 --> Analog IN
	PORTB->PCR[10] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x0));
	// PTB11 --> Mesure du courant de la phase T du corps de chauffe de la cuve 2 --> Analog IN
	PORTB->PCR[11] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x0));
	// PTB16 --> UART0 RX --> RS485 clearfry inline
	PORTB->PCR[16] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x3));
	// PTB17 --> UART0 TX --> RS485 clearfry inline
	PORTB->PCR[17] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x3));
	// PTB18 --> RS485 Transmit/Receive select --> digital out
	PORTB->PCR[18] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTB19 --> Diagnostique de la commande de l'�lectrovanne de remplissage de la cuve 1--> Digital IN --> 1 = erreur
	PORTB->PCR[19] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTB20 --> Commande fil noire l'�lectrovanne de remplissage de la cuve 1 --> Digital OUT 
	PORTB->PCR[20] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTB21 --> Commande fil rouge de l'�lectrovanne de remplissage de la cuve 1 --> Digital OUT 
	PORTB->PCR[21] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTB22 --> Etat fil bleu de l'�lectrovanne de remplissage de la cuve 1 --> Digital IN
	PORTB->PCR[22] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTB23 --> Etat fil blanc de l'�lectrovanne de remplissage de la cuve 1 --> Digital IN
	PORTB->PCR[23] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	
	// ---------------------------------------------------------
	// PORTC
	// Pull-up enable
	// High drive strengh
	// PTC0 --> Digital Output
	PORTC->PCR[0] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTC1 --> Digital Output
	PORTC->PCR[1] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTC2 --> Digital Output
	PORTC->PCR[2] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTC3 --> Digital Output
	PORTC->PCR[3] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTC4 --> Digital Output
	PORTC->PCR[4] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTC5 --> Digital Output
	PORTC->PCR[5] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTC6 --> Digital Output
	PORTC->PCR[6] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTC7 --> Digital Output
	PORTC->PCR[7] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTC8 --> Digital Output
	PORTC->PCR[8] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTC9 --> Digital Output
	PORTC->PCR[9] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTC10 --> I2C 1 SCL --> IO supp TLC59108 tank1 & tank2
	PORTC->PCR[10] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_ODE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x2));
	// PTC11 --> I2C 1 SDA --> IO supp TLC59108 tank1 & tank2
	PORTC->PCR[11] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_ODE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x2));
	// PTC12 --> Etat du relay de s�curit� de la cuve 1 --> Digital IN
	PORTC->PCR[12] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTC13 --> Etat du relay de s�curit� de la cuve 2 --> Digital IN
	PORTC->PCR[13] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTC14 --> Etat du relay de travail de la cuve 1 --> Digital IN
	PORTC->PCR[14] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTC15 --> Etat du relay de travail de la cuve 2 --> Digital IN
	PORTC->PCR[15] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTC16 --> UART3 RX --> RS232 display
	PORTC->PCR[16] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x3));
	// PTC17 --> UART3 TX --> RS232 display
	PORTC->PCR[17] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x3));
	// PTC18 --> Commande du relais de travail de la cuve 1 --> Digital OUT 
	PORTC->PCR[18] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTC19 --> Commande du relais de travail de la cuve 2 --> Digital OUT 
	PORTC->PCR[19] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	
	// ---------------------------------------------------------
	// PORTD
	// Pull-up enable
	// High drive strengh
	// PTD0 --> SPI0 PCS0 --> PT100 interface MAX31865ATP+
	PORTD->PCR[0] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x2));
	// PTD1 --> SPI0 SCK --> PT100 interface MAX31865ATP+
	PORTD->PCR[1] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x2));
	// PTD2 --> SPI0 SOUT --> PT100 interface MAX31865ATP+
	PORTD->PCR[2] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x2));
	// PTD3 --> SPI0 SIN --> PT100 interface MAX31865ATP+
	PORTD->PCR[3] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x2));
	// PTD4 --> SPI0 PCS1 --> PT100 interface MAX31865ATP+
	PORTD->PCR[4] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x2));
	// PTD7 --> PT100 interface MAX31865ATP+ data ready Tank1 --> Digital IN
	PORTD->PCR[7] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTD10 --> PT100 interface MAX31865ATP+ data ready Tank2 --> Digital IN
	PORTD->PCR[10] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTD8 --> I2C 0 SCL --> AD5933
	PORTD->PCR[8] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_ODE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x2));
	// PTD9 --> I2C 0 SDA --> AD5933
	PORTD->PCR[9] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_ODE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x2));
	// PTD11 --> Diagnostique de la commande de l'�lectrovanne de remplissage de la cuve 2--> Digital IN --> 1 = erreur
	PORTD->PCR[11] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTD12 --> Commande fil noire l'�lectrovanne de remplissage de la cuve 2 --> Digital OUT 
	PORTD->PCR[12] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTD13 --> Commande fil rouge de l'�lectrovanne de remplissage de la cuve 2 --> Digital OUT 
	PORTD->PCR[13] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTD14 --> Etat fil bleu de l'�lectrovanne de remplissage de la cuve 2 --> Digital IN
	PORTD->PCR[14] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTD15 --> Etat fil blanc de l'�lectrovanne de remplissage de la cuve 2 --> Digital IN
	PORTD->PCR[15] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	
	// ---------------------------------------------------------
	// PORTE
	// Pull-up enable
	// High drive strengh
	// PTE0 --> SDHC
	PORTE->PCR[0] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x4));
	// PTE1 --> SDHC
	PORTE->PCR[1] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x4));
	// PTE2 --> SDHC
	PORTE->PCR[2] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x4));
	// PTE3 --> SDHC
	PORTE->PCR[3] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x4));
	// PTE4 --> SDHC
	PORTE->PCR[4] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x4));
	// PTE5 --> SDHC
	PORTE->PCR[5] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x4));
	// PTE6 --> SDHC card detect
	PORTE->PCR[6] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));

	
	// PTE7 --> Diagnostique de la commande de l'�lectrovanne de filtrage de la cuve 1--> Digital IN --> 1 = erreur
	PORTE->PCR[7] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTE8 --> Commande fil noire l'�lectrovanne de filtrage de la cuve 1 --> Digital OUT 
	PORTE->PCR[8] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTE9 --> Commande fil rouge de l'�lectrovanne de filtrage de la cuve 1 --> Digital OUT 
	PORTE->PCR[9] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTE10 --> Etat fil bleu de l'�lectrovanne de filtrage de la cuve 1 --> Digital IN
	PORTE->PCR[10] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTE11 --> Etat fil blanc de l'�lectrovanne de filtrage de la cuve 1 --> Digital IN
	PORTE->PCR[11] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	
	// PTE12 --> Diagnostique de la commande de l'�lectrovanne de filtrage de la cuve 2--> Digital IN --> 1 = erreur
	PORTE->PCR[12] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTE16 --> Commande fil noire l'�lectrovanne de filtrage de la cuve 2 --> Digital OUT 
	PORTE->PCR[16] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTE17 --> Commande fil rouge de l'�lectrovanne de filtrage de la cuve 2 --> Digital OUT 
	PORTE->PCR[17] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTE18 --> Etat fil bleu de l'�lectrovanne de filtrage de la cuve 2 --> Digital IN
	PORTE->PCR[18] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTE19 --> Etat fil blanc de l'�lectrovanne de filtrage de la cuve 2 --> Digital IN
	PORTE->PCR[19] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTE24 --> UART4 RX --> RS232 debug
	PORTE->PCR[24] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x3));
	// PTE25 --> UART4 TX --> RS232 debug
	PORTE->PCR[25] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x3));
	
	// PTE26 --> Commande du relais de s�curit� de la cuve 1 --> Digital OUT 
	PORTE->PCR[26] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTE27 --> Commande du relais de s�curit� de la cuve 2 --> Digital OUT 
	PORTE->PCR[27] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
	// PTE28 --> Reset_n --> Digital OUT 
	PORTE->PCR[28] = 0|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_DSE_MASK|PORT_PCR_PFE_MASK|(PORT_PCR_MUX(0x1));
}

//------------------------------------------------------------
// Peripherals clock init
//------------------------------------------------------------
void iCpu_PeriphClkInit(void)
{
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 269
	// System Clock Gating Control Register 2 (SIM_SCGC2)
		SIM->SCGC1|=(SIM_SCGC1_UART4_MASK);
			
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 270
	// System Clock Gating Control Register 3 (SIM_SCGC3)
		SIM->SCGC3|=(SIM_SCGC3_ADC1_MASK|SIM_SCGC3_FTM2_MASK);
	
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 271
	// System Clock Gating Control Register 4 (SIM_SCGC4)
	SIM->SCGC4|=(SIM_SCGC4_UART3_MASK|SIM_SCGC4_UART0_MASK|SIM_SCGC4_I2C0_MASK|SIM_SCGC4_I2C1_MASK);
	
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 273
	// System Clock Gating Control Register 5 (SIM_SCGC5)
	SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK |
								SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | 
								SIM_SCGC5_PORTE_MASK );
	
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 275
	// System Clock Gating Control Register 6 (SIM_SCGC6)
	SIM->SCGC6|=(SIM_SCGC6_RTC_MASK|SIM_SCGC6_FTM1_MASK|SIM_SCGC6_ADC0_MASK|SIM_SCGC6_PIT_MASK|SIM_SCGC6_SPI0_MASK|SIM_SCGC6_FTFL_MASK);
	
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 278
	// System Clock Gating Control Register 7 (SIM_SCGC7)
	SIM->SCGC7|=SIM_SCGC7_DMA_MASK;
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
	
	// Disable MPU
	MPU->CESR &= (~MPU_CESR_VLD_MASK);
	
	// Core and peripheral clock init
	iCpu_PllInit();
	
	// K10 pin functions configuration
	iCpu_PinFctCfg();
	
	// Internal voltage reference init (VREF0)
	iCpu_VREFInit();
}
