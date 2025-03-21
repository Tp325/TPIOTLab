#include <WiFiManager.h>
#include <strings_en.h>
#include <wm_consts_en.h>
#include <wm_strings_en.h>
#include <wm_strings_es.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <LoRa.h>


// Thông tin MQTT Broker
const char* mqtt_server = "103.221.220.183";
const int mqtt_port = 1883;
const char* mqtt_user = "api1@Iotlab";
const char* mqtt_pass = "Iotlab@2023";

// Các chân LoRa
#define SS 5
// #define RST 4
#define RST 13
#define DIO0 2

// Tạo client WiFi và MQTT
WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setup() {
  Serial.begin(9600);
  // Khởi động LoRa
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(433E6)) {  // Tần số 433 MHz
    Serial.println("Khởi động LoRa thất bại!");
    while (1)
      ;
  }
  Serial.println("LoRa đã sẵn sàng để nhận dữ liệu!");

  // Kết nối WiFi
  setupWiFi();
  // Kết nối MQTT
  mqttClient.setServer(mqtt_server, mqtt_port);
  connectMQTT();
}

void loop() {
  if (!mqttClient.connected()) {
    connectMQTT();
  }
  mqttClient.loop();

  // Kiểm tra xem có gói dữ liệu nào từ LoRa
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String receivedData = "";
    while (LoRa.available()) {
      receivedData += (char)LoRa.read();
    }

    Serial.print("Dữ liệu nhận được từ LoRa: ");
    Serial.println(receivedData);

    // Gửi dữ liệu lên MQTT
    if (mqttClient.publish("Soctrangricefield", receivedData.c_str())) {
      Serial.println("Dữ liệu đã được gửi lên MQTT thành công!");
    } else {
      Serial.println("Gửi dữ liệu lên MQTT thất bại!");
    }
  }
}

// Hàm kết nối Wi-Fi
void setupWiFi() {
  Serial.print("Đang kết nối với WiFi...");
  WiFiManager wm;
  bool res;
  res = wm.autoConnect("AutoConnectAP", "AutoConnectAP");
  if (!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  } else {
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
  }
}

// Hàm kết nối MQTT
void connectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Đang kết nối với MQTT Broker...");
    if (mqttClient.connect("ESP3413241", mqtt_user, mqtt_pass)) {
      Serial.println(" Đã kết nối với MQTT Broker!");
    } else {
      Serial.print(" Thất bại với mã lỗi: ");
      Serial.print(mqttClient.state());
      delay(2000);
    }
  }
}
