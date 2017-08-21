/*
------------------------------------------------------------
Copyright 2003-2017 Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	mEm7180.c
Author and date :	damien 20 Aug 2017

Description in the header file (.h)

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/
#include "def.h"
#include "mEm7180.h"
#include "iDio.h"
#include "iI2C.h"


typedef enum Em7180RegisterEnum
{
	//uint8
	kEmHostControl = 0x34,
	kEmMagRate = 0x55,
	kEmAccelRate = 0x56,
	kEmGyroRate = 0x57,
	kEmActualMagRate = 0x45,
	kEmActualAccelRate = 0x46,
	kEmActualGyroRate = 0x47,
	kEmQRateDivisor = 0x32,
	kEmEnableEvents = 0x33,
	kEmEventStatus = 0x35,
	kEmSensorStatus = 0x36,
	kEmSentralStatus = 0x37,
	kEmErrorRegister = 0x50, //0->no error
	//Float32
	kEmQX = 0x00,
	kEmQY = 0x04,
	kEmQZ = 0x08,
	kEmQW = 0x0C,
	//uint16
	kEmQTime = 0x10,
	//int16
	kEmMX = 0x12,
	kEmMY = 0x14,
	kEmMZ = 0x16,
	//uint16
	kEmMTime = 0x18,
	//int16
	kEmAX = 0x1A,
	kEmAY = 0x1C,
	kEmAZ = 0x1E,
	//uint16
	kEmATime = 0x20,
	//int16
	kEmGX = 0x22,
	kEmGY = 0x24,
	kEmGZ = 0x26,
	//uint16
	kEmGTime = 0x28,
	//uint8
	kEmAlgorithmControl = 0x54,
	kEmAlgorithmStatus = 0x38,
	kEmPassThroughControl = 0xA0,
	kEmPassThroughStatus = 0x9E,
	kEmProductID = 0x90, //==0x80
	kEmRevisionID = 0x91,
	//uint16
	kEmRomVersion = 0x70,
	kEmRamVersion = 0x72
};

//Prototypes statiques
static bool mEm7180_GetData(enum Em7180DeviceEnum aDevice,enum Em7180RegisterEnum aRegister,uint8_t *pData);
static bool mEm7180_SetData(enum Em7180DeviceEnum aDevice,enum Em7180RegisterEnum aRegister,uint8_t aData);

void mEm7180_Setup(enum Em7180DeviceEnum aDevive)
{
	uint8_t aDummy=0;

	//Configuration GPIO
	iDio_SetPortDirection(kPortC,kMaskIo4,kIoInput);
	iDio_SetPortDirection(kPortC,kMaskIo5,kIoInput);
	//Configuration du bus I2C
	iI2C_Config();
	iI2C_Enable();
	mEm7180_GetData(kEm7180D0,kEmProductID,&aDummy);

	while(!iDio_GetPort(kPortC,kMaskIo5)); //wait int=1
	//read centralStatus == 0x0B
	mEm7180_GetData(kEm7180D0,kEmSentralStatus,&aDummy); //==0x0B
	//Set sensor ODR
	mEm7180_SetData(kEm7180D0,kEmMagRate,0x1E);
	mEm7180_SetData(kEm7180D0,kEmAccelRate,0x0A);
	mEm7180_SetData(kEm7180D0,kEmGyroRate,0x14);
	//Set quaternion ODR
	mEm7180_SetData(kEm7180D0,kEmQRateDivisor,0x01);
	//Configure trigger
	mEm7180_SetData(kEm7180D0,kEmEnableEvents,0x07);
	//Enter normal operation mode
	mEm7180_SetData(kEm7180D0,kEmHostControl,0x01);

}

bool mEm7180_GetQuaternions(struct Em7180DataStruct* pData)
{
	uint8_t aData=0;
	if(iDio_GetPort(kPortC,kMaskIo5)) //data available
	{
		mEm7180_GetData(kEm7180D0,kEmEventStatus,&aData);
		if((aData & 0x07) == 0x04) //no error
		{
			mEm7180_GetData(kEm7180D0,kEmQX,&aData);
			pData->qX.int32Value = (uint32_t)aData<<24;
			mEm7180_GetData(kEm7180D0,kEmQX+1,&aData);
			pData->qX.int32Value |= (uint32_t)aData<<16;
			mEm7180_GetData(kEm7180D0,kEmQX+2,&aData);
			pData->qX.int32Value |= (uint32_t)aData<<8;
			mEm7180_GetData(kEm7180D0,kEmQX+3,&aData);
			pData->qX.int32Value |= (uint32_t)aData;
			mEm7180_GetData(kEm7180D0,kEmQTime,&aData);
			pData->qTime = (uint16_t)aData<<8;
			mEm7180_GetData(kEm7180D0,kEmQTime+1,&aData);
			pData->qTime |= (uint32_t)aData;

			mEm7180_GetData(kEm7180D0,kEmQY,&aData);
			pData->qY.int32Value = (uint32_t)aData<<24;
			mEm7180_GetData(kEm7180D0,kEmQY+1,&aData);
			pData->qY.int32Value |= (uint32_t)aData<<16;
			mEm7180_GetData(kEm7180D0,kEmQY+2,&aData);
			pData->qY.int32Value |= (uint32_t)aData<<8;
			mEm7180_GetData(kEm7180D0,kEmQY+3,&aData);
			pData->qY.int32Value |= (uint32_t)aData;

			mEm7180_GetData(kEm7180D0,kEmQZ,&aData);
			pData->qZ.int32Value = (uint32_t)aData<<24;
			mEm7180_GetData(kEm7180D0,kEmQZ+1,&aData);
			pData->qZ.int32Value |= (uint32_t)aData<<16;
			mEm7180_GetData(kEm7180D0,kEmQZ+2,&aData);
			pData->qZ.int32Value |= (uint32_t)aData<<8;
			mEm7180_GetData(kEm7180D0,kEmQZ+3,&aData);
			pData->qZ.int32Value |= (uint32_t)aData;
			return true;
		}
		return false;
	}
	else return false;
}

static bool mEm7180_GetData(enum Em7180DeviceEnum aDevice,enum Em7180RegisterEnum aReg,uint8_t *pData)
{
	bool aNoAck = false;
	// Disable transmit ACK
	iI2C_SetAckMode(kNoAck);
	// Attend que le bus soit libre
	while (true == iI2C_ReadStatus(kBUSY));
	//-----------------------------------------------------------------------
	// D'abords en WRITE afin de transmettre le registre
	// que l'on veut lire ainsi que l'adresse du slave
	//-----------------------------------------------------------------------
	// Début de la transmission --> mode write et START condition
	iI2C_TxRxSelect(kTxMode);
	iI2C_SetStartState();
	// Transmission de l'adresse en WRITE du slave dans le registre de
	// données
	// --> obligatoire protocolle I2C, le slave doit d'abord répondre à son
	//adresse
	iI2C_SendData(aDevice | kI2cWrite);
	// Attend la fin de la transmission
	iI2C_WaitEndOfRxOrTx();
	// Est-ce que le slave est content --> Read Ack
	aNoAck = iI2C_ReadStatus(kRxAK);
	if (aNoAck == true)
	{
		// FIN de la lecture
		iI2C_TxRxSelect(kTxMode);
		iI2C_SetAckMode(kNoAck);
		iI2C_SetStopState();
		return false;
	}
	// Transmission de l'adresse du registre que l'on veut lire
	iI2C_SendData(aReg);
	// Attend la fin de la transmission
	iI2C_WaitEndOfRxOrTx();
	// Est-ce que le slave est content --> Read Ack
	aNoAck = iI2C_ReadStatus(kRxAK);
	if (aNoAck == true)
	{
		// FIN de la lecture
		iI2C_TxRxSelect(kTxMode);
		iI2C_SetAckMode(kNoAck);
		iI2C_SetStopState();
		return false;
	}
	// Nouvelle condition START
	iI2C_SetRepeatedStartSate();
	//-----------------------------------------------------------------------
	// Passage en READ
	//-----------------------------------------------------------------------
	// Transmission de l'adresse en READ du slave dans le registre de données
	// --> obligatoire protocolle I2C, le slave doit d'abords répondre à son
	// adresse
	iI2C_SendData(aDevice | kI2cRead);
	// Attend la fin de la transmission
	iI2C_WaitEndOfRxOrTx();
	// Est-ce que le slave est content --> Read Ack
	aNoAck = iI2C_ReadStatus(kRxAK);
	if (aNoAck == true)
	{
		// FIN de la lecture
		iI2C_TxRxSelect(kTxMode);
		iI2C_SetAckMode(kNoAck);
		iI2C_SetStopState();
		return false;
	}
	// Passage en mode READ
	// --> En lecture c'est au master d'envoyer le ACK mais comme on
	// lit q'un byte pas besoin de ACK
	//iI2C_EnableTxAck();
	iI2C_TxRxSelect(kRxMode);
	// Lecture qui ne sert à rien --> permet l'émission de l'horloge
	// nécessaire au
	// slave afin de transmettre sa donnée
	iI2C_ReadData();
	// Attend la fin de la transmission
	iI2C_WaitEndOfRxOrTx();
	// FIN de la lecture
	// Génération de la condition STOP
	// Obligatoire de faire le STOP avant la lecture pour
	// ne pas émettre de clock à nouveau!
	iI2C_TxRxSelect(kTxMode);
	iI2C_SetAckMode(kNoAck);
	iI2C_SetStopState();
	// Lecture de la valeur du registre demandé
	*pData = iI2C_ReadData();
	return true;
}

static bool mEm7180_SetData(enum Em7180DeviceEnum aDevice,enum Em7180RegisterEnum aReg,uint8_t aVal)
{
	bool aNoAck=false;
	// Disable transmit ACK
	iI2C_SetAckMode(kNoAck);
	// Attend que le bus soit libre
	while(true==iI2C_ReadStatus(kBUSY));
	//-----------------------------------------------------------------------
	// D'abords en WRITE afin de transmettre le registre
	// que l'on veut lire ainsi que l'adresse du slave
	//-----------------------------------------------------------------------
	// Début de la transmission --> mode write et START condition
	iI2C_TxRxSelect(kTxMode);
	iI2C_SetStartState();
	// Transmission de l'adresse en WRITE du slave dans le registre de
	// données --> obligatoire protocolle I2C, le slave doit d'abords
	// répondre à son adresse
	iI2C_SendData(aDevice | kI2cWrite);
	// Attend la fin de la transmission
	iI2C_WaitEndOfRxOrTx();
	// Est-ce que le slave est content --> Read Ack
	aNoAck=iI2C_ReadStatus(kRxAK);
	if(aNoAck==true)
	{
		// FIN de la lecture
		iI2C_TxRxSelect(kTxMode);
		iI2C_SetAckMode(kNoAck);
		iI2C_SetStopState();
		return false;
	}
	// Transmission de l'adresse du registre que l'on veut écrire
	iI2C_SendData(aReg);
	// Attend la fin de la transmission
	iI2C_WaitEndOfRxOrTx();
	// Est-ce que le slave est content --> Read Ack
	aNoAck=iI2C_ReadStatus(kRxAK);
	if(aNoAck==true)
	{
		// FIN de la lecture
		iI2C_TxRxSelect(kTxMode);
		iI2C_SetAckMode(kNoAck);
		iI2C_SetStopState();
		return false;
	}
	// Transmission de la donnée que l'on veut écrire dans le registre
	iI2C_SendData(aVal);
	// Attend la fin de la transmission
	iI2C_WaitEndOfRxOrTx();
	// Est-ce que le slave est content --> Read Ack
	aNoAck=iI2C_ReadStatus(kRxAK);
	// FIN de la lecture
	iI2C_TxRxSelect(kTxMode);
	iI2C_SetAckMode(kNoAck);
	iI2C_SetStopState();
	if(aNoAck==true)
	{
		return false;
	}
	else
	{
		return true;
	}

}

