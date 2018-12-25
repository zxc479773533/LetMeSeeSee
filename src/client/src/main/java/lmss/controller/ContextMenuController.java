package lmss.controller;

import javafx.fxml.FXML;
import javafx.scene.control.ContextMenu;
import javafx.scene.input.ContextMenuEvent;
import javafx.scene.layout.VBox;

public class ContextMenuController {

    @FXML
    private VBox vbox;
    @FXML
    private ContextMenu menu;

    @FXML
    public void add() {
        System.out.println("add");
    }

    @FXML
    public void showMenu(ContextMenu event) {
        System.out.println("showMenu");
    }
}
