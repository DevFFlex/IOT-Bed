package com.example.esp32aapp;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.appcompat.app.AlertDialog;

import java.util.ArrayList;

interface WifiListEvent{
    void onSelect(String name);
}

public class Wifi_List extends AlertDialog.Builder {

    public void setWifiListEvent(WifiListEvent wifiListEvent) {
        this.wifiListEvent = wifiListEvent;
    }

    WifiListEvent wifiListEvent;
    View view;
    AlertDialog dialog;

    public Wifi_List(Context context) {
        super(context);
        view = LayoutInflater.from(context).inflate(R.layout.wifi_list, null);
        LinearLayout box = view.findViewById(R.id.wl_layout);

        WifiManage wifiManage = new WifiManage(context);
        wifiManage.scanWifi();
        wifiManage.setWifiEvent(new WifiEvent() {
            @Override
            public void onScanFinish(ArrayList<String> item) {

                box.removeAllViews();
                for (String name:item) {
                    if (name.equals(""))continue;
                    View item_view = LinearLayout.inflate(context,R.layout.wifi_list_item,null);
                    Button b = item_view.findViewById(R.id.wli_btnselect);

                    b.setText(name);
                    b.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            dialog.dismiss();
                            wifiListEvent.onSelect(name);
                        }
                    });

                    box.addView(item_view);
                }

                box.invalidate();

            }
        });


        this.setView(view);

        dialog = this.create();
        dialog.getWindow().setBackgroundDrawableResource(R.drawable.bg_btn_ms7);

//        Window window = dialog.getWindow();
//        if (window != null)window.setWindowAnimations(R.style.MyAlertDialogAnimation);
//        else System.out.println("window == NULL");
        dialog.show();


//        animationOption.startAnimation(input_ph_view,R.animator.animator_set1);

    }
}
