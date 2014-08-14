#ifndef __DFH_JOYSTICKD_H__
#define __DFH_JOYSTICKD_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define btnRIGHT  1
#define btnUP     2
#define btnDOWN   3
#define btnLEFT   4
#define btnSELECT 5
#define btnSTART  6
#define btnEXTERN 7

#define btnNONE   0

/*-----( Class Definition )-----*/

class DFH_JoystickD {
  public:
    DFH_JoystickD(int _adcBtnPin);   // Konstruktor
    int read(void);
    int readRaw(void);


  private:
    int adcBtnPin;
    int adcKey;
    int oldAdcKey;
};

#endif /* __DFH_JOYSTICKD_H__ */

