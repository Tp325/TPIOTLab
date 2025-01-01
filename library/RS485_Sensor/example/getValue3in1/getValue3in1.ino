#include"Soild3in1.h"
Soild3in1 soilSensor(9600,16,17);
//RX pin 16 ESP32
//TX pin 17 ESP32
void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("PH:");
  Serial.println(soilSensor.getPH());
  Serial.println("Temperature:");
  Serial.println(soilSensor.getTemperature());
  Serial.println("Humidity:");
  Serial.println(soilSensor.getHumidity());
  delay(1000);
}
