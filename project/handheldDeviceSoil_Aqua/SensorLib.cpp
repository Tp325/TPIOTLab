#include "SensorLib.h"
#include <HardwareSerial.h>
RS485Sensor ::RS485Sensor() {
}
void RS485Sensor::begin(uint32_t baud_rate) {
  Serial2.begin(baud_rate, SERIAL_8N1, 16, 17);
}
int RS485Sensor::getSensorValue(byte dataForSend[8], uint8_t sizeOfData, uint8_t possion) {
  Serial2.write(dataForSend, 8);
  vTaskDelay(100 / portTICK_PERIOD_MS);
  if (Serial2.available()) {
    Serial2.readBytes(receiveData, sizeOfData - 1);
    return receiveData[possion] << 8 | receiveData[possion + 1];
  } else {
    return 0;
  }
}
float RS485Sensor ::getHumidity() {
  return getSensorValue(_queryTemHum, 10, 3)/10.0;
}
float RS485Sensor ::getTemperature() {
  return getSensorValue(_queryTemHum, 10, 5)/10.0;
}
float RS485Sensor ::getPH() {
  return getSensorValue(_queryPH, 8, 3)/100.0;
}
float RS485Sensor ::getNito() {
  return getSensorValue(_queryN, 8, 3);
}
float RS485Sensor ::getPhotpho() {
  return getSensorValue(_queryP, 8, 3);
}
float RS485Sensor ::getKali() {
  return getSensorValue(_queryK, 8, 3);
}
float RS485Sensor ::getEC() {
  return getSensorValue(_queryEC, 8, 3);
}


AnalogSensor ::AnalogSensor() {
}
void AnalogSensor ::begin(uint32_t baud_rate) {
  Serial2.begin(baud_rate, SERIAL_8N1, 16, 17);
}
void AnalogSensor ::automationScan() {
  Serial2.println(nameOfUartSensor + String("_1"));
  vTaskDelay(100 / portTICK_PERIOD_MS);
  if (Serial2.available()) {
    RawValue = (Serial2.readStringUntil('\n')).toInt();
  } else {
    Serial2.println("SAL_1");
    sensorTimmer = millis();
    while (Serial2.available() != 1 && millis() - sensorTimmer <= 2000)
      vTaskDelay(100 / portTICK_PERIOD_MS);
    if (millis() - sensorTimmer <= 2000) {
      nameOfUartSensor = "SAL";
      RawValue = (Serial2.readStringUntil('\n')).toInt();
    }
    Serial2.println("TMP_1");
    sensorTimmer = millis();
    while (Serial2.available() != 1 && millis() - sensorTimmer <= 2000)
      vTaskDelay(100 / portTICK_PERIOD_MS);
    if (millis() - sensorTimmer <= 2000) {
      nameOfUartSensor = "TMP";
      RawValue = (Serial2.readStringUntil('\n')).toInt();
    }
    Serial2.println("NH4_1");
    sensorTimmer = millis();
    while (Serial2.available() != 1 && millis() - sensorTimmer <= 2000)
      vTaskDelay(100 / portTICK_PERIOD_MS);
    if (millis() - sensorTimmer <= 2000) {
      nameOfUartSensor = "NH4";
      RawValue = (Serial2.readStringUntil('\n')).toInt();
    }
    Serial2.println("PH_1");
    sensorTimmer = millis();
    while (Serial2.available() != 1 && millis() - sensorTimmer <= 2000)
      vTaskDelay(100 / portTICK_PERIOD_MS);
    if (millis() - sensorTimmer <= 2000) {
      nameOfUartSensor = "PH";
      RawValue = (Serial2.readStringUntil('\n')).toInt();
    }
    Serial2.println("O2_1");
    sensorTimmer = millis();
    while (Serial2.available() != 1 && millis() - sensorTimmer <= 2000)
      vTaskDelay(100 / portTICK_PERIOD_MS);
    if (millis() - sensorTimmer <= 2000) {
      nameOfUartSensor = "O2";
      RawValue = (Serial2.readStringUntil('\n')).toInt();
    }
    nameOfUartSensor = "eror";
  }
}
float AnalogSensor ::getVoltage() {
  return (RawValue * 5) / 1023.0;
}
float AnalogSensor ::getSalinity() {
  return getVoltage() * slopeSAL + interceptSAL;
}
float AnalogSensor ::getPH() {
  return getVoltage() * slopePH + interceptPH;
}
float AnalogSensor ::getNH4() {
  return getVoltage() * slopeNH4 + interceptNH4;
}
float AnalogSensor ::getTemperature() {
  measured_resistance = (resistance * getVoltage()) / (5.0 - getVoltage());
  return 1.0 / (Ko + K1 * logf(1000.0 * measured_resistance) + K2 * powf(logf(1000.0 * measured_resistance), 3)) - 273.15;
}
