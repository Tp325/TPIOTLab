#ifndef Storage_h
#define Storage_h
#include <Arduino.h>
#include <EEPROM.h>
#include <vector>
#define EEPROM_SIZE 1024
#define NODE_MAX 50
#define timeOUT 17 * 60 * 1000
struct nodeInF {
  uint16_t ID;
  uint32_t lastTimeSentData = 0;
  bool checkHealth = false;
  bool reset = false;
  bool getData = false;
};
class NodeManager {
private:

public:
  uint16_t buffID;
  std::vector<nodeInF> nodes;
  NodeManager();
  void begin();
  void addNode(uint16_t newID);
  bool exists(uint16_t id);
  nodeInF *findNode(uint16_t id);
  bool removeNode(uint16_t id);
  void updateNode(uint16_t id);
  void updateAllNode();
  size_t size();
  void printNodes();
  void clearEEPROM();
  void loadFromEEPROM();
  void saveToEEPROM();
  uint16_t checkingTimeOut();
};

#endif
