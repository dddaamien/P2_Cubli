/*
------------------------------------------------------------
Copyright 2003-2017 Haute école ARC Ingéniérie, Switzerland. 
All rights reserved.
------------------------------------------------------------
File name :	PI.h
Author and date :	damien 21 Aug 2017

Goal

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/
#ifndef SOURCES_UTILS_PI_H_
#define SOURCES_UTILS_PI_H_

// Structure de données utilisées par le régulateur PI
typedef struct RegPIStruct
{
  float LimTotLow;
  float LimTotHigh;
  float GainP;
  float GainI;
};


//-----------------------------------------------------------------------------
// Régulateur Proportionnel-Intégral avec limitation de la sortie et annulation
// de la dérive de l'intégrateur lorsque la sortie à atteint sa valeur de limitation
// theRegPIStruct: adr. de la struct. contenant les limit. et gains du PI
// theIntPart    : addresse de la partie intégrale
// theErr        : différence entre mesure et consigne
// Temps         : appel + exécution + return = 48.060 us à 24 MHz
//-----------------------------------------------------------------------------
float RegPI(struct RegPIStruct *theRegPIStruct,float *theIntPart,float theErr);


#endif /* SOURCES_UTILS_PI_H_ */
