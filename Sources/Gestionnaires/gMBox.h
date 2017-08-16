/*
------------------------------------------------------------
Copyright 2003-2016 Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name : 	gMBox.h	
Author and date :	Monnerat Serge 29.06.2015

Goal : 

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $


-----------------------------------------------------------------------------
*/
#ifndef __gMBox__
#define __gMBox__

#include "def.h"

//-----------------------------------------------------------------------------
// Input handle structure
//-----------------------------------------------------------------------------
typedef struct
{
	float 	Toto;

}InputStruct;

extern InputStruct gInput;

//-----------------------------------------------------------------------------
// Compute handle structure
//-----------------------------------------------------------------------------




typedef struct
{

	UInt8 Titi;

}ComputeStruct;

extern ComputeStruct gCompute;

//-----------------------------------------------------------------------------
// Host Cmd handle structure
//-----------------------------------------------------------------------------
typedef struct
{
	UInt32 SecurityRelay_Bac1:1;
	UInt32 SecurityRelay_Bac2:1;
	UInt32 WorkingRelay_Bac1:1;
	UInt32 WorkingRelay_Bac2:1;
	UInt32 SolBIB_Bac1:1;
	UInt32 SolBIB_Bac2:1;
	UInt32 SolFilling_Bac1:1;
	UInt32 SolFilling_Bac2:1;
	UInt32 SolFiltering_Bac1:1;
	UInt32 SolFiltering_Bac2:1;
	UInt32 PumpFilling_Bac1:1;
	UInt32 PumpFilling_Bac2:1;
	UInt32 PumpBIB_Bac1:1;
	UInt32 PumpBIB_Bac2:1;
	UInt32 Basket_Bac1:1;
	UInt32 Basket_Bac2:1;
	UInt32 Buzzer_Bac1:1;
	UInt32 Buzzer_Bac2:1;
	UInt32 Reserved:14;
}ActionStruct;
typedef union
{
	UInt32 Action;
	ActionStruct ActionBits;
}ActionUnion;

typedef struct
{
	UInt32 PreHeats_Bac1:1;
	UInt32 PreHeats_Bac2:1;
	UInt32 Heats_Bac1:1;
	UInt32 Heats_Bac2:1;
	UInt32 Standby_Bac1:1;
	UInt32 Standby_Bac2:1;
	UInt32 Reserved:26;
}WorkingStateStruct;
typedef union
{
	UInt32 WorkingState;
	WorkingStateStruct WorkingStateBits;
}WorkingStateUnion;

typedef struct
{
	ActionUnion ActionCmd;
	WorkingStateUnion WorkingStateCmd;
}HostCmdStruct;

extern HostCmdStruct gHostCmd;


#endif
