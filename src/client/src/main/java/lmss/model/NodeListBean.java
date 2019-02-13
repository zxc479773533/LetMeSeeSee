package lmss.model;

import com.google.gson.annotations.SerializedName;

public class NodeListBean {

    @SerializedName("node")
    public String nodeName;
    @SerializedName("data")
    public String[] dataName;
}
