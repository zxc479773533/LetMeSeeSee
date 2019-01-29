package lmss.view;

import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.event.ActionEvent;

import lmss.MainApp;
import lmss.utils.net.RequestCmd;

import java.io.IOException;


public class RootLayoutController {

    public String server_address;

    @FXML
    private MenuBar menuBar;
    @FXML
    private TextField addressField;
    @FXML
    private Button connectButton;
    @FXML
    private ProgressBar progressBar;

    private MainApp mainApp;

    public void setMainApp(MainApp mainApp) {
        this.mainApp = mainApp;
    }

    public RootLayoutController() {

    }

    @FXML
    public void handleConnect() {
        connectButton.setOnAction( (ActionEvent event) -> {
            if (
                    (addressField.getText() != null && !addressField.getText().isEmpty())
            ) {
                server_address = addressField.getText();
                RequestCmd requestCmd = new RequestCmd();
                try {
                    String getResponse = requestCmd.run("https://"+server_address, "nodelist");
                    System.out.println(getResponse);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            } else {
                System.out.println("empty");
            }
        });
    }

    @FXML
    private void handleExit() {
        System.exit(0);
    }


}
