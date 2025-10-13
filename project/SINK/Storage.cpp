#include "Storage.h"
NodeManager::NodeManager() {
}
void NodeManager::addNode(uint16_t newID) {
  if (!exists(newID)) {
    nodeInF node;
    node.ID = newID;
    node.lastTimeSentData = millis();
    nodes.push_back(node);
  }
}
bool NodeManager::exists(uint16_t id) {
  for (auto &n : nodes) {
    if (n.ID == id) return true;
  }
  return false;
}
nodeInF *NodeManager::findNode(uint16_t id) {
  for (auto &n : nodes) {
    if (n.ID == id) return &n;
  }
  return nullptr;
}
bool NodeManager::removeNode(uint16_t id) {
  for (auto it = nodes.begin(); it != nodes.end(); ++it) {
    if (it->ID == id) {
      nodes.erase(it);
      saveToEEPROM();
      return true;
    }
  }
  return false;
}
void NodeManager::updateNode(uint16_t id) {
  nodeInF *n = findNode(id);
  if (n) {
    n->lastTimeSentData = millis();
  }
}
void NodeManager::updateAllNode() {
  for (auto &n : nodes) {
    n.lastTimeSentData = millis();
  }
}
size_t NodeManager::size() {
  return nodes.size();
}
void NodeManager::printNodes() {
  Serial.println("=== Danh sách node ===");
  for (auto &n : nodes) {
    Serial.print("ID: ");
    Serial.print(n.ID);
    Serial.print(" | LastTime: ");
    Serial.println((millis() - n.lastTimeSentData)/(1000*60));
  }
}
uint16_t NodeManager::checkingTimeOut() {
  buffID = 0;
  for (auto &n : nodes) {
    if (millis() - n.lastTimeSentData >= timeOUT) {
      buffID = n.ID;
    }
  }
  return buffID;
}
void NodeManager::begin() {
  EEPROM.begin(EEPROM_SIZE);
  loadFromEEPROM();
  updateAllNode();
}
void NodeManager::saveToEEPROM() {
  int addr = 0;
  uint16_t count = nodes.size();
  EEPROM.put(addr, count);
  addr += sizeof(count);

  for (auto &n : nodes) {
    EEPROM.put(addr, n);
    addr += sizeof(nodeInF);
  }
  EEPROM.commit();
}
void NodeManager::loadFromEEPROM() {
  nodes.clear();
  int addr = 0;
  uint16_t count;
  EEPROM.get(addr, count);
  addr += sizeof(count);

  if (count > NODE_MAX) count = NODE_MAX;

  for (int i = 0; i < count; i++) {
    nodeInF n;
    EEPROM.get(addr, n);
    addr += sizeof(nodeInF);
    nodes.push_back(n);
  }
}
void NodeManager::clearEEPROM() {
  for (int i = 0; i < EEPROM_SIZE; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  nodes.clear();
}