#ifndef __DFH_JOYSTICKA_H__
#define __DFH_JOYSTICKA_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define btnRIGHT  1
#define btnUP     2
#define btnDOWN   4
#define btnLEFT   8
#define btnSELECT 16
 

/*-----( Class Definition )-----*/

class DFH_JoystickA {
  public:
    DFH_JoystickA(int _xPin, int _yPin, int _selPin);   // Konstruktor
    void begin(void);
    int read(void);
    int read(int *x, int *y);
    int readRaw(int *x, int *y);


  private:
    int xPin;
    int yPin;
    int selPin;
    int xNull;
    int yNull;
};

#endif /* __DFH_JOYSTICKA_H__ */

