/* Digital Fotografie Hacks
 * AdafruitLCDModulTest: 
 * LCD-Modul Test fuer LCD Module mit Adafruit I2C LCD Backpack
 * v0.1, 2014-06-02, Peter Recktenwald 
*/
/*-----( Importiere benoetigte Bibliotheken )-----*/
#include <Arduino.h>  // Comes with Arduino IDE 
#include <Wire.h>
// Die LCD I2C Bibliothek befindet sich hier: 
// http://forums.adafruit.com/viewtopic.php?f=19&t=21586&p=113177 
#include <LiquidTWI2.h>

// Verbnindung ueber I2C, mit der Standard Adresse #0 (A0-A2 Jumper nicht gesetzt)
LiquidTWI2 lcd(0);

void setup()
{
  lcd.setMCPType(LTI_TYPE_MCP23008); 
  lcd.begin(16,4);               // Initialisiere LCD Modul mit 16 x 4 Zeichen
  lcd.home ();                   // loesche LCD Bildschirm, Cursor auf Anfang 
  lcd.print("Hallo Arduino ");  
}

void loop()
{
  int i;
  
  for(i=1; i<4; i++)
  {
    lcd.setCursor ( 0, i );
    lcd.print("Line ");
    lcd.print(i+1, DEC);
  }
}

