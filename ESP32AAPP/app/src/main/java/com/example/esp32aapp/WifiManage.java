package com.example.esp32aapp;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiManager;
import android.widget.ArrayAdapter;

import java.util.ArrayList;
import java.util.List;

//import android.support.v7.app.AppCompatActivity;

interface WifiEvent{
    void onScanFinish(ArrayList<String> item);
}

public class WifiManage {
    private Context context;

    public void setOnEvent(OnEvent onEvent) {
        this.onEvent = onEvent;
    }
    public void setWifiEvent(WifiEvent wifiEvent) {
        this.wifiEvent = wifiEvent;
    }

    private OnEvent onEvent = null;
    private WifiEvent wifiEvent = null;

    private WifiManager wifiManager;
    private List<ScanResult> results;
    private ArrayList<String> arrayList = new ArrayList<>();
    private ArrayAdapter adapter;

    public ArrayList<String> getArrayList() {
        return arrayList;
    }

    public ArrayAdapter getAdapter() {
        return adapter;
    }

    public WifiManage(Context context){
        this.context = context;

        wifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);

        if (!wifiManager.isWifiEnabled()) { ;
            wifiManager.setWifiEnabled(true);
        }

//        adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, arrayList);
//        scanWifi();
    }

    public boolean isWifiEnabled(){
        return wifiManager.isWifiEnabled();
    }

    public void connectToWifi(String selectedWifi) {
        String ssid = selectedWifi.substring(0, selectedWifi.indexOf(" - "));

        WifiConfiguration wifiConfig = new WifiConfiguration();
        wifiConfig.SSID = "\"" + ssid + "\"";
        wifiConfig.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);

        int networkId = wifiManager.addNetwork(wifiConfig);
        wifiManager.disconnect();
        wifiManager.enableNetwork(networkId, true);
        wifiManager.reconnect();

    }


    public void scanWifi() {
        arrayList.clear();
        context.registerReceiver(wifiReceiver, new IntentFilter(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION));
        wifiManager.startScan();
    }

    BroadcastReceiver wifiReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            results = wifiManager.getScanResults();
            context.unregisterReceiver(this);

            for (ScanResult scanResult : results) {
                String freq_str = (scanResult.frequency > 5000) ? "5.0Ghz" : "2.4Ghz";
                arrayList.add(scanResult.SSID);
//                adapter.notifyDataSetChanged();
            }

            if(onEvent != null)onEvent.onScanWifiSuccess(arrayList);
            if(wifiEvent != null)wifiEvent.onScanFinish(arrayList);

        }
    };
}
