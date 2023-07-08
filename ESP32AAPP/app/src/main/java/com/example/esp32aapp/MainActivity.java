package com.example.esp32aapp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.DialogInterface;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;

import java.util.ArrayList;
import java.util.List;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import java.util.ArrayList;
import java.util.List;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiManager;
import android.os.Bundle;
//import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity implements OnEvent{

    private WifiManage wifiManage;
    private Comunity comunity;

    private ListView listView;
    private Button buttonScan,btn_config,btn_load,main_btn_restartapp;
    public TextView tv_bti,tv_btn,tv_bat,tv_wssid,tv_wpass,tv_wcooldown;
    public TextView main_board_status,main_header;

    private boolean restart_app = false;

    private void output(String str){
        Toast.makeText(getApplicationContext(),str,Toast.LENGTH_SHORT).show();
    }

    public void output(TextView t,String text){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                t.setText(text);
            }
        });
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        main_board_status = findViewById(R.id.main_board_status);

        wifiManage = new WifiManage(getApplicationContext());
        wifiManage.setOnEvent(MainActivity.this);

        comunity = new Comunity(this,this);

        tv_wssid = findViewById(R.id.main_tv_wssid);
        tv_wpass = findViewById(R.id.main_tv_wpass);
        tv_wcooldown = findViewById(R.id.main_tv_wcooldown);
        main_header = findViewById(R.id.main_header);

        buttonScan = findViewById(R.id.btn_connect);
        buttonScan.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
//                wifiManage.scanWifi();
//                comunity.connect("192.168.4.1",80);
                comunity.sendToServer("SET:RESTART=NULL");
            }
        });

        btn_config = findViewById(R.id.btn_config);
        btn_config.setOnClickListener(this::onClickConfig);

        btn_load = findViewById(R.id.btn_load);
        btn_load.setOnClickListener(this::onClickLoad);

        main_btn_restartapp = findViewById(R.id.main_btn_restartapp);
        main_btn_restartapp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(!comunity.isConnect){
                    comunity.connect(comunity.SERVER_IP, comunity.SERVER_PORT);
                }
            }
        });

        new Thread(new Runnable() {
            @Override
            public void run() {
                while (!restart_app){
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            if (comunity.isConnect){
                                main_board_status.setBackgroundResource(R.drawable.status_circle_green);
                                main_btn_restartapp.setVisibility(View.INVISIBLE);
                            }
                            else if(comunity.isReconnect){
                                main_board_status.setBackgroundResource(R.drawable.status_circle_restart);
                                tv_wssid.setText("-");
                                tv_wpass.setText("-");
//                                main_btn_restartapp.setVisibility(View.VISIBLE);

                            }

                        }
                    });

                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        }).start();

    }

    private void restartApp(){
        Intent intent = new Intent(getApplicationContext(), MainActivity.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP);
        startActivity(intent);
        finish();
    }

    public void onClickLoad(View v) {

        new Thread(new Runnable() {
            @Override
            public void run() {
                comunity.getData();
            }
        }).start();

    }

    public void onClickConfig(View v){
        main_header.setVisibility(View.GONE);
        InputDialog inputDialog = new InputDialog(this,MainActivity.this);

        inputDialog.dialog.setOnDismissListener(new DialogInterface.OnDismissListener() {
            @Override
            public void onDismiss(DialogInterface dialogInterface) {
                main_header.setVisibility(View.VISIBLE);
                System.out.println("SSSSSSSSSSSSSSSS");
            }
        });
    }


    @Override
    public void onScanWifiSuccess(ArrayList<String> wifilist) {
        output("scan success " + String.valueOf(wifilist.size()) + " wifi");
    }

    @Override
    public void onDialogClickOk(ArrayList<String> list) {
       comunity.sendData(list);
    }

    @Override
    public void onClientDisconnected() {
        finish();
    }
}