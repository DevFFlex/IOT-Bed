#include <WiFiClient.h>
#include "WiFiServer.h"
#include <WiFi.h>


class WManager {
private:

  String SSID_SERVER = "MicroShip0345";
  String PASS_SERVER = "123456789";

  WiFiServer *server;
  WiFiClient client;

  std::function<void(String)> onMessageCallback;

public:


  WManager() {
    server = new WiFiServer(80);
  }

  ~WManager() {
    delete server;
  }

  void exit(){
    Serial.println("stop");
    client.stop();

    server->end();
    server->close();
    server->stop();
  }

  void setup();
  void loop();
  void setOnMessageListener(std::function<void(String)> callback);

  void send(String data);
};

void WManager::setup() {
  WiFi.softAP(SSID_SERVER.c_str(), PASS_SERVER.c_str());

  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP().toString());

  server->begin();
}

void WManager::loop() {

  static bool first_client = true;

  if (!client) client = server->available();
  else {

    if (first_client) {
      first_client = false;
      Serial.println("new connect ip = " + client.localIP().toString());
    }

    if (client) {

      if (client.available()) {
        String data = "";
        static int buffer_limite = 500;
        int buffer_count = 0;
        while (true) {
          char ccc = client.read();
          delay(10);
          if (ccc == '$') break;
          else {
            data += String(ccc);
            buffer_count++;
            if (buffer_count >= buffer_limite) break;
          }
        }
        data.trim();
        if (data != "") onMessageCallback(data);
      }
    } else {
      client.stop();
      Serial.println("disconnected");
      first_client = true;
      client = NULL;
    }
  }
}

void WManager::setOnMessageListener(std::function<void(String)> callback) {
  onMessageCallback = callback;
}

void WManager::send(String send_str) {
  if(!client)return;
  // Serial.println("sending --- " + send_str);

  String dataStr = send_str + "$";
  byte buffer_size = dataStr.length() + 1;
  char buffer[buffer_size];

  dataStr.toCharArray(buffer, buffer_size);
  client.write(buffer);
  client.flush();
}
