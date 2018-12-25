package lmss.view;

import javafx.scene.control.ContextMenu;
import javafx.scene.layout.Pane;

public class ContextMenuOverview extends Pane {

    private ContextMenu contextMenu;

    public void setContextMenu(ContextMenu contextMenu) {
        this.contextMenu = contextMenu;
    }

    public ContextMenu getContextMenu() {
        return contextMenu;
    }
}
