package lmss.model;

import javafx.beans.property.IntegerProperty;
import javafx.beans.property.SimpleIntegerProperty;
import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;

import java.io.File;

/**
 * Model class for lmss file node
 * @author jyxk
 */
public class FileNode {

    private final StringProperty nodeName;
    private final StringProperty dataName;


    public FileNode() {
        this(null, null);
    }

    public FileNode(String nodeName, String dataName) {
        this.nodeName = new SimpleStringProperty(nodeName);
        this.dataName = new SimpleStringProperty(dataName);
    }

    public String getnodeName() {
        return nodeName.get();
    }

    public void setnodeName(String nodeName) {
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

}
