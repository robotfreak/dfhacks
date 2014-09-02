/* Digital Fotografie Hacks
 * I2C Servo Controller v0.1
 *
 */

#include <wire.h>

// define i2c slave address 
#define I2C_ADDRESS 0x2A
#define I2C_CMD_LEN  2

#define servo1Pin  7
#define servo2Pin  8

volatile char i2cCmd[I2C_CMD_LEN];
volatile bool i2cCmdRcv; 
volatile int i2cCmdIdx;

Servo myServo1();
Servo myServo2();

// callback for received data
void receiveData(int byteCount) 
{
  int i=0;
  while(Wire.available()>0) 
  {
    i2cCmd[i2cCmdIdx] = Wire.read(); 
    i2cCmdIdx++;
  }
  if (i2cCmdIdx == 2)
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

  // attach servos to servo pins
  myServo1.attach(servo1Pin);
  myServo2.attach(servo2Pin);

  // center servos
  myServo1.write(90);
  myServo2.write(90);
}

void loop()
{
  if (i2cCmdRcv == true)
  {
    i2cCmdRcv = false;
    if (i2cCmd[0] == 1)
      myServo1.write(cmd[1]);
    else if (i2cCmd[0] == 2)
      myServo2.write(cmd[1]);
  }
}


