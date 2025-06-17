#include "GPS.h"
TinyGPSPlus gps;
GPS::GPS(int RX, int TX) {
  GPSSerial = new SoftwareSerial(RX, TX);
}
bool GPS::process() {
  if (GPSSerial->available()) {
    gps.encode(GPSSerial->read());
    if (gps.location.isValid()) {
      this->LAT = gps.location.lat();
      this->LON = gps.location.lng();
    } else {
      this->LAT = 404;
      this->LON = 404;
    }
    if (gps.date.isValid()) {
      this->year = gps.date.year();
      this->month = gps.date.month();
      this->day = gps.date.day();
    } else {
      this->year = 404;
      this->month = 404;
      this->day = 404;
    }
    if (gps.time.isValid()) {
      this->hour = gps.time.hour();
      this->minute = gps.time.minute();
      this->second = gps.time.second();
    } else {
      this->hour = 404;
      this->minute = 404;
      this->second = 404;
    }
    return 1;
  } else {
    this->LAT = 404;
    this->LON = 404; 
    this->year = 404;
    this->month = 404;
    this->day = 404;
    this->hour = 404;
    this->minute = 404;
    this->second = 404;
    return 0;
  }
}
void GPS::begin() {
  GPSSerial->begin(4800);
}
double GPS::getLAT() {
  return this->LAT;
}
double GPS::getLON() {
  return this->LON;
}
int GPS::getHour() {
  return this->hour;
}
int GPS::getMinute() {
  return this->minute;
}
int GPS::getSecond() {
  return this->second;
}
int GPS::getYear() {
  return this->year;
}
int GPS::getMonth() {
  return this->month;
}
int GPS::getDay() {
  return this->day;
}