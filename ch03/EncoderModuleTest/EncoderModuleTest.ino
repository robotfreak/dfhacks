/* Digital Fotografie Hacks
 * EncoderModuleTest: 
 * Test des Drehgeber Moduls
 * v0.1, 2014-01-28, Peter Recktenwald 
*/
/*-----( benoetigte Bibliotheken )-----*/  
#include "DFH_Encoder.h"

/*-----( Konstanten Deklaration )-----*/  
#define encaPin   10
#define encbPin   11

/*-----( Objekt Deklaration )-----*/  
DFH_Encoder myEnc(encaPin,encbPin);

/*-----( Variablen Deklaration )-----*/  
int oldPosition  = -999;
int newPosition = 200;

void setup() 
{
  // Initialisierung serielle Schnittstelle
  Serial.begin(57600);
  Serial.println("Drehgeber Modul Test v0.1");
}

void loop() 
{
  newPosition = myEnc.read4();
  if (newPosition != oldPosition) 
  {
    oldPosition = newPosition;
    Serial.print("Encoder: ");
    Serial.println(newPosition, DEC);
  }
}
