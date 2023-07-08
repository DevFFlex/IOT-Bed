package com.example.esp32aapp;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import androidx.appcompat.app.AlertDialog;

import java.util.ArrayList;
import java.util.List;

public class InputDialog extends AlertDialog.Builder {

    OnEvent listener;

    View input_ph_view;
    AlertDialog dialog;

    EditText et_bti,et_btn,et_bat,et_wssid,et_wpass;

    public InputDialog(Context context, OnEvent listener) {
        super(context);
        this.listener = listener;

        input_ph_view = LayoutInflater.from(context).inflate(R.layout.input_dialog, null);

        Button input_btnok = input_ph_view.findViewById(R.id.input_btnOk);
        Button input_btnScanWifi = input_ph_view.findViewById(R.id.input_btnScanWifi);

        et_wssid = input_ph_view.findViewById(R.id.input_et_wssid);
        et_wpass = input_ph_view.findViewById(R.id.input_et_wpass);

        input_btnok.setOnClickListener(this::onOk);
        input_btnScanWifi.setOnClickListener(this::onScanWifi);

        this.setView(input_ph_view);

        dialog = this.create();
        dialog.getWindow().setBackgroundDrawableResource(R.drawable.bg_layout2);

//        Window window = dialog.getWindow();
//        if (window != null)window.setWindowAnimations(R.style.MyAlertDialogAnimation);
//        else System.out.println("window == NULL");
        dialog.show();


//        animationOption.startAnimation(input_ph_view,R.animator.animator_set1);

    }


    public void onScanWifi(View v){
        Wifi_List wifi_list = new Wifi_List(getContext());
        wifi_list.setWifiListEvent(new WifiListEvent() {
            @Override
            public void onSelect(String name) {
                et_wssid.setText(name);
            }
        });
    }


    public void onOk(View v){
        ArrayList<String> list = new ArrayList<String>();

        String wssid = et_wssid.getText().toString();
        String wpass = et_wpass.getText().toString();

        if(wssid.equals(""))wssid = "-";
        if(wpass.equals(""))wpass = "-";

        list.add(wssid);
        list.add(wpass);

        listener.onDialogClickOk(list);

        dialog.dismiss();
    }
}
