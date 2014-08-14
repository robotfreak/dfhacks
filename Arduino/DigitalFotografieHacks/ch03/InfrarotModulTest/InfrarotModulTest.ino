/* Digital Fotografie Hacks
 * IR CameraRemoteTest: 
 * Test des Infrarot Kamera Fernsteuer Moduls
 * v0.1, 2014-01-28, Peter Recktenwald 
*/

#include <DFH_IrCam.h>
 
const int irLED = 13;
const int SWITCH = 7;
boolean running = false;
long previousMillis = 0;        // will store last time LED was updated
long interval = 200;           // interval at which to blink (milliseconds)

unsigned int pulseDuration = 10; //microseconds 
//The required 15 microseconds pulseDuration didn't work since digitalWrite consumes some additional time
//thats adds to pulseDuration value. 10 to 12 microseconds worked for me.

unsigned int photo = 7330; //A 7330 microseconds delay between bursts shoots a photo.
unsigned int video = 5360; //A 5360 microseconds delay between bursts starts/stops video recording. 

void setup() 
{
  Serial.begin(38400); // uncomment for debugging
  pinMode(irLED, OUTPUT);

}

void loop() 
{ //run again and again 

  if (running == true) // This part of the loop is to test if the camera has been triggered to record 
    // and blinks the power LED while recording.
  {

    Serial.println("running"); //uncomment for debugging

    unsigned long currentMillis = millis();

    if(currentMillis - previousMillis > interval) {
      // save the last time you blinked the LED 
      previousMillis = currentMillis;   
    }
  }

  if (digitalRead(SWITCH) == LOW) 
  { //read switch input
    shoot(photo); //start photo shooting
    if (running == false)  //detect the state of recording and invert it
    {
      running = true;
    }
    else
    {
      running = !running;
    }
    delay(500);
  }
}

void shoot(unsigned int delayBetweenBursts) 
{ //sends the IR signal

  //send first 16 bursts
  for(int i=0; i&lt;16; i++) 
  { 
    digitalWrite(irLED, HIGH);
    delayMicroseconds(pulseDuration);
    digitalWrite(irLED, LOW);
    delayMicroseconds(pulseDuration);
  } 

  delayMicroseconds(delayBetweenBursts); 

  //send second 16 bursts
  for(int i=0; i&lt;16; i++) 
  { 
    digitalWrite(irLED, HIGH);
    delayMicroseconds(pulseDuration);
    digitalWrite(irLED, LOW);
    delayMicroseconds(pulseDuration);
  } 
  return;
}
