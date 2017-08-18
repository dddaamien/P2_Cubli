/*
------------------------------------------------------------
Copyright 2003-2016 Haute �cole ARC Ing�ni�rie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	iI2C.c
Author and date :	Monnerat Serge 17.06.2015

Description in the header (.h)

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $

-----------------------------------------------------------------------------
*/
#include "iI2C.h"
#include "MK10D5.h"


#define BUSSPEED 		50000000
#define I2CBAUDRATE 400000

//------------------------------------------------------------
// I2C module config
//------------------------------------------------------------
void iI2C_Config(I2CEnum aI2C)
{
	if(kI2C0==aI2C)
		{
			// Baud rate speed and I2C timing
			// I2C Frequency Divider register (I2Cx_F)
			// K60 Sub-Family Reference Manual, Rev. 2 Jun 2012 page 1457
			// I2C clock rate=390,625 kHz (max 400kHz)
			// SDA Hold = 0.42us (max 0.9us)
			// SCL start Hold = 1.16 us (min 0.6us)
			// SCL stop Hold = 1.3 us (min 0.6us)
			I2C0->F=0;
			I2C0->F|=I2C_F_ICR(0x17)|I2C_F_MULT(0);
		}
}


//------------------------------------------------------------
// I2C interface enable
//------------------------------------------------------------
void iI2C_Enable(I2CEnum aI2C)
{
	if(kI2C0==aI2C)
		{
			// I2C0 enable
			// I2C Control Register 1 (I2Cx_C1)
			// K60 Sub-Family Reference Manual, Rev. 2 Jun 2012 page 1458
			I2C0->C1 |= I2C_C1_IICEN_MASK;
		}
}

//------------------------------------------------------------
// I2C interface disable
//------------------------------------------------------------
void iI2C_stop(I2CEnum aI2C)
{
	if(kI2C0==aI2C)
		{
			// I2C0 disable
			// I2C Control Register 1 (I2Cx_C1)
			// K60 Sub-Family Reference Manual, Rev. 2 Jun 2012 page 1458
			I2C0->C1 &= (~I2C_C1_IICEN_MASK);
		}
}

//------------------------------------------------------------
// Set START state
//------------------------------------------------------------
void iI2C_SetStartState(I2CEnum aI2C)
{
	if(kI2C0==aI2C)
		{
			// I2C Control Register 1 (I2Cx_C1)
			// K60 Sub-Family Reference Manual, Rev. 2 Jun 2012 page 1458
			I2C0->C1 |= I2C_C1_MST_MASK;
		}
}

//------------------------------------------------------------
// Set repeated START state
//------------------------------------------------------------
void iI2C_SetRepeatedStartSate(I2CEnum aI2C)
{
	if(kI2C0==aI2C)
		{
			// I2C Control Register 1 (I2Cx_C1)
			// K60 Sub-Family Reference Manual, Rev. 2 Jun 2012 page 1458
			I2C0->C1 |= I2C_C1_RSTA_MASK;
		}
}

//------------------------------------------------------------
// Set STOP state
//------------------------------------------------------------
void iI2C_SetStopState(I2CEnum aI2C)
{
	if(kI2C0==aI2C)
		{
			// I2C Control Register 1 (I2Cx_C1)
			// K60 Sub-Family Reference Manual, Rev. 2 Jun 2012 page 1458
			I2C0->C1 &= (~I2C_C1_MST_MASK);
		}
}

//------------------------------------------------------------
// Generate automatic ACK or not
//------------------------------------------------------------
void iI2C_SetAckMode(I2CEnum aI2C,I2CAckEnum aAck)
{
	if(kI2C0==aI2C)
		{
			if (aAck == kAckAuto)
				{
					// I2C Control Register 1 (I2Cx_C1)
					// K60 Sub-Family Reference Manual, Rev. 2 Jun 2012 page 1458
					I2C0->C1 &= (~I2C_C1_TXAK_MASK);
				}
			else if (aAck == kNoAck)
				{
					// I2C Control Register 1 (I2Cx_C1)
					// K60 Sub-Family Reference Manual, Rev. 2 Jun 2012 page 1458
					I2C0->C1 |= I2C_C1_TXAK_MASK;
				}
		}
}

//------------------------------------------------------------
// Select if we transmit or receive
//------------------------------------------------------------
void iI2C_TxRxSelect(I2CEnum aI2C,I2CTransmiteModeEnum aMode)
{
	if(kI2C0==aI2C)
		{
			if (kTxMode == aMode)
				{
					// TX
					I2C0->C1 |= I2C_C1_TX_MASK;
				}
			else if (kRxMode == aMode)
				{
					// RX
					I2C0->C1 &= ~I2C_C1_TX_MASK;
				}
		}
}

//------------------------------------------------------------
// Send a data
//------------------------------------------------------------
void iI2C_SendData(I2CEnum aI2C,UInt8 aData)
{
	if(kI2C0==aI2C)
		{
			// I2C Data I/O register (I2Cx_D)
			// K60 Sub-Family Reference Manual, Rev. 2 Jun 2012 page 1461
			I2C0->D = aData;
		}
}

//------------------------------------------------------------
// Wait End of transmit or receive
//------------------------------------------------------------
void iI2C_WaitEndOfRxOrTx(I2CEnum aI2C)
{
	if(kI2C0==aI2C)
		{
			// Wait for IICIF flag
			// I2C Status register (I2Cx_S)
			// K60 Sub-Family Reference Manual, Rev. 2 Jun 2012 page 1460
			while ((I2C0->S & I2C_S_IICIF_MASK) == 0);
			// Clear the IICIF flag
			I2C0->S |= I2C_S_IICIF_MASK;
		}
}

//------------------------------------------------------------
// Read received data, ! generate I2C clock if not in STOP mode
//------------------------------------------------------------
UInt8 iI2C_ReadData(I2CEnum aI2C)
{
	UInt8 aReturnData = 0;
	
	if(kI2C0==aI2C)
		{
			// I2C Data I/O register (I2Cx_D)
			// K60 Sub-Family Reference Manual, Rev. 2 Jun 2012 page 1461
			aReturnData = I2C0->D;
		}
	
	return aReturnData;
}

//------------------------------------------------------------
// Get I2C status flags
//------------------------------------------------------------
bool iI2C_ReadStatus(I2CEnum aI2C,I2CStatusEnum aStatus)
{
	bool aRet=false;
	
	if(kI2C0==aI2C)
		{
			aRet=(bool)(((I2C0->S&aStatus)==aStatus));
		}
	return aRet;
}
