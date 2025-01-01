#include <LiquidCrystal_I2C.h>
#include "Sensor.h"
CO2 co2(9600, 2, 3);
CH4 ch4(4800, 2, 3);
LiquidCrystal_I2C lcd(0x27, 20, 4);
void load() {
  lcd.setCursor(2, 1);
  lcd.print("HANDHELD DEVICES");
  lcd.setCursor(13, 2);
  lcd.print("air");
  delay(2000);
  lcd.clear();
}
void all() {
  lcd.setCursor(0, 0);
  lcd.print("Co2:");
  lcd.setCursor(4, 0);
  lcd.print(co2.getCO2());
  lcd.setCursor(11, 0);
  lcd.print("CH4:");
  lcd.setCursor(15, 0);
  lcd.print(ch4.getCH4());
}
void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  load();
}


void loop() {
  all();
}
