#include "Storage.h"
NodeManager::NodeManager() {
}
void NodeManager::begin() {
  EEPROM.begin(EEPROM_SIZE);
  // clearUint16();
  // saveUint16(4);
}
void NodeManager::saveUint16(uint16_t num) {
  EEPROM.writeUShort(ADDR, num);
  EEPROM.commit();
}

uint16_t NodeManager::loadFromEEPROM() {
  return EEPROM.readUShort(ADDR);
}

void NodeManager::clearUint16() {
  EEPROM.writeUShort(ADDR, 0);
  EEPROM.commit();
}
