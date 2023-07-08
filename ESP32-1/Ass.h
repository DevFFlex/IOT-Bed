class Timer {
public:
  Timer(unsigned long interval);
  boolean isExpired();
  void reset();
  void setInterval(unsigned long interval);
private:
  unsigned long _interval;
  unsigned long _previousMillis;
};

Timer::Timer(unsigned long interval) {
  _interval = interval;
  _previousMillis = 0;
}

boolean Timer::isExpired() {
  unsigned long currentMillis = millis();
  if ((unsigned long)(currentMillis - _previousMillis) >= _interval) {
    _previousMillis = currentMillis;
    return true;
  }
  return false;
}

void Timer::reset() {
  _previousMillis = millis();
}

void Timer::setInterval(unsigned long interval) {
  _interval = interval;
}


#include "Database.h"
#include "Community.h"

#define TIME_WAIT_CONNECT 60


class Ass {
private:
  bool wifi_status = false;
  bool blynk_status = false;

  Community *community;

  bool time_aleady_status = false;

  Timer timer_wifi_cooldown;
  int wifi_cooldown = TIME_WAIT_CONNECT;

public:
  Database *db;
  bool setupAss = true;
  bool setup_work = true;
  String SBTI = "";
  String SBTN = "";
  String SBAT = "";
  String SSSID = "";
  String SPASS = "";

  Ass()
    : timer_wifi_cooldown(1000) {
    db = new Database();
    community = new Community(db);
  }

  ~Ass() {
    delete db;
    delete community;
  }

  void start();
  void update();

  bool isReady();
};

void Ass::start() {
  community->start();
  db->start();
}

void Ass::update() {
  community->update();
  db->update();


  if (timer_wifi_cooldown.isExpired()) {
    if (wifi_cooldown <= 0) {
      time_aleady_status = true;
      // community->exit();
      delay(1000);
    } else {
      wifi_cooldown--;
      Serial.println("time = " + String(wifi_cooldown));
    }

    community->send("SET:W_COOLDOWN=" + String(wifi_cooldown));
  }
}

bool Ass::isReady() {
  bool ready = false;
  String wssid = db->getWSSID();
  String wpass = db->getWPASS();

  if (time_aleady_status) ready = true;
  return ready;
}