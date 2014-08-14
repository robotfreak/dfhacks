/* Digital Fotografie Hacks
 * CameraDelayTest:
 * Messen der Kamera Ausloeseverzoegerung
 * v0.1, 2014-01-24, Peter Recktenwald 
*/

/*-----( Konstanten Deklaration )-----*
#define btn1Pin 11
#define btn2Pin 10

#define focusPin   8
#define shutterPin 9

#define led1Pin  7

#define ldrPin   A0
#define levelPin A2

/*-----( Variablen Deklaration )-----*/
unsigned long time;
int ldrVal;
int levelVal;

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

void setup()
{
  /* Initialisierung der Ausgangs Pins */
  pinMode(led1Pin, OUTPUT);
  pinMode(focusPin, OUTPUT);
  pinMode(shutterPin, OUTPUT);

  / Initialisierung serielle Schnittstelle */
  Serial.begin(57600);
  Serial.println("Camera Delay v0.1");

  ldrVal = analogRead(ldrPin);
  levelVal = analogRead(levelPin);
  printAnalogVals();
}


void loop()
{
  if (digitalRead(btn1Pin) == LOW)        /* Taster 1 gedrueckt */
  {
    while (digitalRead(btn1Pin) == LOW);  /* warten, bis Taster 1 losgelassen */
    digitalWrite(led1Pin, HIGH);          /* LED 1 an */
    digitalWrite(focusPin, HIGH);         /* Fokus an */
    digitalWrite(shutterPin, HIGH);       /* Ausloeser an */
    time = millis();
    while(ldrVal &lt; levelVal)           /* Zeit messen, bis Blitz ausloest
    {
      ldrVal = analogRead(ldrPin);
      levelVal = analogRead(levelPin);
      if (digitalRead(btn2Pin) == LOW)
        break;
    }
    digitalWrite(led1Pin, LOW);           /* LED 1 aus */
    digitalWrite(focusPin, LOW);          /* Fokus aus */
    digitalWrite(shutterPin, LOW);        /* Ausloeser aus */
    printTime();
    delay(200);
  }
  else if (digitalRead(btn2Pin) == LOW)   /* Taster 2 gedrueckt */
  {
    while (digitalRead(btn2Pin) == LOW);  /* warten, bis Taster 1 losgelassen */
    ldrVal = analogRead(ldrPin);
    levelVal = analogRead(levelPin);
    printAnalogVals();
    delay(200);
  }
}

