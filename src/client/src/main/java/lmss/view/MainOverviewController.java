package lmss.view;

import javafx.fxml.FXML;
import javafx.scene.control.*;
import lmss.MainApp;
import lmss.model.FileNode;
import lmss.model.SharedData;
import lmss.utils.net.RequestCmd;
import javafx.event.ActionEvent;
import java.io.IOException;
import java.util.Date;

public class MainOverviewController {
    @FXML
    private TableView<FileNode> fileNodeTable;
    @FXML
    private TableColumn<FileNode, String> nodeNameColumn;
    @FXML
    private TableColumn<FileNode, String> dataNameColumn;

    @FXML
    private Label nodeNameLabel;
    @FXML
    private Label dataNameLabel;
    @FXML
    private Label ValueLabel;

    @FXML
    private Button watchButton;
    @FXML
    private Button saveButton;

    public static SharedData sharedData = new SharedData();

    private String serverUrl;
    private FileNode fileNode;


    private MainApp mainApp;

    /**
     * The constructor is called before the initialize()
     */
    public MainOverviewController() {

    }

    /**
     * Initializes the controller class. This method is automatically called
     * after the fxml file has been loaded.
     */
    @FXML
    private void initialize() {
        //Initialze the file node table with the two column
        nodeNameColumn.setCellValueFactory(
                cellData->cellData.getValue().nodeNameProperty());
        dataNameColumn.setCellValueFactory(
                cellData->cellData.getValue().dataNameProperty());

        // Clear data details
        showMainDetails(null);

        // Listen for selection changes and show the data details when changed
        fileNodeTable.getSelectionModel().selectedItemProperty().addListener(
                (observable, oldValue, newValue) -> showMainDetails(newValue));
        sharedData.serverUrlProperty().addListener(
                (observable, oldValue, newValue) -> serverUrl = sharedData.getServerUrl());
    }


    public void setMainApp(MainApp mainApp) {
        this.mainApp = mainApp;

        fileNodeTable.setItems(mainApp.getFileNodeData());
    }

    /**
     * Fills all text fields to show details about the data.
     * If the specified data is null. All text are cleared.
     *
     * @param fileNode
     */
    private void showMainDetails(FileNode fileNode) {
        this.fileNode = fileNode;
        if (fileNode != null) {
            //Fill the labels with info from the person object.
            nodeNameLabel.setText(fileNode.getNodeName());
            dataNameLabel.setText(fileNode.getDataName());
            ValueLabel.setText(fileNode.getValue());
        } else {
            nodeNameLabel.setText("");
            dataNameLabel.setText("");
            ValueLabel.setText("");
        }
    }

    @FXML
    private void handleWatch() {
        watchButton.setOnAction( (ActionEvent event) -> {
            if (!serverUrl.isEmpty() || RequestCmd.isHttpUrl(serverUrl)) {
                RequestCmd requestCmd = new RequestCmd();
                try {
                    String data = requestCmd.run(serverUrl, nodeNameLabel.getText());
                    sharedData.setMessage(data);
                    fileNode.setValue(data);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
    }

    @FXML
    private void handleSave() {

    }

}
