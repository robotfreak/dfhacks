/* Digital Fotografie Hacks
 * CameraDelay: 
 * Messen der Kamera Ausloeseverzoegerung
 * v0.1, 2014-01-24, Peter Recktenwald 
*/


#define btn1Pin 11
#define btn2Pin 10

#define focusPin   8
#define shutterPin 9

#define led1Pin  7
#define led2Pin  6

#define ldrPin   A0
#define levelPin A2

unsigned long time;  
int ldrVal;
int levelVal;
  

void printDigitalVals()
{
  Serial.print("btn1: ");
  Serial.print(digitalRead(btn1Pin), DEC);
  Serial.print("\tbtn2: ");
  Serial.println(digitalRead(btn2Pin), DEC);

}

void printAnalogVals()
{
  Serial.print("ldr: "); 
  Serial.print(ldrVal, DEC);
  Serial.print("\tlevel: ");
  Serial.println(levelVal, DEC);
  
}

void printTime()
{
  Serial.print("Delay: ");
  Serial.print(millis() - time, DEC);
  Serial.println(" ms");
}

void cameraShutterFocusOn()
{
  digitalWrite(led1Pin, HIGH);
  digitalWrite(focusPin, HIGH);
  digitalWrite(shutterPin, HIGH);
}

void cameraShutterFocusOff()
{
  digitalWrite(led1Pin, LOW);
  digitalWrite(focusPin, LOW);
  digitalWrite(shutterPin, LOW);
}

void setup()
{
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(focusPin, OUTPUT);
  pinMode(shutterPin, OUTPUT);
  cameraShutterFocusOff  ();
  
  Serial.begin(57600);
  Serial.println("Camera Delay v0.1");
  
  ldrVal = analogRead(ldrPin);
  levelVal = analogRead(levelPin);
  printAnalogVals();
  
  printDigitalVals();
}


void loop()
{
  if (digitalRead(btn1Pin) == LOW)
  {
    while (digitalRead(btn1Pin) == LOW);
    cameraShutterFocusOn();
    time = millis();
    while(ldrVal < levelVal)
    {
      ldrVal = analogRead(ldrPin);
      levelVal = analogRead(levelPin);
      if (digitalRead(btn2Pin) == LOW)
        break;
    }
    cameraShutterFocusOff();
    printTime();
    delay(200);
  }
  else if (digitalRead(btn2Pin) == LOW)
  {
    while (digitalRead(btn2Pin) == LOW);
    ldrVal = analogRead(ldrPin);
    levelVal = analogRead(levelPin);
    printAnalogVals();
    delay(200);
  }
    
}
  
