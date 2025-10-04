#ifndef Sensor_h
#define Sensor_h
#include <EEPROM.h>
#include "Arduino.h"
#include <SoftwareSerial.h>
#include <ModbusMaster.h>
#include <SimpleKalmanFilter.h>
#include <algorithm>

struct SensorCfg {
  uint8_t slaveID;
  uint16_t regAddr;
  uint32_t baud;
  const char *name;
};
class EMAFilter {
private:
  float alpha;
  float estimate;
  bool initialized;
public:
  EMAFilter(float a)
    : alpha(a), estimate(0), initialized(false) {}
  float update(float value) {
    if (!initialized) {
      estimate = value;
      initialized = true;
    } else {
      estimate = alpha * value + (1 - alpha) * estimate;
    }
    return estimate;
  }
};
class Sensor {
private:
  int getSensorValue(uint8_t slaveID, uint32_t baudrate, uint16_t reg, uint8_t reTries);
  int BatSense3V3;
  HardwareSerial *comunication;
  ModbusMaster *node;
  EMAFilter emaTem = EMAFilter(0.3);
  EMAFilter emaHum = EMAFilter(0.3);
  EMAFilter emaEC = EMAFilter(0.3);
  EMAFilter emaPH = EMAFilter(0.3);
  EMAFilter emaNi = EMAFilter(0.3);
  EMAFilter emaPhotpho = EMAFilter(0.3);
  EMAFilter emaKali = EMAFilter(0.3);
  EMAFilter emaDistance = EMAFilter(0.3);
  float tempRaw[5];
  float humRaw[5];
  float ECRaw[5];
  float PHRaw[5];
  float niRaw[5];
  float photphoRaw[5];
  float kaliRaw[5];
  float distanceRaw[5];
  float median(float numbers[], int size);
  float readVotage(int RawValue);
  void eepromWriteFloat(int address, float value);
  float eepromReadFloat(int address);
public:
  Sensor(int BatSense3V3,
         HardwareSerial *comunication,
         ModbusMaster *node);
  void begin();
  void getValueOfSensor();
  float getTemperature();  // oC
  float getHumidity();     //%
  float getPH();           // H+
  float getNito();         // mg/kg
  float getPhotpho();      // mg/kg
  float getKali();         // mg/kg
  float getEC();           // us/cm
  float getDistance();     // cm
  float getPinVoltage();   // V
};


#endif