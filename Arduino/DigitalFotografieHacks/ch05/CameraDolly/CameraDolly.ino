/* Digital Fotografie Hacks
 * FocusStacking: 
 * Macro Focus Stacking mit 4-Wege-Schiene und Schrittmotor
 * v0.1, 2014-07-11, Peter Recktenwald 
*/ 
/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// http://forums.adafruit.com/viewtopic.php?f=19&t=21586&p=113177
//#include <LiquidTWI2.h>  // for Adafruit LCD I2C Backpack
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
#include <LiquidCrystal_I2C.h>  // for SainSmart, YwRobot I2C LCD
// Get the Encoder Library here: 
#include <QuadratureEncoder.h>
#include "DFH_DCMotor.h"
#include "DFH_OptoCam.h"

/*-----[Konstanten Deklaration]--------*/
#define dirPin  3
#define pwmPin 2

#define FORWARD   false
#define BACKWARD  true

#define focusPin    8 
#define shutterPin  9

#define encaPin     10
#define encbPin     11

#define greenLedPin 12
#define redLedPin   13

#define limitPin    A6    // End Schalter

#define adcBtnPin   A0

#define btnRIGHT  1
#define btnUP     2
#define btnDOWN   3
#define btnLEFT   4
#define btnSELECT 5
#define btnSTART  6

#define btnNONE   0

#define LDC_ROWS  4
#define LCD_COLS  16

#define USE_I2C_LCD

/*-----( Declare objects )-----*/
#ifdef USE_I2C_LCD
// set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // YwRobot Set the LCD I2C address
//LiquidCrystal_I2C lcd(0x3F); /*, 20, 4);  // Sainsmart 2004 Set the LCD I2C address */
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Sainsmart 2004 (new) Set the LCD I2C address #endif 
//LiquidTWI2 lcd(0);   // Adafruit LCD I2C Backpack
#endif
/*-----[Variablen Deklaration]--------*/ 
int limitValue = 0;   // variable to store the value coming from the sensor
int oldLimit = -1;
int speedValue = 100;   
int repeatValue = 20;  
int oldRepeat = -1;
int oldPosition  = -999;
int newPosition = 0;

int btnVal;
int oldBtnVal = -1;
int adcKey;
int oldAdcKey = -1;


QuadratureEncoder myEnc(encaPin, encbPin);

DFH_OptoCam myCam(focusPin, shutterPin);
DFH_DCMotor myMotor(dirPin, pwmPin);

int menuSelected = true;

enum eMode 
{
  MOD_INIT,
  MOD_RUN,
  MOD_MENU,
  MOD_LAUNCH,
  MOD_LAUNCHED,
};

enum eMode mode = MOD_INIT;


const char * lcdMenu[4] = {
"Camera Dolly    ",  
"S:     R:       ",  
"B:              ",  
"Init            "};

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
  if (adcKey < 450)  return btnSELECT; 
  if (adcKey < 580)  return btnLEFT; 
  if (adcKey < 710)  return btnSELECT; 
  if (adcKey < 850)  return btnSTART;  

  return btnNONE;  // when all others fail, return this...
}

void readSensors(void)
{
  limitValue = analogRead(limitPin);    
  btnVal = readAdcButtons();
}  

void lcdPrintDec(int value)
{
#ifdef USE_I2C_LCD
    if (value < 10)
      lcd.print("   ");
    else if (value < 100)
      lcd.print("  ");
    else if (value < 1000)
      lcd.print(" ");
    lcd.print(value, DEC);
#endif
}
 
void initDisplay()
{
  int i;
  Serial.println("Focus Stacking v0.1");
#ifdef USE_I2C_LCD
  for(i=0; i<4; i++)
  {
    lcd.setCursor(0, i);
    lcd.print(lcdMenu[i]);
  }
#endif
}

void updateDisplay(void)
{
  switch (mode)
  {
    case MOD_INIT:
      initDisplay();
    break;
    case MOD_RUN:
      lcd.setCursor(0, 3);
      lcd.print("Run    ");
    break;
    case MOD_MENU:
      lcd.setCursor(0, 3);
      lcd.print("Menu   ");
    break;
    case MOD_LAUNCH:
      lcd.setCursor(0, 3);
      lcd.print("Launch  ");
    break;
    case MOD_LAUNCHED:
      lcd.setCursor(0, 3);
      lcd.print("Shoot   ");
    break;

    break;
    default:
    break;
  }
  if (btnVal != oldBtnVal)
  {
    oldBtnVal = btnVal;
#ifdef USE_I2C_LCD
    lcd.setCursor(3, 2);
    lcdPrintDec(btnVal);
#endif
    Serial.print("Btn: ");
    Serial.println(btnVal, DEC);
  }
  if (limitValue != oldLimit)
  { 
    oldLimit = limitValue;
#ifdef USE_I2C_LCD
    lcd.setCursor(10, 2);
    lcdPrintDec(limitValue);                  
#endif
    Serial.print("Limit: ");
    Serial.println(limitValue);             
  }
  if (repeatValue != oldRepeat)
  { 
    oldRepeat = repeatValue;
#ifdef USE_I2C_LCD
    lcd.setCursor(10, 1);
    lcdPrintDec(repeatValue);                  
#endif
    Serial.print("Repeat: ");
    Serial.println(repeatValue);             
  }
  if (menuSelected)
  {
    Serial.println("activ");
  }
  else
  {
    Serial.println("inactiv");
  }    
} 

enum eColor{
  COLOR_OFF,
  COLOR_GREEN,
  COLOR_RED,
  COLOR_YELLOW
};

enum eColor color = COLOR_OFF;

void initStatusLED(void)
{
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
}


void updateStatusLED(void)
{
  switch (color)
  {
    case COLOR_OFF:
      digitalWrite(greenLedPin, 0);
      digitalWrite(redLedPin, 0);
    break;
    case COLOR_GREEN:
      digitalWrite(greenLedPin, 1);
      digitalWrite(redLedPin, 0);
    break;
    case COLOR_RED:
      digitalWrite(greenLedPin, 0);
      digitalWrite(redLedPin, 1);
    break;
    case COLOR_YELLOW:
      digitalWrite(greenLedPin, 1);
      digitalWrite(redLedPin, 1);
    break;
    default:
      digitalWrite(greenLedPin, 0);
      digitalWrite(redLedPin, 0);
    break;
  }
}

void setup()
{
  // set up
  myEnc.write(speedValue);
  
  mode = MOD_INIT;
  Serial.begin(57600);
#ifdef USE_I2C_LCD
//  lcd.begin(16,4);         // initialize the lcd for 16 chars 4 lines, turn on backlight
  lcd.begin(20,4);         // initialize the lcd for 16 chars 4 lines, turn on backlight
  lcd.backlight();         // Backlight on
#endif
  initDisplay();
  initStatusLED();
  color = COLOR_GREEN;
  updateStatusLED();
  mode = MOD_RUN;   
}

void loop()
{
  int r, s;
  // read the sensor & analog keys:
  readSensors();    

  if (btnVal == btnSELECT)
  {
    Serial.print("Menu");
    while(btnVal != btnNONE)
    {
      btnVal = readAdcButtons();
    }
    menuSelected = (menuSelected) ? false : true;
  }
  
  if (menuSelected == true)
  {
    newPosition = myEnc.read4();
    if (newPosition != oldPosition) {
      oldPosition = newPosition;
      speedValue =  newPosition;
      updateDisplay();
    }
    mode = MOD_MENU;   
 //   delay(100);
  }
  else
  {
    r = repeatValue;
    while (r >= 0)
    {
      mode = MOD_LAUNCH;   
      color = COLOR_RED;
      updateDisplay();
      updateStatusLED();
      myCam.shoot();
      delay(1000);
      myMotor.motorMove(speedValue);
      mode = MOD_LAUNCHED;   
      color = COLOR_YELLOW;
      updateDisplay();
      updateStatusLED();
      delay(1000);
      myMotor.motorMove(0);
      delay(2000);
      color = COLOR_GREEN;
      updateStatusLED();
      r--;
    }
    s = speedValue*repeatValue;
    Serial.print("Total Steps: ");
    Serial.print(s);
  
//    myMotor.motorMove(-s);
    mode = MOD_MENU;
    menuSelected = true;
    updateDisplay();
    color = COLOR_GREEN;
    updateStatusLED();
    delay(2000);
  }
}
