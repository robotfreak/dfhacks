#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "DFH_DCMotor.h"

DFH_DCMotor::DFH_DCMotor(int _dirPin, int _pwmPin)
{
  this->dirPin = _dirPin;
  this->pwmPin = _pwmPin;

  pinMode(this->dirPin, OUTPUT);

}

// move the motor n steps
void DFH_DCMotor::motorMove(int _speed)
{
  if (_speed < 0)
    digitalWrite(this->dirPin, BACKWARD);
  else
    digitalWrite(this->dirPin, FORWARD);
  analogWrite(this->pwmPin, abs(_speed));  
}
 

