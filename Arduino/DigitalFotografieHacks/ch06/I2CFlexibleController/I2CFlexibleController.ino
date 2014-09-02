/* Digital Fotografie Hacks
 * I2C Flexible Controller v0.1
 *
 */

#include <Wire.h>
//#include <DFH_Stepper.h>
//#include <DFH_DCMotor.h>
//#include <DFH_OptoCam.h>
//#include <DFH_OptoFlash.h>

// define i2c slave address 
#define I2C_ADDRESS 0x2A
#define I2C_CMD_LEN_MAX  16

#define PIN_TOTAL 20
#define SERVO_TOTAL 6

enum eConfig {
  CFG_INPUT,
  CFG_OUTPUT,
  CFG_SERVO = 4,
//  CFG_MOTOR,
//  CFG_STEPPER,
//  CFG_VALVE,
//  CFG_CAMERA,
//  CFG_FLASH,
//  CFG_SENSOR,
  CFG_LAST = 0xF
};

enum eCommand {
  CMD_FIRMWARE,
  CMD_CONFIG   = 0x10,
  CMD_DIGITAL  = 0x20,
  CMD_ANALOG   = 0x30,
  CMD_SERVO    = 0x40,
//  CMD_MOTOR    = 0x50,
//  CMD_STEPPER  = 0x60,
//  CMD_VALVE    = 0x70,
//  CMD_CAMERA   = 0x80,
//  CMD_FLASH    = 0x90,
//  CMD_SENSOR   = 0xA0,
//  CMD_RF24     = 0xE0,
  CMD_UART     = 0xF0
};


volatile char i2cCmd[I2C_CMD_LEN];
volatile bool i2cCmdRcv; 
volatile int i2cCmdIdx;
volative int i2CCmdLen;
volatile int i2cWrtCommand;

struct pinConfig
{
  int mode;
  int pin;
  int val;
  int num;
};

struct pinMode pinCfg[PIN_TOTAL];
// available pins are 2..8 (digital),  14..17 e.g A0..A4 (analog)
bool pinsAvailable[PIN_TOTAL] = { 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0};
// reserved pins are 0,1 (uart), 9..13 (rf24), 18,19 e.g A4, A5 (i2c)
bool pinsReserved[PIN_TOTAL] =  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1};

int servoUsed;
Servo myServos[SERVO_TOTAL];

//DFH_Stepper myStepper();
//DFH_DCMotor myMotor();
//DFH_OptoCam myCam();
//DFH_OptoFlash myFlash();
//DFH_OptoFlash myFlash2();

// callback for received data
void receiveData(int byteCount) 
{
  while(Wire.available()>0) 
  {
    i2cCmd[i2cCmdIdx] = Wire.read(); 
    i2cCmdIdx++;
  }
  if (i2cCmdIdx == 1)  // first byte cmd & len
  {
    i2cWrtCommand = i2cCmd[0] & 0xFF;
    i2cCmdLen = i2cCmd[0] & 0x0F;
  }
  else if (i2cCmdIdx >= i2cCmdLen)  // command completed
  {
    i2cCmdRcv = true;
    i2cCmdIdx = 0;
  }
}

// callback for sending data
void sendData()
{ 
}

void setup() 
{
  Serial.begin(57600);
  Serial.println("I2C Servo Controller v0.1");
  // initialize i2c as slave
  Wire.begin(I2C_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  //  Wire.onRequest(sendData); 
 
  i2cCmdRcv = false;
  i2cCmdIdx = 0;
  i2cCmdLen = 0;
  i2cWrtCommand = 0;

  servoUsed = 0;
}


void parseConfigCmd(void)
{
  byte i, pin, val;
  for (i=1; i<i2cCmdRcv; i+=2)
  {
    pin = i2cCmd[i];
    val = i2cCmd[i+1];
    if (pin < PIN_TOTAL && pinsAvailable[pin])
    {
      if (pinCfg[pin].mode == CFG_DIGITAL) // was pin configured digital pin
        pinMode(pin, INPUT);              // set to input first

      if (pinCfg[pin].mode == CFG_SERVO)  // was pin configured for servo?
      {
        myServos[servoUsed].detach(pin);  // detach servo first
        pinCfg[pin].num = 0;
        servoUsed--;
      }
      switch(val)
      {
        case CFG_DIGITAL:
          pinMode(pin, val);
          pinCfg[pin].mode = val;
          pinCfg[pin].pin = pin;
          pinCfg[pin].val = 0;
          pinCfg[pin].num = 0;
        break;
        case CFG_ANALOG:
          if (pin == 3 || or pin == 5 || pin == 6)  // only pwm pins
          {
            pinCfg[pin].mode = val;
            pinCfg[pin].pin = pin;
            pinCfg[pin].val = 0;
            pinCfg[pin].num = 0;
          }
        break;
        case CFG_SERVO:
          if (servoUsed < SERVO_TOTAL)
          {
            myServos[servoUsed].attach(pin);
            pinCfg[pin].mode = val;
            pinCfg[pin].pin = pin;
            pinCfg[pin].val = 0;
            pinCfg[pin].num = servoUsed;
            servoUsed++;
          }
        break;
      }
    }
  }
}

void parseDigitalCmd(void)
{
  byte i, pin, val;
  for (i=1; i<i2cCmdRcv; i+=2)
  {
    pin = i2cCmd[i];
    val = i2cCmd[i+1];
    if (pin < PIN_TOTAL && pinsAvailable[pin] && pinCfg[pin].mode == CFG_DIGITAL)
    {
      DigitalWrite(pin, val);
    }
  }
}


void parseAnalogCmd(void)
{
  byte i, pin, val;
  for (i=1; i<i2cCmdRcv; i+=2)
  {
    pin = i2cCmd[i];
    val = i2cCmd[i+1];
    if (pin < PIN_TOTAL && pinsAvailable[pin] && pinCfg[pin].mode == CFG_ANALOG)
    {
      AnalogWrite(pin, val);
    }
  }
}

void parseServoCmd(void)
{
  byte i, pin, val;
  for (i=1; i<i2cCmdRcv; i+=2)
  {
    pin = i2cCmd[i];
    val = i2cCmd[i+1];
    if (pin < PIN_TOTAL && pinsAvailable[pin] && pinCfg[pin].mode == CFG_SERVO)
    {
      myServos[pin].write(val);
    }
  }
}

void loop()
{
  if (i2cCmdRcv == true)
  {
    i2cCmdRcv = false;
    switch (i2cWrtCommand)
    {
      case CMD_CONFIG:
        parseConfigCmd();
      break;
      case CMD_DIGITAL:
        parseDigitalCmd();
      break;
      case CMD_ANALOG:
        parseAnalogCmd();
      break;
      case CMD_SERVO:
        parseServoCmd();
      break;
      default:
      break;
    }
  }
}


