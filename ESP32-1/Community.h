#include <ESP.h>
#include "WManager.h"

#define GET "GET"
#define SET "SET"

struct ProcessResult {
  String head;
  String com;
  String value;
};

class Community : public WManager {
private:
  Database* db;
  void split(String* databox, String text, String symbol, int size);
public:

  Community(Database* dbIn)
    : WManager() {
    WManager::setOnMessageListener(std::bind(&Community::onMessange, this, std::placeholders::_1));
    db = dbIn;
  }

  void start();
  void update();

  String enformat();
  void setData(String strin);

  struct ProcessResult process(String strin);
  void onMessange(String data);
};


void Community::start() {
  WManager::setup();
}

void Community::update() {
  WManager::loop();
}


void Community::split(String* databox, String text, String symbol, int size) {

  char charArrayText[text.length() + 1];
  text.toCharArray(charArrayText, text.length() + 1);

  char charArraySymbol[symbol.length() + 1];
  symbol.toCharArray(charArraySymbol, symbol.length() + 1);

  char* symbol1 = charArraySymbol;

  char* part = strtok(charArrayText, symbol1);
  for (int i = 0; i < size; i++) {
    databox[i] = part;

    part = strtok(NULL, symbol1);
  }
}

struct ProcessResult Community::process(String strin) {
  struct ProcessResult res;
  String str1[2];
  split(str1, strin, ":", 2);
  String str2[2];
  split(str2, str1[1], "=", 2);

  res.head = str1[0];
  res.com = str2[0];
  res.value = str2[1];

  return res;
}


void Community::onMessange(String data) {
  struct ProcessResult res = process(data);

  if (res.head == GET) {
    Serial.println("g : " + data);
    if (res.com == "DATA") WManager::send("SET:DATA=" + enformat());

  } else if (res.head == SET) {

    if (res.com == "DATA")setData(res.value);
    if (res.com == "RESTART") ESP.restart();
  }
}


String Community::enformat() {
  String str = "";
  str += db->getWSSID() + ",";
  str += db->getWPASS();
  return str;
}

void Community::setData(String strin) {
  String item[2];
  split(item, strin, ",",2);

  Serial.println("item[0] = " + item[0]);
  Serial.println("item[1] = " + item[1]);

  db->setWSSID(item[0]);
  db->setWPASS(item[1]);
 
}