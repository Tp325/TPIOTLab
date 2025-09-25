#ifndef Sensor_h
#define Sensor_h
#include "Arduino.h"
#include <SoftwareSerial.h>
#include <ModbusMaster.h>
#include <EEPROM.h>
#define ADDR2 (0 + sizeof(uint16_t))
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
  float es;
  uint16_t lastRain;
  SoftwareSerial *comunication;
  ModbusMaster *node;
  float readVotage(int RawValue);
public:
  Sensor(int BatSense3V3,
         SoftwareSerial *comunication,
         ModbusMaster *node);
  void begin();
  float getWindSpeed();       //m/s
  String getWindDirection();  // độ
  // 0°
  // Gió bắc
  // 45°
  // Gió đông bắc
  // 90°
  // Gió đông
  // 135°
  // Gió đông nam
  // 180°
  // Gió nam
  // 225°
  // Gió tây nam
  float getAirTemperature();  //oc
  float getAirHumidity();     // %Rh
  float getAirPressure();     //KPa
  float getLight();           //lux
  float getNoise();           //db
  float getPM_2_5();          //mg/m^2
  float getPM_10();           //mg/m^2
  float getVaporPressureDeficit();

  float getRain();  // mm/phút

  float getWaterTem();  // oc
  float readBat();      //V
};
#endif