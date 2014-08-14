/* Digital Fotografie Hacks
 * FlashRemote: 
 * Test des Flash Remote Moduls
 * v0.1, 2014-01-25, Peter Recktenwald 
*/


#define btn1Pin 11
#define btn2Pin 10

#define flash2Pin   3
#define flash1Pin 2

#define led1Pin  7
#define led2Pin  6

void printDigitalVals()
{
  Serial.print("btn1: ");
  Serial.print(digitalRead(btn1Pin), DEC);
  Serial.print("\tbtn2: ");
  Serial.println(digitalRead(btn2Pin), DEC);
}

void flash1On()
{
  digitalWrite(led1Pin, HIGH);
  digitalWrite(flash1Pin, HIGH);
}

void flash1Off()
{
  digitalWrite(led1Pin, LOW);
  digitalWrite(flash1Pin, LOW);
}

void flash2On()
{
  digitalWrite(led2Pin, HIGH);
  digitalWrite(flash2Pin, HIGH);
}

void flash2Off()
{
  digitalWrite(led2Pin, LOW);
  digitalWrite(flash2Pin, LOW);
}

void setup()
{
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(flash1Pin, OUTPUT);
  pinMode(flash2Pin, OUTPUT);
  flash1Off();
  flash2Off();
  
  Serial.begin(57600);
  Serial.println("Flash Remote v0.1");
  
  printDigitalVals();
}


void loop()
{
  if (digitalRead(btn1Pin) == LOW)
  {
    flash1On();
    Serial.println("Flash1 pressed");
    printDigitalVals();
    while (digitalRead(btn1Pin) == LOW);
    flash1Off();
    Serial.println("Flash1 released");
  }
  if (digitalRead(btn2Pin) == LOW)
  {
    flash2On();
    Serial.println("Flash2 pressed");
    printDigitalVals();
    while (digitalRead(btn2Pin) == LOW);
    flash2Off();
    Serial.println("Flash2 released");
  }
  delay(100);  
}
  
