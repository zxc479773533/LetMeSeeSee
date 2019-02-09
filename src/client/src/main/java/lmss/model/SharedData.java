package lmss.model;

import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;

public class SharedData {
    private StringProperty serverUrl = new SimpleStringProperty();
    private StringProperty message = new SimpleStringProperty();

    public void AppModel() {
        this.serverUrl = new SimpleStringProperty();
        this.message = new SimpleStringProperty();
    }

    public StringProperty serverUrlProperty() {
        return serverUrl;
    }

    public StringProperty messageProperty() {
        return message;
    }

    public final String getServerUrl() {
        return serverUrl.get();
    }

    public final String getMessage() {
        return message.get();
    }

    public final void setServerUrl(String serverUrl) {
        serverUrlProperty().set(serverUrl);
    }

    public final void setMessage(String message) {
        messageProperty().set(message);
    }
}
