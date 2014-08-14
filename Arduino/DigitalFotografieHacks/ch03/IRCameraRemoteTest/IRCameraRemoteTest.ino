/* Digital Fotografie Hacks
 * IR CameraRemoteTest: 
 * Test des Infrarot Kamera Fernsteuer Moduls
 * v0.1, 2014-01-28, Peter Recktenwald 
*/
#include <multiCameraIrControl.h>

#define cameraPin 7

Canon EOS7D(cameraPin);

void setup()
{
}

void loop()
{
  EOS7D.shutterNow();
  delay(5000);
}
