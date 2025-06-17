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
  return getSensorValue(_queryTemHum, 10, 3) / 10.0;
}
float RS485Sensor ::getTemperature() {
  return getSensorValue(_queryTemHum, 10, 5) / 10.0;
}
float RS485Sensor ::getPH() {
  return getSensorValue(_queryPH, 8, 3) / 100.0;
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
void AnalogSensor ::autoScan() {
  Serial2.println(nameOfUartSensor + String("_1"));
  // Serial.println(nameOfUartSensor);
  vTaskDelay(200 / portTICK_PERIOD_MS);
  if (Serial2.available()) {
    RawValue = (Serial2.readStringUntil('\n')).toInt();
    Serial.println("ok");
  } else {
    nameOfUartSensor = "404";
    Serial2.println("SAL_1");
    vTaskDelay(200 / portTICK_PERIOD_MS);
    if (Serial2.available()) {
      nameOfUartSensor = "SAL";
      RawValue = (Serial2.readStringUntil('\n')).toInt();
      Serial.println("ok");
    }
    Serial2.println("TMP_1");
    vTaskDelay(200 / portTICK_PERIOD_MS);
    if (Serial2.available()) {
      nameOfUartSensor = "TMP";
      RawValue = (Serial2.readStringUntil('\n')).toInt();
    }
    Serial2.println("NH4_1");
    vTaskDelay(200 / portTICK_PERIOD_MS);
    if (Serial2.available()) {
      nameOfUartSensor = "NH4";
      RawValue = (Serial2.readStringUntil('\n')).toInt();
    }
    Serial2.println("PH_1");
    vTaskDelay(200 / portTICK_PERIOD_MS);
    if (Serial2.available()) {
      nameOfUartSensor = "PH";
      RawValue = (Serial2.readStringUntil('\n')).toInt();
    }
    Serial2.println("O2_1");
    vTaskDelay(200 / portTICK_PERIOD_MS);
    if (Serial2.available()) {
      nameOfUartSensor = "O2";
      RawValue = (Serial2.readStringUntil('\n')).toInt();
    }
    if (nameOfUartSensor == "404") {
      RawValue = 0;
    }
  }
}
float AnalogSensor ::getVoltage() {
  Serial.println((RawValue * 5) / 1023.0);
  return (RawValue * 5) / 1023.0;
}
float AnalogSensor ::getSalinity() {
  if (getVoltage() * slopeSAL + interceptSAL <= 0)
    return 0;
  else
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
