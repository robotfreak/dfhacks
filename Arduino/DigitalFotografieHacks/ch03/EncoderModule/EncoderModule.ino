/* Digital Fotografie Hacks
 * EncoderModule: 
 * Test des Encoder Moduls
 * v0.1, 2014-01-25, Peter Recktenwald 
*/
#include "QuadratureEncoder.h"

QuadratureEncoder myEnc(13,12);

int oldPosition  = -999;
int newPosition = 200;

int btnVal;
int oldBtnVal = -1;
int adcKey;
int oldAdcKey = -1;

#define adcBtnPin   A1

#define btnRIGHT  1
#define btnUP     2
#define btnDOWN   3
#define btnLEFT   4
#define btnSELECT 5
#define btnSTART  6

#define btnNONE   0

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

void setup() 
{
  // set up
  Serial.begin(57600);
  Serial.println("Encoder Module v0.1");
}

void loop() 
{
  btnVal = readAdcButtons();
  if (btnVal != oldBtnVal)
  {
    oldBtnVal = btnVal;
    Serial.print("Btn: ");
    Serial.println(btnVal, DEC);
  }
  
  newPosition += myEnc.read();
  if (newPosition != oldPosition) 
  {
    oldPosition = newPosition;
    Serial.print("Enc: ");
    Serial.println(newPosition, DEC);
  }
//  delay(100);
}
