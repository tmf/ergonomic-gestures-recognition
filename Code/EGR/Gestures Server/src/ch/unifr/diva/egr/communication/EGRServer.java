/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ch.unifr.diva.egr.communication;

import java.net.*;
import java.io.*;

/**
 *
 * @author tmf
 */
public class EGRServer implements Runnable {

    public static final int SERVER_PORT = 1234;
    public static final int MODE_TRAIN = 1;
    public static final int MODE_CLASSIFY = 2;
    public static final int PACKET_VECTOR = 3;
    public static final int LOG = 255;
    private ServerSocket server;
    private Socket client;
    private Thread thread;
    private static EGRServer instance;

    public static EGRServer getInstance() {
        return (instance == null) ? instance = new EGRServer() : instance;
    }

    private EGRServer() {
        try {
            server = null;
            thread = new Thread(this);
            thread.start();
        } catch (Exception e) {
        }
    }

    public void run() {

        client = null;
        DataInputStream dis = null;

        while (true) {
            try {
                if (client == null || !client.isConnected()) {
                    System.out.println("waiting for connection");
                    if (server != null) {
                        server.close();
                    }
                    server = new ServerSocket(SERVER_PORT);
                    client = server.accept();
                    System.out.println("connection established");
                    dis = new DataInputStream(client.getInputStream());
                }
                int command = dis.read();
                switch (command) {
                    case LOG:
                    
                        BufferedReader br = new BufferedReader(new InputStreamReader(client.getInputStream()));
                        System.out.println("LOG: " + br.readLine());
                        break;
                    default:
                        break;
                }

            } catch (SocketException se) {
                System.out.println(se.getMessage());
                try {
                    client.close();
                    client = null;

                } catch (IOException ioe) {
                }
            } catch (IOException ioe) {
            }
        }
    }
}
