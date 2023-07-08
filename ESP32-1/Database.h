#include <EEPROM.h>

#define WSSID 0
#define WPASS 1

typedef struct DataObj_Struct {
  String SSID = "-";
  String PASS = "-";
} DataObj;

class Database {
private:
  int address = 0;
  int value = 123;
  int addr[2] = {
    0,100
  };
  Timer timer_update;
public:
  DataObj data;

  Database() : timer_update(1000){
  }

  void start() {
    EEPROM.begin(512);
  }
  void update(){
    if(timer_update.isExpired()){
      data.SSID = getWSSID();
      data.PASS = getWPASS();
    }
  }
  void put(short index,short data) {
    EEPROM.put(index, data);
    EEPROM.commit();
  }

  int get(short index){
    int readValue;
    EEPROM.get(address, readValue);
    return readValue;
  }

  void writeStringToDB(int addr, const String str);
  String readStringFromDB(int addr);

  void setData(String *str_arr);
 
  void setWSSID(String str);
  String getWSSID();

  void setWPASS(String str);
  String getWPASS();
};


void Database::setWSSID(String s){
  writeStringToDB(addr[WSSID],s);
}

String Database::getWSSID(){
  return readStringFromDB(addr[WSSID]);
}

void Database::setWPASS(String s){
  writeStringToDB(addr[WPASS],s);
}

String Database::getWPASS(){
  return readStringFromDB(addr[WPASS]);
}

void Database::setData(String *str_arr){
  
}



void Database::writeStringToDB(int addr,String str) {
  Serial.println("write : " + str);
  int length = str.length();
  EEPROM.write(addr, length);

  for (int i = 0; i < length; i++) {
    EEPROM.write(addr + 1 + i, str[i]);
  }

  EEPROM.commit();
}

String Database::readStringFromDB(int addr) {
  int length = EEPROM.read(addr);
  String str;

  for (int i = 0; i < length; i++) {
    str += char(EEPROM.read(addr + 1 + i));
  }

  return str;
}