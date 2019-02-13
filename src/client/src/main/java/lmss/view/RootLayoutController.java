package lmss.view;

import com.google.gson.reflect.TypeToken;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.event.ActionEvent;
import java.io.IOException;
import java.util.Date;
import java.util.List;

import com.google.gson.*;
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
            if (addressField.getText().isEmpty() || !RequestCmd.isHttpUrl(addressField.getText())) {
                setLogArea("请检查URL格式, 示例：https://www.baidu.com");
                addressField.clear();
            }
            else if (Integer.parseInt(portField.getText()) < 1 ||
                     Integer.parseInt(portField.getText()) > 65535) {
                setLogArea("端口号越界！");
                portField.clear();
            }
            else if ((addressField.getText() != null &&
                     !addressField.getText().isEmpty())) {
                server_address = addressField.getText() + ":" + portField.getText();
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

    @FXML
    private void handleExit() {

        System.exit(0);
    }

    private void setLogArea(String log) {
        Date time = new Date();
        logArea.appendText("\n["+time.toString()+"]:"+log);
    }


}
