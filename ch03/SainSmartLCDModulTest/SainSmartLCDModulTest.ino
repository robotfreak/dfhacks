/* Digital Fotografie Hacks
 * SainSmartLCDModulTest: 
 * LCD-Modul Test fuer Sainsmart YwRobot LCD Module
 * v0.1, 2014-06-02, Peter Recktenwald 
*/
/*-----( Importiere benoetigte Bibliotheken )-----*/
#include <Arduino.h>  // Comes with Arduino IDE 
#include <Wire.h>
// Die LCD I2C Bibliothek befindet sich hier: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
#include <LiquidCrystal_I2C.h>  // fuer SainSmart, YwRobot I2C LCD 

// Setze die LCD Addresse auf 0x27 for ein 20 chars 4 line display
// Setze die Pin Belegung des I2C chips fuer die LCD Anbindung:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // YwRobot 
//LiquidCrystal_I2C lcd(0x3F);                                    // Sainsmart 2004
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);    // Sainsmart 2004 (neu)

void setup()
{
  lcd.begin(20,4);               // Initialisiere LCD Modul mit 16 x 4 Zeichen
  lcd.home ();                   // go home
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

