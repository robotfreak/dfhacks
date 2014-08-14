#ifndef __DFH_STATUSLED_H__
#define __DFH_STATUSLED_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif 

#define COLOR_RED    1
#define COLOR_GREEN  2
#define COLOR_YELLOW 3

/*-----( Class Definition )-----*/

class DFH_StatusLED {
  public:
    DFH_StatusLED(int _greenLedPin, int _redLEdPin);   // Konstruktor
    void on(int color);
    void off(void);
    void blink(int color, int time);


  private:
    int redLedPin;
    int greenLedPin;
};

#endif /* __DFH_STATUSLED_H__ */

