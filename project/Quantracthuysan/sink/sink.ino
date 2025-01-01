#include "Function.h"
TransmitToNode master;
Rs485Sensor node;
Wifi wifi;
TransmitToSever esp32;
Sleep espSleep;
void setup() {
  Serial.begin(9600);
  wifi.start("Phong_4", "1234512345");
  esp32.start();
  master.start();
  espSleep.setup(15);
}
void loop() {
  master.processDataFromNode(node);
  esp32.sendDataToSever(master.processedData, "dataOfMasterFromAqua");
  espSleep.begin();
}
