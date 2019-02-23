package lmss.model;

import java.util.List;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;


/**
 * Helper class to wrap a list of nodes.This is used for saving the
 * list of nodes to XML.
 *
 */
@XmlRootElement(name = "nodes")
public class NodeListWrapper {
    private List<FileNode> nodes;

    @XmlElement(name = "node")
    public List<FileNode> getNodes() {
        return nodes;
    }

    public void setNodes(List<FileNode> nodes) {
        this.nodes = nodes;
    }
}
