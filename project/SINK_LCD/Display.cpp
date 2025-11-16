#include "Display.h"
#include <LiquidCrystal_I2C.h>
SemaphoreHandle_t xI2C_Mutex;
// portMUX_TYPE lcdMux = portMUX_INITIALIZER_UNLOCKED;
Display::Display(uint8_t address, int columns, int rows) {
  lcd = new LiquidCrystal_I2C(address, columns, rows);
}

void Display::begin() {
  xI2C_Mutex = xSemaphoreCreateMutex();
  lcd->init();
  lcd->backlight();
  lcd->clear();
  logo();
}
void Display::logo() {
  if (xSemaphoreTake(xI2C_Mutex, portMAX_DELAY) == pdTRUE) {
    lcd->setCursor(6, 1);
    lcd->print("IOT LAB");
    lcd->setCursor(6, 2);
    lcd->print("_______");
    xSemaphoreGive(xI2C_Mutex);
  }
}
void Display::print(int collum, int row, String msg) {
  lcd->setCursor(collum, row);
  lcd->print(msg);
}
void Display::print(int collum, int row, float msg) {
  lcd->setCursor(collum, row);
  lcd->print(msg, 1);
}
void Display::homePage(float Do,float tem, float Sal, float PH, float NH4, float pin) {
  if (xSemaphoreTake(xI2C_Mutex, portMAX_DELAY) == pdTRUE) {
    print(0, 0, "Do:");
    print(4, 0, Do);
    print(10, 0, "Tmp:");
    print(14, 0, tem);
    print(0, 1, "SAL:");
    print(4, 1, Sal);
    print(11, 1, "PH:");
    print(14, 1, PH);
    print(0, 2, "NH4:");
    print(4, 2, NH4);
    print(11, 2, "Pin:");
    print(15, 2, pin);
    xSemaphoreGive(xI2C_Mutex);
  }
}

void Display::shutDownDisplay() {
  if (xSemaphoreTake(xI2C_Mutex, portMAX_DELAY) == pdTRUE) {
    lcd->noDisplay();
    lcd->noBacklight();
    xSemaphoreGive(xI2C_Mutex);
  }
}
void Display::onDisplay() {
  if (xSemaphoreTake(xI2C_Mutex, portMAX_DELAY) == pdTRUE) {

    lcd->display();
    lcd->backlight();
    xSemaphoreGive(xI2C_Mutex);
  }
}
void Display::clear() {
  if (xSemaphoreTake(xI2C_Mutex, portMAX_DELAY) == pdTRUE) {
    lcd->clear();
    xSemaphoreGive(xI2C_Mutex);
  }
}