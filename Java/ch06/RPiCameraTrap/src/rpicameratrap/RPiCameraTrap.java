/* Digital Fotografie Hacks
 * Raspberry Pi Camera Trap: 
 * Kamera Falle mit dem Raspberry Pi Board, Kamera und PIR Sensor
 * v0.1, 2014-05-01, Peter Recktenwald 
 */

package rpicameratrap;

import com.pi4j.io.gpio.GpioController;
import com.pi4j.io.gpio.GpioFactory;
import com.pi4j.io.gpio.GpioPinDigitalInput;
import com.pi4j.io.gpio.PinPullResistance;
import com.pi4j.io.gpio.RaspiPin;
import com.pi4j.io.gpio.event.GpioPinDigitalStateChangeEvent;
import com.pi4j.io.gpio.event.GpioPinListenerDigital;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

/**
 *
 * @author Peter
 */
public class RPiCameraTrap {

public static String GetActDate() {
    DateFormat df = new SimpleDateFormat("yyyy-MM-dd-HH-mm-ss");

    // Get the date today using Calendar object.
    Date today = Calendar.getInstance().getTime();        
    // Using DateFormat format method we can create a string 
    // representation of a date with the defined format.
    String reportDate = df.format(today);
    return reportDate;    
}
   /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws InterruptedException {
        // TODO code application logic here
         Camera piCam = new Camera(); 
        // create gpio controller
        final GpioController gpio = GpioFactory.getInstance();

        // provision gpio pin #02 as an input pin with its internal pull down resistor enabled
        final GpioPinDigitalInput myButton = gpio.provisionDigitalInputPin(RaspiPin.GPIO_00, PinPullResistance.PULL_DOWN);

        // create and register gpio pin listener
        myButton.addListener(new GpioPinListenerDigital() {
            @Override
            public void handleGpioPinDigitalStateChangeEvent(GpioPinDigitalStateChangeEvent event) {
                // display pin state on console
                System.out.println(" --> GPIO PIN STATE CHANGE: " + event.getPin() + " = " + event.getState());
                piCam.takePicture("pic" + GetActDate());
    }
            
        });
        
        System.out.println(" ... complete the GPIO #00 circuit and see the listener feedback here in the console.");
        
        // keep program running until user aborts (CTRL-C)
        for (;;) {
            Thread.sleep(500);
        }
      }
    
}
