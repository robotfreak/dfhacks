/* Digital Fotografie Hacks
 * Timelapse: 
 * Timelapse Fotografie
 * v0.1, 2014-03-03, Peter Recktenwald 
*/ 
/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
/*-----( Declare Constants )-----*/
#define focusPin    8 
#define shutterPin  9
#define launchPin   13

#define encaPin     2
#define encbPin     3

#define FOCUS_DELAY  59
#define SHUTTER_DELAY  100

#define adcBtnPin   A6

#define btnRIGHT  1
#define btnUP     2
#define btnDOWN   3
#define btnLEFT   4
#define btnSELECT 5
#define btnSTART  6

#define btnNONE   0

#define USE_I2C_LCD

/*-----( Declare objects )-----*/
#ifdef USE_I2C_LCD
// set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
#endif

Encoder myEnc(encaPin, encbPin);

/*-----( Declare Variables )-----*/

long oldPosition  = -999;
long newPosition = 300;

int btnVal;
int oldBtnVal = -1;
int adcKey;
int oldAdcKey = -1;

int menuSelected = true;

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
  if (adcKey < 850)  return btnSTART;  

  return btnNONE;  // when all others fail, return this...
}
  
void setup() {
  // set up
  pinMode(shutterPin, OUTPUT);
  pinMode(focusPin, OUTPUT);
  digitalRead(launchPin);
  Serial.begin(57600);  // Used to type in characters
  Serial.println("Timelapse v0.1");
  Serial.print(newPosition, DEC);
  Serial.println(" ms");
#ifdef USE_I2C_LCD
  lcd.begin(20,4);         // initialize the lcd for 20 chars 4 lines, turn on backlight
  // Print a message to the LCD.
  lcd.print("Timelapse v0.1");
  lcd.setCursor(0, 1);
  lcd.print(newPosition, DEC);
  lcd.print(" ms");
#endif
}

void launchShutter()
{
  digitalWrite(focusPin, HIGH);
  delay(FOCUS_DELAY);
  digitalWrite(shutterPin, HIGH);

  delay(SHUTTER_DELAY);
  digitalWrite(shutterPin, LOW);
  digitalWrite(focusPin, LOW);
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

  btnVal = readAdcButtons();
  if (btnVal == btnSELECT)
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
    if (btnVal == btnSTART)
    {
      Serial.println("launch pressed");
      while(btnVal != btnNONE)
      {
        btnVal = readAdcButtons();
      }
      while(true)
      { 
        delay(newPosition);
        launchShutter();
      }
    }
    else
    {
#ifdef USE_I2C_LCD
      lcd.setCursor(0, 3);
      lcd.print("ready   ");
#endif
    }
  }

}
