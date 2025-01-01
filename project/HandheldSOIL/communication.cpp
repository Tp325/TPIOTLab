#include "communication.h"
WiFiClient espClient ;
PubSubClient client (espClient);
communication::communication() {
  wm = new WiFiManager;
}
void communication::begin() {
  client.setServer(mqtt_server, mqtt_port);
}
void communication::connectToWiFi() {
  res = wm->autoConnect("AutoConnectAP", "AutoConnectAP");
}
bool communication::isWiFiConnnected() {
  return res;
}
void communication::connectMqtt() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(500);
    }
  }
}
void communication::sendToMqtt(String msg, String topic) {
  client.publish(topic.c_str(), msg.c_str());
}