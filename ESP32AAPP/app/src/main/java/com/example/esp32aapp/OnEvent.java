package com.example.esp32aapp;

import java.util.ArrayList;

public interface OnEvent {
    void onScanWifiSuccess(ArrayList<String> wifilist);
    void onDialogClickOk(ArrayList<String> list);
    void onClientDisconnected();
}
