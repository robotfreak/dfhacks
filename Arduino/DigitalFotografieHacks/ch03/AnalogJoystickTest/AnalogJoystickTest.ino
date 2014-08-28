/* Digital Fotografie Hacks
 * AnalogJoystickTest: 
 * Kalibrierung des analogen Joystick Moduls
 * v0.1, 2014-08-14, Peter Recktenwald 
*/

#include <DFH_JoystickA.h>

/*-----( Konstanten Deklaration )-----*/  
#define xPin A1   // horizontal Pin
#define yPin A3   // vertikal Pin
#define selPin 13 // select Pin

/*-----( Variablen Deklaration )-----*/  
int minVal = 1023;
int maxVal = 0;
int xVal;
int yVal;
int selVal;
int btnVal;

DFH_JoystickA joystick(xPin, yPin, selPin);

void setup() 
{
  // Initialisierung serielle Schnittstelle
  Serial.begin(57600);
  Serial.println("Analog Joystick Kalibrierung v0.1");
  joystick.begin();
}

void loop() 
{
  minVal = 1023;
  maxVal = 0;
  btnVal = joystick.read();
/*
  selVal = joystick.read(&xVal, &yVal);
  if (selVal == 0)  // Select Taste gedrueckt?
  {
    Serial.println("Select Key pressed");
  }
*/  
  if (btnVal)
  {
    Serial.print("Result: ");
    Serial.print("btn: ");
    Serial.println(btnVal, DEC);
  }
/*  
  Serial.print(" x: ");
  Serial.print(xVal, DEC);
  Serial.print(" y: ");
  Serial.println(yVal, DEC);
*/
  delay(250);
}
