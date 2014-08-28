/* Digital Fotografie Hacks
 * DigitalJoystickCalibrate: 
 * Kalibrierung des digitalen Joystick Moduls
 * v0.1, 2014-08-14, Peter Recktenwald 
*/

#include <DFH_JoystickD.h>

/*-----( Konstanten Deklaration )-----*/  
#define adcBtnPin A0

/*-----( Variablen Deklaration )-----*/  
int minVal = 1023;
int maxVal = 0;
int adcVal;
int oldAdcVal = -1;

DFH_JoystickD joystick(adcBtnPin);

void setup() 
{
  // Initialisierung serielle Schnittstelle
  Serial.begin(57600);
  Serial.println("Joystick Modul Kalibrierung v0.1");
}

void loop() 
{
  minVal = 1023;
  maxVal = 0;
  Serial.println("Press a key");
  adcVal = joystick.readRaw();
  while (adcVal > 1020)
  {
    adcVal = joystick.readRaw();
  }
  while (adcVal < 1020)
  {
    adcVal = joystick.readRaw();
    if (adcVal < minVal && adcVal < 1020)
      minVal = adcVal;
    if (adcVal > maxVal && adcVal < 1020)
      maxVal = adcVal;
  }
  Serial.print("Result: ");
  Serial.print("min: ");
  Serial.print(minVal, DEC);
  Serial.print(" max: ");
  Serial.println(maxVal, DEC);
}
