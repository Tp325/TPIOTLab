#include "Communication.h"
#include "Sensor.h"
#include "config.h"
Communication communication;
Sensor sensor;
void setup() {
  // ID:1  //doman
  // ID:2 //Ph
  // ID:3 //NH4
  // ID:4 //TMP
  // ID:5 //O2
  setID(4);
  Serial.begin(9600);
  communication.begin();
  sensor.begin();
}

void loop() {
  communication.receiveFromMaster();
  if (sendSensorData == 1) {
    communication.sendToMaster(String("{\"ID\":" + String(IDOfSensor) + ",\"da\":" + String(sensor.readVotage(sig1Pin)) + "}"));
    communication.sendToMaster();
    sendSensorData = 0;
  }
}
