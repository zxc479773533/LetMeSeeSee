package lmss.view;

import javafx.fxml.FXML;
import javafx.scene.control.*;
import lmss.MainApp;
import lmss.model.FileNode;

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
    private Label AddressLabel;

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
        if (fileNode != null) {
            //Fill the labels with info from the person object.
            dataNameLabel.setText(fileNode.getnodeName());
            ValueLabel.setText(fileNode.getValue());
            AddressLabel.setText(fileNode.getAddress());
        } else {
            dataNameLabel.setText("");
            ValueLabel.setText("");
            AddressLabel.setText("");
        }
    }


}
