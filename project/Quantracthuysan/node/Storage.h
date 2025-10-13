#ifndef Storage_h
#define Storage_h
#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_SIZE 64
#define ADDR 0

class NodeManager {
private:
public:
NodeManager ();
void begin();
void saveUint16(uint16_t num);
uint16_t loadFromEEPROM();
void clearUint16();
};
#endif