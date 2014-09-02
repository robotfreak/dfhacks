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

enum eCommand {
  cmdNONE,
  cmdCANCEL,
  cmdTIMELAPSE
};

enum eMode {
  modNONE,
  modRUNNING
};

/*-----( Declare objects )-----*/
SerialCommand SCmd;   // The demo SerialCommand object
DFH_OptoCam myCam(focusPin, shutterPin);

/*-----( Declare Variables )-----*/

long repeatCount = 300;
long delayTime = 5000;
int Command = cmdNONE;
long cnt, dly;
int mode = modNONE;

// Start command Handler 
void processTimeLapseCmd()
{
  int aNumber;
  char *arg;

  Serial.println("We're in processTimeLapseCmd");
  arg = SCmd.next();
  if (arg != NULL)
  {
    aNumber = atoi(arg);  // Converts a char string to an integer
    delayTime = aNumber;
    arg = SCmd.next();
    if (arg != NULL)
    {
      aNumber = atoi(arg);  // Converts a char string to an integer
      repeatCount = aNumber;
    }
  }
  if ((repeatCount > 0) && (delayTime > 0))
  {
    Command = cmdTIMELAPSE;
    Serial.print("Delay Time: ");
    Serial.println(delayTime);
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
  Serial.println("Time lapse: 'TL <delaytime> <repeatcount>'");
  Serial.println("Cancel: 'C'");
}

void setup() {
  // set up
  SCmd.addCommand("TL", processTimeLapseCmd); // Handler for Time Lapse command
  SCmd.addCommand("C", processCancelCmd);     // Handler for Cancel command
  SCmd.addDefaultHandler(unrecognized);       // default handler

  Serial.begin(57600);  // Used to type in characters
  Serial.println("Timelapse v0.1");
  printHelp();  
}


void loop()
{
  SCmd.readSerial();     // process serial commands
  
  switch(Command)
  {
    case cmdTIMELAPSE:   // received timelapse command
      cnt = repeatCount;
      dly = delayTime;
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
      Serial.print("shoot ");
      Serial.println(cnt, DEC);
      delay(dly);
      cnt--;
    }
    else
    {
      mode = modNONE;
    }
  }
}
