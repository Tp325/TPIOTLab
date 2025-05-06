#include "communication.h"
WiFiClient espClient;
PubSubClient client(espClient);
WiFiManager wm;
communication::communication() {
}
void communication::begin() {
  WiFi.mode(WIFI_STA);
  wm.setConfigPortalBlocking(false);
  if (wm.autoConnect("HANDY SOIL")) {
    Serial.println("connected...yeey :)");
  } else {
    Serial.println("Configportal running");
  }
}
void communication::processWiFi() {
  wm.process();
  if (WiFi.status() == WL_CONNECTED) {
    if (isWifiConnect == 0) {
      client.setServer(mqtt_server, mqtt_port);
      connectMqtt();
      isWifiConnect = 1;
    }
  } else {
    isWifiConnect = 0;
    isSaveData = 0;
  }
  if (isWifiConnect == 1 && isSaveData == 0) {
    Serial.println("connected...yeey :)");
    wifiPass = wm.getWiFiPass();
    wifiUser = wm.getWiFiSSID();
    isSaveData = 1;
  }
}
void communication::connectMqtt() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("esP32", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      client.subscribe("fsddsdf");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
void communication::processMQTT() {
  if (!client.connected()) {
    connectMqtt();
  }
  client.loop();
}
void communication::sendToMqtt(String msg, String topic) {
  client.publish(topic.c_str(), msg.c_str());
}