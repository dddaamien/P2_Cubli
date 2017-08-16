/*
------------------------------------------------------------
Copyright 2003-2016 Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :				iI2C.h
Author and date :	serge.monnerat, 17.06.2015

Goal :  I2C 0 interface

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

History:


-----------------------------------------------------------------------------
*/

#ifndef I2C_H_
#define I2C_H_

#include "def.h"

// I2C enum
typedef enum
{
	kI2C0,
	kI2C1
}I2CEnum;

typedef enum
{
	kAckAuto,
	kNoAck
}I2CAckEnum;

typedef enum
{
	kTransferComplete=0x80,
	InterruptFlag=0x2,
	RxAK=0x1,
	kBUSY=0x20
}I2CStatusEnum;

typedef enum
{
	kTxMode,
	kRxMode
}I2CTransmiteModeEnum;

//------------------------------------------------------------
// I2C module config
//------------------------------------------------------------
void iI2C_Config(I2CEnum aI2C);

//------------------------------------------------------------
// I2C interface enable
//------------------------------------------------------------
void iI2C_Enable(I2CEnum aI2C);

//------------------------------------------------------------
// I2C interface disable
//------------------------------------------------------------
void iI2C_stop(I2CEnum aI2C);

//------------------------------------------------------------
// Set START state
//------------------------------------------------------------
void iI2C_SetStartState(I2CEnum aI2C);

//------------------------------------------------------------
// Set repeated START state
//------------------------------------------------------------
void iI2C_SetRepeatedStartSate(I2CEnum aI2C);

//------------------------------------------------------------
// Set STOP state
//------------------------------------------------------------
void iI2C_SetStopState(I2CEnum aI2C);

//------------------------------------------------------------
// Generate automatic ACK or not
//------------------------------------------------------------
void iI2C_SetAckMode(I2CEnum aI2C,I2CAckEnum aAck);

//------------------------------------------------------------
// Select if we transmit or receive
//------------------------------------------------------------
void iI2C_TxRxSelect(I2CEnum aI2C,I2CTransmiteModeEnum aMode);

//------------------------------------------------------------
// Send a data
//------------------------------------------------------------
void iI2C_SendData(I2CEnum aI2C,UInt8 aData);

//------------------------------------------------------------
// Wait End of transmit or receive
//------------------------------------------------------------
void iI2C_WaitEndOfRxOrTx(I2CEnum aI2C);

//------------------------------------------------------------
// Read received data, ! generate I2C clock if not in STOP mode
//------------------------------------------------------------
UInt8 iI2C_ReadData(I2CEnum aI2C);

//------------------------------------------------------------
// Get I2C status flags
//------------------------------------------------------------
bool iI2C_ReadStatus(I2CEnum aI2C,I2CStatusEnum aStatus);


#endif /* I2C_H_ */
