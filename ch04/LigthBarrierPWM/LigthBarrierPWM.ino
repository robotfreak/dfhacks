/* Digital Fotografie Hacks
 * IR LightBarrierTest: 
 * Test der Infrarot Linchtschranke
 * v0.1, 2014-01-28, Peter Recktenwald 
*/
/*-----( Importiere benoetigte Bibliotheken )-----*/ 

/*-----( Konstanten Deklaration )-----*/  
#define irLedPin 13
#define irRcvPin A3

#define IR_CLOCK_RATE    36000L

int state, oldState;

ISR(TIMER1_COMPA_vect)
{
  digitalWrite(irLedPin, digitalRead(irLedPin) ^1);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(irLedPin, OUTPUT);

  // initialize timer1 
  noInterrupts();           // disable all interrupts
 // toggle on compare, clk/1
  OCR1A = (F_CPU/(IR_CLOCK_RATE*2L)-1);
  TCCR1A |= ((1 << WGM21) | (1 << COM2A0));
  TCCR1B |= (1 << CS20);
  TIMSK1 |= (1 << OCIE1A);
  // 36kHz carrier/timer
  interrupts();             // enable all interrupts
  
  Serial.begin(57600);
  Serial.println("Lichtschranken Test v0.1");
  
  oldState = -1;
  
}

void loop() {
  
/*  
  analogWrite(irLedPin, 128);
  delay(10);
  // put your main code here, to run repeatedly:
  state = analogRead(irRcvPin);
  if (state != oldState)
  {
    oldState = state;
    Serial.print("State: ");
    Serial.println(state, DEC);
  }
  analogWrite(irLedPin, 0);
  delay(200);
  */
}


