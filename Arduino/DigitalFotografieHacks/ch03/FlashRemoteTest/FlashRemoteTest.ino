/* Digital Fotografie Hacks
 * FlashRemoteTest: 
 * Test des Blitz Fernsteuer Moduls
 * v0.1, 2014-01-28, Peter Recktenwald 
*/

/*-----( Konstanten Deklaration )-----*/ 
#define btn1Pin   11    /* Taster1 Testboard */
#define btn2Pin   10    /* Taster2 Testboard */

#define led1Pin    7    /* LED1 Testboard */
#define led2Pin    6    /* LED2 Testboard */

#define flash1Pin   2    /* Blitz 1 Ausgang Blitz-Remote-Modul */
#define flash2Pin   3    /* Blitz 2 Ausgang Blitz-Remote-Modul */


void setup()
{
  /* Initialisierung der Ausgangs Pins */
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(flash1Pin, OUTPUT);
  pinMode(flash2Pin, OUTPUT);
  
  /* Initialisierung serielle Schnittstelle */
  Serial.begin(57600);
  Serial.println("Flash Fernsteuer Test v0.1");
}

void loop()
{
  /* Taster 1 steuert Blitz 1*/
  if (digitalRead(btn1Pin) == LOW)        /* Taster 1 gedrueckt */
  {
    digitalWrite(led1Pin, HIGH);          /* LED 1 an */
    digitalWrite(flash1Pin, HIGH);        /* Blitz 1 an */
    Serial.println("Blitz 1 an");
    while (digitalRead(btn1Pin) == LOW);  /* warten, bis Taster 1 losgelassen */
    digitalWrite(led1Pin, LOW);           /* LED 1 aus */
    digitalWrite(flash1Pin, LOW);         /* Blitz 1 aus */
    Serial.println("Blitz 1 aus");
  }

  /* Taster 2 steuert Blitz 2 */
  if (digitalRead(btn2Pin) == LOW)        /* Taster 2 gedrueckt */
  {
    digitalWrite(led2Pin, HIGH);          /* LED 2 an */
    digitalWrite(flash2Pin, HIGH);        /* Blitz 2 an */
    Serial.println("Blitz 2 an");
    while (digitalRead(btn2Pin) == LOW);  /* warten, bis Taster 2 losgelassen */
    digitalWrite(led2Pin, LOW);           /* LED 2 aus */
    digitalWrite(flash2Pin, LOW);         /* Blitz 2 aus */
    Serial.println("Blitz 2 aus");
  }
  delay(100);  
}
