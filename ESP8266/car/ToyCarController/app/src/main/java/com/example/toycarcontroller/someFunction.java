package com.example.toycarcontroller;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class someFunction {
    public static void socketCommunication(String uploadText) {
        try {
            DatagramSocket ds = new DatagramSocket(8888);
            String str = uploadText;
            byte[] buf = str.getBytes();
            DatagramPacket dp =
                    new DatagramPacket(buf, buf.length, InetAddress.getByName("192.168.4.1"), 80);
            ds.send(dp);
            ds.close();
        } catch (Exception e) {
            return;
        }
    }
}
