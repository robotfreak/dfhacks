/* Digital Fotografie Hacks
 * EncoderModuleTest: 
 * Test des Drehgeber Moduls
 * v0.1, 2014-01-28, Peter Recktenwald 
*/
/*-----( benoetigte Bibliotheken )-----*/  
#include "QuadratureEncoder.h"

/*-----( Konstanten Deklaration )-----*/  
#define encaPin   13
#define encbPin   12

#define adcBtnPin   A1

#define btnRIGHT  1
#define btnUP     2
#define btnDOWN   3
#define btnLEFT   4
#define btnSELECT 5
#define btnSTART  6

#define btnNONE   0

/*-----( Objekt Deklaration )-----*/  
QuadratureEncoder myEnc(encaPin,encbPin);

/*-----( Variablen Deklaration )-----*/  
int oldPosition  = -999;
int newPosition = 200;

int btnVal;
int oldBtnVal = -1;
int adcKey;
int oldAdcKey = -1;


// Einlesen der analog angebunden Taster
int readAdcButtons()
{
  adcKey = analogRead(adcBtnPin); 

  if ((adcKey < oldAdcKey -5) || (adcKey > oldAdcKey +5)) 
  {
    oldAdcKey = adcKey;
    Serial.print("adc: ");
    Serial.println(adcKey, DEC);
  }

  if (adcKey > 1000) return btnNONE; 
  if (adcKey < 50)   return btnRIGHT;  
  if (adcKey < 250)  return btnUP; 
  if (adcKey < 450)  return btnDOWN; 
  if (adcKey < 580)  return btnLEFT; 
  if (adcKey < 710)  return btnSELECT; 
  if (adcKey < 850)  return btnSTART;  

  return btnNONE;  // wenn alles andere fehlschlaegt...
}

void setup() 
{
  // Initialisierung serielle Schnittstelle
  Serial.begin(57600);
  Serial.println("Drehgeber Modul Test v0.1");
}

void loop() 
{
  btnVal = readAdcButtons();
  if (btnVal != oldBtnVal)
  {
    oldBtnVal = btnVal;
    Serial.print("Taste: ");
    Serial.println(btnVal, DEC);
  }
  
  newPosition += myEnc.read();
  if (newPosition != oldPosition) 
  {
    oldPosition = newPosition;
    Serial.print("Encoder: ");
    Serial.println(newPosition, DEC);
  }
}
}
