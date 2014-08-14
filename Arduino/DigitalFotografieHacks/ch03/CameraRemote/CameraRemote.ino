/* Digital Fotografie Hacks
 * CameraRemote: 
 * Test des Kamera Remote Moduls
 * v0.1, 2014-01-25, Peter Recktenwald 
*/

/*-----( Konstanten Deklaration )-----*/ 
#define btn1Pin   11    /* Taster1 Testboard */
#define btn2Pin   10    /* Taster2 Testboard */

#define led1Pin    7    /* LED1 Testboard */
#define led2Pin    6    /* LED2 Testboard */

#define focusPin   8    /* Fokus Kamera-Remote-Modul */
#define shutterPin 9    /* Ausloeser Kamera-Remote-Modul */


void printDigitalVals()
{
  Serial.print("btn1: ");
  Serial.print(digitalRead(btn1Pin), DEC);
  Serial.print("\tbtn2: ");
  Serial.println(digitalRead(btn2Pin), DEC);

}

void cameraShutterOn()
{
  digitalWrite(led1Pin, HIGH);
  digitalWrite(shutterPin, HIGH);
}

void cameraShutterOff()
{
  digitalWrite(led1Pin, LOW);
  digitalWrite(shutterPin, LOW);
}

void cameraFocusOn()
{
  digitalWrite(led2Pin, HIGH);
  digitalWrite(focusPin, HIGH);
}

void cameraFocusOff()
{
  digitalWrite(led2Pin, LOW);
  digitalWrite(focusPin, LOW);
}

void setup()
{
  
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(focusPin, OUTPUT);
  pinMode(shutterPin, OUTPUT);
  cameraShutterOff();
  cameraFocusOff();
  
  Serial.begin(57600);
  Serial.println("Camera Remote v0.1");
  
  printDigitalVals();
}


void loop()
{
  /* Taster 1 steuert Ausloeser*/
  if (digitalRead(btn1Pin) == LOW)
  {
    cameraShutterOn();
    Serial.println("Shutter pressed");
    printDigitalVals();
    while (digitalRead(btn1Pin) == LOW);
    cameraShutterOff();
    Serial.println("Shutter released");
  }

  /* Taster 2 steuert Fokus */
  if (digitalRead(btn2Pin) == LOW)
  {
    cameraFocusOn();
    Serial.println("Focus pressed");
    printDigitalVals();
    while (digitalRead(btn2Pin) == LOW);
    cameraFocusOff();
    Serial.println("Focus released");
  }
  delay(100);  
}
  
