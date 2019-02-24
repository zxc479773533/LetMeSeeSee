package lmss.view;

import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.stage.FileChooser;
import lmss.MainApp;
import lmss.model.FileNode;
import lmss.model.SharedData;
import lmss.utils.net.RequestCmd;
import javafx.event.ActionEvent;
import javafx.scene.control.Alert.*;

import java.io.*;
import java.nio.charset.StandardCharsets;

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
    private Button deleteButton;
    @FXML
    private Button saveButton;

    public static SharedData sharedData = new SharedData();
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
            if (sharedData.getServerUrl() != null && !sharedData.getServerUrl().isEmpty()) {
                RequestCmd requestCmd = new RequestCmd();
                try {
                    String data = requestCmd.run(sharedData.getServerUrl(), nodeNameLabel.getText());
                    sharedData.setMessage(data);
                    fileNode.setValue(data);
                    ValueLabel.setText(fileNode.getValue());
                } catch (IOException e) {
                    e.printStackTrace();
                    sharedData.setMessage("ERROR:IOException!");
                    Alert alert = new Alert(AlertType.ERROR);
                    alert.setTitle("Error Dialog");
                    alert.setHeaderText("Look, an Error Dialog");
                    alert.setContentText("Ooops, there was an error!");
                    alert.showAndWait();
                }
            }
            else {
                sharedData.setMessage("error");
                System.out.println("error:"+sharedData.getServerUrl());
            }
        });
    }

    @FXML
    private void handleDelete() {
        int selectedIndex = fileNodeTable.getSelectionModel().getSelectedIndex();
        if (selectedIndex >= 0) {
            fileNodeTable.getItems().remove(selectedIndex);
        } else {
            sharedData.setMessage("ERROR:void delete");
            Alert alert = new Alert(AlertType.ERROR);
            alert.setTitle("Error Dialog");
            alert.setHeaderText("Look, an Error Dialog");
            alert.setContentText("Ooops, there was an error!");
            alert.showAndWait();
        }
    }

    @FXML
    private void handleSave() {
        saveButton.setOnAction( (ActionEvent) -> {
            FileChooser fileChooser = new FileChooser();
            // Set extension filter
            FileChooser.ExtensionFilter extensionFilter = new FileChooser.ExtensionFilter("All files (*.*)", "*.*");
            fileChooser.getExtensionFilters().add(extensionFilter);

            // Show save file dialog
            File file = fileChooser.showSaveDialog(mainApp.getPrimaryStage());

            try {
                OutputStream outputStream = new FileOutputStream(file);
                OutputStreamWriter writer = new OutputStreamWriter(outputStream, StandardCharsets.UTF_8);
                writer.write(ValueLabel.toString());
                writer.close();
                outputStream.close();
            }
            catch (Exception exception) {
                Alert alert = new Alert(Alert.AlertType.ERROR);
                alert.setTitle("Error Dialog");
                alert.setHeaderText("Look, an Error Dialog");
                alert.setContentText("Ooops, there was an error!");
                alert.showAndWait();
            }
        });
    }

}
