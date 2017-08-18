/*
------------------------------------------------------------
Copyright 2003-2010 Haute �cole ARC Ing�ni�rie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name : 	iUart.h	
Author and date :	Monnerat Serge 5 mars 2012

Goal : 

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $


-----------------------------------------------------------------------------
*/
#ifndef __iUart__
#define __iUart__

#include "def.h"

//------------------------------------------------------------
// Uart enum
//------------------------------------------------------------
typedef enum
{
	kUart0,
	kUart1,
	kUart2
}UARTEnum;

//------------------------------------------------------------
// UART status flag
//------------------------------------------------------------
typedef enum
{
	kSciTransmitDataRegEmpty=0x80,
	kSciTransmitComplete=0x40,
	kSciReceiveDataRegFull=0x20,
	kSciIdleLineDetect=0x10,
	kSciOverrunErr=0x08,
	kSciNoiseErr=0x04,
	kSciFramingErr=0x02,
	kSciParityErr=0x01
}UartStatusEnum;

//------------------------------------------------------------
// UART fifo status flag
//------------------------------------------------------------
typedef enum
{
	kTxFifoEmpty=0x80,
	kRxFifoEmpty=0x40,
	kTxFifoOverflow=0x02,
	kRxFifoUnderflow=0x01
}UartFifoStatusEnum;

//-----------------------------------------------------------------------
// UART 2 module configuration
//-----------------------------------------------------------------------
void iUart_Config(void);

//------------------------------------------------------------
// Get Uart2 flags state
// aStatus: which flag to read
// retour	: flag state
//------------------------------------------------------------
bool iUart_GetStatus(UARTEnum aUart,UartStatusEnum aStatus);

//------------------------------------------------------------
// Get Uart2 fifo flags state
// aStatus: which flag to read
// retour	: flag state
//------------------------------------------------------------
bool iUart_GetFifoStatus(UARTEnum aUart,UartFifoStatusEnum aStatus);

//------------------------------------------------------------
// Uart2 Data register write
// aData: datas to transmit
//------------------------------------------------------------
void iUart_SetData(UARTEnum aUart,UInt8 aData);

//------------------------------------------------------------
// Check if the buffer is empty
// Return : true --> buffer empty, false --> buffer not empty
//------------------------------------------------------------
bool iUart_IsBufferEmpty(UARTEnum aUart);

//------------------------------------------------------------
// Byte read in the RX buffer
// Return : oldest byte in the buffer
//------------------------------------------------------------
Int8 iUart_GetCharFromBuffer(UARTEnum aUart);


#endif
