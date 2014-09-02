/* Digital Fotografie Hacks
 * Raspberry Pi Camera Class: 
 * Kamera Klasse fuer die Raspberry Pi Kamera
 * v0.1, 2014-05-01, Peter Recktenwald 
 */

package rpidigicam;

import java.io.IOException;

public class Camera {  
   public Camera() {  
   }  
   public void takePicture(String pFileName)  
   {  
     executeShellCommand("raspistill -o "+pFileName+".jpg");  
   }  
   public void takeVideo(String pFileName,long pTimeInMilliSeconds)  
   {  
     executeShellCommand("raspivid -o "+pFileName+".h264 -t "+pTimeInMilliSeconds);  
   }  
   private void executeShellCommand(String pCommand)  
   {  
     try {  
       Runtime run = Runtime.getRuntime() ;  
       Process pr = run.exec(pCommand) ;  
       pr.waitFor() ;  
     } catch (IOException | InterruptedException ex) {  
    	 System.out.println(ex);  
     }  
   }  
 }  

