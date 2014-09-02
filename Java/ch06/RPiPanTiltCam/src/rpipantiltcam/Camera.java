/* Digital Fotografie Hacks
 * Raspberry Pi Camera Class: 
 * Kamera Klasse fuer die Raspberry Pi Kamera
 * v0.1, 2014-05-01, Peter Recktenwald 
 */

package rpipantiltcam;

import java.io.IOException;

public class Camera {  
   public Camera() {  
   }  
   public void takePicture(String pFileName)  
   {  
      String execCmd = "raspistill -o "+pFileName+".jpg";
      executeShellCommand(execCmd);  
   }  
   public void takeVideo(String pFileName,long pTimeInMilliSeconds)  
   {  
      String execCmd = "raspivid -o "+pFileName+".h264 -t "+pTimeInMilliSeconds;  
      executeShellCommand(execCmd);  
   } 
   public void takeTimelapse(String pFileName,long pTimeInMilliSeconds, long pDelayTime)  
   {  
//      String execCmd = "nohup raspistill -o "+pFileName+"%04d.jpg -t "+pTimeInMilliSeconds+" -tl "+pDelayTime+" > /dev/null 2>&1 &";
      String execCmd = "raspistill -o "+pFileName+"%04d.jpg -t "+pTimeInMilliSeconds+" -tl "+pDelayTime+" &";
      executeShellCommand(execCmd);  
   }
   public void stopTimelapse()
   {
      String execCmd =  "sudo pkill raspistill"; 
      executeShellCommand(execCmd);  
   }
   private void executeShellCommand(String pCommand)  
   {  
     try {  
       System.out.println("System: "+pCommand);
       Runtime run = Runtime.getRuntime() ;  
       Process pr = run.exec(pCommand) ;  
       pr.waitFor() ;  
     } catch (IOException | InterruptedException ex) {  
    	 System.out.println(ex);  
     }  
   }  
 }  

