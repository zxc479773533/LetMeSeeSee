package lmss.utils.net;

import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;
import java.io.IOException;
import java.net.ConnectException;
import java.util.concurrent.TimeUnit;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import lmss.model.SharedData;
import org.apache.commons.codec.digest.DigestUtils;

/**
 * the class that is used to request node list or a single node data
 * when cmd = "nodelist" is to get the whole node list from server
 * and  cmd = nodename is for get a single node data.
 *
 * e.g. "CMD: nodelist" for getting the whole list
 * and  "CMD: node1" for getting the data of "node1"
 */
public class RequestCmd {
    private OkHttpClient client = new OkHttpClient.Builder().retryOnConnectionFailure(true).connectTimeout(30, TimeUnit.SECONDS).build();

    public String run(String url, String cmd, String password) throws IOException {
        Request request = new Request.Builder()
                .url(url)
                .header("CMD", cmd)
                .addHeader("Password", DigestUtils.md5Hex(password))
                .addHeader("Connection", "close")
                .build();

        try (Response response = client.newCall(request).execute()) {
            if (response.code() == 404) {
                System.out.println("Wrong password!");
                return "密码错误";
            }
            else {
                return response.body().string();
            }
        } catch (NullPointerException e) {
            System.out.println("NullPointerException!");
            return "NullPointerException!";
        } catch (ConnectException connectEx) {
            System.out.println("ConnectException");
            return "ConnectException!";
        } catch (IOException IoEx) {
            System.out.println("IOException");
            IoEx.printStackTrace();
            return "IOException!";
        }

    }

    public static boolean isHttpUrl(@org.jetbrains.annotations.NotNull String urls) {
        boolean isUrl = false;
        String regex = "(((https|http)?://)?([a-z0-9]+[.])|(www.))"
                + "\\w+[.|\\/]([a-z0-9]{0,})?[[.]([a-z0-9]{0,})]+((/[\\S&&[^,;\u4E00-\u9FA5]]+)+)?([.][a-z0-9]{0,}+|/?)";
        Pattern pat = Pattern.compile(regex.trim());
        Matcher mat = pat.matcher(urls.trim());
        isUrl = mat.matches();
        return isUrl;
    }
}
