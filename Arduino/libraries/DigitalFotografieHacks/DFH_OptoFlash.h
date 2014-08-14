#ifndef __DFH_OPTOFLASH_H__
#define __DFH_OPTOFLASH_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define FLASH_DELAY  100

/*-----( Class Definition )-----*/

class DFH_OptoFlash {
  public:
    DFH_OptoFlash(int _flashPin);   // Konstruktor
    void fire(void);


  private:
    int flashPin;
};

#endif /* __DFH_OPTOFLASH_H__ */
