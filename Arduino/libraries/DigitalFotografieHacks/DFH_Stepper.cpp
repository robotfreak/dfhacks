#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "DFH_Stepper.h"

DFH_Stepper::DFH_Stepper(int _dirPin, int _stepPin)
{
  this->dirPin = _dirPin;
  this->stepPin = _stepPin;

  pinMode(this->dirPin, OUTPUT);
  pinMode(this->stepPin, OUTPUT);

}

// set motor direction
void DFH_Stepper::motorDirection(boolean direction)
{
  digitalWrite(this->dirPin, direction);
  delayMicroseconds(100);
}

// do a single step
void DFH_Stepper::motorStep()
{
  digitalWrite(this->stepPin, HIGH);
  delayMicroseconds(100);
  digitalWrite(this->stepPin, LOW);
  delayMicroseconds(100);
  delay(5);
}

// move the motor n steps
void DFH_Stepper::motorMove(int steps)
{
  if (steps < 0)
    motorDirection(BACKWARD);
  else
    motorDirection(FORWARD);
  for(int i=0; i<abs(steps); i++)
  {
    motorStep();
  }
}

 
