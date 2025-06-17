#include "communication.h"
WiFiClient espClient;
PubSubClient client(espClient);
WiFiManager wm;
communication::communication() {
}
void onWiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.println("[WiFiEvent] Connected to WiFi!");
      isWifiConnect = 1;
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("[WiFiEvent] WiFi lost, reconnecting");
      isWifiConnect = 0;
      break;
    default:
      break;
  }
}
void communication::begin() {
  WiFi.onEvent(onWiFiEvent);
  WiFi.mode(WIFI_STA);
  client.setServer(mqtt_server, mqtt_port);
  wm.setConfigPortalBlocking(false);
  wm.setDarkMode(true);
  isWebAPStart = 1;
  if (wm.autoConnect("HANDY DEVICE")) {
    Serial.println("WIFI connected");
  } else {
    Serial.println("Web AP running");
  }
}
void communication::processWiFi() {
  wm.process();
  if (isWifiConnect == 1) {
    if (isWifiConnect == 0) {
      isWebAPStart = 0;
    }
  } else {
    isSaveData = 0;
  }
  if (isWifiConnect == 1 && isSaveData == 0) {
    wifiPass = wm.getWiFiPass();
    wifiUser = wm.getWiFiSSID();
    isSaveData = 1;
  }
}
void communication::reconnectWifi() {
  if (millis() - timeOutReconnectWiFi > 20000 && isWifiConnect == 0) {
    timeOutReconnectWiFi = millis();
    if (isWebAPStart == 0) {
      Serial.print("....");
      WiFi.disconnect();
      WiFi.reconnect();
    }
  }
}
void communication::processMQTT() {
  if (isWifiConnect == 1 && client.connected()) {
    client.loop();
  }
}
void communication::reconnectMQTT() {
  if (isWifiConnect == 1 && !client.connected()) {
    connectMqttWithTimeOut();
  }
}
void communication::connectMqttWithTimeOut() {
  if (millis() - timeOutReconnectMQTT > 2000) {
    timeOutReconnectMQTT = millis();
    Serial.println("Attempting MQTT connection...");
    // if (client.connect("HANDHELDDEVICE", mqtt_user, mqtt_pass)) {
    if (client.connect("HANDHELDDEVICE1", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      client.subscribe(topicReceive.c_str());
    }
  } else {
    Serial.println("reconnect MQTT...");
  }
}

void communication::sendToServer(String msg) {
  if (!isFull(buffDataToServer)) {
    enqueueData(buffDataToServer, msg.c_str());
  }
}
void communication::sendToServer() {
  while (!isEmpty(buffDataToServer)) {
    msgToServer = "";
    // Serial.print("send to server: ");
    msgToServer = dequeue(buffDataToServer);
    // Serial.println(msgToServer);
    client.publish(topicSend.c_str(), msgToServer.c_str());
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}