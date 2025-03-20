#include <LoRa.h>
#include "Sensor.h"
#include "esp_sleep.h"
#include <SPI.h>

#define StationID 11
#define sleeptime 15 * 60  // Giây
#define ANALOG_PIN 26 
#define LED_blue 14
#define LED_red 27
#define LED_yellow 12
// Khởi động cảm biến
#define Sensor1 32
#define Sensor2 33
// Các chân LoRa
#define SS 5
#define RST 13
#define DIO0 2
//
soild7in1 soilSensor(16, 17);
distance myDistanceSensor(16, 17);

void setup() {
  pinMode(Sensor1, OUTPUT);
  pinMode(Sensor2, OUTPUT);
  digitalWrite(Sensor1, HIGH);
  digitalWrite(Sensor2, HIGH);
  pinMode(LED_blue, OUTPUT);
  pinMode(LED_red, OUTPUT);
  pinMode(LED_yellow, OUTPUT);
  digitalWrite(LED_red, HIGH);
  digitalWrite(LED_blue, LOW);
  digitalWrite(LED_yellow, LOW);
  delay(30000);
  Serial.begin(9600);

  LoRa.setPins(SS, RST, DIO0);

  while (!LoRa.begin(433E6)) {  // Chọn tần số LoRa (433 MHz)
    Serial.println("Khởi động LoRa thất bại!");
  }
  Serial.println("LoRa đã sẵn sàng để truyền!");

  for (int i = 0; i < 3; i++) {
    // Thu thập dữ liệu từ cảm biến

    digitalWrite(LED_yellow, HIGH);
    int batteryAnalog = analogRead(ANALOG_PIN);
    soilSensor.begin(9600);
    int humidity = soilSensor.getHumidity();
    int kali = soilSensor.getKali();
    int nito = soilSensor.getNito();
    int photpho = soilSensor.getPhotpho();
    int ph = soilSensor.getPH();
    int temperature = soilSensor.getTemperature();
    int ec = soilSensor.getEC();
    myDistanceSensor.begin(115200);
    int distance = myDistanceSensor.getDistance() / 10;

    // Tạo gói dữ liệu
    String dataPacket = String("StationID:") + StationID + ",Humidity:" + humidity + ",Kali:" + kali + ",Nito:" + nito + ",Photpho:" + photpho + ",pH:" + ph + ",Temperature:" + temperature + ",EC:" + ec + ",Distance:" + distance + ",Battery:" + batteryAnalog;

    // In gói dữ liệu ra Serial Monitor
    delay(500);
    digitalWrite(LED_yellow, LOW);
    digitalWrite(LED_blue, HIGH);
    Serial.println("Gói dữ liệu chuẩn bị gửi:");
    Serial.println(dataPacket);

    // Gửi dữ liệu qua LoRa
    LoRa.beginPacket();
    LoRa.print(dataPacket);
    LoRa.endPacket();

    // Xác nhận gói dữ liệu đã gửi
    delay(500);
    digitalWrite(LED_blue, LOW);
    Serial.println("Gói dữ liệu đã gửi thành công!");
    // Đợi trước khi gửi lần tiếp theo
    delay(500);
  }

  // Đưa ESP32 vào chế độ ngủ sau khi hoàn thành
  digitalWrite(Sensor1, LOW);
  digitalWrite(Sensor2, LOW);
  Serial.flush();
  delay(500);
  esp_sleep_enable_timer_wakeup(sleeptime * 1000000ULL);  // ngủ sau 5 giây
  esp_deep_sleep_start();
}

void loop() {
}
