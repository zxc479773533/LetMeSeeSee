package lmss;

import javafx.application.Application;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.EventHandler;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.BorderPane;
import javafx.stage.Modality;
import javafx.stage.Stage;

import javafx.stage.WindowEvent;
import lmss.model.FileNode;
import lmss.model.NodeListWrapper;
import lmss.view.AboutDialogController;
import lmss.view.MainOverviewController;
import lmss.view.RootLayoutController;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import java.io.File;
import java.io.IOException;
import java.util.prefs.BackingStoreException;
import java.util.prefs.Preferences;


public class MainApp extends Application {

    private Stage primaryStage;
    private BorderPane rootLayout;
    private ObservableList<FileNode> fileNodeData = FXCollections.observableArrayList();
    private Preferences prefs = Preferences.userNodeForPackage(MainApp.class);

    /**
     * Constructor
     */
    public MainApp() {

    }

    public ObservableList<FileNode> getFileNodeData() {

        return fileNodeData;
    }

    public static void main(String[] args) {

        launch(args);
    }


    @Override
    public void start(Stage primaryStage) {
        this.primaryStage = primaryStage;
        this.primaryStage.setTitle("Let Me See See");
        //this.primaryStage.initStyle(StageStyle.UNIFIED);
        initRootLayout();

        showMainOverview();

        primaryStage.setOnHiding(new EventHandler<WindowEvent>() {
            @Override
            public void handle(WindowEvent event) {
                try {
                    prefs.removeNode();
                } catch (BackingStoreException exception) {
                    exception.printStackTrace();
                }
            }
        });
    }

    private void initRootLayout() {
        try {
            // Load root Layout from fxml file
            FXMLLoader loader = new FXMLLoader();
            System.out.println(MainApp.class.getResource("/view/RootLayout.fxml"));

            loader.setLocation(MainApp.class.getResource("/view/RootLayout.fxml"));
            rootLayout = (BorderPane) loader.load();

            Scene scene = new Scene(rootLayout);
            primaryStage.setScene(scene);

            RootLayoutController controller = loader.getController();
            controller.setMainApp(this);

            primaryStage.show();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void showMainOverview() {
        try {
            FXMLLoader loader = new FXMLLoader();
            loader.setLocation(MainApp.class.getResource("/view/MainOverview.fxml"));
            AnchorPane mainOverview = (AnchorPane) loader.load();
            //Set fileNode overview into the center of root layout
            rootLayout.setCenter(mainOverview);

            // Give the controller access to the main app
            MainOverviewController controller = loader.getController();
            controller.setMainApp(this);

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public Stage getPrimaryStage() {
        return primaryStage;
    }

    /**
     * Return the node list file preference, i.e. the file that was last opened.
     * The preference is read from the OS specific registry, If no such
     * preference can be found, null is returned.
     * @return
     */
    public File getNodeListFilePath() {
        String filePath = prefs.get("filePath", null);
        if (filePath != null) {
            return new File(filePath);
        } else {
            return null;
        }
    }

    public void setNodeListFilePath(File file) {
        if (file != null) {
            prefs.put("filePath", file.getPath());

            //Update the stage title.
            primaryStage.setTitle("Lmss - " + file.getName());
        } else {
            prefs.remove("filePath");

            //Update the stage title.
            primaryStage.setTitle("Let me see see");
        }
    }

    /**
     * Loads node data from the specified file.The current node data will
     * be replaced.
     *
     * @param file
     */
    public void loadNodeDataFromFile(File file) {
        try {
            JAXBContext context = JAXBContext
                    .newInstance(NodeListWrapper.class);
            Unmarshaller unmarshaller = context.createUnmarshaller();

            //Reading XML from the file and unmarshalling.
            NodeListWrapper wrapper = (NodeListWrapper)unmarshaller.unmarshal(file);

            fileNodeData.clear();
            fileNodeData.addAll(wrapper.getNodes());


            //Save the file path ro the registry.
            setNodeListFilePath(file);
        } catch (Exception e) {
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle("Error Dialog");
            alert.setHeaderText("Look, an Error Dialog");
            alert.setContentText("Ooops, there was an error!");
            alert.showAndWait();
        }
    }

    public void saveNodeDataFromFile(File file) {
        try {
            JAXBContext context = JAXBContext
                    .newInstance(NodeListWrapper.class);
            Marshaller marshaller = context.createMarshaller();
            marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true);

            //Wrapping our node data.
            NodeListWrapper wrapper = new NodeListWrapper();
            wrapper.setNodes(fileNodeData);

            //Marshalling and saving XML to the file.
            marshaller.marshal(wrapper, file);

            //Save the file path to the registry.
            setNodeListFilePath(file);
        } catch (Exception e) {
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle("Error Dialog");
            alert.setHeaderText("Look, an Error Dialog");
            alert.setContentText("Ooops, there was an error!");
            alert.showAndWait();
        }
    }

    public void showAboutDialog() {
        try {
            FXMLLoader loader = new FXMLLoader();
            loader.setLocation(MainApp.class.getResource("/view/AboutDialog.fxml"));
            //about = FXMLLoader.load(getClass().getResource("/view/AboutDialog.fxml"));
            AnchorPane page = (AnchorPane) loader.load();

            Stage dialogStage = new Stage();
            dialogStage.setTitle("About");
            dialogStage.initModality(Modality.WINDOW_MODAL);
            dialogStage.initOwner(primaryStage);
            Scene scene = new Scene(page);
            scene.getStylesheets().add(getClass().getResource("/view/DefaultTheme.css").toExternalForm());

            dialogStage.setScene(scene);

            AboutDialogController controller = loader.getController();
            controller.setDialogStage(dialogStage);
            dialogStage.showAndWait();
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }
}

