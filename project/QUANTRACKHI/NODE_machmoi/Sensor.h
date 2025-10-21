#ifndef Sensor_h
#define Sensor_h
#include "Arduino.h"
#include <SoftwareSerial.h>
#include <ModbusMaster.h>
struct SensorCfg {
  uint8_t slaveID;
  uint16_t regAddr;
  uint32_t baud;
  const char *name;
};
class Sensor {
private:
  int getSensorValue(uint8_t slaveID, uint32_t baudrate, uint16_t reg, uint8_t reTries);
  int BatSense3V3;
  int BatSense12V;
  float es;
  SoftwareSerial *comunication;
  ModbusMaster *node;
  float readVotage(int RawValue);
public:
  Sensor(int BatSense3V3, int BatSense12V,
         SoftwareSerial *comunication,
           ModbusMaster *node);
  void begin();
  float getWindSpeed();
  float getWindDirection();

  float getAirTemperature();
  float getAirHumidity();
  float getAirPressure();
  float getLight();
  float getNoise();
  float getPM_2_5();
  float getPM_10();
  float getVaporPressureDeficit();

  float getRain();

  float getWaterTem();
  float readBat();
};
#endif