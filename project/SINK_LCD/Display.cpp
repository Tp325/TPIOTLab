#include "Display.h"
#include <LiquidCrystal_I2C.h>

Display::Display(uint8_t address, int columns, int rows) {
  lcd = new LiquidCrystal_I2C(address, columns, rows);
}

void Display::begin() {
  lcd->init();
  lcd->backlight();
  lcd->clear();
  logo();
}
void Display::logo() {
  lcd->setCursor(5, 2);
  lcd->print("IOT LAB");
  lcd->clear();
}
void Display::print(int collum, int row, String msg) {
  lcd->setCursor(collum, row);
  lcd->print(msg);
}
void Display::print(int collum, int row, float msg) {
  lcd->setCursor(collum, row);
  lcd->print(String(msg));
}
void Display::homePage(float tem, float Sal, float PH, float NH4, float pin) {
  // lcd->clear();
  print(0, 0, "VALUE");
  print(11, 0, "Tmp");
  print(14, 0, tem);
  print(0, 1, "SAL:");
  print(4, 1, Sal);
  print(11, 1, "PH:");
  print(14, 1, PH);
  print(0, 2, "NH4:");
  print(4, 2, NH4);
  print(11, 2, "Pin:");
  print(14, 2, pin);
}

void Display::shutDownDisplay() {
  lcd->noDisplay();
  lcd->noBacklight();
}
void Display::onDisplay() {
  lcd->display();
  lcd->backlight();
}
void Display::clear() {
  lcd->clear();
}