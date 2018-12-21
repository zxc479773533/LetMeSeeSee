package lmss.view;

import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
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
        nodeNameColumn.setCellValueFactory(cellData->cellData.getValue().nodeNameProperty());
        dataNameColumn.setCellValueFactory(cellData->cellData.getValue().dataNameProperty());
    }

    public void setMainApp(MainApp mainApp) {
        this.mainApp = mainApp;

        fileNodeTable.setItems(mainApp.getFileNodeData());
    }


}
