#include "Soild7in1.h"
//RX pin 16 ESP32
//TX pin 17 ESP32
Soild7in1 soilSensor(9600,16,17);
void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("Humidity:");
  Serial.println(soilSensor.getHumidity());
  Serial.println("Kali:");
  Serial.println(soilSensor.getKali());
  Serial.println("Nito:");
  Serial.println(soilSensor.getNito());
  Serial.println("Photpho:");
  Serial.println(soilSensor.getPhotpho());
  Serial.println("Ph:");
  Serial.println(soilSensor.getPH());
  Serial.println("Temperature:");
  Serial.println(soilSensor.getTemperature());
  delay(1000);
}
