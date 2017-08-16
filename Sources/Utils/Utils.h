/*
------------------------------------------------------------
Copyright 2003-2016 Haute école ARC Ingéniérie, Switzerland.
All rights reserved.
------------------------------------------------------------
Nom du fichier : 	Utils.h
Auteur et Date :	Monnerat Serge 14.7.2015

But : Some general settings

-----------------------------------------------------------------------------
Historique:
-----------------------------------------------------------------------------

$History: $

-----------------------------------------------------------------------------
*/

#ifndef UTILS_H_
#define UTILS_H_

#include"def.h"

//------------------------------------------------------------
// Codage de la trame à transmettre
// Codage des messages binaire sur une ligne série.
// Ce codage utilise 3 bytes de contrôles réservés:
//		SOH 0x01 Début du message.
//		NUL 0x00 Fin du message.
//		STX 0x02 Préfixe pour un byte du message contenant une valeur comprise entre 253 et 255
// Tous le bytes du message qui contiennent une valeur comprise entre 0 et 252 sont transmits avec un offset de +3
//------------------------------------------------------------
UInt8 BuildCodedFrame(UInt8 *aSrcFrame,UInt8 *aDestFrame,UInt8 aSrcFrameSize);


//------------------------------------------------------------
// Décodage des trames RS232 de commande de la carte GUI
//------------------------------------------------------------
UInt8 DecodeFrame(UInt8 *aBuffPtr,UInt8 aSize,UInt8 *aNewBuffPtr);

#endif
