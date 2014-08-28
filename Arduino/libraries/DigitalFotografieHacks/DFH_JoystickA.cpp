#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "DFH_JoystickA.h"

/*-----( Konstruktor )-----*/

DFH_JoystickA::DFH_JoystickA(int _xPin, int _yPin, int _selPin)
{
  this->xPin = _xPin;
  this->yPin = _yPin;
  this->selPin = _selPin;
}

/*-----( begin )-----*/

void DFH_JoystickA::begin(void)
{
  this->xNull = (analogRead(this->xPin) >> 2);    // Nullstellung X Achse
  this->yNull = (analogRead(this->yPin) >> 2);    // Nullstellung Y Achse
  pinMode(this->selPin,INPUT);     // Select Pin als Eingang
  digitalWrite(this->selPin, HIGH);  // interner PullUp Widerstand ein
}

/*-----( read digital )-----*/

int DFH_JoystickA::read(void)
{
  int x,y;
  int btn = 0;
  x = ((analogRead(this->xPin) >> 2) - this->xNull);
  y = ((analogRead(this->yPin) >> 2) - this->yNull);
  if (x > 4)
    btn |= btnRIGHT;
  if (x < -4)
    btn |= btnLEFT;
  if (y > 4)
    btn |= btnUP;
  if (y < -4)
    btn |= btnDOWN;
  if (digitalRead(this->selPin) == 0)
    btn |= btnSELECT;
  return btn;
}

/*-----( read analog )-----*/

int DFH_JoystickA::read(int *x, int *y)
{
  *x = ((analogRead(this->xPin) >> 2) - this->xNull);
  *y = ((analogRead(this->yPin) >> 2) - this->yNull);
  return digitalRead(this->selPin);
}

/*-----( readRaw )-----*/

int DFH_JoystickA::readRaw(int *x, int *y)
{
  *x = analogRead(this->xPin); 
  *y = analogRead(this->yPin); 
  return digitalRead(this->selPin);
}


