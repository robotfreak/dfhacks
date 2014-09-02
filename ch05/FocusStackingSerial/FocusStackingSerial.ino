/* Digital Fotografie Hacks
 * FocusStacking: 
 * Macro Focus Stacking mit 4-Wege-Schiene und Schrittmotor
 * v0.1, 2014-07-11, Peter Recktenwald 
*/ 
/*-----( Import needed libraries )-----*/
#include <SoftwareSerial.h>   // needed by SerialCommand
// https://github.com/scogswell/ArduinoSerialCommand
#include <SerialCommand.h>
#include "DFH_Stepper.h"
#include "DFH_OptoCam.h"

/*-----[Konstanten Deklaration]--------*/
#define dirPin  2
#define stepPin 3

#define focusPin    8 
#define shutterPin  9


#define limitPin    A6    // End Schalter

enum eCommand {
  cmdNONE,
  cmdCANCEL,
  cmdFOCUSSTACK
};

enum eMode {
  modNONE,
  modRUNNING
};

/*-----( Declare objects )-----*/
SerialCommand SCmd;   // The demo SerialCommand object
DFH_OptoCam myCam(focusPin, shutterPin);
DFH_Stepper myMotor(dirPin, stepPin);

/*-----[Variablen Deklaration]--------*/ 
int Command = cmdNONE;
int limitValue = 0;   // variable to store the value coming from the sensor
int delayTime = 1000;
int stepCount = 10;
int repeatCount = 20;
int cnt, steps;
int mode = modNONE;

void readSensors(void)
{
  limitValue = analogRead(limitPin);
}

// Drop Photos command Handler 
void processFocusStackingCmd()
{
  int aNumber;
  char *arg;

  Serial.println("We're in processFocusStackingCmd");
  arg = SCmd.next();
  if (arg != NULL)
  {
    aNumber = atoi(arg);  // Converts a char string to an integer
    delayTime = aNumber;
  }
  arg = SCmd.next();
  if (arg != NULL)
  {
    aNumber = atoi(arg);  // Converts a char string to an integer
    stepCount = aNumber;
  }
  arg = SCmd.next();
  if (arg != NULL)
  {
    aNumber = atoi(arg);  // Converts a char string to an integer
    repeatCount = aNumber;
  }
  if ((repeatCount > 0) && (delayTime > 0))
  {
    Command = cmdFOCUSSTACK;
    Serial.print("Delay Time: ");
    Serial.println(delayTime);
    Serial.print("Step Count: ");
    Serial.println(stepCount);
    Serial.print("Repeat Count: ");
    Serial.println(repeatCount);
  }
  else
    Serial.println("invalid arguments");
}

// Cancel command Handler
void processCancelCmd()
{
  Serial.println("We're in processCancelCmd");
  Command = cmdCANCEL;
}

// default handler
void unrecognized()
{
  Serial.println("error: unknown command!");
  printHelp();
}

// print help screen
void printHelp(void)
{
  Serial.println("available commands:");
  Serial.println("Focus Stacking: 'FS <delaytime> <stepcount> <repeatcount>'");
  Serial.println("Cancel: 'C'");
}

void setup()
{
  // set up
  SCmd.addCommand("FS", processFocusStackingCmd); // Handler for Drop Photos command
  SCmd.addCommand("C", processCancelCmd);      // Handler for Cancel command
  SCmd.addDefaultHandler(unrecognized);        // default handler
  Serial.begin(57600);
  Serial.println("Focus Stacking v0.1");
  printHelp();
}

void loop()
{
  // read the sensor
  readSensors();

  SCmd.readSerial();     // process serial commands
  
  switch(Command)
  {
    case cmdFOCUSSTACK:   // received timelapse command
      cnt = repeatCount;
      mode = modRUNNING;
      Command = cmdNONE;
      Serial.println("running..");
    break;
    case cmdCANCEL:     // received cancel command
      mode = modNONE;
      Command = cmdNONE;
      Serial.println("operation canceled..");
    break;
    default:
    break;
  }
    
  if (mode == modRUNNING)
  {
    if (cnt)
    {
      myCam.shoot();
      delay(delayTime);
      myMotor.motorMove(stepCount);
      Serial.print(" shoot ");
      Serial.println(cnt, DEC);
      cnt--;
      delay(2000);
    }
    else
    {
      steps = stepCount*repeatCount;
      Serial.print("Total Steps: ");
      Serial.print(steps);
      myMotor.motorMove(-steps);
      mode = modNONE;
      Serial.println("done..");
    }
  }
}
