package com.rei;

import java.io.IOException;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class GetMethod {
    OkHttpClient client = new OkHttpClient();

    String run(String url, String cmd) throws IOException {
        Request request = new Request.Builder()
                .url(url)
                .header("CMD", cmd)
                .build();

        try (Response response = client.newCall(request).execute()) {
            return response.body().string();
        }
    }

}