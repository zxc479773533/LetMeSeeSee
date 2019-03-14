package lmss.model;

import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;

/**
 * Model class for lmss file node
 * @author jyxk
 */
public class FileNode {

    private final StringProperty nodeName;
    private final StringProperty dataName;
    private final StringProperty value;
    private final StringProperty serverIP;
    private final StringProperty port;
    private final StringProperty password;

    public FileNode() {
        this(null, null, null, null, null, null);
    }

    public FileNode(String nodeName, String dataName, String value, String serverIP, String port, String password) {
        this.nodeName = new SimpleStringProperty(nodeName);
        this.dataName = new SimpleStringProperty(dataName);
        this.value = new SimpleStringProperty(value);
        this.serverIP = new SimpleStringProperty(serverIP);
        this.port = new SimpleStringProperty(port);
        this.password = new SimpleStringProperty(password);
    }

    //get and set methods for nodeName, DataName, value and Address

    public String getNodeName() {
        return nodeName.get();
    }

    public void setNodeName(String nodeName) {
        this.nodeName.set(nodeName);
    }

    public StringProperty nodeNameProperty() {
        return nodeName;
    }

    public String getDataName() {
        return dataName.get();
    }

    public void setDataName(String dataName) {
        this.dataName.set(dataName);
    }

    public StringProperty dataNameProperty() {
        return dataName;
    }

    public String getValue() { return value.get();}

    public void setValue(String value) { this.value.set(value);}

    public StringProperty valueProperty() {return value;}

    public String getServerIP() {return serverIP.get();}

    public void setServerIP(String serverIP) {
        this.serverIP.set(serverIP);
    }

    public StringProperty serverIPProperty() {
        return serverIP;
    }

    public String getPort() {
        return port.get();
    }

    public void setPort(String port) {
        this.port.set(port);
    }

    public StringProperty portProperty() {
        return port;
    }

    public String getPassword() {
        return password.get();
    }

    public void setPassword(String password) {
        this.password.set(password);
    }

    public StringProperty passwordProperty() {
        return password;
    }
}
