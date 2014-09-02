/* Digital Fotografie Hacks
 * SensorTrigger: 
 * Analoger Sensor als Kamera Ausloeser
 * v0.1, 2014-05-22, Peter Recktenwald 
*/
/*-----( Import needed libraries )-----*/
#include <SoftwareSerial.h>   // needed by SerialCommand
// https://github.com/scogswell/ArduinoSerialCommand
#include <SerialCommand.h>
#include <DFH_OptoCam.h>
#include <DFH_OptoFlash.h>

/*-----[Konstanten Deklaration]--------*/
#define focusPin    8 
#define shutterPin  9
#define flashPin    7
#define sensorPin   A2    // select the input pin for the potentiometer

enum eCommand {
  cmdNONE,
  cmdCANCEL,
  cmdSENSORTRIGGER
};

enum eMode {
  modNONE,
  modRUNNING
};

/*-----( Declare objects )-----*/
SerialCommand SCmd;   // The demo SerialCommand object
DFH_OptoCam myCam(focusPin, shutterPin);
DFH_OptoFlash myFlash(flashPin);

/*-----[Variablen Deklaration]--------*/
long repeatCount = 1;
int delayTime = 4000;
int delayIncr = 0;
int Command = cmdNONE;
int sensorValue = 0;   // variable to store the value coming from the sensor
int levelValue = 100;  
long cnt, dly;
int mode = modNONE;

// Drop Photos command Handler 
void processSensorTriggerCmd()
{
  int aNumber;
  char *arg;

  Serial.println("We're in processSensorTriggerCmd");
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
    levelValue = aNumber;
  }
  arg = SCmd.next();
  if (arg != NULL)
  {
    aNumber = atoi(arg);  // Converts a char string to an integer
    repeatCount = aNumber;
  }
  if ((repeatCount > 0) && (delayTime > 0))
  {
    Command = cmdSENSORTRIGGER;
    Serial.print("Delay Time: ");
    Serial.println(delayTime);
    Serial.print("Level Value: ");
    Serial.println(levelValue);
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
  Serial.println("Sensor Trigger: 'ST <delaytime> <level> <repeatcount>'");
  Serial.println("Cancel: 'C'");
}

void readSensors(void)
{
  sensorValue = analogRead(sensorPin);    
} 

int runSensorTrigger(void
{
  int ret = 0;
  unsigned long currentMillis = millis();
  myCam.launchFocusShutter();
  readSensors();
  if (sensorValue < levelValue)
  {
    myFlash.fire();
    myCam.releaseFocusShutter();
    ret = 1;
  }
  return ret;
}

void setup() {
  // set up
  SCmd.addCommand("ST", processSensorTriggerCmd); // Handler forSensor Trigger command
  SCmd.addCommand("C", processCancelCmd);      // Handler for Cancel command
  SCmd.addDefaultHandler(unrecognized);        // default handler
  Serial.begin(57600);
  Serial.println("Trigger Sensor v0.1");
  printHelp();
}

void loop() {

  // read the sensor
  readSensors();

  SCmd.readSerial();     // process serial commands
  
  switch(Command)
  {
    case cmdSENSORTRIGGER:   // received timelapse command
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
      if (runSensorTrigger() == true)
      {
        Serial.print("Sensor: ");
        Serial.println(sensorValue);
        cnt--;
      }
    }
    else
    {
      mode = modNONE;
      Serial.println("done..");
    }
  }


}
