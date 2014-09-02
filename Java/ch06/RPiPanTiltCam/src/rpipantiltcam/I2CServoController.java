/* Digital Fotografie Hacks
 * Raspberry Pi Pan Tilt Camera: 
 * I2CServoController class
 * v0.1, 2014-07-02, Peter Recktenwald 
 */

package rpipantiltcam;

import com.pi4j.io.i2c.I2CBus;
import com.pi4j.io.i2c.I2CDevice;
import com.pi4j.io.i2c.I2CFactory;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

public class I2CServoController {
    
    public I2CDevice arduino;
    
    public I2CServoController() {

        System.out.println("I2C Servo Controller starting ...");
        
        System.out.println("get bus 1");
        // get I2C bus instance
        final I2CBus bus;
        try {
            bus = I2CFactory.getInstance(I2CBus.BUS_1); /* Bus0 for Rev.1 Bus1 vor Rev.2 */
            System.out.println("get device with id 42");
            arduino = bus.getDevice(0x2A);
        
        } catch (IOException ex) {
            Logger.getLogger(RPiPanTiltCam.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void setPan(byte pos) throws IOException {
        System.out.println("setPan " + pos);
        byte[] buffer = new byte[2];
        buffer[0] = 1;
        buffer[1] = pos;
        if (arduino != null) {
            arduino.write(buffer, 0, buffer.length);
        }
    }
            
    public void setTilt(byte pos) throws IOException {
        System.out.println("setTilt " + pos);
        byte[] buffer = new byte[2];
        buffer[0] = 2;
        buffer[1] = pos;
        if (arduino != null) {
            arduino.write(buffer, 0, buffer.length);
        }
        
    }
    
}
