#include <LiquidCrystal_I2C.h>
#include "URM08.h"
LiquidCrystal_I2C lcd(0x27, 16, 4);
URM08 sensor(2, 3);
void setup() {
  lcd.init();
  lcd.backlight();
  sensor.begin(19200);
  Serial.begin(9600);
  delay(30000);
}

void loop() {
  Serial.print("Distance:  ");
  Serial.println(sensor.getDistance());
  lcd.setCursor(0, 0);
  lcd.print("Distance:");
  lcd.setCursor(10, 0);
  lcd.print("      ");
  lcd.setCursor(10, 0);
  lcd.print(sensor.getDistance());
  delay(500);
  Serial.print("Temperature:  ");
  Serial.println(sensor.getTemperature());
  lcd.setCursor(0, 1);
  lcd.print("Temperature:");
  lcd.setCursor(13, 1);
  lcd.print("       ");
  lcd.setCursor(13, 1);
  lcd.print(sensor.getTemperature());
  delay(500);
}
