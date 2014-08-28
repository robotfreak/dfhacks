#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "DFH_StatusLED.h"

/*-----( Konstruktor )-----*/
DFH_StatusLED::DFH_StatusLED(int _greenLedPin, int _redLedPin)
{
  this->greenLedPin = _greenLedPin;
  this->redLedPin = _redLedPin;
}

/*-----( StatusLED begin )-----*/
void DFH_StatusLED::begin(void){
  pinMode(this->greenLedPin, OUTPUT);
  pinMode(this->redLedPin, OUTPUT); 
}

/*-----( StatusLED on )-----*/
void DFH_StatusLED::on(int color){
  if (color == COLOR_RED)
  {
    digitalWrite(this->redLedPin,HIGH); //set the pin HIGH and thus turn red LED on
    digitalWrite(this->greenLedPin,LOW); //set the pin LOW and thus turn green LED off
  }
  else if (color == COLOR_GREEN)
  {
    digitalWrite(this->redLedPin,LOW); //set the pin HIGH and thus turn red LED off
    digitalWrite(this->greenLedPin,HIGH); //set the pin LOW and thus turn green LED on
  }
  else if (color == COLOR_YELLOW)
  {
    digitalWrite(this->redLedPin,HIGH); //set the pin HIGH and thus turn red LED on
    digitalWrite(this->greenLedPin,HIGH); //set the pin LOW and thus turn green LED on
  }
}
 
/*-----( StatusLED off )-----*/
void DFH_StatusLED::off(void){
  digitalWrite(this->redLedPin,LOW); //set the pin LOW and thus turn LED off
  digitalWrite(this->greenLedPin,LOW); //set the pin LOW and thus turn green LED off
}
 
/*-----( StatusLED blink )-----*/
void DFH_StatusLED::blink(int color, int times){
  int c = times;
  int time = 1000;
  while (c--)
  {
    on(color);                    // sets the LED on
    delay(time/2);                // waits for a second
    off();                        // sets the LED off
    delay(time/2);                // waits for a second
  }
}

