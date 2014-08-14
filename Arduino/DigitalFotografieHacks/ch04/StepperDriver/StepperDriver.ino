/* Digital Fotografie Hacks
 * StepperDriver: 
 * Schrittmotor Treiber Ansteuerung
 * v0.1, 2014-03-03, Peter Recktenwald 
*/ 

/*-----( Declare Constants )-----*/
#define dirPin  2
#define stepPin 3

#define FORWARD   true
#define BACKWARD  false

// set motor direction
void motorDirection(boolean direction)
{
  digitalWrite(dirPin, direction);
  delayMicroseconds(100);
}

// do a single step
void motorStep()
{
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(100);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(100);
  delay(5);
}

// move the motor n steps
void motorMove(int steps)
{
  if (steps < 0)
    motorDirection(BACKWARD);
  else
    motorDirection(FORWARD);
  for(int i=0; i<steps; i++)
  {
    motorStep();
  }
}

// init the stepper motor driver
void motorInit()
{
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
}

void setup()
{
  motorInit();
}

void loop()
{
  motorMove(200);
  delay(2000);
  motorMove(-200);
  delay(2000);
}
