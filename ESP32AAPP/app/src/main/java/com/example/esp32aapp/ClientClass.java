package com.example.esp32aapp;

import java.io.BufferedReader;
import java.io.Console;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.SocketException;
import java.util.Random;


public class ClientClass{

    private ClientOnEventListener listener;
    public void setClientOnEventListener(ClientOnEventListener listener) {
        this.listener = listener;
    }

    private PrintWriter output = null;
    private BufferedReader input = null;

    private Socket socket;
    public Socket getSocket() {
        return socket;
    }

    public boolean isConnect = false;
    public boolean isReconnect = true;
    public boolean first = true;

    public boolean loop_status = true;

    public boolean thread_recv = false;
    public boolean thread_checkserver = false;

    int cooldown_count = 900;
    OnEvent onEvent;

    public String SERVER_IP = "";
    public int SERVER_PORT = 80;

    public void disconnect() {
        try{
            socket.close();
//            socket = null;
            input.close();
            output.close();
//            input = null;
//            output = null;
            isConnect = false;
            isReconnect = true;
        }
        catch (Exception e){
            System.out.println("disconnect error");
        }

        onEvent.onClientDisconnected();
    }


    public ClientClass(String SERVER_IP,int SERVER_PORT,OnEvent onEvent){
        this.onEvent = onEvent;
        this.SERVER_IP = SERVER_IP;
        this.SERVER_PORT = SERVER_PORT;

        isConnect = false;

        if(!isConnect)connect(SERVER_IP,SERVER_PORT);
    }

    public void connect(String SERVER_IP,int SERVER_PORT){
        new Thread(new Runnable() {
            @Override
            public void run() {
//                if(isConnect)disconnect();

                try {
                    socket = new Socket(SERVER_IP, SERVER_PORT);
                    output = new PrintWriter(socket.getOutputStream());
                    input = new BufferedReader(new InputStreamReader(socket.getInputStream()));

//                    if(input == null || output == null || socket == null){
//                        disconnect();
//                        return;
//                    }
                    isConnect = true;

                    if(isConnect && first){
                        first = false;
                        recv();
//                        checkServer();
                    }

                    System.out.println("loop connect sucess");

                    isReconnect = false;

                } catch (IOException e) {
                    System.out.println("disconnect at connect");
//                    disconnect();
                    System.out.println("loop connect error");
                }
            }
        }).start();
    }

    private void recv(){

        new Thread(new Runnable() {

            public String message = "";
            public char c = ' ';

            private char endKeyword = '$';
            @Override
            public void run() {
                thread_recv = true;
                while (loop_status) {

                    if(cooldown_count <= 0){
                        disconnect();
                    }


                    if(input != null && socket.isConnected()){
                        try {

                            char c = (char) input.read();

                            if (c == endKeyword){
                                if (message != "")listener.onMessage(message);
                                message = "";
                            }else{
                                message += String.valueOf(c);
                            }
                            Thread.sleep(5);
                        } catch (IOException | InterruptedException e) {
//                            System.out.println("recv error----------------------------");
                        }
                    }
                    try {
                        Thread.sleep(5);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                }
                thread_recv = false;
                System.out.println("off thread recv");

            }
        }).start();
    }

    private void checkServer(){
        new Thread(new Runnable() {
            @Override
            public void run() {
                thread_checkserver = true;
                while (loop_status){


                    try {
                        Socket v_socket = new Socket(SERVER_IP, SERVER_PORT);

                        System.out.println(String.format("%b",v_socket.isConnected()));
                    } catch (IOException e ) {
                        System.out.println("disconnect at checkserver");
                        disconnect();
                    }

                    if (isConnect){

                    }else{
                        if(cooldown_count > 5)connect(SERVER_IP,SERVER_PORT);
                    }

                    try {
                        Thread.sleep(2000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                thread_checkserver = false;
            }
        }).start();
    }

    public void sendToServer(String textSend){
        new Thread(new Runnable() {

            private String message = textSend + "$";

            @Override
            public void run() {

                if(output != null && isConnect){
                    try{
                        output.write(message);
                        output.flush();
                        System.out.println("sendToServer : " + textSend);
                    }catch (Exception e){
                        System.out.println("send error----------------------------");
                    }
                }
            }
        }).start();
    }



}
