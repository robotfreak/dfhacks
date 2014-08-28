/* Digital Fotografie Hacks
 * IR LightBarrierTest: 
 * Test der Infrarot Lichtschranke
 * v0.1, 2014-08-26, Peter Recktenwald 
*/
/*-----( Importiere benoetigte Bibliotheken )-----*/ 
#include <TimerOne.h>

/*-----( Konstanten Deklaration )-----*/  
#define irLedPin 13
#define irRcvPin 12

/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void timerIsr()
{
    // Toggle LED
    digitalWrite( irLedPin, digitalRead( irLedPin ) ^ 1 );
}

void setup() {
  Serial.begin(57600);
  Serial.println("IR light barrier test v0.1");
  pinMode(irLedPin, OUTPUT);    // IR LED pin as output
  
  Timer1.initialize(14); // set a timer of length 14 microseconds, 
                         // the IR LED will blink with ~36kHz
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here
}

void loop() {
  if (digitalRead(irRcvPin) == LOW)
  {
    noInterrupts();           // disable all interrupts 
    Serial.println("Object");
    interrupts();
    delay(200);
  }
}
