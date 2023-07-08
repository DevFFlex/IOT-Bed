package com.example.esp32aapp;

import android.widget.TextView;

import java.io.IOException;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.List;

public class Comunity extends ClientClass implements ClientOnEventListener {
    private MainActivity mainActivity;
    private OnEvent onEvent;

    public Comunity(MainActivity mainActivity,OnEvent onEvent){
        super("192.168.4.1",80,onEvent);
        this.onEvent = onEvent;
        this.setClientOnEventListener(this::onMessage);

        this.mainActivity = mainActivity;
    }


    public void sendData(ArrayList<String> list){
        String str = "";
        for (String i:list) {
            str += i + ",";
        }
        sendToServer("SET:DATA=" + str);
    }

    public void getData(){
        sendToServer("GET:DATA=");
    }





    @Override
    public void onMessage(String message) {
        if (!message.contains(":"))return;
        String header,commands;
        try{
            String[] data = message.split(":");
            header = data[0].trim();
            commands = data[1];

            System.out.println(message);
        }catch (Exception e){
            return;
        }


        if(header.equals("RESP")){

            if (commands.equals("S_KICK")){
                this.disconnect();
            }

        }else if(header.equals("SET")){
            if(!commands.contains("=") && ! commands.contains(":"))return;

            String data2[] = commands.split("=");
            String command = data2[0].trim();
            String value = data2[1].trim();

            String output = "message = " + message + "\n";
            output += "command = " + command + "\n";
            output += "value = " + value + "\n";

            if (command.equals("DATA")){
               String [] data = value.split(",");
                mainActivity.output(mainActivity.tv_wssid,data[0]);
                mainActivity.output(mainActivity.tv_wpass,data[1]);
            }
            if (command.equals("W_COOLDOWN")){
                mainActivity.output(mainActivity.tv_wcooldown,value);

                try{
                    cooldown_count = Integer.valueOf(value);
                }catch (Exception e){}
            }

        }


    }
}

