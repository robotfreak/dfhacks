/* Digital Fotografie Hacks
 * PIRSensor:
 * PIR Sensor Test
 * v0.1, 2014-08-25, Peter Recktenwald 
*/  
/*-----( Import needed libraries )-----*/ 
#include <DFH_OptoCam.h>

/*-----[Konstanten Deklaration]--------*/ 
#define sensePin   2
#define ledPin    13

#define focusPin   8
#define shutterPin 9

/*-----[Variablen Deklaration]--------*/  
DFH_OptoCam myCam(focusPin, shutterPin);

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(sensePin, INPUT);
  digitalWrite(sensePin, HIGH);  /* enable internal PullUp resistor */
}

void loop() {
  int senseVal;
  
  senseVal = digitalRead(sensePin);
  digitalWrite(ledPin, senseVal);
  if (senseVal == LOW)  // object detected
  {
    myCam.shoot();      // take photo
    delay(1000);
  }
}
