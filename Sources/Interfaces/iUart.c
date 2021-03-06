/*
------------------------------------------------------------
Copyright 2003-2012 Haute �cole ARC Ing�ni�rie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	iUart.c
Author and date :	Monnerat Serge 5 mars 2012

Description in the header (.h)

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $

-----------------------------------------------------------------------------
*/

#include "iUart.h"
#include "iPit.h"
#include "MK10D5.h"

// 200 bytes buffer
#define kUartRecBufSize ((UInt8)(100))


// RX buffer control struct
static struct
{
	Int8     SciRecBuf[kUartRecBufSize];
	UInt16 	  InIndex;
	UInt16 	  OutIndex;
	UInt16    ByteCount;
	bool      BufferIsFull;
}sUart0RecBufStruct;



//-----------------------------------------------------------------------
// UART 2 module configuration
//-----------------------------------------------------------------------
void iUart_Config(void)
{
	UInt16 aSbr;
	UInt8 aTmp;
	UInt16 aBrfa;
	
	// -------------------------------------------------------------------
	// UART0 --> RS485 clearfry inline
	// -------------------------------------------------------------------
	// UART Control Register 2 (UARTx_C2)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1266
	// Disable TX & RX before setup
	UART0->C2&= ~(UART_C2_TE_MASK|UART_C2_RE_MASK );
	// DUI0553A_cortex_m4_dgug.pdf page page 222 
	// Interrupt Clear-pending Registers
	// Clear prending interrupt
	NVIC_ClearPendingIRQ(UART0_RX_TX_IRQn);
	// DUI0553A_cortex_m4_dgug.pdf page page 220
	// Interrupt Set-enable Registers
	// Interrupt enable
	NVIC_EnableIRQ(UART0_RX_TX_IRQn);
	// DUI0553A_cortex_m4_dgug.pdf page page 223
	// Interrupt Priority Registers
	// Set interrupt priority
	NVIC_SetPriority(UART0_RX_TX_IRQn,kUart0_IntPriority);
	
	// UART Control Register 1 (UARTx_C1)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1263
	// Loop Mode Select=0,Normal operation 
	// UARTSWAI=0, UART clock continues to run in wait mode
	// RSRC=0,Selects internal loop back mode and receiver input is internally connected to transmitter output
	// M=0, Normal - start + 8 data bits (MSB/LSB first as determined by MSBF) + stop
	// WAKE=0,Idle-line wakeup
	// ILT=0,Idle character bit count starts after start bit
	// PE=0,Parity function disabled
	// PT=0,Even parity 
	UART0->C1= 0;	/* We need all default settings, so entire register is cleared */
	
	// UART Baud Rate Registers:High (UARTx_BDH)
	// UART Baud Rate Registers: Low (UARTx_BDL)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1262
	// Calculate baud rate settings
	aSbr = (UInt16)((kCoreClockCpukHz*1000)/(kUart0BaudRate * 16));
	// Save off the current value of the UARTx_BDH except for the SBR field
	aTmp = UART0->BDH & ~(UART_BDH_SBR(0x1F));
	UART0->BDH= aTmp |  UART_BDH_SBR(((aSbr & 0x1F00) >> 8));
	UART0->BDL= (UInt8)(aSbr & UART_BDL_SBR_MASK);
	
	// UART Control Register 4 (UARTx_C4)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1276
	// BRFA, Baud Rate Fine Adjust
	// Determine if a fractional divider is needed to get closer to the baud rate
	aBrfa = (((kCoreClockCpukHz*32000)/(kUart0BaudRate * 16)) - (aSbr * 32));
	// Save off the current value of the UARTx_C4 register except for the BRFA field */
	aTmp = UART0->C4 & ~(UART_C4_BRFA(0x1F));
	UART0->C4= aTmp | UART_C4_BRFA(aBrfa);
	
	// UART FIFO Parameters (UARTx_PFIFO)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1281
	// TXFE=1,Transmit FIFO is enabled. Buffer is depth indicted by TXFIFOSIZE
	// TXFIFOSIZE=3,Transmit FIFO/Buffer Depth = 16 Datawords 
	UART0->PFIFO=0;
	UART0->PFIFO|=(UART_PFIFO_TXFE_MASK|UART_PFIFO_TXFIFOSIZE(3));
	
	// UART FIFO Control Register (UARTx_CFIFO)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1283
	// Flush RX & TX fifo
	UART0->CFIFO|=(UART_CFIFO_TXFLUSH_MASK|UART_CFIFO_RXFLUSH_MASK);
	
	// UART Control Register 2 (UARTx_C2)
	// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1265
	// TIE=0, TDRE interrupt and DMA transfer requests disabled
	// TCIE=0, TC interrupt requests disabled
	// RIE=1,RDRF interrupt or DMA transfer requests enabled
	// ILIE=0,IDLE interrupt requests disabled
	// TE & RE =1, Enable receiver and transmitter
	UART0->C2=0;
	UART0->C2|= (UART_C2_TE_MASK|UART_C2_RE_MASK|UART_C2_RIE_MASK);
}

//------------------------------------------------------------
// Get Uartx flags state
// aStatus: which flag to read
// retour	: flag state
//------------------------------------------------------------
bool iUart_GetStatus(UARTEnum aUart,UartStatusEnum aStatus)
{
	bool aRet=false;
	
	if(aUart==kUart0)
		{
			// UART Status Register 1 (UARTx_S1)
			// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1267
			aRet=((UART0->S1&aStatus)==aStatus);
		}
	return aRet;
}

//------------------------------------------------------------
// Get Uartx fifo flags state
// aStatus: which flag to read
// retour	: flag state
//------------------------------------------------------------
bool iUart_GetFifoStatus(UARTEnum aUart,UartFifoStatusEnum aStatus)
{
	bool aRet=false;
	
	if(aUart==kUart0)
		{
			// UART Status Register 1 (UARTx_S1)
			// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1267
			aRet=((UART0->SFIFO&aStatus)==aStatus);
		}
	return aRet;
}


//------------------------------------------------------------
// Uartx Data register write
// aData: datas to transmit
//------------------------------------------------------------
void iUart_SetData(UARTEnum aUart,UInt8 aData)
{
	bool aRet=false;
	
	if(aUart==kUart0)
		{
			// UART Data Register (UARTx_D)
			// K10 Sub-Family Reference Manual, Rev. 6, Nov 2011 page 1274
			UART0->D=aData;
		}
}

//------------------------------------------------------------
// Check if the buffer is empty
// Return : true --> buffer empty, false --> buffer not empty
//------------------------------------------------------------
bool iUart_IsBufferEmpty(UARTEnum aUart)
{
	bool aRet=false;
		
	if(aUart==kUart0)
		{
			if(sUart0RecBufStruct.ByteCount==0)
				{
					aRet=true;
				}
			else
				{
					aRet=false;
				}
		}

	return aRet;
}


	
//------------------------------------------------------------
// Byte read in the RX buffer
// Return : oldest byte in the buffer
//------------------------------------------------------------
Int8 iUart_GetCharFromBuffer(UARTEnum aUart)
{
  Int8 aChar=0;
	
  if(aUart==kUart0)
  	{
			// Byte read
			aChar=sUart0RecBufStruct.SciRecBuf[sUart0RecBufStruct.OutIndex];
			
			// Index inc
			sUart0RecBufStruct.OutIndex++;
			
			// Turning buffer
			if(sUart0RecBufStruct.OutIndex>=kUartRecBufSize)
			{
				sUart0RecBufStruct.OutIndex=0;
			}
			
			// Byte counter dec
			sUart0RecBufStruct.ByteCount--;
  	}
	 
	// Retourne un byte du buffer
	return aChar;
}

//---------------------------------------------------------------------------
// UART 0 RX interrupt 
//---------------------------------------------------------------------------
void UART0_RX_TX_IRQHandler(void)
{
	//remise à 0 du flag RDRF
	//registre read-only, on le lit pour clearer
	uint8_t uarts1 = UART0->S1;// & UART_S1_RDRF_MASK;
	if(sUart0RecBufStruct.BufferIsFull==false)
	{
		sUart0RecBufStruct.SciRecBuf[sUart0RecBufStruct.InIndex] = UART0->D;
		sUart0RecBufStruct.ByteCount++;
		if((++sUart0RecBufStruct.InIndex) > kUartRecBufSize) sUart0RecBufStruct.InIndex = 0;
		if(sUart0RecBufStruct.InIndex == sUart0RecBufStruct.OutIndex) sUart0RecBufStruct.BufferIsFull = true;
	}
}

