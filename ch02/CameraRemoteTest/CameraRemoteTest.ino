/* Digital Fotografie Hacks
 * CameraRemoteTest: 
 * Test des Kamera Remote Moduls
 * v0.1, 2014-01-28, Peter Recktenwald 
*/

/*-----( Konstanten Deklaration )-----*/ 
#define btn1Pin   11    /* Taster1 Testboard */
#define btn2Pin   10    /* Taster2 Testboard */

#define focusPin   8    /* Fokus Ausgang  Kamera-Remote-Modul */
#define shutterPin 9    /* Ausloeser Ausgang Kamera-Remote-Modul */

void setup()
{
  /* Initialisierung der Ausgangs Pins */
  pinMode(focusPin, OUTPUT);
  pinMode(shutterPin, OUTPUT);
  
  /* Initialisierung serielle Schnittstelle */
  Serial.begin(57600);
  Serial.println("Camera Remote v0.1");
}

void loop()
{
  /* Taster 1 steuert Ausloeser*/
  if (digitalRead(btn1Pin) == LOW)        /* Taster 1 gedrueckt */
  {
    digitalWrite(shutterPin, HIGH);       /* Ausloeser an */
    Serial.println("Shutter pressed");
    while (digitalRead(btn1Pin) == LOW);  /* warten, bis Taster 1 losgelassen */
    digitalWrite(shutterPin, LOW);        /* Ausloeser aus */
    Serial.println("Shutter released");
  }

  /* Taster 2 steuert Fokus */
  if (digitalRead(btn2Pin) == LOW)        /* Taster 2 gedrueckt */
  {
    digitalWrite(focusPin, HIGH);         /* Fokus an */
    Serial.println("Focus pressed");
    while (digitalRead(btn2Pin) == LOW);  /* warten, bis Taster 2 losgelassen */
    digitalWrite(focusPin, LOW);          /* Fokus aus */
    Serial.println("Focus released");
  }
  delay(100);
}
