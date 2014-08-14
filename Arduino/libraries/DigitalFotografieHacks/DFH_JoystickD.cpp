#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "DFH_JoystickD.h"

/*-----( Konstruktor )-----*/

DFH_JoystickD::DFH_JoystickD(int _adcBtnPin)
{
  this->adcBtnPin = _adcBtnPin;
  this->adcKey = 0;
  this->oldAdcKey = -1;
}

/*-----( read )-----*/

int DFH_JoystickD::read(void) 
{
  int btn = btnNONE;
  this->adcKey = analogRead(this->adcBtnPin); 

  if ((this->adcKey < this->oldAdcKey -5) || (this->adcKey > this->oldAdcKey +5)) 
  {
    this->oldAdcKey = this->adcKey;
  }

  if (this->adcKey > 1000) btn = btnNONE; 
  else if (this->adcKey < 250)  btn = btnRIGHT; 
  else if (this->adcKey < 340)  btn = btnSELECT; 
  else if (this->adcKey < 530)  btn = btnDOWN; 
  else if (this->adcKey < 720)  btn = btnLEFT; 
  else if (this->adcKey < 850)  btn = btnUP;  
  else if (this->adcKey < 900)  btn = btnSTART;  
  else if (this->adcKey < 930)  btn = btnEXTERN;   
  
  return btn;
}

/*-----( readRaw )-----*/

int DFH_JoystickD::readRaw(void) 
{
  return analogRead(this->adcBtnPin); 
}


