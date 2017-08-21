/*
------------------------------------------------------------
Copyright 2003-2016 Haute �cole ARC Ing�ni�rie, Switzerland. 
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

enum I2CModeEnum
{
	kI2cWrite=0,
	kI2cRead=1
};

typedef enum
{
	kAckAuto,
	kNoAck
}I2CAckEnum;

typedef enum
{
	kTransferComplete=0x80,
	kInterruptFlag=0x2,
	kRxAK=0x1,
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
void iI2C_Config(void);

//------------------------------------------------------------
// I2C interface enable
//------------------------------------------------------------
void iI2C_Enable(void);

//------------------------------------------------------------
// I2C interface disable
//------------------------------------------------------------
void iI2C_Disable(void);

//------------------------------------------------------------
// Set START state
//------------------------------------------------------------
void iI2C_SetStartState(void);

//------------------------------------------------------------
// Set repeated START state
//------------------------------------------------------------
void iI2C_SetRepeatedStartSate(void);

//------------------------------------------------------------
// Set STOP state
//------------------------------------------------------------
void iI2C_SetStopState(void);

//------------------------------------------------------------
// Generate automatic ACK or not
//------------------------------------------------------------
void iI2C_SetAckMode(I2CAckEnum aAck);

//------------------------------------------------------------
// Select if we transmit or receive
//------------------------------------------------------------
void iI2C_TxRxSelect(I2CTransmiteModeEnum aMode);

//------------------------------------------------------------
// Send a data
//------------------------------------------------------------
void iI2C_SendData(uint8_t aData);

//------------------------------------------------------------
// Wait End of transmit or receive
//------------------------------------------------------------
void iI2C_WaitEndOfRxOrTx(void);

//------------------------------------------------------------
// Read received data, ! generate I2C clock if not in STOP mode
//------------------------------------------------------------
uint8_t iI2C_ReadData(void);

//------------------------------------------------------------
// Get I2C status flags
//------------------------------------------------------------
bool iI2C_ReadStatus(I2CStatusEnum aStatus);


/*enum I2CModeEnum
{
	kI2cWrite=0,
	kI2cRead=1
};

typedef enum
{
	kAckAuto,
	kNoAck
}I2CAckEnum;

typedef enum
{
	kTransferComplete=0x80,
	kInterruptFlag=0x2,
	kRxAK=0x1,
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
void iI2C_Config(void);

//------------------------------------------------------------
// I2C interface enable
//------------------------------------------------------------
void iI2C_Enable(void);

//------------------------------------------------------------
// I2C interface disable
//------------------------------------------------------------
void iI2C_stop(void);

//------------------------------------------------------------
// Set START state
//------------------------------------------------------------
void iI2C_SetStartState(void);

//------------------------------------------------------------
// Set repeated START state
//------------------------------------------------------------
void iI2C_SetRepeatedStartSate(void);

//------------------------------------------------------------
// Set STOP state
//------------------------------------------------------------
void iI2C_SetStopState(void);

//------------------------------------------------------------
// Generate automatic ACK or not
//------------------------------------------------------------
void iI2C_SetAckMode(I2CAckEnum aAck);

//------------------------------------------------------------
// Select if we transmit or receive
//------------------------------------------------------------
void iI2C_TxRxSelect(I2CTransmiteModeEnum aMode);

//------------------------------------------------------------
// Send a data
//------------------------------------------------------------
void iI2C_SendData(UInt8 aData);

//------------------------------------------------------------
// Wait End of transmit or receive
//------------------------------------------------------------
void iI2C_WaitEndOfRxOrTx(void);

//------------------------------------------------------------
// Read received data, ! generate I2C clock if not in STOP mode
//------------------------------------------------------------
UInt8 iI2C_ReadData(void);

//------------------------------------------------------------
// Get I2C status flags
//------------------------------------------------------------
bool iI2C_ReadStatus(I2CStatusEnum aStatus);
*/

#endif /* I2C_H_ */
