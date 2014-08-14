#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "DFH_OptoCam.h"

/*-----( Konstruktor )-----*/

DFH_OptoCam::DFH_OptoCam(int _focusPin, int _shutterPin)
{
  this->focusPin = _focusPin;
  this->shutterPin = _shutterPin;

  pinMode(this->focusPin, OUTPUT);
  pinMode(this->shutterPin, OUTPUT);

  digitalWrite(this->shutterPin, LOW);
  digitalWrite(this->focusPin, LOW);
}

/*-----( shoot )-----*/

void DFH_OptoCam::shoot(void)
{
  digitalWrite(this->focusPin, HIGH);
  digitalWrite(this->shutterPin, HIGH);

  delay(SHUTTER_DELAY);

  digitalWrite(this->shutterPin, LOW);
  digitalWrite(this->focusPin, LOW);
}

/*-----( launchFocusShutter )-----*/

void DFH_OptoCam::launchFocusShutter(void)
{
  digitalWrite(this->focusPin, HIGH);
  digitalWrite(this->shutterPin, HIGH);
}

/*-----( releaseFocusShutter )-----*/

void DFH_OptoCam::releaseFocusShutter(void)
{
  digitalWrite(this->shutterPin, LOW);
  digitalWrite(this->focusPin, LOW);
}

/*-----( launchShutter )-----*/

void DFH_OptoCam::launchShutter(void)
{
  digitalWrite(this->shutterPin, HIGH);
}

/*-----( releaseShutter )-----*/

void DFH_OptoCam::releaseShutter(void)
{
  digitalWrite(this->shutterPin, LOW);
}

/*-----( launchFocus )-----*/

void DFH_OptoCam::launchFocus(void)
{
  digitalWrite(this->focusPin, HIGH);
}

/*-----( releaseFocus )-----*/

void DFH_OptoCam::releaseFocus(void)
{
  digitalWrite(this->focusPin, LOW);
}


