/* Digital Fotografie Hacks
 * CameraModulTest: 
 * Test des Kamera Fernsteuer Moduls
 * v0.1, 2014-08-08, Peter Recktenwald 
*/
#include <DFH_OptoCam.h>

#define focusPin   8    /* Fokus Kamera-Remote-Modul */
#define shutterPin 9    /* Ausloeser Kamera-Remote-Modul */
 
DFH_OptoCam myCam(focusPin, shutterPin); /* Kamera Objekt anlegen */

void setup() {

}

void loop() {
    myCam.shoot();     /* Kamera ausloesen */
    delay(5000);       /* 5 Sekunden warten */
}
