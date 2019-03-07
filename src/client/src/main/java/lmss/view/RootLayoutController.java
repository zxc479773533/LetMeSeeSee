package lmss.view;

import com.google.gson.reflect.TypeToken;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.event.ActionEvent;

import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.Date;
import java.util.List;

import com.google.gson.*;
import javafx.stage.FileChooser;
import lmss.MainApp;
import lmss.model.FileNode;
import lmss.model.NodeListBean;
import lmss.utils.net.RequestCmd;
import lmss.view.MainOverviewController;

import static lmss.view.MainOverviewController.sharedData;


public class RootLayoutController {

    public String server_address;

    @FXML
    private MenuBar menuBar;
    @FXML
    private TextField addressField;
    @FXML
    private TextField portField;
    @FXML
    private Button connectButton;
    @FXML
    private TextArea logArea;

    private ObservableList<FileNode> fileNodeList;
    private MainApp mainApp;

    @FXML
    private void initialize() {
        logArea.setEditable(false);
        logArea.clear();

        //更新log
        sharedData.messageProperty().addListener(
                (observable, oldValue, newValue) -> setLogArea(sharedData.getMessage()));
    }

    public void setMainApp(MainApp mainApp) {

        this.mainApp = mainApp;
    }

    public RootLayoutController() {
    }

    @FXML
    public void handleConnect() {
        connectButton.setOnAction( (ActionEvent event) -> {
            fileNodeList = mainApp.getFileNodeData();
            fileNodeList.clear();
            if (addressField.getText().isEmpty() || !RequestCmd.isHttpUrl("http://"+addressField.getText())) {
                setLogArea("请检查URL格式, 示例：123.123.123.123");
                addressField.clear();
            }
            else if (Integer.parseInt(portField.getText()) < 1 ||
                     Integer.parseInt(portField.getText()) > 65535) {
                setLogArea("端口号越界！");
                portField.clear();
            }
            else if ((addressField.getText() != null &&
                     !addressField.getText().isEmpty())) {
                server_address = "http://"+addressField.getText() + ":" + portField.getText();
                sharedData.setServerUrl(server_address);
                RequestCmd requestCmd = new RequestCmd();
                try {
                    Gson gson = new GsonBuilder().setLenient().create();
                    String getResponse = requestCmd.run(server_address, "nodelist");
                    if (getResponse.equals("NullPointerException!")
                            || getResponse.equals("ConnectException!")
                            || getResponse.equals("IOException!")) {
                        setLogArea(getResponse);
                    }
                    else {
                        setLogArea(getResponse);
                        List<NodeListBean> nodeList = gson.fromJson(getResponse.trim(), new TypeToken<List<NodeListBean>>(){}.getType());
                        fileNodeList = mainApp.getFileNodeData();
                        for (NodeListBean node: nodeList) {
                            for (String dataName: node.dataName) {
                                fileNodeList.add(new FileNode(node.nodeName, dataName, null));
                            }
                        }
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            else {
                setLogArea("Connect error!");
            }
        });
    }


    /**
     * Creates an empty address book.
     */
    @FXML
    private void handleNew() {
        mainApp.getFileNodeData().clear();
        mainApp.setNodeListFilePath(null);
    }

    /**
     * Opens a FileChooser to let the user select an address book to load
     */
    @FXML
    private void handleOpen() {
        FileChooser fileChooser = new FileChooser();

        //Set extension filter
        FileChooser.ExtensionFilter extensionFilter = new FileChooser.ExtensionFilter(
                "XML file (*.xml)", "*.xml"
        );
        fileChooser.getExtensionFilters().add(extensionFilter);

        //Show save file dialog
        File file = fileChooser.showOpenDialog(mainApp.getPrimaryStage());

        if (file != null) {
            mainApp.loadNodeDataFromFile(file);
        }
    }

    /**
     * Saves the file to the person file that is currently open. If there is no
     * open file, the "save as" dialog is shown.
     */
    @FXML
    private void handleSave() {
        File nodeFile = mainApp.getNodeListFilePath();
        if (nodeFile != null) {
            mainApp.saveNodeDataFromFile(nodeFile);
        } else {
            handleSaveAs();
        }
    }

    /**
     *
     */
    @FXML
    private void handleSaveAs() {
        FileChooser fileChooser = new FileChooser();

        //Set extension filter
        FileChooser.ExtensionFilter extensionFilter = new FileChooser.ExtensionFilter(
                "XML files (*.xml)", "*.xml"
        );
        fileChooser.getExtensionFilters().add(extensionFilter);

        File file = fileChooser.showSaveDialog(mainApp.getPrimaryStage());

        if (file != null) {
            // Make sure it has the correct extension
            if (!file.getPath().endsWith(".xml")) {
                file = new File(file.getPath() + ".xml");
            }
            mainApp.saveNodeDataFromFile(file);
        }

    }


    @FXML
    private void handleSaveLog() {
        FileChooser fileChooser = new FileChooser();

        FileChooser.ExtensionFilter extensionFilter = new FileChooser.ExtensionFilter(
                "Log files (*.log)", "*.log"
        );
        fileChooser.getExtensionFilters().add(extensionFilter);

        File file = fileChooser.showSaveDialog(mainApp.getPrimaryStage());

        try {
            OutputStream outputStream = new FileOutputStream(file);
            OutputStreamWriter writer = new OutputStreamWriter(outputStream, StandardCharsets.UTF_8);
            writer.write(logArea.getText());
            writer.close();
            outputStream.close();
        }
        catch (FileNotFoundException exception) {
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle("Error Dialog");
            alert.setHeaderText("File Error Dialog");
            alert.setContentText("File not found!");
            alert.showAndWait();
        }
        catch (IOException exception) {
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle("Error Dialog");
            alert.setHeaderText("IO Error Dialog");
            alert.setContentText("IO error");
            alert.showAndWait();
        }
        catch (NullPointerException NullEx) {

        }
    }

    @FXML
    private void handleAbout() {
        mainApp.showAboutDialog();
    }

    @FXML
    private void handleExit() {

        System.exit(0);
    }

    private void setLogArea(String log) {
        Date time = new Date();
        logArea.appendText("\n["+time.toString()+"]:"+"\n"+log);
    }


}
