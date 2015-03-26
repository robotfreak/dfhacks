/* Digital Fotografie Hacks
 * DropPhotos: 
 * Tropfen Fotografie mit dem Foto Shield
 * v0.1, 2014-01-26, Peter Recktenwald 
*/ 
/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// http://forums.adafruit.com/viewtopic.php?f=19&t=21586&p=113177
#include <LiquidTWI.h>
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
//#include <LiquidCrystal_I2C.h>
#include "DFH_Encoder.h"
#include "DFH_OptoCam.h"
#include "DFH_OptoFlash.h"
#include "DFH_StatusLED.h"
#include "DFH_JoystickA.h"

/*-----( Declare Constants )-----*/
#define focusPin    8 
#define shutterPin  9

#define flashPin    7

#define encaPin     10
#define encbPin     11

#define greenLedPin 4
#define redLedPin   5

#define adcBtnPin   A0

#define xPin A1   // horizontal Pin
#define yPin A3   // vertikal Pin
#define selPin 12 // select Pin

#define FOCUS_DELAY  59
#define SHUTTER_DELAY  100

#define valvePin    A7

#define DROP_LENGTH  20 
#define DROP_DELAY  20 

#define btnRIGHT  1
#define btnUP     2
#define btnDOWN   3
#define btnLEFT   4
#define btnSELECT 5
#define btnSTART  6

#define btnNONE   0

#define USE_I2C_LCD

#define LDC_ROWS  4
#define LCD_COLS  16

/*-----( Declare objects )-----*/
#ifdef USE_I2C_LCD
// set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // YwRobot Set the LCD I2C address
//LiquidCrystal_I2C lcd(0x3F); /*, 20, 4);  // Sainsmart 2004 Set the LCD I2C address */
//LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, NEGATIVE);  // Sainsmart 2004 (new) Set the LCD I2C address #endif
LiquidTWI lcd(0);   // Adafruit LCD I2C Backpack
#endif

DFH_Encoder myEnc(encaPin, encbPin);
DFH_OptoCam myCam(focusPin, shutterPin);
DFH_OptoFlash myFlash(flashPin);
DFH_StatusLED myStatusLED(greenLedPin, redLedPin);
DFH_JoystickA joystick(xPin, yPin, selPin);

/*-----( Declare Variables )-----*/

long oldPosition  = -999;
long newPosition = 300;

int btnVal;
int oldBtnVal = -1;
int adcKey;
int oldAdcKey = -1;

int menuSelected = true;
int minVal = 1023;
int maxVal = 0;
int xVal;
int yVal;
int selVal;
int joyVal;
int oldJoyVal = -1;

// read the buttons
int readAdcButtons()
{
  adcKey = analogRead(adcBtnPin); 

  if ((adcKey < oldAdcKey -5) || (adcKey > oldAdcKey +5)) 
  {
    oldAdcKey = adcKey;
    Serial.print("adc: ");
    Serial.println(adcKey, DEC);
  }

  if (adcKey > 1000) return btnNONE; 
  if (adcKey < 50)   return btnRIGHT;  
  if (adcKey < 250)  return btnUP; 
  if (adcKey < 450)  return btnDOWN; 
  if (adcKey < 580)  return btnLEFT; 
  if (adcKey < 710)  return btnSELECT; 
  if (adcKey < 950)  return btnSTART;  

  return btnNONE;  // when all others fail, return this...
}
  
void setup() {
  // set up
  myEnc.write(newPosition);

  pinMode(valvePin, OUTPUT);
  digitalWrite(valvePin, LOW);
  Serial.begin(57600);  // Used to type in characters
  Serial.println("Drop Photos v0.1");
  Serial.print(newPosition, DEC);
  Serial.println(" ms");
#ifdef USE_I2C_LCD
  lcd.begin(20,4);         // initialize the lcd for 20 chars 4 lines, turn on backlight
  // Print a message to the LCD.
  lcd.print("Drop Photos v0.1");
  lcd.setCursor(0, 1);
  lcd.print(newPosition, DEC);
  lcd.print(" ms");
#endif
  myStatusLED.begin();
  myStatusLED.on(COLOR_GREEN);
  joystick.begin();
}

void launchDrop()
{
  digitalWrite(valvePin, HIGH);
  delay(DROP_LENGTH);
  digitalWrite(valvePin, LOW);
  delay(DROP_DELAY);
  digitalWrite(valvePin, HIGH);
  delay(DROP_LENGTH);
  digitalWrite(valvePin, LOW);
  
}

void loop() 
{
  
  btnVal = readAdcButtons();
  if (btnVal != oldBtnVal)
  {
    oldBtnVal = btnVal;
#ifdef USE_I2C_LCD
    lcd.setCursor(0, 2);
    lcd.print("Btn: ");
    lcd.print(btnVal, DEC);
#endif
    Serial.print("Btn: ");
    Serial.println(btnVal, DEC);
  } 
  joyVal = joystick.read();
  if (joyVal)
  {
    Serial.print("Result: ");
    Serial.print("btn: ");
    Serial.println(joyVal, DEC);
  }

  btnVal = readAdcButtons();
  if (btnVal == btnSELECT || joyVal & 0x10)
  {
    Serial.print("Menu");
    while(btnVal != btnNONE)
    {
      btnVal = readAdcButtons();
    }
    menuSelected = (menuSelected) ? false : true;
    if (menuSelected)
    {
      Serial.println("activ");
#ifdef USE_I2C_LCD
      lcd.setCursor(0, 3);
      lcd.print("Menu    ");
#endif
    }
    else
    {
      Serial.println("inactiv");
#ifdef USE_I2C_LCD
      lcd.setCursor(0, 3);
      lcd.print("Launch  ");
#endif
    }    
  }
  
  if (menuSelected == true)
  {
    newPosition = myEnc.read();
    if (newPosition != oldPosition) {
      oldPosition = newPosition;
#ifdef USE_I2C_LCD
      lcd.setCursor(0, 1);
      lcd.print(newPosition, DEC);
      lcd.print(" ms  ");
#endif
      Serial.print(newPosition);
      Serial.println(" ms");
    }
  }

  btnVal = readAdcButtons();
  if (menuSelected == false)
  {
    if (btnVal == btnSTART || joyVal & 0x10)
    {
      Serial.println("launch pressed");
      while(btnVal != btnNONE)
      {
        btnVal = readAdcButtons();
      }
      myStatusLED.on(COLOR_RED);
      launchDrop();
      delay(newPosition);
      myCam.shoot();
#ifdef USE_I2C_LCD
      lcd.setCursor(0, 3);
      lcd.print("launched");
#endif
      Serial.println("launched");
      delay(1000);
      myStatusLED.on(COLOR_GREEN);
    }
    else
    {
#ifdef USE_I2C_LCD
      lcd.setCursor(0, 3);
      lcd.print("ready   ");
#endif
      if (joyVal & 0x01 || joyVal & 0x08)
      {
         menuSelected = true;
#ifdef USE_I2C_LCD
        lcd.setCursor(0, 3);
        lcd.print("Menu    ");
#endif
      }
    }
  }

}
