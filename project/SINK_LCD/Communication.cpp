#include "Communication.h"
SPIClass spiLoRa(HSPI);
SX1278 radio = new Module(Nss, Dio0, Rst, Dio1, spiLoRa);
DynamicJsonDocument doc(1024);
DynamicJsonDocument doc2(1024);
DynamicJsonDocument doc1(1024);

WiFiManager wm;
WiFiClient espClient;
PubSubClient client(espClient);
Communication* Communication::instance = nullptr;
int state;
int trasmitState;
volatile bool receiveFlag = false;
bool isSended;
bool isWifiConnect = 0;
void setReceiveFlag() {
  //Serial.println("flag");
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
Communication::Communication(uint16_t SID, String clientmqtt,
                             String topicReceive, String topicSend,
                             float carrierFrequency, float bandwidth, uint8_t spreadingFactor, uint8_t codingRate, uint8_t syncWord, uint8_t outputPower, uint8_t preambleLength, uint8_t amplifierGain,
                             char* mqtt_server, int mqtt_port, char* mqtt_user, char* mqtt_pass) {
  Communication::instance = this;
  this->SID = SID;
  this->clientmqtt = clientmqtt;

  this->topicReceive = topicReceive;
  this->topicSend = topicSend;

  this->mqtt_server = mqtt_server;
  this->mqtt_port = mqtt_port;
  this->mqtt_user = mqtt_user;
  this->mqtt_pass = mqtt_pass;

  this->carrierFrequency = carrierFrequency;
  this->bandwidth = bandwidth;
  this->spreadingFactor = spreadingFactor;
  this->codingRate = codingRate;
  this->syncWord = syncWord;
  this->outputPower = outputPower;
  this->preambleLength = preambleLength;
  this->amplifierGain = amplifierGain;
}
void Communication::process() {
  processWiFi();
  processMQTT();
}
void Communication::blocking() {
  reconnectWifi();
  reconnectMQTT();
}
void Communication::begin() {
  spiLoRa.begin(14, 12, 13, Nss);
  state = radio.begin(carrierFrequency, bandwidth, spreadingFactor, codingRate, syncWord, outputPower, preambleLength, amplifierGain);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("success!");
  } else {
    Serial.print("failed, code ");
    Serial.println(state);
  }
  radio.setPacketReceivedAction(setReceiveFlag);
  Serial.print(F("[SX1278] Starting to listen ... "));
  state = radio.startReceive();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
  }
  WiFi.onEvent(onWiFiEvent);
  WiFi.mode(WIFI_STA);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callbackWrapper);
  timeOutStartWeb = millis();
  wm.setConfigPortalBlocking(false);
  wm.setDarkMode(true);
  isWebAPStart = 1;
  if (wm.autoConnect("Aqua Station")) {
    Serial.println("WIFI connected");
  } else {
    Serial.println("Web AP running");
  }
}
void Communication::sendToNode() {
  while (!isEmpty(buffDataToNode)) {
    vTaskDelay(1500 / portTICK_PERIOD_MS);
    msgToNode = dequeue(buffDataToNode);
    isSended = 1;
    trasmitState = radio.transmit(msgToNode);
    if (trasmitState == RADIOLIB_ERR_NONE) {
      Serial.print("send to node: ");
      Serial.println(msgToNode);
      Serial.println("transmission finished!");
    } else {
      Serial.print("failed, code ");
      Serial.println(state);
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
    state = radio.startReceive();
  }
}
void Communication::sendToNode(String msg) {
  if (!isFull(buffDataToNode)) {
    enqueueData(buffDataToNode, msg.c_str());
  }
}


void Communication::receiveFromNode() {
  if (receiveFlag) {
    receiveFlag = false;
    state = radio.readData(msgFromNode);
    if (state == RADIOLIB_ERR_NONE) {
      doc.clear();
      deserializeJson(doc, msgFromNode);
      if (doc.containsKey("SID")) {
        if (doc["SID"].as<uint16_t>() == this->SID) {
          if (!isFull(buffDataFromNode)) {
            Serial.print("receive From Node: ");
            Serial.println(msgFromNode);
            enqueueData(buffDataFromNode, msgFromNode.c_str());
          }
          Nodebuffer = manager.findNode(doc["ID"].as<uint16_t>());
          if (Nodebuffer == nullptr) {
            manager.addNode(doc["ID"].as<uint16_t>());
            manager.saveToEEPROM();
            manager.printNodes();
          } else {
            if (doc["ID"].as<uint16_t>() < 767 && doc["ID"].as<uint16_t>() >= 512) {
              Do = doc["Do"].as<float>();
              tem = doc["Tem"].as<float>();
              Sal = doc["Sa"].as<float>();
              PH = doc["PH"].as<float>();
              NH4 = doc["NH4"].as<float>();
              pin = doc["Pin"].as<float>();
            }
          }
          manager.updateNode(doc["ID"].as<uint16_t>());
          sendToNode(String(String("{\"SS\":0,\"SID\":") + String(SID) + String(",\"ID\":") + doc["ID"].as<String>() + String(",\"CM\":\"ACK\"}")));
        }
      } else if (doc.containsKey("CM")) {
        if (doc["CM"].as<String>() == "WTJN") {
          Serial.print("receive From Node: ");
          Serial.println(msgFromNode);
          Nodebuffer = manager.findNode(doc["ID"].as<uint16_t>());
          if (Nodebuffer == nullptr) {
            manager.addNode(doc["ID"].as<uint16_t>());
            manager.saveToEEPROM();
            manager.printNodes();
          }
          sendToNode(String(String("{\"SS\":0,\"SID\":") + String(SID) + String(",\"ID\":") + doc["ID"].as<String>() + String(",\"CM\":\"ACK\"}")));
        }
      }
    }
    msgFromNode = "";
  }
}
void Communication::sendToServer() {
  while (!isEmpty(buffDataToServer)) {
    msgToServer = "";
    Serial.print("send to server: ");
    msgToServer = dequeue(buffDataToServer);
    Serial.println(msgToServer);
    client.publish(topicSend.c_str(), msgToServer.c_str());
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}
void Communication::sendToServer(String msg) {
  if (!isFull(buffDataToServer)) {
    enqueueData(buffDataToServer, msg.c_str());
  }
}
void Communication::receiveFromServer() {
  if (msgFromServer != "") {
    if (!isFull(buffDataFromServer)) {
      enqueueData(buffDataFromServer, msgFromServer.c_str());
    }
    if (!isFull(buffDataToServer)) {
      enqueueData(buffDataToServer, msgFromServer.c_str());
    }
    Serial.print("receive From Server: ");
    Serial.println(msgFromServer);
    msgFromServer = "";
  }
}
void Communication::analizeData() {
  while (!isEmpty(buffDataFromServer)) {
    buffMsgFromServer = dequeue(buffDataFromServer);
    deserializeJson(doc1, buffMsgFromServer);
    if (doc1["SID"].as<uint16_t>() == this->SID) {
      if (doc1["CM"].as<String>() == "IS") {
        mode = "installation";
      } else if (doc1["CM"].as<String>() == "NIS") {
        mode = "normal";
      }
    }
    Nodebuffer = manager.findNode(doc1["ID"].as<uint16_t>());
    if (doc1["SID"].as<uint16_t>() == this->SID) {
      if (doc1["CM"].as<String>() == "CH") {
        if (Nodebuffer != nullptr)
          Nodebuffer->checkHealth = 1;
      } else if (doc1["CM"].as<String>() == "RS") {
        if (doc1["ID"].as<int>() == 0) {
          haveToReset = 1;
        } else {
          if (Nodebuffer != nullptr)
            Nodebuffer->reset = 1;
        }
      } else if (doc1["CM"].as<String>() == "GD") {
        if (doc1["ID"].as<int>() == 0) {
          ////
        } else {
          if (Nodebuffer != nullptr)
            Nodebuffer->getData = 1;
        }
      } else if (doc1["CM"].as<String>() == "CLW") {
        if (doc1["ID"].as<int>() == 0) {
          wm.resetSettings();
          haveToReset = 1;
        }
      } else if (doc1["CM"].as<String>() == "SCN") {
        if (doc1["ID"].as<int>() == 0) {
          for (auto& n : manager.nodes) {
            sendToServer(String(String("{\"SS\":0,\"SID\":\"") + String(SID) + String("\",\"ID\":") + String(n.ID) + String(",\"CM\":\"SCN\"") + String(",\"LTS\":") + String((millis() - n.lastTimeSentData) / (1000.0 * 60)) + String("}")));
          }
        }
      } else if (doc1["CM"].as<String>() == "CLN") {
        if (doc1["ID"].as<int>() == 0) {
          manager.clearEEPROM();
        }
      }
    }

    vTaskDelay(50 / portTICK_PERIOD_MS);
  }

  while (!isEmpty(buffDataFromNode)) {
    buffMsgFromNode = dequeue(buffDataFromNode);
    deserializeJson(doc2, buffMsgFromNode);
    Nodebuffer = manager.findNode(doc2["ID"].as<uint16_t>());
    if (Nodebuffer != nullptr) {
      if (Nodebuffer->checkHealth == 1) {
        sendToNode(String(String("{\"SS\":1,\"SID\":\"") + String(SID) + String("\",\"ID\":") + String(Nodebuffer->ID) + String(",\"CM\":\"CH\"}")));
        Nodebuffer->checkHealth = 0;
      }
      if (Nodebuffer->reset == 1) {
        sendToNode(String(String("{\"SS\":1,\"SID\":\"") + String(SID) + String("\",\"ID\":") + String(Nodebuffer->ID) + String(",\"CM\":\"RS\"}")));
        Nodebuffer->reset = 0;
      }
      if (Nodebuffer->getData == 1) {
        sendToNode(String(String("{\"SS\":1,\"SID\":\"") + String(SID) + String("\",\"ID\":") + String(Nodebuffer->ID) + String(",\"CM\":\"GD\"}")));
        Nodebuffer->getData = 0;
      }
    } else {
      manager.addNode(doc2["ID"].as<uint16_t>());
      manager.saveToEEPROM();
      manager.printNodes();
    }
    if (!isFull(buffDataToServer)) {
      enqueueData(buffDataToServer, buffMsgFromNode.c_str());
    }
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}
void Communication::processWiFi() {
  if (millis() - timeOutStartWeb >= 1000 * 60 * 5 && isWebAPStart == 1) {
    wm.stopConfigPortal();
    isWebAPStart = 0;
  }
  if (isWebAPStart == 1) {
    wm.process();
  }
  if (isWifiConnect == 1 || !wm.getConfigPortalActive()) {
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
    if (client.connect(clientmqtt.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      client.subscribe(topicReceive.c_str());
    }
  } else {
    Serial.println("reconnect MQTT...");
  }
}

void Communication::callbackmqtt(char* topic, byte* message, unsigned int length) {
  msgFromServer = "";
  for (int i = 0; i < length; i++) {
    msgFromServer += (char)message[i];
  }
}