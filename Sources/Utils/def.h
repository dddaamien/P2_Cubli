/*
------------------------------------------------------------
Copyright 2003-2016 Haute école ARC Ingéniérie, Switzerland.
All rights reserved.
------------------------------------------------------------
Nom du fichier : 	def.h	
Auteur et Date :	Monnerat Serge 14.7.2015

But : Some general settings

-----------------------------------------------------------------------------
Historique:
-----------------------------------------------------------------------------

$History: $

-----------------------------------------------------------------------------
*/

#ifndef DEF_H_
#define DEF_H_

#include "stddef.h"

//------------------------------------------------------------
// Macros
//------------------------------------------------------------
// Enable maskable interrupts
#define EnableInterrupts __asm("CPSIE i");

// Disable maskable interrupts
#define DisableInterrupts __asm("CPSID i");
/***********************************************************************/

//------------------------------------------------------------
// Constants
//------------------------------------------------------------
// Core clock
#define kCoreClockCpu 		100.0 	// 100MHz
#define kCoreClockCpukHz 	100000 	// 100MHz

// Bus clock
#define kBusClock 		(kCoreClockCpu/2.0) 		// 50MHz
#define kBusClockkHz 	(kCoreClockCpukHz/2.0)	// 50MHz

// PIT0 period --> 1 ms
#define kPit0Period 1 
// PIT1 period --> 3 s
#define kPit1Period 3000.0

// UART baud rates
#define kUart0BaudRate 9600
#define kUart3BaudRate 115200
#define kUart4BaudRate 115200


//------------------------------------------------------------
// Type
//------------------------------------------------------------
// PIT enum
typedef enum
{
	kPit0=0,
	kPit1=1,
	kPit2=2,
	kPit3=3
}PitEnum;

//------------------------------------------------------------
// Interrupt priority (O.highest, 16 lowest)
//------------------------------------------------------------
#define kPIT_IntPriority 		0x7		// Priority 3
#define kUart4_IntPriority  0x01  // Priorité 1
#define kUart3_IntPriority  0x00  // Priorité 0
#define kUart0_IntPriority	0x01	// Priorité 1

//------------------------------------------------------------
// bool and standard type
//------------------------------------------------------------
typedef int bool;
#define TRUE 1
#define true 1
#define FALSE 0
#define false 0

typedef unsigned char				UInt8;  /*  8 bits */
typedef unsigned short int	UInt16; /* 16 bits */
typedef unsigned long int		UInt32; /* 32 bits */

typedef char			    			Int8;   /*  8 bits */
typedef short int	        	Int16;  /* 16 bits */
typedef int		            	Int32;  /* 32 bits */

typedef volatile Int8				VInt8;  /*  8 bits */
typedef volatile Int16			VInt16; /* 16 bits */
typedef volatile Int32			VInt32; /* 32 bits */

typedef volatile UInt8			VUInt8;  /*  8 bits */
typedef volatile UInt16			VUInt16; /* 16 bits */
typedef volatile UInt32			VUInt32; /* 32 bits */



#endif
