#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "DFH_OptoFlash.h"

/*-----( Konstruktor )-----*/

DFH_OptoFlash::DFH_OptoFlash(int _flashPin)
{
  this->flashPin = _flashPin;

  pinMode(this->flashPin, OUTPUT);

  digitalWrite(this->flashPin, LOW);
}

/*-----( fire )-----*/

void DFH_OptoFlash::fire(void)
{
  digitalWrite(this->flashPin, HIGH);

  delay(FLASH_DELAY);
  digitalWrite(this->flashPin, LOW);
}

