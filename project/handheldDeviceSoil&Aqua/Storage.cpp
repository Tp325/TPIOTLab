#include "Storage.h"
Storage::Storage() {}
void Storage::begin() {
  EEPROM.begin(16);
  vTaskDelay(200 / portTICK_PERIOD_MS);
  deviceMode = readFromEEPROM(1);
}
void Storage::saveToEEPROM() {
  deviceModeInEEPROM = readFromEEPROM(1);
  if (deviceModeInEEPROM != deviceMode) {
    EEPROM.put(1, deviceMode);
    needCommit = true;
  }
  if (needCommit) {
    EEPROM.commit();
    Serial.println("Save to EEPROM");
    needCommit = false;
  }
}
float Storage::readFromEEPROM(int adrress) {
  EEPROM.get(adrress, buffReadData);
  return buffReadData;
}