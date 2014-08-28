/* Digital Fotografie Hacks
 * Timelapse:
 * Timelapse Fotografie
 * v0.1, 2014-03-03, Peter Recktenwald
*/
/*-----( Import needed libraries )-----*/
#include <SoftwareSerial.h>   // We need this even if we're not using a SoftwareSerial object
// Due to the way the Arduino IDE compiles
// https://github.com/scogswell/ArduinoSerialCommand
#include <SerialCommand.h>
#include <DFH_OptoCam.h>

/*-----( Declare Constants )-----*/
#define focusPin    8
#define shutterPin  9

#define btnSTART  6
#define btnNONE   0

#define modNONE    0
#define modRUNNING 1

/*-----( Declare objects )-----*/
SerialCommand SCmd;   // The demo SerialCommand object
DFH_OptoCam myCam(focusPin, shutterPin);

/*-----( Declare Variables )-----*/

long repeatCount = 300;
long delayTime = 5000;
int Command = btnNONE;
long cnt, dly;
int mode = modNONE;

void setup() {
  // set up
  SCmd.addCommand("D", processDelayCmd);
  SCmd.addCommand("R", processRepeatCmd);
  SCmd.addCommand("S", processStartCmd);
  SCmd.addDefaultHandler(unrecognized);  // Handler for command that isn't matched  (says "What?")

  Serial.begin(57600);  // Used to type in characters
  Serial.println("Timelapse v0.1");
  Serial.println("available commands:");
  Serial.println("D <delaytime>");
  Serial.println("R <repeatcount>");
  Serial.println("S");
}

void processDelayCmd()
{
  int aNumber;
  char *arg;

  Serial.println("We're in processDelayCmd");
  arg = SCmd.next();
  if (arg != NULL)
  {
    aNumber = atoi(arg);  // Converts a char string to an integer
    Serial.print("Delay Time: ");
    Serial.println(aNumber);
    delayTime = aNumber;
  }
  else {
    Serial.println("No arguments");
  }
}

void processRepeatCmd()
{
  int aNumber;
  char *arg;

  Serial.println("We're in processRepeatCmd");
  arg = SCmd.next();
  if (arg != NULL)
  {
    aNumber = atoi(arg);  // Converts a char string to an integer
    Serial.print("Repeat Count: ");
    Serial.println(aNumber);
    repeatCount = aNumber;
  }
  else {
    Serial.println("No arguments");
  }
}

void processStartCmd()
{
  Serial.println("We're in processStartCmd");
  Command = btnSTART;
}

// This gets set as the default handler, and gets called when no other command matches.
void unrecognized()
{
  Serial.println("What?");
}

void loop()
{
  SCmd.readSerial();     // We don't do much, just process serial commands

  if (Command == btnSTART)
  {
    cnt = repeatCount;
    dly = delayTime;
    mode = modRUNNING;
    Command = btnNONE;
    Serial.println("running..");
  }

  if (mode == modRUNNING)
  {
    if (cnt)
    {
      delay(dly);
      myCam.shoot();
      Serial.print("shoot ");
      Serial.println(cnt, DEC);
      cnt--;
    }
    else
    {
      mode = modNONE;
    }
  }
}
