/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL5daptOuM"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "5D-o_OTnMOGG9s-1j3dQd7JWOXH_dkyf"
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <SPI.h>
#include <RFID.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
HardwareSerial SerialPort(2);  // use UART2
#define SS_PIN 5
#define RST_PIN 15
bool id;
RFID rfid(SS_PIN, RST_PIN);
// Setup variables:
int serNum0;
int serNum1;
int serNum2;
int serNum3;
int serNum4;
#define LMoter1_PIN 27
#define RMoter1_PIN 14
#define LMoter2_PIN 25
#define RMoter2_PIN 26
#define LMoter3_PIN 13
#define RMoter3_PIN 12
#define LPWM_Moter1 1
#define RPWM_Moter1 2
#define LPWM_Moter2 3
#define RPWM_Moter2 4
#define LPWM_Moter3 5
#define RPWM_Moter3 6
#define BUZZER_PIN 4
#define motor_speed 450
#define led_Green 2
#define RXp2 16
#define TXp2 17
String datain, Sub_HeartRate, Sub_SpO2;
int HeartRate, SpO2;
// Your WiFi credentials.
// Set password to "" for open networks.
String ssid = "NNN";
String pass = "12345678";

#include "Ass.h"
Ass *ass = new Ass();

BlynkTimer timer;
BLYNK_CONNECTED() {
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime  every second to Virtual Pin 2.
void setup() {
  Serial.begin(115200);
  // Debug console

  ass->start();
}

void loop() {
  ass->update();
  if (ass->isReady()) {
    Serial.println("is Ready");
    if (ass->setup_work) {
      ass->setup_work = false;

      
      ssid = (ass->db->getWSSID() != "-") ? ass->db->getWSSID() : ssid;
      pass = (ass->db->getWPASS() != "-") ? ass->db->getWPASS() : pass;
    
      process_setup();
    }

    process_loop();
    // Serial.println("-----------------------------");
    // Serial.println("ssid = " + ssid);
    // Serial.println("pass = " + pass);
  }
}


void process_setup() {
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid.c_str(), pass.c_str());
  //SerialPort.begin(115200, SERIAL_8N1, 16, 17);
  timer.setInterval(1000L, RFID_tag);
  timer.setInterval(100L, max30100);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(led_Green, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(led_Green, 1);
  digitalWrite(32, 0);
  digitalWrite(33, 0);
  digitalWrite(35, 0);
  digitalWrite(BUZZER_PIN, 0);
  id = 0;
  // Turn on the blacklight and print a message.
  // Setup a function to be called every second
  ledcAttachPin(LMoter1_PIN, 1);
  ledcAttachPin(RMoter1_PIN, 2);
  ledcAttachPin(LMoter2_PIN, 3);
  ledcAttachPin(RMoter2_PIN, 4);
  ledcAttachPin(LMoter3_PIN, 5);
  ledcAttachPin(RMoter3_PIN, 6);
  ledcSetup(LPWM_Moter1, 490, 10);
  ledcSetup(RPWM_Moter1, 490, 10);
  ledcSetup(LPWM_Moter2, 490, 10);
  ledcSetup(RPWM_Moter2, 490, 10);
  ledcSetup(LPWM_Moter3, 490, 10);
  ledcSetup(RPWM_Moter3, 490, 10);
  ledcWrite(LPWM_Moter1, 0);
  ledcWrite(RPWM_Moter1, 0);
  ledcWrite(LPWM_Moter2, 0);
  ledcWrite(RPWM_Moter2, 0);
  ledcWrite(LPWM_Moter3, 0);
  ledcWrite(RPWM_Moter3, 0);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("touch card");
  SPI.begin();
  rfid.init();
}

void process_loop() {
  while (id) {
    Blynk.run();
    timer.run();
    RFID_tag();
    while (!Blynk.connected()) {
      digitalWrite(led_Green, 0);
      Blynk.begin(BLYNK_AUTH_TOKEN, ssid.c_str(), pass.c_str());
      //delay(200);
    }
    if (Blynk.connected())
      digitalWrite(led_Green, 1);
  }

  RFID_tag();
  //digitalWrite(led_Green, 0);
}


BLYNK_WRITE(V0) {
  // Set incoming value from pin V0 to a variable
  if (param.asInt() == 1) {
    ledcWrite(LPWM_Moter1, 0);
    ledcWrite(RPWM_Moter1, motor_speed);
    digitalWrite(32, 1);
    digitalWrite(33, 1);
    digitalWrite(35, 1);

  } else {
    ledcWrite(LPWM_Moter1, 0);
    ledcWrite(RPWM_Moter1, 0);
    digitalWrite(32, 1);
    digitalWrite(33, 1);
    digitalWrite(35, 1);
  }
}
BLYNK_WRITE(V1) {
  // Set incoming value from pin V0 to a variable
  if (param.asInt() == 1) {
    ledcWrite(LPWM_Moter1, motor_speed);
    ledcWrite(RPWM_Moter1, 0);
    digitalWrite(32, 1);
    digitalWrite(33, 1);
    digitalWrite(35, 1);

  } else {
    ledcWrite(LPWM_Moter1, 0);
    ledcWrite(RPWM_Moter1, 0);
    digitalWrite(32, 1);
    digitalWrite(33, 1);
    digitalWrite(35, 1);
  }
}
BLYNK_WRITE(V2) {
  // Set incoming value from pin V0 to a variable
  if (param.asInt() == 1) {
    ledcWrite(LPWM_Moter2, 0);
    ledcWrite(RPWM_Moter2, motor_speed);
    digitalWrite(32, 1);
    digitalWrite(33, 1);
    digitalWrite(35, 1);

  } else {
    ledcWrite(LPWM_Moter2, 0);
    ledcWrite(RPWM_Moter2, 0);
    digitalWrite(32, 1);
    digitalWrite(33, 1);
    digitalWrite(35, 1);
  }
}
BLYNK_WRITE(V4) {
  // Set incoming value from pin V0 to a variable
  if (param.asInt() == 1) {
    ledcWrite(LPWM_Moter2, motor_speed);
    ledcWrite(RPWM_Moter2, 0);
    digitalWrite(32, 1);
    digitalWrite(33, 1);
    digitalWrite(35, 1);

  } else {
    ledcWrite(LPWM_Moter2, 0);
    ledcWrite(RPWM_Moter2, 0);
    digitalWrite(32, 1);
    digitalWrite(33, 1);
    digitalWrite(35, 1);
  }
}
BLYNK_WRITE(V5) {
  // Set incoming value from pin V0 to a variable
  if (param.asInt() == 1) {
    ledcWrite(LPWM_Moter3, 600);
    ledcWrite(RPWM_Moter3, 0);
    digitalWrite(32, 1);
    digitalWrite(33, 1);
    digitalWrite(35, 1);
  } else {
    ledcWrite(LPWM_Moter3, 0);
    ledcWrite(RPWM_Moter3, 0);
    digitalWrite(32, 1);
    digitalWrite(33, 1);
    digitalWrite(35, 1);
  }
}
BLYNK_WRITE(V6) {
  // Set incoming value from pin V0 to a variable
  if (param.asInt() == 1) {
    ledcWrite(LPWM_Moter3, 0);
    ledcWrite(RPWM_Moter3, 600);
    digitalWrite(32, 1);
    digitalWrite(33, 1);
    digitalWrite(35, 1);

  } else {
    ledcWrite(LPWM_Moter3, 0);
    ledcWrite(RPWM_Moter3, 0);
    digitalWrite(32, 1);
    digitalWrite(33, 1);
    digitalWrite(35, 1);
  }
}
void max30100() {
  datain = Serial2.readString();
  if (datain.toInt() < 10000) {
    Sub_HeartRate = datain.substring(0, 2);
    Sub_SpO2 = datain.substring(2, 4);
  } else {
    Sub_HeartRate = datain.substring(0, 3);
    Sub_SpO2 = datain.substring(3, 5);
  }
  Serial.print("datain:");
  Serial.print(datain);
  Serial.print("HeartRate:");
  Serial.print(Sub_HeartRate);
  Serial.print("SpO2:");
  Serial.println(Sub_SpO2);
  ;
  HeartRate = Sub_HeartRate.toInt();
  SpO2 = Sub_SpO2.toInt();
  Blynk.virtualWrite(V7, HeartRate);
  Blynk.virtualWrite(V8, SpO2);
}
void RFID_tag() {
  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
      /* With a new cardnumber, show it. */
      Serial.println(" ");
      Serial.println("Card found");
      serNum0 = rfid.serNum[0];
      serNum1 = rfid.serNum[1];
      serNum2 = rfid.serNum[2];
      serNum3 = rfid.serNum[3];
      serNum4 = rfid.serNum[4];

      //Serial.println(" ");
      Serial.println("Cardnumber:");
      Serial.print("Dec: ");
      Serial.print(rfid.serNum[0], DEC);
      Serial.print(", ");
      Serial.print(rfid.serNum[1], DEC);
      Serial.print(", ");
      Serial.print(rfid.serNum[2], DEC);
      Serial.print(", ");
      Serial.print(rfid.serNum[3], DEC);
      Serial.print(", ");
      Serial.print(rfid.serNum[4], DEC);
      Serial.println(" ");

      Serial.print("Hex: ");
      Serial.print(rfid.serNum[0], HEX);
      Serial.print(", ");
      Serial.print(rfid.serNum[1], HEX);
      Serial.print(", ");
      Serial.print(rfid.serNum[2], HEX);
      Serial.print(", ");
      Serial.print(rfid.serNum[3], HEX);
      Serial.print(", ");
      Serial.print(rfid.serNum[4], HEX);
      Serial.println(" ");
    }
    if (rfid.serNum[0] == 33 && rfid.serNum[1] == 105 && rfid.serNum[2] == 177 && rfid.serNum[3] == 35 && rfid.serNum[4] == 218) {
      Serial.println("Narongrit");
      lcd.setCursor(0, 0);
      lcd.print("pass....");
      id = 1;
      SerialPort.print(1);
      lcd.clear();
      delay(1000);
      digitalWrite(BUZZER_PIN, 1);
      delay(300);


    } else {
      Serial.println("not Me");
      id = 0;
      SerialPort.print(0);
      lcd.setCursor(0, 0);
      lcd.print("try again...");
      digitalWrite(BUZZER_PIN, 1);
      delay(100);
      digitalWrite(BUZZER_PIN, 0);
      delay(150);
      digitalWrite(BUZZER_PIN, 1);
      delay(150);
    }
  }
  rfid.halt();
  digitalWrite(BUZZER_PIN, 0);
}