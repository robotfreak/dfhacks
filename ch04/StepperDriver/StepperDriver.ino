/* Digital Fotografie Hacks
 * StepperDriver: 
 * Schrittmotor Treiber Ansteuerung
 * v0.1, 2014-08-23, Peter Recktenwald 
*/ 
/*-----( Importiere benoetigte Bibliotheken )-----*/  
#include <DFH_Stepper.h>

/*-----( Declare Constants )-----*/
#define dirPin  2
#define stepPin 3

/*-----( Variablen )-----*/  
DFH_Stepper myStepper(dirPin, stepPin);

void setup()
{
}

void loop()
{
  myStepper.motorMove(200);
  delay(2000);
  myStepper.motorMove(-200);
  delay(2000);
}
