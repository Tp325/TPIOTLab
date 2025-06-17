#include "Communication.h"
#include "Execution.h"
#include "Sensor.h"
#include "config.h"
#define sleeptime 15 * 1000 * 60  // Giây
Communication communication;
Execution execution(2);
Sensor sensor;
String stationID = "aquaTestDemo";
long long int time_4 = 0;
void setup() {
  Serial.begin(9600);
  communication.begin();
  execution.begin();
  sensor.begin();
  execution.setAllLow();
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
}

void loop() {
  execution.setAllHigh();
  digitalWrite(8, HIGH);
  delay(1000 / portTICK_PERIOD_MS);
  communication.sendToNode("SAL_2");
  communication.sendToNode();
  time_4 = millis();
  delay(200 / portTICK_PERIOD_MS);
  if (communication.receiveFromNode() == 1) {
    sensorName = "SAL";
    buffMsgToSink = "";
    SAL.value = sensor.getSalinity(String(dequeue(buffDataFromNode)).toInt());
    buffMsgToSink += "{\"SID\":\"" + stationID + "\",\"ID\":\"" + sensorName + "\",\"data\":" + String(SAL.value) + "}";
    sensorName = "";
    SAL.value = 0;
    if (!isFull(buffDataToServer)) {
      enqueueData(buffDataToServer, buffMsgToSink.c_str());
    }
  } else {
    sensorName = "";
    Serial.println("nodata SAL_2");
  }
  delay(1000 / portTICK_PERIOD_MS);
  time_4 = millis();
  communication.sendToNode("PH_2_1");
  communication.sendToNode();
  delay(200 / portTICK_PERIOD_MS);
  if (communication.receiveFromNode() == 1) {
    sensorName = "PH";
    buffMsgToSink = "";
    PH.value = sensor.getPH(String(dequeue(buffDataFromNode)).toInt());
    buffMsgToSink += "{\"SID\":\"" + stationID + "\",\"ID\":\"" + sensorName + "\",\"data\":" + String(PH.value) + "}";
    sensorName = "";
    PH.value = 0;
    if (!isFull(buffDataToServer)) {
      enqueueData(buffDataToServer, buffMsgToSink.c_str());
    }

  } else {
    sensorName = "";
    Serial.println("nodata PH_2_1");
  }
  delay(1000 / portTICK_PERIOD_MS);
  time_4 = millis();
  communication.sendToNode("TMP_2_1");
  communication.sendToNode();
  delay(200 / portTICK_PERIOD_MS);
  if (communication.receiveFromNode() == 1) {
    sensorName = "TMP";
    buffMsgToSink = "";
    TMP.value = sensor.getTemperature(String(dequeue(buffDataFromNode)).toInt());
    buffMsgToSink += "{\"SID\":\"" + stationID + "\",\"ID\":\"" + sensorName + "\",\"data\":" + String(TMP.value) + "}";
    if (!isFull(buffDataToServer)) {
      enqueueData(buffDataToServer, buffMsgToSink.c_str());
    }
  } else {
    sensorName = "";
    Serial.println("nodata TMP_2_1");
  }
  delay(1000 / portTICK_PERIOD_MS);
  time_4 = millis();
  communication.sendToNode("NH4_2_1");
  communication.sendToNode();
  delay(200 / portTICK_PERIOD_MS);
  if (communication.receiveFromNode() == 1) {
    sensorName = "NH4";
    buffMsgToSink = "";
    NH4.value = sensor.getNH4(String(dequeue(buffDataFromNode)).toInt());
    buffMsgToSink += "{\"SID\":\"" + stationID + "\",\"ID\":\"" + sensorName + "\",\"data\":" + String(NH4.value) + "}";
    if (!isFull(buffDataToServer)) {
      enqueueData(buffDataToServer, buffMsgToSink.c_str());
    }
  } else {
    sensorName = "";
    Serial.println("nodata NH4_2_1");
  }
  buffMsgToSink = "";
  buffMsgToSink += "{\"SID\":\"" + stationID + "\",\"ID\":\"pinStatus\"" + String(",\"data\":") + String(sensor.readBat(BatSense3V3)) + "}";
  if (!isFull(buffDataToServer)) {
    enqueueData(buffDataToServer, buffMsgToSink.c_str());
  }
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
  execution.setAllLow();
  delay(2000 / portTICK_PERIOD_MS);
  execution.setPin(9, 0);
  execution.setPin(10, 1);
  // communication.processMQTT();
  communication.sendToServer();
  digitalWrite(9, LOW);
  delay(15 * 1000 * 60);
}
