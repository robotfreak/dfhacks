/* Digital Fotografie Hacks
 * CameraRemoteTest: 
 * Test des Kamera Fernsteuer Moduls
 * v0.1, 2014-01-28, Peter Recktenwald 
*/

#include <DFH_OptoCam.h>

/*-----( Konstanten Deklaration )-----*/ 
#define btn1Pin   11    /* Taster1 Testboard */
#define btn2Pin   10    /* Taster2 Testboard */

#define focusPin   8    /* Fokus Ausgang Kamera-Remote-Modul */
#define shutterPin 9    /* Ausloeser Ausgang Kamera-Remote-Modul */

DFH_OptoCam myCam(focusPin, shutterPin); 

void setup()
{
  /* Initialisierung serielle Schnittstelle */
  Serial.begin(57600);
  Serial.println("Kamera Fernsteuer Test v0.1");
}

void loop()
{
  /* Taster 1 steuert Ausloeser*/
  if (digitalRead(btn1Pin) == LOW)        /* Taster 1 gedrueckt */
  {
    myCam.launchShutter();                /* Ausloeser an */
    Serial.println("Ausloeser an");
    while (digitalRead(btn1Pin) == LOW);  /* warten, bis Taster 1 losgelassen */
    myCam.releaseShutter();               /* Ausloeser aus */
    Serial.println("Ausloeser aus");
  }

  /* Taster 2 steuert Fokus */
  if (digitalRead(btn2Pin) == LOW)        /* Taster 2 gedrueckt */
  {
    myCam.launchFocus();                  /* Fokus an */
    Serial.println("Fokus an");
    while (digitalRead(btn2Pin) == LOW);  /* warten, bis Taster 2 losgelassen */
    myCam.releaseFocus();                 /* Fokus aus */
    Serial.println("Fokus aus");
  }
  delay(100);  
}
