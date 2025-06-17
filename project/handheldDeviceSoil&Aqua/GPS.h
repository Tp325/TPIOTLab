#ifndef GPS_h
#define GPS_h
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
class GPS {
private:
  SoftwareSerial *GPSSerial;
  double LAT, LON;
  int day, month, year;
  int hour, minute, second;
public:
  GPS(int RX, int TX);
  bool process();
  void begin();
  double getLAT();
  double getLON();
  int getHour();
  int getMinute();
  int getSecond();
  int getYear();
  int getMonth();
  int getDay();
};
#endif