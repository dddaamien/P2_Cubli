#include "PI.h"
#include "def.h"

//-----------------------------------------------------------------------------
// Régulateur Proportionnel-Intégral avec limitation de la sortie et annulation 
// de la dérive de l'intégrateur lorsque la sortie à atteint sa valeur de limitation 
// theRegPIStruct: adr. de la struct. contenant les limit. et gains du PI
// theIntPart    : addresse de la partie intégrale
// theErr        : différence entre mesure et consigne
// Temps         : appel + exécution + return = 48.060 us à 24 MHz   
//----------------------------------------------------------------------------- 
float RegPI(struct RegPIStruct *theRegPIStruct,float *theIntPart,float theErr)
{
  float y;
  float ysat;
  float eLim;
 
  // Calcul de la sortie du régulateur avant saturation y = (Err * GainP) + partie int.
  y=(theErr*(theRegPIStruct->GainP))+(*theIntPart);
 
  // Saturation  du résultat
  if(y>theRegPIStruct->LimTotHigh)
    {
      ysat=theRegPIStruct->LimTotHigh;
    }
  else if(y<theRegPIStruct->LimTotLow)
    {
      ysat=theRegPIStruct->LimTotLow;
    }
  else
    {
      ysat=y;
    }
 
  if((theRegPIStruct->GainP>0)&&(theRegPIStruct->GainI>0))
    {
      // Calcul de l'écart de réglage fictif, permettant d'éviter une dérive de l'intégrale
      eLim= (y-ysat)/((theRegPIStruct->GainP)+theRegPIStruct->GainI);
    }
  else
    {
      eLim=0;
    }
     
  // partie intégrale = partie intégrale + (erreur - écart de réglage fictif) * GainI
  (*theIntPart)=((theErr-eLim)*(theRegPIStruct->GainI))+(*theIntPart);
 
  return ysat;                                     
}
