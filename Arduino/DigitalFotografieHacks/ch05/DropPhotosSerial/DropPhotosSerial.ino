/* Digital Fotografie Hacks
 * DropPhotos: 
 * Tropfen Fotografie mit dem Foto Shield
 * v0.1, 2014-01-26, Peter Recktenwald 
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

#define valvePin    7

#define btnTEST   7
#define btnSTART  6
#define btnNONE   0

#define modNONE    0
#define modRUNNING 1

/*-----( Declare objects )-----*/
SerialCommand SCmd;   // The demo SerialCommand object
DFH_OptoCam myCam(focusPin, shutterPin);

/*-----( Declare Variables )-----*/

long repeatCount = 1;
int delayTime = 300;
int Command = btnNONE;
int valveParam[10];
int timeParam[10];
long cnt, dly, dlyofs;
int mode = modNONE;

void processDelayCmd(void);   
void processValveCmd(void);   
void processStartCmd(void);   
void processTestCmd(void);   
void unrecognized(void);
  
void setup() {
  // set up
  pinMode(valvePin, OUTPUT);
  digitalWrite(valvePin, LOW);

  SCmd.addCommand("D", processDelayCmd);
  SCmd.addCommand("R", processRepeatCmd);
  SCmd.addCommand("V", processValveCmd);
  SCmd.addCommand("S", processStartCmd);
  SCmd.addCommand("T", processTestCmd);
  SCmd.addDefaultHandler(unrecognized);  // Handler for command that isn't matched  (says "What?")

  Serial.begin(57600);  // Used to type in characters
  Serial.println("Drop Photos v0.1");
  Serial.println("available commands:");
  Serial.println("D <delaytime>");
  Serial.println("R <repeatcount>");
  Serial.println("V <number> <ontime1> <offtime1> <ontime2> offtime2> <ontime3> <offtime3>");
  Serial.println("S");
  Serial.println("T <from> <to> <steps>");
  valveParam[0]= 20;
  valveParam[1]= 20;
  valveParam[2]= 20;
}

void processDelayCmd()    
{
  int aNumber;  
  char *arg; 

  Serial.println("We're in processDelayCmd"); 
  arg = SCmd.next(); 
  if (arg != NULL) 
  {
    aNumber=atoi(arg);    // Converts a char string to an integer
    Serial.print("Shutter Delay: "); 
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

void processValveCmd()    
{
  int aNumber, paramIdx;  
  char *arg; 

  Serial.println("We're in processValveCmd"); 
  paramIdx = 0;
  do {
    arg = SCmd.next();
    if (arg != NULL) 
    {
      aNumber=atoi(arg);    // Converts a char string to an integer
      Serial.print("Valve"); 
      if ((paramIdx & 1) == 0)
        Serial.print(" on: "); 
      else
        Serial.print(" off: "); 
      Serial.println(aNumber); 
      valveParam[paramIdx++] = aNumber;
    }    
  } while(arg != NULL);
  valveParam[paramIdx++] = 0;    
}

void processStartCmd()
{
  Serial.println("We're in processStartCmd");
  Command = btnSTART;
}

void processTestCmd()    
{
  int aNumber, paramIdx;  
  char *arg; 

  Serial.println("We're in processTestCmd"); 
  paramIdx = 0;
  do {
    arg = SCmd.next();
    if (arg != NULL) 
    {
      aNumber=atoi(arg);    // Converts a char string to an integer
      Serial.print("Time: "); 
      Serial.println(aNumber); 
      timeParam[paramIdx++] = aNumber;
    }    
  } while(arg != NULL);
  timeParam[paramIdx++] = 0;  
  if (paramIdx == 4 && (timeParam[0] < timeParam[1]) && (timeParam[2] > 0) && ((timeParam[1] - timeParam[0]) > timeParam[2]) ) 
    Command = btnTEST;
  else
    Serial.println("Invalid params"); 
   
}

// This gets set as the default handler, and gets called when no other command matches. 
void unrecognized()
{
  Serial.println("What?"); 
}

void launchDrop()
{
  int idx, val;
  
  for(idx=0; idx < 6; idx++)
  {
    val = valveParam[idx];
    if (val && ((idx & 1) == 0) )
    {
      digitalWrite(valvePin, HIGH);
      delay(val);
      digitalWrite(valvePin, LOW);
    }
    else if (val)
    {
      digitalWrite(valvePin, LOW);
      delay(val);
    }
    else
      break;
  }
}

void loop() 
{

  SCmd.readSerial();     // We don't do much, just process serial commands
  
  if (Command == btnSTART)
  {
    cnt = repeatCount;
    dly = delayTime;
    dlyofs = 0;
    mode = modRUNNING;
    Command = btnNONE;
    Serial.println("running..");
  }

  if (Command == btnTEST)
  {
    cnt = (timeParam[1]-timeParam[0])/timeParam[2];
    dly = timeParam[0];
    dlyofs = timeParam[2];
    mode = modRUNNING;
    Command = btnNONE;
    Serial.println("running..");
  }

  if (mode == modRUNNING)
  {
    if (cnt)
    {
      launchDrop();
      delay(dly);
      myCam.shoot();
      Serial.print("shoot ");
      Serial.println(cnt, DEC);
      dly += dlyofs;
      cnt--;
      delay(5000);
    }
    else
    {
      mode = modNONE;
      Serial.println("done..");
    }
  }
}
