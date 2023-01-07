/*
------------------------------------------------------------
Copyright 2003-2017 Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	gTerminal.c
Author and date :	damien 20 Aug 2017

Description in the header file (.h)

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/
#include "def.h"
#include "gTerminal.h"
#include "gMBox.h"
#include "mRs232.h"
#include "mDelay.h"

#define kMonitoringRefreshTime 500
#define kGainStep 0.1

// Etat possible du terminal
typedef enum
{
	kTerminalInit, kPrintMenu, kWaitChar, kMonitoring, kMotor, kServo, kConfig
} TerminalStateEnum;
static TerminalStateEnum sTerminalState;

typedef enum
{
	kMonitoringInit, kMonitoringActive, kMonitoringQuit
} MonitoringStateEnum;
static MonitoringStateEnum sMonitoringState;

//-------------------------------------------------------------------------
// Configuration du gestionnaire
//-------------------------------------------------------------------------
void gTerminal_Setup(void)
{
	mRs232_Setup();
	mRs232_Open();
	// Initialisation de la machine d'état
	sTerminalState = kTerminalInit;
}

//-------------------------------------------------------------------------
// Exécution du gestionnaire
//-------------------------------------------------------------------------
void gTerminal_Execute(void)
{
	uint8_t aChar = mRs232_ReadCharFromBuffer();

	switch(sTerminalState)
	{
	case kTerminalInit:
		if(aChar)
		{
			sTerminalState = kPrintMenu;
		}
		break;
	case kPrintMenu:
		mRs232_WriteString("0: Monitoring\r\n1: Moteur\r\n2: Servo\r\n3 Config\r\n");
		sTerminalState = kWaitChar;
		break;
	case kWaitChar:
		if(aChar)
		{
			if((aChar<'0') || (aChar>'3')) sTerminalState = kPrintMenu; // Entrée invalide
			else
			{
				switch(aChar)
				{
				case '0': //Monitoring
					sTerminalState = kMonitoring;
					break;
				case '1': //Moteur
					sTerminalState = kMotor;
					break;
				case '2': //Servo
					sTerminalState = kServo;
					break;
				case '3':
					sTerminalState = kConfig;
					break;
				}
			}
		}
		break;
	case kMonitoring:
		if(gTerminal_Monitoring(aChar)) sTerminalState=kPrintMenu;
		break;
	case kMotor:
		if(gTerminal_Motor(aChar)) sTerminalState=kPrintMenu;
		break;
	case kServo:
		if(gTerminal_Servo(aChar)) sTerminalState=kPrintMenu;
		break;
	case kConfig:
		if(gTerminal_Config(aChar)) sTerminalState=kPrintMenu;
		break;
	}
}

static bool gTerminal_Monitoring(uint8_t aChar)
{
	static int16_t sMonitoringDelay;
	char tabText[50];

	switch(sMonitoringState)
	{
	case kMonitoringInit:
		sMonitoringDelay = mDelay_GetDelay(kPit0,kMonitoringRefreshTime/kPit0Period);
		mRs232_WriteString("Monitoring: (q to quit)\r\n");
		sMonitoringState = kMonitoringActive;
		break;
	case kMonitoringActive:
		if(mDelay_IsDelayDone(kPit0,sMonitoringDelay))
		{
			mDelay_ReStart(kPit0, sMonitoringDelay, kMonitoringRefreshTime / kPit0Period);
			sprintf(tabText,"qX=%4.2f, qY=%4.2f, qZ=%4.2f\r\n",gInput.sentralData.qX.float32Value,gInput.sentralData.qY.float32Value,gInput.sentralData.qZ.float32Value);
			mRs232_WriteString(tabText);
		}
		if(aChar=='q')
		{
			mDelay_DelayRelease(kPit0,sMonitoringDelay);
			sMonitoringState=kMonitoringQuit;
		}
		break;
	case kMonitoringQuit:

		break;
	}
	return sMonitoringState==kMonitoringQuit;
}

static bool gTerminal_Motor(uint8_t aChar)
{
	char tabText[50];

	if(aChar)
	{
		switch(aChar)
		{
		case '+':
			((gOutput.motorSpeed<60000) ? (gOutput.motorSpeed+=5000) : (gOutput.motorSpeed=0xFFFF));
			break;
		case '-':
			((gOutput.motorSpeed>5000) ? (gOutput.motorSpeed-=5000) : (gOutput.motorSpeed=0));
			break;
		case 'f':
			gOutput.motorDirection=kForward;
			break;
		case 'b':
			gOutput.motorDirection=kBackward;
			break;
		case 's':
			sprintf(tabText,"Vitesse: %u\%, Direction: ",((uint32_t)gOutput.motorSpeed*100)/0xFFFF);
			mRs232_WriteString(tabText);
			((gOutput.motorDirection==kForward) ? (mRs232_WriteString("forward\r\n")) : (mRs232_WriteString("backward\r\n")));
			break;
		default:
			mRs232_WriteString("Vitesse: [+]augmenter,[-]diminuer\r\nDirection: [f]avant,[b]arriere\r\n[s]status, [q] pour quitter\r\n");
			break;
		}
	}
	return aChar=='q';
}

static bool gTerminal_Servo(uint8_t aChar)
{
	char tabText[50];
	if(aChar)
	{
		switch(aChar)
		{
		case '+':
			((gOutput.servoAngle<170) ? (gOutput.servoAngle+=10) : (gOutput.servoAngle=180));
			break;
		case '-':
			((gOutput.servoAngle>10) ? (gOutput.servoAngle-=10) : (gOutput.servoAngle=0));
			break;
		case 's':
			sprintf(tabText,"Angle du servo: %u\r\n",gOutput.servoAngle);
			mRs232_WriteString(tabText);
			break;
		default:
			mRs232_WriteString("Angle: [+]augmenter,[-]diminuer\r\n[s]status, [q] pour quitter\r\n");
			break;
		}
	}
	return aChar=='q';
}

static bool gTerminal_Config(uint8_t aChar)
{
	char tabText[50];

	if(aChar)
	{
		switch(aChar)
		{
		case 'P':
			gCompute.regPiConfig.GainP+=kGainStep;
			break;
		case 'p':
			gCompute.regPiConfig.GainP-=kGainStep;
			break;
		case 'I':
			gCompute.regPiConfig.GainI+=kGainStep;
			break;
		case 'i':
			gCompute.regPiConfig.GainI-=kGainStep;
			break;
		case 's':
			sprintf(tabText,"P=%5.3f, I=%5.3f\r\n",gCompute.regPiConfig.GainP,gCompute.regPiConfig.GainI);
			break;
		case 'M':
			gCompute.mode=kModeManuel;
			break;
		case 'A':
			gCompute.mode=kModeAuto;
			break;
		default:
			mRs232_WriteString("Proportionnelle:[p]-[P]+, Integrale:[i]-[I]+\r\nMode manuel[M],Mode auto[A]\r\nStatus:[s],Quit:[q]\r\n");
			break;
		}
	}
	return aChar=='q';
}
