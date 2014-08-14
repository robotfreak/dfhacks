#ifndef __DFH_STEPPER_H__
#define __DFH_STEPPER_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define FORWARD   true
#define BACKWARD  false

class DFH_Stepper {
  public:
    DFH_Stepper(int _dirPin, int _stepPin);
    void motorMove(int _steps);
    
  private:
    int dirPin;
    int stepPin;
    
    void motorDirection(boolean direction);
    void motorStep();
  
};

#endif /* __DFH_STEPPER_H__ */

