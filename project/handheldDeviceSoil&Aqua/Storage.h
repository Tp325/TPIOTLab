#ifndef Storage_h
#define Storage_h
#include "Arduino.h"
#include <EEPROM.h>
#include "config.h"
class Storage {
private:
  int buffReadData;
  int IDOfPool;
  bool hasData = false;
  bool needCommit = false;
  int deviceModeInEEPROM;
public:
  Storage();
  void begin();
  void saveToEEPROM();
  float readFromEEPROM(int adrress);
};
#endif