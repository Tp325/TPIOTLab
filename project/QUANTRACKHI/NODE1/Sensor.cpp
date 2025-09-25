#include "Sensor.h"
static SensorCfg sensors[] = {
  { 1, 0x0000, 9600, "WindSpeed" },      // 0
  { 1, 0x0001, 4800, "WindDirection" },  // 1
  { 2, 0x01F5, 4800, "AirTemp" },        // 2
  { 2, 0x01F4, 4800, "AirHumi" },        // 3
  { 2, 0x01F9, 4800, "Pressure" },       // 4
  { 2, 0x01FA, 4800, "LightHigh" },      // 5
  { 2, 0x01FB, 4800, "LightLow" },       // 6
  { 2, 0x01F6, 4800, "Noise" },          // 7
  { 2, 0x01F7, 4800, "PM2_5" },          // 8
  { 2, 0x01F8, 4800, "PM10" },           // 9
  { 3, 0x0000, 4800, "Rain" },           // 10
  { 4, 0x0001, 9600, "WaterTem" }        // 11
};

Sensor::Sensor(int BatSense3V3,
               SoftwareSerial *comunication,
               ModbusMaster *node) {
  this->BatSense3V3 = BatSense3V3;
  this->comunication = comunication;
  this->node = node;
}
void Sensor::begin() {
  pinMode(BatSense3V3, INPUT);
  lastRain = EEPROM.readUShort(ADDR2);
  Serial.print("last rain: ");
  Serial.println(lastRain);
  node->begin(sensors[0].slaveID, *comunication);
}
int Sensor::getSensorValue(uint8_t slaveID, uint32_t baudrate, uint16_t reg, uint8_t reTries) {
  comunication->flush();
  comunication->end();
  comunication->begin(baudrate);
  comunication->setTimeout(2000);
  node->begin(slaveID, *comunication);
  for (uint8_t t = 0; t < reTries; t++) {
    if (node->readHoldingRegisters(reg, 1) == node->ku8MBSuccess) {
      uint16_t value = node->getResponseBuffer(0);
      node->clearResponseBuffer();
      Serial.println(value);
      return value;
    } else {
      Serial.print("Retry: ");
      Serial.println(t + 1);
    }
    delay(500);
  }
  return 0;
}
float Sensor::getWindSpeed() {
  return getSensorValue(sensors[0].slaveID, sensors[0].baud, sensors[0].regAddr, 3) / 10.0;
}
String Sensor::getWindDirection() {
  int angle = getSensorValue(sensors[1].slaveID, sensors[1].baud, sensors[1].regAddr, 3) / 10;
  if (angle >= 0 && angle <= 45)
    return "North";
  else if (angle > 45 && angle <= 90)
    return "Northeast";
  else if (angle > 90 && angle <= 135)
    return "East";
  else if (angle > 135 && angle <= 180)
    return "Southeast";
  else if (angle > 180 && angle <= 225)
    return "South";
  else if (angle > 225 && angle <= 270)
    return "Southwest";
  else if (angle > 270 && angle <= 315)
    return "West";
  else if (angle > 315 && angle <= 360)
    return "Northwest";
}

float Sensor::getAirTemperature() {
  return getSensorValue(sensors[2].slaveID, sensors[2].baud, sensors[2].regAddr, 3) / 10.0;
}
float Sensor::getAirHumidity() {
  return getSensorValue(sensors[3].slaveID, sensors[3].baud, sensors[3].regAddr, 3) / 10.0;
}
float Sensor::getAirPressure() {
  return getSensorValue(sensors[4].slaveID, sensors[4].baud, sensors[4].regAddr, 3) / 10.0;
}
float Sensor::getVaporPressureDeficit() {
  this->es = 0.6108 * exp((17.27 * getAirTemperature()) / (237.317 + getAirTemperature()));
  return (es * (1 - getAirHumidity() / 100));
}
float Sensor::getLight() {
  uint32_t high = (uint32_t)getSensorValue(sensors[5].slaveID, sensors[5].baud, sensors[5].regAddr, 3);
  uint32_t low = (uint32_t)getSensorValue(sensors[6].slaveID, sensors[6].baud, sensors[6].regAddr, 3);
  return ((high << 16) | low) / 10.0;
}
float Sensor::getNoise() {
  return getSensorValue(sensors[7].slaveID, sensors[7].baud, sensors[7].regAddr, 3) / 10.0;
}
float Sensor::getPM_2_5() {
  return getSensorValue(sensors[8].slaveID, sensors[8].baud, sensors[8].regAddr, 3) / 10.0;
}
float Sensor::getPM_10() {
  return getSensorValue(sensors[9].slaveID, sensors[9].baud, sensors[9].regAddr, 3) / 10.0;
}
float Sensor::getRain() {
  uint16_t bufferRain = getSensorValue(sensors[10].slaveID, sensors[10].baud, sensors[10].regAddr, 3);
  EEPROM.writeUShort(ADDR2, bufferRain);
  EEPROM.commit();
  return (bufferRain / 10.0 - lastRain / 10.0) / 17.0;
}

float Sensor::getWaterTem() {
  return getSensorValue(sensors[11].slaveID, sensors[11].baud, sensors[11].regAddr, 3) / 10.0;
}

float Sensor::readVotage(int analogPin) {
  return analogRead(analogPin) * (3.3 / 4095.0);
}

float Sensor::readBat() {
  Serial.print("Voltage: ");
  Serial.println(readVotage(BatSense3V3));
  return readVotage(BatSense3V3) * 1.27;
}