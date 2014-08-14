/* Digital Fotografie Hacks
 * CameraModulTest: 
 * Test des Kamera Fernsteuer Moduls
 * v0.1, 2014-08-08, Peter Recktenwald 
*/
#include <DFH_OptoFlash.h>

#define flash2Pin   8    /* Fokus Kamera-Remote-Modul */
#define flash1Pin   9    /* Ausloeser Kamera-Remote-Modul */
 
DFH_OptoFlash myFlash1(flash1Pin); /* Blitz1 Objekt anlegen */
DFH_OptoFlash myFlash2(flash2Pin); /* Blitz2 Objekt anlegen */

void setup() {

}

void loop() {
    myFlash1.fire();   /* Blitz1 ausloesen */
    delay(5000);       /* 5 Sekunden warten */
    myFlash2.fire();   /* Kamera ausloesen */
    delay(5000);       /* 5 Blitz2 warten */
}
