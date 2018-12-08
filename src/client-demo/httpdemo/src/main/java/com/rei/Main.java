package com.rei;

import java.io.IOException;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) throws IOException {
        Scanner in = new Scanner(System.in);
        System.out.print("Please input the URL:");
        String url = in.nextLine();

        System.out.print("Please input the command you need:");
        String cmd = in.nextLine();

        GetMethod getExample = new GetMethod();
        String getResponse = getExample.run(url, cmd);
        System.out.println(getResponse);
    }
}
