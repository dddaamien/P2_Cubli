/*
------------------------------------------------------------
Copyright 2003-2016 Haute école ARC Ingéniérie, Switzerland.
All rights reserved.
------------------------------------------------------------
File name :	Utils.c
Author and date :	Monnerat Serge 14.7.2015

Description in the header (.h)

-----------------------------------------------------------------------------
History:
-----------------------------------------------------------------------------

$History: $

-----------------------------------------------------------------------------
*/

#include"Utils.h"

// Caractère ASCII de contrôle STX
#define kSTX  				0x02

//------------------------------------------------------------
// Codage de la trame à transmettre
// Codage des messages binaire sur une ligne série.
// Ce codage utilise 3 bytes de contrôles réservés:
//		SOH 0x01 Début du message.
//		NUL 0x00 Fin du message.
//		STX 0x02 Préfixe pour un byte du message contenant une valeur comprise entre 253 et 255
// Tous le bytes du message qui contiennent une valeur comprise entre 0 et 252 sont transmits avec un offset de +3
//------------------------------------------------------------
UInt8 BuildCodedFrame(UInt8 *aSrcFrame,UInt8 *aDestFrame,UInt8 aSrcFrameSize)
{
  UInt8 i,j=0;

  for(i=0,j=0;i<aSrcFrameSize;i++,j++)
    {
      if(aSrcFrame[i]>252)
        {
         aDestFrame[j]=kSTX;
         j++;
         aDestFrame[j]=aSrcFrame[i];
        }
      else if((i!=0)&&(i<(aSrcFrameSize-1)))
        {
         aDestFrame[j]=(UInt8)(aSrcFrame[i]+3);
        }
      else
        {
         aDestFrame[j]=aSrcFrame[i];
        }
    }

  return j;
}


//------------------------------------------------------------
// Décodage des trames RS232 de commande de la carte GUI
//------------------------------------------------------------
UInt8 DecodeFrame(UInt8 *aBuffPtr,UInt8 aSize,UInt8 *aNewBuffPtr)
{
  bool aNext = false;
  UInt8 aNewSize=0;

  while(aSize>0)
    {
      if ((*aBuffPtr != 0x00) && (*aBuffPtr != 0x01))
        {
          if (aNext == false)
            {
              if (*aBuffPtr ==0x02)
                {
                  aNext = true;
                }
              else
                {
                  *aNewBuffPtr=*aBuffPtr;
                  *aNewBuffPtr-=3;
                  aNewBuffPtr++;
                  aNewSize++;
                }
            }
          else
            {
              *aNewBuffPtr=*aBuffPtr;
              aNewBuffPtr++;
              aNewSize++;
              aNext = false;
            }
        }

      aBuffPtr++;
      aSize--;
    }

  return aNewSize;
}

