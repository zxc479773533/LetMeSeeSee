package lmss.utils.net;

import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

import java.io.IOException;

/**
 * the class that is used to request node list or a single node data
 * when cmd = "nodelist" is to get the whole node list from server
 * and  cmd = nodename is for get a single node data.
 *
 * e.g. "CMD: nodelist" for getting the whole list
 * and  "CMD: node1" for getting the data of "node1"
 */
public class RequestCmd {
    OkHttpClient client = new OkHttpClient();

    public String run(String url, String cmd) throws IOException {
        Request request = new Request.Builder()
                .url(url)
                .header("CMD", cmd)
                .build();

        try (Response response = client.newCall(request).execute()) {
            return response.body().string();
        }
        catch (NullPointerException e) {
            System.out.println("NullPointerException!");
            return null;
        }

    }
}
