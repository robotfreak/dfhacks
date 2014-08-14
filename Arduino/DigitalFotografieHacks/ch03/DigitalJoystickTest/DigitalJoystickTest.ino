/* Digital Fotografie Hacks
 * DigitalJoystickTest: 
 * Test des digitalen Joystick Moduls
 * v0.1, 2014-08-13, Peter Recktenwald 
*/

#include <DFH_JoystickD.h>

/*-----( Konstanten Deklaration )-----*/  
#define adcBtnPin A1

/*-----( Variablen Deklaration )-----*/  
int btnVal;
int oldBtnVal = -1;
int adcVal;
int oldAdcVal = -1;

DFH_JoystickD joystick(adcBtnPin);

void setup() 
{
  // Initialisierung serielle Schnittstelle
  Serial.begin(57600);
  Serial.println("Drehgeber Modul Test v0.1");
}

void loop() 
{
  btnVal = joystick.read();
  if (btnVal != oldBtnVal)
  {
    oldBtnVal = btnVal;
    Serial.print("Taste: ");
    switch(btnVal)
    {
      case btnNONE:
        Serial.print("None   ");
      break;
      case btnLEFT:
        Serial.print("Left   ");
      break;
      case btnRIGHT:
        Serial.print("Right  ");
      break;
      case btnUP:
        Serial.print("Up     ");
      break;
      case btnDOWN:
        Serial.print("Down   ");
      break;
      case btnSELECT:
        Serial.print("Select ");
      break;
      case btnSTART:
        Serial.print("Start  ");
      break;
      case btnEXTERN:
        Serial.print("Extern ");
      break;
      default:
        Serial.print("Unknown");
      break;
    }
    Serial.println(btnVal, DEC);
    adcVal = joystick.readRaw();
    Serial.print("Raw: ");
    Serial.println(adcVal, DEC);
  }
}
