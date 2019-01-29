package lmss;

import javafx.application.Application;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;

import lmss.model.FileNode;
import lmss.view.MainOverviewController;
import lmss.view.RootLayoutController;

import java.io.IOException;

public class MainApp extends Application {

    private Stage primaryStage;
    private BorderPane rootLayout;
    private ObservableList<FileNode> fileNodeData = FXCollections.observableArrayList();

    /**
     * Constructor
     */
    public MainApp() {
        fileNodeData.add(new FileNode("a", "a", "123", "0x123"));
        fileNodeData.add(new FileNode("b", "b", "456", "0x456"));
        fileNodeData.add(new FileNode("c", "c", "789", "0x789"));
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

        initRootLayout();

        showMainOverview();
    }

    public void initRootLayout() {
        try {
            // Load root Layout from fxml file
            FXMLLoader loader = new FXMLLoader();
            System.out.println(MainApp.class.getResource("/view/RootLayout.fxml"));

            loader.setLocation(MainApp.class.getResource("/view/RootLayout.fxml"));
            rootLayout = (BorderPane) loader.load();

            Scene scene = new Scene(rootLayout);
            primaryStage.setScene(scene);
            primaryStage.show();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void showMainOverview() {
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

}

