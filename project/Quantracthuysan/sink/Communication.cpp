#include "Communication.h"
SPIClass spiLoRa(HSPI);
SX1278 radio = new Module(Nss, Dio0, Rst, Dio1, spiLoRa);
DynamicJsonDocument doc(1024);
WiFiManager wm;
WiFiClient espClient;
PubSubClient client(espClient);
Communication* Communication::instance = nullptr;
int state;
int trasmitState;
volatile bool receiveFlag = false;
void setReceiveFlag() {
  // Serial.println("flag");
  receiveFlag = true;
  if (isSended == 1) {
    receiveFlag = false;
    isSended = 0;
  }
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
Communication::Communication() {
}
void Communication::begin() {
  // spiLoRa.begin(14, 12, 13, Nss);
  // Serial.println("[SX1278] Initializing ... ");
  // state = radio.begin(carrierFrequency, bandwidth, spreadingFactor, codingRate, syncWord, outputPower, preambleLength, amplifierGain);
  // // state = radio.begin();
  // if (state == RADIOLIB_ERR_NONE) {
  //   Serial.println("success!");
  // } else {
  //   Serial.print("failed, code ");
  //   Serial.println(state);
  //   while (true) { vTaskDelay(100 / portTICK_PERIOD_MS); }
  // }
  // radio.setPacketReceivedAction(setReceiveFlag);
  // Serial.print(F("[SX1278] Starting to listen ... "));
  // state = radio.startReceive();
  // if (state == RADIOLIB_ERR_NONE) {
  //   Serial.println(F("success!"));
  // } else {
  //   Serial.print(F("failed, code "));
  //   Serial.println(state);
  //   while (true) { delay(10); }
  // }
  LoRa.setPins(Nss, Rst, Dio0);
  while (!LoRa.begin(433E6)) {  // Chọn tần số LoRa (433 MHz)
    Serial.println("Khởi động LoRa thất bại!");
  }
  Serial.println("LoRa đã sẵn sàng để truyền!");
  WiFi.onEvent(onWiFiEvent);
  WiFi.mode(WIFI_STA);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callbackWrapper);
  wm.setConfigPortalBlocking(false);
  wm.setDarkMode(true);
  isWebAPStart = 1;
  if (wm.autoConnect("CTUTestDemo")) {
    Serial.println("WIFI connected");
  } else {
    Serial.println("Web AP running");
  }
}

void Communication::sendToNode() {
  while (!isEmpty(buffDataFromServer)) {
    msgToNode = dequeue(buffDataFromServer);
    // isSended = 1;
    // trasmitState = radio.transmit(msgToNode);
    // if (trasmitState == RADIOLIB_ERR_NONE) {
    //   Serial.print("send: ");
    //   Serial.println(msgToNode);
    //   Serial.println("transmission finished!");
    // } else {
    //   Serial.print("failed, code ");
    //   Serial.println(state);
    // }
    LoRa.beginPacket();
    LoRa.print(msgToServer);
    LoRa.endPacket();
    vTaskDelay(100 / portTICK_PERIOD_MS);
    // state = radio.startReceive();
  }
}

void Communication::receiveFromNode() {
  // if (receiveFlag) {
  //   receiveFlag = false;
  //   state = radio.readData(msgFromNode);
  //   if (state == RADIOLIB_ERR_NONE) {
  packetSize = LoRa.parsePacket();
  if (packetSize) {
    msgFromNode = "";
    for (int i = 0; i < packetSize; i++) {
      msgFromNode += (char)LoRa.read();
    }
    if (!isFull(buffDataFromNode)) {
      Serial.print("receive: ");
      Serial.println(msgFromNode);
      enqueueData(buffDataFromNode, msgFromNode.c_str());
    }
  }
  // }
  // }
}
void Communication::sendToSever() {
  while (!isEmpty(buffDataFromNode)) {
    msgToServer = dequeue(buffDataFromNode);
    client.publish(topicSend.c_str(), msgToServer.c_str());
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}
void Communication::processWiFi() {
  wm.process();
  if (isWifiConnect == 1) {
    isWebAPStart = 0;
  }
}
void Communication::reconnectWifi() {
  if (millis() - timeOutReconnectWiFi > 20000 && isWifiConnect == 0) {
    timeOutReconnectWiFi = millis();
    if (isWebAPStart == 0) {
      Serial.print("wait");
      WiFi.disconnect();
      WiFi.reconnect();
    }
  }
}
void Communication::processMQTT() {
  if (isWifiConnect == 1 && client.connected()) {
    client.loop();
  }
}
void Communication::reconnectMQTT() {
  if (isWifiConnect == 1 && !client.connected()) {
    connectMqttWithTimeOut();
  }
}
void Communication::connectMqttWithTimeOut() {
  if (millis() - timeOutReconnectMQTT > 2000) {
    timeOutReconnectMQTT = millis();
    Serial.println("Attempting MQTT connection...");
    if (client.connect("dsafsd", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      client.subscribe(topicReceive.c_str());
    }
  } else {
    Serial.println("reconnect MQTT...");
  }
}

void Communication::callbackmqtt(char* topic, byte* message, unsigned int length) {
  for (int i = 0; i < length; i++) {
    msg += (char)message[i];
  }
  if (!isFull(buffDataFromServer)) {
    Serial.println(msg);
    enqueueData(buffDataFromServer, msg.c_str());
  }
  msg = "";
}