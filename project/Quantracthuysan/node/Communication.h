#ifndef Communication_h
#define Communication_h
#include "config.h"
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <SoftwareSerial.h>
extern DynamicJsonDocument doc;

class Communication {
private:

public:
  Communication();
  void begin();
  void receiveFromMaster();
  void sendToMaster();
  void sendToMaster(String msg);
  void analizeDataFromMaster();
};
#endif