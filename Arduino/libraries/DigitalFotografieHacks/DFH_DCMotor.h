#ifndef __DFH_DCMOTOR_H__
#define __DFH_DCMOTOR_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define FORWARD   true
#define BACKWARD  false

class DFH_DCMotor {
  public:
    DFH_DCMotor(int _dirPin, int _pwmPin);
    void motorMove(int _speed);
    
  private:
    int dirPin;
    int pwmPin;
  
};

#endif /* __DFH_DCMOTOR_H__ */
