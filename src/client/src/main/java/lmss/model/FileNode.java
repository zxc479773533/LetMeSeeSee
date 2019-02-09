package lmss.model;

import javafx.beans.property.IntegerProperty;
import javafx.beans.property.SimpleIntegerProperty;
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

    public FileNode() {
        this(null, null, null);
    }

    public FileNode(String nodeName, String dataName, String value) {
        this.nodeName = new SimpleStringProperty(nodeName);
        this.dataName = new SimpleStringProperty(dataName);
        this.value = new SimpleStringProperty(value);
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

}
