#include <WiFiManager.h>
#include <strings_en.h>
#include <wm_consts_en.h>
#include <wm_strings_en.h>
#include <wm_strings_es.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <LoRa.h>
// Thông tin MQTT Broker
const char* mqtt_server = "dev.iotlab.net.vn";
const int mqtt_port = 1883;
const char* mqtt_user = "api1@Iotlab";
const char* mqtt_pass = "Iotlab@2023";

// Các chân LoRa
#define SS 5
#define RST 4
// #define RST 13
#define DIO0 2

bool isWifiConnect = 0;
long int timeOutReconnectWiFi = 0;
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
  reconnectWifi();
  if (isWifiConnect == 1) {
    connectMQTT();
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
      if (mqttClient.publish("ThoaiSonOrganic", receivedData.c_str())) {
        Serial.println("Dữ liệu đã được gửi lên MQTT thành công!");
      } else {
        Serial.println("Gửi dữ liệu lên MQTT thất bại!");
      }
    }
  }
}

// Hàm kết nối Wi-Fi
void setupWiFi() {
  WiFi.onEvent(onWiFiEvent);
  WiFi.mode(WIFI_STA);
  Serial.print("Đang kết nối với WiFi...");
  WiFiManager wm;
  bool res;
  res = wm.autoConnect("QuanTracLua");
}

// Hàm kết nối MQTT
void connectMQTT() {
  if (!mqttClient.connected()) {
    Serial.println("Đang kết nối với MQTT Broker...");
    if (mqttClient.connect("ESP3re241", mqtt_user, mqtt_pass)) {
      Serial.println(" Đã kết nối với MQTT Broker!");
    } else {
      Serial.print(" Thất bại với mã lỗi: ");
      Serial.println(mqttClient.state());
      delay(2000);
    }
  }
}
void reconnectWifi() {
  if (millis() - timeOutReconnectWiFi > 20000 && isWifiConnect == 0) {
    timeOutReconnectWiFi = millis();
    WiFi.disconnect();
    WiFi.reconnect();
  }
}
void onWiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.println("[WiFiEvent] Connected to WiFi!");
      isWifiConnect = 1;
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("[WiFiEvent] WiFi lost, reconnecting...");
      isWifiConnect = 0;
      break;
    default:
      break;
  }
}