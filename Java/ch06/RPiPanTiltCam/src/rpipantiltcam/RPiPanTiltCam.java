/* Digital Fotografie Hacks
 * Raspberry Pi Pan Tilt Camera: 
 * Pan Tilt Kamera mit dem Raspberry Pi Board Pi Kamera PWM Controller und 2 Servos
 * gesteuert vom Smartphone/Tablet ueber die NetIO App http://netio.davideickhoff.de/
 * v0.1, 2014-07-02, Peter Recktenwald 
 * based on VerySimpleServer by Sean R. Owens, sean at guild dot net  http://darksleep.com/player
 */

package rpipantiltcam;


import java.net.Socket;
import java.net.ServerSocket;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.IOException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

// An example of a very simple socket server.  Start by looking at the
// static main() method at the bottom of this file.
public class RPiPanTiltCam {
    private int serverPort = 0;
    private ServerSocket serverSock = null;
    private int timelapseCount=10;
    private int timelapseDelay=5;
    
    public RPiPanTiltCam(int serverPort) {
        this.serverPort = serverPort;

        try {
            serverSock = new ServerSocket(serverPort);
        }
        catch (IOException e){
            e.printStackTrace(System.err);
        }
    }
    
    public static String GetActDate() {
        DateFormat df = new SimpleDateFormat("yyyy-MM-dd-HH-mm-ss");

        // Get the date today using Calendar object.
        Date today = Calendar.getInstance().getTime();        
        // Using DateFormat format method we can create a string 
        // representation of a date with the defined format.
        String reportDate = df.format(today);
        return reportDate;    
    }

    // All this method does is wait for some bytes from the
    // connection, read them, then write them back again, until the
    // socket is closed from the other side.
    public void handleConnection(InputStream sockInput, OutputStream sockOutput) {

        final  I2CServoController  ServoControl = new I2CServoController();
        final Camera piCam = new Camera(); 

        while(true) {
            byte[] buf=new byte[1024];
            int bytes_read = 0;
            try {
                // This call to read() will wait forever, until the
                // program on the other side either sends some data,
                // or closes the socket.
                bytes_read = sockInput.read(buf, 0, buf.length);

                // If the socket is closed, sockInput.read() will return -1.
                if(bytes_read < 0) {
                    System.err.println("Tried to read from socket, read() returned < 0,  Closing socket.");
                    return;
                }
                String inpline = new String(buf, 0, bytes_read);
                String[] splitStr = inpline.split("\\s+");
                System.err.println("Received "+bytes_read
                                   +" bytes, data="
                                   +inpline);
                if ("Foto".equals(splitStr[0]))
                {
                   System.out.println("Take Foto");
                   piCam.takePicture("pic"+GetActDate());
                }
                else if ("Movie".equals(splitStr[0]))
                {
                   System.out.println("Take Movie");
                   piCam.takeVideo("vid"+GetActDate(), 10000);
                }
                else if ("Pan".equals(splitStr[0]))
                {
                   System.out.println("Pan:" + splitStr[1]);
                   ServoControl.setPan((byte) (180-Integer.parseInt(splitStr[1])));
                }
                else if ("Tilt".equals(splitStr[0]))
                {
                   System.out.println("Tilt:" + splitStr[1]);
                   ServoControl.setTilt((byte) (180-Integer.parseInt(splitStr[1])));
                }
                else if ("Count".equals(splitStr[0]))
                {
                   System.out.println("TimelapseCount:" + splitStr[1]);
                   timelapseCount = Integer.parseInt(splitStr[1]);
                }
                else if ("Delay".equals(splitStr[0]))
                {
                   System.out.println("TimelapseDelay:" + splitStr[1]);
                   timelapseDelay = Integer.parseInt(splitStr[1]);
                }
                else if ("Timelapse".equals(splitStr[0]))
                {
                   if ("on".equals(splitStr[1]))
                     piCam.takeTimelapse("tl", (long)(timelapseCount*timelapseDelay*1000), (long)(timelapseDelay*1000));
                   else if ("off".equals(splitStr[1]))
                     piCam.stopTimelapse();
                }
                Thread.sleep(100);
                sockOutput.write(buf, 0, bytes_read);
                // This call to flush() is optional - we're saying go
                // ahead and send the data now instead of buffering
                // it.
                sockOutput.flush();
            }
            catch (Exception e){
                System.err.println("Exception reading from/writing to socket, e="+e);
                e.printStackTrace(System.err);
                return;
            }
        }

    }

    public void waitForConnections() {
        Socket sock = null;
        InputStream sockInput = null;
        OutputStream sockOutput = null;
        while (true) {
            try {
                // This method call, accept(), blocks and waits
                // (forever if necessary) until some other program
                // opens a socket connection to our server.  When some
                // other program opens a connection to our server,
                // accept() creates a new socket to represent that
                // connection and returns.
                sock = serverSock.accept();
                System.err.println("Have accepted new socket.");

                // From this point on, no new socket connections can
                // be made to our server until we call accept() again.

                sockInput = sock.getInputStream();
                sockOutput = sock.getOutputStream();
            }
            catch (IOException e){
                e.printStackTrace(System.err);
            }

            // Do something with the socket - read bytes from the
            // socket and write them back to the socket until the
            // other side closes the connection.
            handleConnection(sockInput, sockOutput);

            // Now we close the socket.
            try {
                System.err.println("Closing socket.");
                sock.close();
            }
            catch (Exception e){
                System.err.println("Exception while closing socket.");
                e.printStackTrace(System.err);
            }

            System.err.println("Finished with socket, waiting for next connection.");
        }
    }

    public static void main(String argv[]) {
        int port;
        System.out.println("RPi Servo Pan Server ... started.");
               
        
        if (!"".equals(argv[0]))
        {
          port = Integer.parseInt(argv[0]);
        }
        else
        {
          port = 54321;
        }
        RPiPanTiltCam server = new RPiPanTiltCam(port);
        server.waitForConnections();
    }
    
    
} 