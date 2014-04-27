/* Digital Fotografie Hacks
 * DropPhotos: 
 * Tropfen Fotografie mit dem Foto Shield
 * v0.1, 2014-01-26, Peter Recktenwald 
*/ 
/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>
// Get the LCD Encoder Library here: 
// http://www.pjrc.com/teensy/td_libs_Encoder.html
#include <Encoder.h>
#include <SoftwareSerial.h>   // We need this even if we're not using a SoftwareSerial object
                              // Due to the way the Arduino IDE compiles
// Get the SerialCommand Library here
// https://github.com/scogswell/ArduinoSerialCommand
#include <SerialCommand.h>

/*-----( Declare Constants )-----*/
#define focusPin    8 
#define shutterPin  9
#define launchPin   11

#define encaPin     2
#define encbPin     3

#define SHUTTER_DELAY  100

#define valvePin    7

#define DROP_LENGTH  20 

#define adcBtnPin   A6

#define btnRIGHT  1
#define btnUP     2
#define btnDOWN   3
#define btnLEFT   4
#define btnSELECT 5
#define btnSTART  6

#define btnNONE   0

//#define USE_I2C_LCD

/*-----( Declare objects )-----*/
#ifdef USE_I2C_LCD
// set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
#endif

Encoder myEnc(encaPin, encbPin);

SerialCommand SCmd;   // The demo SerialCommand object

/*-----( Declare Variables )-----*/

int oldPosition  = -999;
int newPosition = 0;

int btnVal;
int oldBtnVal = -1;
int adcKey;
int oldAdcKey = -1;

int valveNr = 0;

int valves[3][10];

int menuSelected = false;

void processShutterCmd(void);   
void processValveCmd(void);   
void unrecognized(void);

// read the buttons
int readAdcButtons()
{
  adcKey = analogRead(adcBtnPin); 

  if ((adcKey < oldAdcKey -5) || (adcKey > oldAdcKey +5)) 
  {
    oldAdcKey = adcKey;
//    Serial.print("adc: ");
//    Serial.println(adcKey, DEC);
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
  pinMode(valvePin, OUTPUT);
  digitalWrite(valvePin, LOW);
  digitalRead(launchPin);
  Serial.begin(57600);  // Used to type in characters
  Serial.println("Drop Photos v0.1");
  Serial.print(newPosition+60, DEC);
  Serial.println(" ms");

  SCmd.addCommand("SD",processShutterCmd);  // Converts two arguments to integers and echos them back 
  SCmd.addCommand("VV",processValveCmd);  // Converts two arguments to integers and echos them back 
  SCmd.addDefaultHandler(unrecognized);  // Handler for command that isn't matched  (says "What?") 

#ifdef USE_I2C_LCD
  lcd.begin(20,4);         // initialize the lcd for 20 chars 4 lines, turn on backlight
  // Print a message to the LCD.
  lcd.print("Drop Photos v0.1");
  lcd.setCursor(0, 1);
  lcd.print(newPosition+60, DEC);
  lcd.print(" ms");
#endif
  valves[0][0]= 1;
  valves[0][1]= 20;
  valves[0][2]= 20;
  valves[0][3]= 20;
}

void processShutterCmd()    
{
  int aNumber;  
  char *arg; 

  Serial.println("We're in processShutterCmd"); 
  arg = SCmd.next(); 
  if (arg != NULL) 
  {
    aNumber=atoi(arg);    // Converts a char string to an integer
    Serial.print("Shutter Delay: "); 
    Serial.println(aNumber); 
    newPosition = aNumber;
  } 
  else {
    Serial.println("No arguments"); 
  }

}


void processValveCmd()    
{
  int aNumber, valveNr, valveIdx;  
  char *arg; 

  Serial.println("We're in processValveCmd"); 
  arg = SCmd.next(); 
  valveIdx = 4;
  if (arg != NULL) 
  {
    aNumber=atoi(arg);    // Converts a char string to an integer
    Serial.print("Valve Nr: "); 
    Serial.println(aNumber); 
    valveNr = aNumber;
    do {
      arg = SCmd.next();
      if (arg != NULL) 
      {
        aNumber=atoi(arg);    // Converts a char string to an integer
        Serial.print("Valve"); 
        if (valveIdx & 1)
          Serial.print(" on: "); 
        else
          Serial.print(" off: "); 
        Serial.println(aNumber); 
        valves[valveNr][valveIdx++] = aNumber;
      }    
    } while(arg != NULL);
    valves[valveNr][valveIdx++] = 0;    
  } 
  else {
    Serial.println("No arguments"); 
  }
  

}

// This gets set as the default handler, and gets called when no other command matches. 
void unrecognized()
{
  Serial.println("What?"); 
}

void launchShutter()
{
  digitalWrite(focusPin, HIGH);
  digitalWrite(shutterPin, HIGH);

  delay(SHUTTER_DELAY);
  digitalWrite(shutterPin, LOW);
  digitalWrite(focusPin, LOW);
}

void launchDrop()
{
  int idx, val;
  
  for(idx=0; idx < 6; idx++)
  {
    val = valves[0][idx];
    if (idx & 1 && val)
    {
      digitalWrite(valvePin, HIGH);
      delay(val);
      digitalWrite(valvePin, LOW);
    }
    else if (val)
    {
      digitalWrite(valvePin, LOW);
      delay(val);
    }
    else
      break;
  }
  
}

void loop() 
{
  SCmd.readSerial();     // We don't do much, just process serial commands
  
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
      lcd.print(newPosition+60, DEC);
      lcd.print(" ms  ");
#endif
      Serial.print(newPosition+60);
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
      launchDrop();
      delay(newPosition);
      launchShutter();
#ifdef USE_I2C_LCD
      lcd.setCursor(0, 3);
      lcd.print("launched");
#endif
      Serial.println("launched");
      delay(1000);
    }
  }

}
