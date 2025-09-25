#include "Sensor.h"
static SensorCfg sensors[] = {
  { 0x01, 0x0013, 9600, "Temperature" },  // 0
  { 0x01, 0x0012, 9600, "Humidity" },     // 1
  { 0x01, 0x0015, 9600, "EC" },           // 2
  { 0x01, 0x0006, 9600, "PH" },           // 3
  { 0x01, 0x001E, 9600, "Ni" },           // 4
  { 0x01, 0x001F, 9600, "Photpho" },      // 5
  { 0x01, 0x0020, 9600, "Kali" },         // 6
  { 0x50, 0x0034, 115200, "distance" }    // 7
};
Sensor::Sensor(int BatSense3V3,
               SoftwareSerial *comunication,
               ModbusMaster *node)
  : kalmanTem(2, 2, 0.01),
    kalmanHum(2, 2, 0.01),
    kalmanEC(2, 2, 0.01),
    kalmanPH(2, 2, 0.01),
    kalmanNi(2, 2, 0.01),
    kalmanPhotpho(2, 2, 0.01),
    kalmanKali(2, 2, 0.01),
    kalmandistance(2, 2, 0.01) {
  this->BatSense3V3 = BatSense3V3;
  this->comunication = comunication;
  this->node = node;
}
void Sensor::begin() {
  pinMode(BatSense3V3, INPUT);
  node->begin(sensors[0].slaveID, *comunication);
}
void Sensor::getValueOfSensor() {
  for (int i = 0; i < 5; i++) {
    tempRaw[i] = getSensorValue(sensors[0].slaveID, sensors[0].baud, sensors[0].regAddr, 3) / 10.0;
    delay(300);
    humRaw[i] = getSensorValue(sensors[1].slaveID, sensors[1].baud, sensors[1].regAddr, 3) / 10.0;
    delay(300);
    ECRaw[i] = getSensorValue(sensors[2].slaveID, sensors[2].baud, sensors[2].regAddr, 3) / 10.0;
    delay(300);
    PHRaw[i] = getSensorValue(sensors[3].slaveID, sensors[3].baud, sensors[3].regAddr, 3) / 100.0;
    delay(300);
    niRaw[i] = getSensorValue(sensors[4].slaveID, sensors[4].baud, sensors[4].regAddr, 3);
    delay(300);
    photphoRaw[i] = getSensorValue(sensors[5].slaveID, sensors[5].baud, sensors[5].regAddr, 3);
    delay(300);
    kaliRaw[i] = getSensorValue(sensors[6].slaveID, sensors[6].baud, sensors[6].regAddr, 3);
    delay(300);
    distanceRaw[i] = 40 - (getSensorValue(sensors[7].slaveID, sensors[7].baud, sensors[7].regAddr, 3) / 10.0);
    delay(300);
  }
}
float Sensor::median(float numbers[], int size) {
  if (size <= 0) return 0;
  float temp[size];
  for (int i = 0; i < size; i++) {
    temp[i] = numbers[i];
  }
  std::sort(temp, temp + size);
  if (size % 2 == 1) {
    return temp[size / 2];
  } else {
    return (temp[size / 2 - 1] + temp[size / 2]) / 2.0;
  }
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
float Sensor::getTemperature() {
  return kalmanTem.updateEstimate(median(tempRaw, 5));
}
float Sensor::getHumidity() {
  return kalmanHum.updateEstimate(median(humRaw, 5));
}
float Sensor::getEC() {
  return kalmanEC.updateEstimate(median(ECRaw, 5));
}
float Sensor::getPH() {
  return kalmanPH.updateEstimate(median(PHRaw, 5));
}
float Sensor::getNito() {
  return kalmanNi.updateEstimate(median(niRaw, 5));
}
float Sensor::getPhotpho() {
  return kalmanPhotpho.updateEstimate(median(photphoRaw, 5));
}
float Sensor::getKali() {
  return kalmanKali.updateEstimate(median(kaliRaw, 5));
}
float Sensor::getDistance() {
  return kalmandistance.updateEstimate(median(distanceRaw, 5));
}
float Sensor::getPinVoltage() {
  return ((analogRead(BatSense3V3) * 3.3) / 4095.0) * 1.27;
}