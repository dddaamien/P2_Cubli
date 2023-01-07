/*
------------------------------------------------------------
Copyright 2003-2017 Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	gTerminal.h
Author and date :	damien 20 Aug 2017

Goal

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/
#ifndef SOURCES_GESTIONNAIRES_GTERMINAL_H_
#define SOURCES_GESTIONNAIRES_GTERMINAL_H_

//-------------------------------------------------------------------------
// Configuration du gestionnaire
//-------------------------------------------------------------------------
void gTerminal_Setup(void);

//-------------------------------------------------------------------------
// Exécution du gestionnaire
//-------------------------------------------------------------------------
void gTerminal_Execute(void);

//static
static bool gTerminal_Monitoring(uint8_t aChar);
static bool gTerminal_Motor(uint8_t aChar);
static bool gTerminal_Servo(uint8_t aChar);
static bool gTerminal_Config(uint8_t aChar);

#endif /* SOURCES_GESTIONNAIRES_GTERMINAL_H_ */
