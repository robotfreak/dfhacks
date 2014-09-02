/* Digital Fotografie Hacks
 * Raspberry Pi Digi Cam: 
 * Digital Kamera mit dem Raspberry Pi Board und Kamera
 * v0.1, 2014-05-01, Peter Recktenwald 
 */

package rpidigicam;

import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;

/**
 *
 * @author Peter
 */
public class RPiDigiCam extends Application {
    
    @Override
    public void start(Stage primaryStage) {
        Camera piCam = new Camera(); 

        Button btnFoto = new Button();
        btnFoto.setPrefWidth(100);
        btnFoto.setPrefHeight(48);
        btnFoto.setText("Foto");
        btnFoto.setOnAction(new EventHandler<ActionEvent>() {
            
            @Override
            public void handle(ActionEvent event) {
                System.out.println("Take Foto");
                piCam.takePicture("pic");
            }
        });
        
        Button btnFilm = new Button();
        btnFilm.setPrefWidth(100);
        btnFilm.setPrefHeight(48);
        btnFilm.setText("Film");
        btnFilm.setOnAction(new EventHandler<ActionEvent>() {
            
            @Override
            public void handle(ActionEvent event) {
                System.out.println("Take Film");
                piCam.takeVideo("vid", 10000);
            }
        });

        Button btnExit = new Button();
        btnExit.setPrefWidth(100);
        btnExit.setPrefHeight(48);
        btnExit.setText("Exit");
        btnExit.setOnAction(new EventHandler<ActionEvent>() {
            
            @Override
            public void handle(ActionEvent event) {
                System.out.println("Exit");
                stop(primaryStage);
            }
        });

        GridPane grid = new GridPane();
        grid.setAlignment(Pos.CENTER);
        grid.setHgap(10);
        grid.setVgap(10);
        grid.setPadding(new Insets(25, 25, 25, 25));
        

        grid.add(btnFoto, 0,0);
        grid.add(btnFilm, 0,1);
        grid.add(btnExit, 0,2);
        
        Scene scene = new Scene(grid, 320, 240);
                
        primaryStage.setTitle("RPi DigiCam");
        primaryStage.setScene(scene);
        primaryStage.show();
    }
    
    public void stop(Stage primaryStage) {
    	primaryStage.close();
    }

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        launch(args);
    }
    
}
