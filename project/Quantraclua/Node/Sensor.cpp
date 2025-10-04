#include "Sensor.h"
RTC_DATA_ATTR bool isStartFirstTime = 1;
RTC_DATA_ATTR float lastSensorValueTem = 0;
RTC_DATA_ATTR float lastSensorValueHum = 0;
RTC_DATA_ATTR float lastSensorValueEC = 0;
RTC_DATA_ATTR float lastSensorValuePH = 0;
RTC_DATA_ATTR float lastSensorValueNi = 0;
RTC_DATA_ATTR float lastSensorValuePhotpho = 0;
RTC_DATA_ATTR float lastSensorValueKali = 0;
RTC_DATA_ATTR float lastSensorValuedistance = 0;
RTC_DATA_ATTR float lastSensorValuepin = 0;
static SensorCfg sensors[] = {
  { 0x01, 0x0013, 9600, "Temperature" },  // 0
  { 0x01, 0x0012, 9600, "Humidity" },     // 1
  { 0x01, 0x0015, 9600, "EC" },           // 2
  { 0x01, 0x0006, 9600, "PH" },           // 3
  { 0x01, 0x001E, 9600, "Ni" },           // 4
  { 0x01, 0x001F, 9600, "Photpho" },      // 5
  { 0x01, 0x0020, 9600, "Kali" },         // 6
  { 0x50, 0x34, 115200, "distance" }      // 7
};
Sensor::Sensor(int BatSense3V3,
               HardwareSerial* comunication,
               ModbusMaster* node)
  : emaTem(0.3),
    emaHum(0.3),
    emaEC(0.3),
    emaPH(0.3),
    emaNi(0.3),
    emaPhotpho(0.3),
    emaKali(0.3),
    emaDistance(0.3) {
  this->BatSense3V3 = BatSense3V3;
  this->comunication = comunication;
  this->node = node;
}
void Sensor::begin() {
  pinMode(BatSense3V3, INPUT);
  node->begin(sensors[0].slaveID, *comunication);
  if (isStartFirstTime == 0) {
    emaTem.update(lastSensorValueTem);
    emaHum.update(lastSensorValueHum);
    emaEC.update(lastSensorValueEC);
    emaPH.update(lastSensorValuePH);
    emaNi.update(lastSensorValueNi);
    emaPhotpho.update(lastSensorValuePhotpho);
    emaKali.update(lastSensorValueKali);
    emaDistance.update(lastSensorValuedistance);
    delay(200);
    Serial.println("Restored sensor last values from EEPROM!");
  } else {
    isStartFirstTime = 0;
    Serial.println("New station deteched");
  }
}
void Sensor::getValueOfSensor() {
  for (int i = 0; i < 5; i++) {
    tempRaw[i] = getSensorValue(sensors[0].slaveID, sensors[0].baud, sensors[0].regAddr, 3) / 10.0;
    Serial.print("tem: ");
    Serial.println(tempRaw[i]);
    delay(300);
    humRaw[i] = getSensorValue(sensors[1].slaveID, sensors[1].baud, sensors[1].regAddr, 3) / 10.0;
    Serial.print("hum: ");
    Serial.println(humRaw[i]);
    delay(300);
    ECRaw[i] = getSensorValue(sensors[2].slaveID, sensors[2].baud, sensors[2].regAddr, 3) / 10.0;
    Serial.print("EC: ");
    Serial.println(ECRaw[i]);
    delay(300);
    PHRaw[i] = getSensorValue(sensors[3].slaveID, sensors[3].baud, sensors[3].regAddr, 3) / 100.0;
    Serial.print("PH: ");
    Serial.println(PHRaw[i]);
    delay(300);
    niRaw[i] = getSensorValue(sensors[4].slaveID, sensors[4].baud, sensors[4].regAddr, 3);
    Serial.print("ni: ");
    Serial.println(niRaw[i]);
    delay(300);
    photphoRaw[i] = getSensorValue(sensors[5].slaveID, sensors[5].baud, sensors[5].regAddr, 3);
    Serial.print("photpho: ");
    Serial.println(photphoRaw[i]);
    delay(300);
    kaliRaw[i] = getSensorValue(sensors[6].slaveID, sensors[6].baud, sensors[6].regAddr, 3);
    Serial.print("kali: ");
    Serial.println(kaliRaw[i]);
    delay(300);
    distanceRaw[i] = 40 - (getSensorValue(sensors[7].slaveID, sensors[7].baud, sensors[7].regAddr, 3) / 10.0);
    // distanceRaw[i] = 40 - (getSensorValue(0x50, 115200, 0x34, 3) / 10.0);
    Serial.print("dis: ");
    Serial.println(distanceRaw[i]);
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
  comunication->begin(baudrate, SERIAL_8N1, 16, 17);
  delay(200);
  // comunication->setTimeout(2000);
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
  lastSensorValueTem = median(tempRaw, 5);
  // return emaTem.update(median(tempRaw, 5));
  return lastSensorValueTem;
}
float Sensor::getHumidity() {
  lastSensorValueHum = median(humRaw, 5);
  // return emaHum.update(median(humRaw, 5));
  return lastSensorValueHum;
}
float Sensor::getEC() {
  lastSensorValueEC = median(ECRaw, 5);
  // return emaEC.update(median(ECRaw, 5));
  return lastSensorValueEC;
}
float Sensor::getPH() {
  lastSensorValuePH = median(PHRaw, 5);
  // return emaPH.update(median(PHRaw, 5));
  return lastSensorValuePH;
}
float Sensor::getNito() {
  lastSensorValueNi = median(niRaw, 5);
  // return emaNi.update(median(niRaw, 5));
  return lastSensorValueNi;
}
float Sensor::getPhotpho() {
  lastSensorValuePhotpho = median(photphoRaw, 5);
  // return emaPhotpho.update(median(photphoRaw, 5));
  return lastSensorValuePhotpho;
}
float Sensor::getKali() {
  lastSensorValueKali = median(kaliRaw, 5);
  // return emaKali.update(median(kaliRaw, 5));
  return lastSensorValueKali;
}
float Sensor::getDistance() {
  lastSensorValuedistance = median(distanceRaw, 5);
  // return emaDistance.update(median(distanceRaw, 5));
  return lastSensorValuedistance;
}
float Sensor::getPinVoltage() {
  return ((analogRead(BatSense3V3) * 3.3) / 4095.0) * 1.27;
}
void Sensor::eepromWriteFloat(int address, float value) {
  byte* p = (byte*)(void*)&value;
  for (int i = 0; i < 4; i++) {
    EEPROM.write(address + i, p[i]);
  }
  EEPROM.commit();
}

float Sensor::eepromReadFloat(int address) {
  float value = 0;
  byte* p = (byte*)(void*)&value;
  for (int i = 0; i < 4; i++) {
    p[i] = EEPROM.read(address + i);
  }
  return value;
}
