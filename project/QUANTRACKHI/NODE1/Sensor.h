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
  float es;
  uint16_t lastRain;
  HardwareSerial *comunication;
  ModbusMaster *node;
  EMAFilter emaWindS = EMAFilter(0.3);
  EMAFilter emaWindD = EMAFilter(0.3);
  EMAFilter emaAirTem = EMAFilter(0.3);
  EMAFilter emaAirHum = EMAFilter(0.3);
  EMAFilter emaAirPressure = EMAFilter(0.3);
  EMAFilter emaRain = EMAFilter(0.3);
  EMAFilter emaWaterTem = EMAFilter(0.3);
  EMAFilter emaVaporPressureDeficit = EMAFilter(0.3);
  float windSRaw[5];
  float windDRaw[5];
  float airTemRaw[5];
  float airHumRaw[5];
  float airPressureRaw[5];
  float rainRaw[5];
  float waterTemRaw[5];
  float getVaporPressureDeficitRaw[5];
  float median(float numbers[], int size);
  float readVotage(int RawValue);
public:
  Sensor(int BatSense3V3,
         HardwareSerial *comunication,
         ModbusMaster *node);
  void begin();
  void getValueOfSensor();
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