#ifndef __DFH_OPTOCAM_H__
#define __DFH_OPTOCAM_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define SHUTTER_DELAY  100

/*-----( Class Definition )-----*/

class DFH_OptoCam {
  public:
    DFH_OptoCam(int _focusPin, int _shutterPin);   // Konstruktor
    void shoot(void);
    void launchFocusShutter(void);
    void releaseFocusShutter(void);
    void launchShutter(void);
    void releaseShutter(void);
    void launchFocus(void);
    void releaseFocus(void);


  private:
    int focusPin;
    int shutterPin;
};

#endif /* __DFH_OPTOCAM_H__ */
