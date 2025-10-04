#include "Sensor.h"

Sensor::Sensor(int BatSense3V3, int BatSense12V,
               SoftwareSerial* comunication, long baurate) {
  this->BatSense3V3 = BatSense3V3;
  this->BatSense12V = BatSense12V;
  this->comunication = comunication;
  this->baurate = baurate;
}
void Sensor::begin() {
  pinMode(BatSense3V3, INPUT);
  pinMode(BatSense12V, INPUT);
}
float Sensor::readVotage(int analogPin) {
  return analogRead(analogPin) * (3.3 / 4095.0);
}

float Sensor::readBat() {
  return readVotage(BatSense3V3) * 1.68;
}
float Sensor ::getVoltage(int RawValue) {
  return (RawValue * 5.0) / 1023.0;
}
int Sensor::sentToSensor(String data) {
  comunication->begin(baurate);
  comunication->println(data);
  vTaskDelay(200 / portTICK_PERIOD_MS);
  if (comunication->available()) {
    return (int)(comunication->readStringUntil('\n'));
  } else {
    Serial.println("ERROR");
    return 0;
  }
}
float Sensor ::getSalinity() {
  return (getVoltage(sentToSensor("SAL_1")) * SAL.slope + SAL.intercept);
}
float Sensor ::getPH() {
  return (getVoltage(sentToSensor("PH_2_1")) * PH.slope + PH.intercept);
}
float Sensor ::getNH4() {
  return ((getVoltage(sentToSensor("NH4_2_1")) * NH4.slope + NH4.intercept) > 0 ? (getVoltage(sentToSensor("NH4_2_1")) * NH4.slope + NH4.intercept) : 0.0);
}
float Sensor ::getTemperature() {
  measured_resistance = (resistance * getVoltage(sentToSensor("TMP_2_1"))) / (5.0 - getVoltage(sentToSensor("TMP_2_1")));
  return (1.0 / (Ko + K1 * logf(1000.0 * measured_resistance) + K2 * powf(logf(1000.0 * measured_resistance), 3)) - 273.15);
}
