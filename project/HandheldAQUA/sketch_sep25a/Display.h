#ifndef Display_h
#define Display_h
#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include "Sensor.h"
class Number {
private:

  byte ngangtren[8] = {
    B11111,
    B11111,
    B11111,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
  };
  byte ngangduoi[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B11111,
    B11111,
  };
  byte full[8] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
  };
  byte blank[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
  };
  byte middle[8] = {
    B00000,
    B00000,
    B00000,
    B11111,
    B11111,
    B00000,
    B00000,
    B00000,
  };
  byte middleduoi[8]{
    B00000,
    B00000,
    B00000,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
  };
  byte middletren[8]{
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B00000,
    B00000,
    B00000,
  };
public:
  Number();
  void charOfNumber();
  void number0(int column, int row);
  void number1(int column, int row);
  void number2(int column, int row);
  void number3(int column, int row);
  void number4(int column, int row);
  void number5(int column, int row);
  void number6(int column, int row);
  void number7(int column, int row);
  void number8(int column, int row);
  void number9(int column, int row);
  void print(float intputNumber, int column, int row);
};
class Pointer {
private:
  byte arrow[8] = {
    B10000,
    B11000,
    B01100,
    B00110,
    B00110,
    B01100,
    B11000,
    B10000
  };
  int presentColumn = 0;
  int presentRow = 1;
  int stepColumn = 0;
  int stepRow = 0;
  int top = 4, bottom =5 , left = 2, right = 3, enter = 6, back = 7;
  unsigned int waitTime = 0;
public:
  int page = 0;
  Pointer();
  void begin(int stepColumn, int stepRow);
  void createPointer(int column, int row);
  void createPointer();
  void buttomStatusForHome();
  void buttomStatusForCalibrate();
  void buttomStatusForSelectSensor();
  void buttomFuntion();
};

class Display {
private:
public:
  Display();
  void begin();
  void home(Pointer pointer);
  void logo();
  void calibrate(Pointer pointer);
  void setting();
  void Auto();
  void stored();
  void waitting();
  void selectSensor(Pointer pointer);
};
#endif