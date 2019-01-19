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
    private final StringProperty Value;
    private final StringProperty Address;


    public FileNode() {
        this(null, null, null, null);
    }

    public FileNode(String nodeName, String dataName, String Value, String Address) {
        this.nodeName = new SimpleStringProperty(nodeName);
        this.dataName = new SimpleStringProperty(dataName);
        this.Value = new SimpleStringProperty(Value);
        this.Address = new SimpleStringProperty(Address);
    }

    //get and set methods for nodeName, DataName, Value and Address

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

    public String getValue() { return Value.get();}

    public void setValue(String Value) { this.Value.set(Value);}

    public StringProperty ValueProperty() {return Value;}

    public String getAddress() { return Address.get();}

    public void setAddress(String Address) { this.Address.set(Address);}

    public StringProperty AddressProperty() {return Address;}



}
