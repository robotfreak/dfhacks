/* Digital Fotografie Hacks
 * SensorTrigger: 
 * Analoger Sensor als Kamera Ausloeser
 * v0.1, 2014-05-22, Peter Recktenwald 
*/
/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// http://forums.adafruit.com/viewtopic.php?f=19&t=21586&p=113177
#include <LiquidTWI2.h>
// Get the Encoder Library here: 
#include <QuadratureEncoder.h>

/*-----[Konstanten Deklaration]--------*/
#define focusPin    8 
#define shutterPin  9

#define encaPin     10
#define encbPin     11

#define sensorPin   A2    // select the input pin for the potentiometer

#define SHUTTER_DELAY  100
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
//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
LiquidTWI2 lcd(0);
#endif

/*-----[Variablen Deklaration]--------*/
int sensorValue = 0;   // variable to store the value coming from the sensor
int oldSensor = -1;
int levelValue = 100;  

int oldPosition  = -999;
int newPosition = 0;

int btnVal;
int oldBtnVal = -1;
int adcKey;
int oldAdcKey = -1;


QuadratureEncoder myEnc(encaPin, encbPin);
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
"Sensor Trigger  ",  
"L:     S:       ",  
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
  sensorValue = analogRead(sensorPin);    
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
  Serial.println("Sensor Trigger v0.1");
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
    case MOD_MENU:
    case MOD_LAUNCH:
    case MOD_LAUNCHED:

    break;
    default:
    break;
  }
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
  if(sensorValue!= oldSensor)
  {
    oldSensor = sensorValue;
#ifdef USE_I2C_LCD
    lcd.setCursor(8, 1);
    lcdPrintDec(sensorValue);
#endif
    Serial.print("Sensor: ");
    Serial.println(sensorValue, DEC);
  }
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

void setup() {
  // set up
  pinMode(shutterPin, OUTPUT);
  pinMode(focusPin, OUTPUT);
  digitalWrite(shutterPin, LOW);
  digitalWrite(focusPin, LOW);

  myEnc.write(levelValue);
  
  mode = MOD_INIT;
  Serial.begin(57600);
#ifdef USE_I2C_LCD
  lcd.begin(16,4);         // initialize the lcd for 16 chars 4 lines, turn on backlight
#endif
  initDisplay();
  mode = MOD_RUN;
}

void launchShutter()
{
  digitalWrite(focusPin, HIGH);
  digitalWrite(shutterPin, HIGH);

  delay(SHUTTER_DELAY);
  digitalWrite(shutterPin, LOW);
  digitalWrite(focusPin, LOW);
}

void loop() {

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
      levelValue =  newPosition;
#ifdef USE_I2C_LCD
      lcd.setCursor(2, 1);
      lcd.print(levelValue, DEC);
#endif
      Serial.print("Level: ");
      Serial.print(levelValue);
    }
 //   delay(100);
  }
  else
  {
    if (sensorValue > levelValue)
    {
      launchShutter();
      lcd.setCursor(0, 3);
      lcd.print("Launched!");
      delay(1000);
      lcd.setCursor(0, 3);
      lcd.print("Launch   ");
    }
  }
#ifdef USE_I2C_LCD
  lcd.setCursor(10, 1);
  lcd.print(sensorValue, DEC);                  
#endif
  Serial.print("Sensor: ");
  Serial.println(sensorValue);                  
}
