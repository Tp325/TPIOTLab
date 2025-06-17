#include "Communication.h"
DynamicJsonDocument doc(1024);
SoftwareSerial mySerial(RXPin, TXPin);
Communication::Communication() {
}
void Communication::begin() {
  mySerial.begin(115200);
}
void Communication::receiveFromMaster() {
  if (mySerial.available()) {
    msgFromMaster = mySerial.readStringUntil('\n');
    if (!isFull(buffDataFromMaster)) {
      enqueueData(buffDataFromMaster, msgFromMaster.c_str());
    }
  }
}
void Communication::sendToMaster(String msg) {
  if (!isFull(buffDataToMaster)) {
    enqueueData(buffDataToMaster, msg.c_str());
  }
}
void Communication::sendToMaster() {
  while (!isEmpty(buffDataToMaster)) {
    Serial.print("send to display: ");
    msgToMaster = dequeue(buffDataToMaster);
    mySerial.println(msgToMaster);
    delay(20);
  }
}
void Communication::analizeDataFromMaster() {
  while (!isEmpty(buffDataFromMaster)) {
    DeserializationError error = deserializeJson(doc, dequeue(buffDataFromMaster));
    if (!error) {
      if (IDOfSensor == doc["ID"].as<int>() && doc["cm"].as<int>() == -1) {
        sendSensorData = 1;
      }
    }
    delay(20);
  }
}
