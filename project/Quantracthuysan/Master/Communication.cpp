#include "Communication.h"
DynamicJsonDocument doc(1024);
// SX1278 radio = new Module(Nss, Dio0, Rst, Dio1);
SoftwareSerial mySerial(RXPin, TXPin);
// WiFiManager wm;
// WiFiClient espClient;
// PubSubClient client(espClient);
// Communication* Communication::instance = nullptr;
// int state;
// int trasmitState = -1;
// volatile bool receiveFlag = false;
// void setReceiveFlag() {
//   // Serial.println("flag");
//   receiveFlag = true;
//   if (isSended == 1) {
//     receiveFlag = false;
//     isSended = 0;
//   }
// }
Communication::Communication() {
}
void Communication::begin() {
  mySerial.begin(9600);
  LoRa.setPins(SS, RST, DIO0);
  while (!LoRa.begin(433E6)) {  // Chọn tần số LoRa (433 MHz)
    Serial.println("Khởi động LoRa thất bại!");
  }
  Serial.println("LoRa đã sẵn sàng để truyền!");
  // Serial.println("[SX1278] Initializing ... ");
  // state = radio.begin(carrierFrequency, bandwidth, spreadingFactor, codingRate, syncWord, outputPower, preambleLength, amplifierGain);
  // if (state == RADIOLIB_ERR_NONE) {
  //   Serial.println("success!");
  // } else {
  //   Serial.print("failed, code ");
  //   Serial.println(state);
  //   while (true) { delay(100); }
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
  // }  WiFi.mode(WIFI_STA);
  // wm.setConfigPortalBlocking(false);
  // if (wm.autoConnect("Aquastation")) {
  //   Serial.println("connected...yeey :)");
  // } else {
  //   Serial.println("Configportal running");
  // }
  // bool res;
  // res = wm.autoConnect("Aquastation");
  // if (!res) {
  //   Serial.println("Failed to connect");
  //   // ESP.restart();
  // } else {
  //   //if you get here you have connected to the WiFi
  //   Serial.println("connected...yeey :)");
  // }
  // client.setServer(mqtt_server, mqtt_port);
}
bool Communication::receiveFromNode() {
  if (mySerial.available()) {
    msgFromNode = mySerial.readStringUntil('\n');
    Serial.print("receive from node: ");
    Serial.println(msgFromNode);
    if (!isFull(buffDataFromNode)) {
      enqueueData(buffDataFromNode, msgFromNode.c_str());
    }
    return 1;
  } else {
    return 0;
  }
}
void Communication::sendToNode(String msg) {
  if (!isFull(buffDataToNode)) {
    enqueueData(buffDataToNode, msg.c_str());
  }
}
void Communication::sendToNode() {
  while (!isEmpty(buffDataToNode)) {
    Serial.print("send to node: ");
    msgToNode = dequeue(buffDataToNode);
    Serial.println(msgToNode);
    mySerial.println(msgToNode);
    delay(200 / portTICK_PERIOD_MS);
  }
}

void Communication::sendToServer() {
  while (!isEmpty(buffDataToServer)) {
    Serial.print("send to server: ");
    msgToServer = dequeue(buffDataToServer);
    Serial.println(msgToServer);
    LoRa.beginPacket();
    LoRa.print(msgToServer);
    LoRa.endPacket();
    // Serial.println(msgToServer);
    // client.publish(topicSend.c_str(), msgToServer.c_str());
    delay(20 / portTICK_PERIOD_MS);
  }
}
void Communication::receiveFromServer() {
  if (msgFromServer != "") {
    if (!isFull(buffDataFromServer)) {
      enqueueData(buffDataFromServer, msgFromServer.c_str());
    }
    Serial.print("receive From Server: ");
    Serial.println(msgFromServer);
    msgFromServer = "";
  }
}
void Communication::processWiFi() {
  // wm.process();
  // if (WiFi.status() == WL_CONNECTED) {
  //   if (isWifiConnect == 0) {
  //     client.setServer(mqtt_server, mqtt_port);
  //     client.setCallback(callbackWrapper);
  //     connectMqtt();
  //     isWifiConnect = 1;
  //   }
  // } else {
  //   isWifiConnect = 0;
  // }
  // }
  // void Communication::connectMqtt() {
  //   if (!client.connected()) {
  //     Serial.print("Attempting MQTT connection...");
  //     if (client.connect("AquastationDemoHonDatKienGiang", mqtt_user, mqtt_pass)) {
  //       Serial.println("connected");
  //       client.subscribe(topicReceive.c_str());
  //     } else {
  //       Serial.print("failed, rc=");
  //       Serial.print(client.state());
  //     }
  //     delay(200 / portTICK_PERIOD_MS);
  //   }
}
void Communication::processMQTT() {
  // if (!client.connected()) {
  //   connectMqtt();
  // }
  // client.loop();
}
void Communication::callbackmqtt(char* topic, byte* message, unsigned int length) {
  // msgFromServer = "";
  // for (int i = 0; i < length; i++) {
  //   msgFromServer += (char)message[i];
  // }
}
// void Communication::receiveFromSink() {
//   if (receiveFlag) {
//     receiveFlag = false;
//     state = radio.readData(msgFromSink);
//     if (state == RADIOLIB_ERR_NONE) {
//       if (!isFull(buffDataFromSink)) {
//         Serial.print("receive: ");
//         Serial.println(msgFromSink);
//         enqueueData(buffDataFromSink, msgFromSink.c_str());
//       }
//     }
//     delay(30 / portTICK_PERIOD_MS);
//   }
// }
// void Communication::sendToSink(String msg) {
//   if (!isFull(buffDataToSink)) {
//     enqueueData(buffDataToSink, msg.c_str());
//   }
// }
// void Communication::sendToSink() {
//   while (!isEmpty(buffDataToSink)) {
//     // delay(30 / portTICK_PERIOD_MS);
//     delay(30);
//     msgToSink = dequeue(buffDataToSink);
//     isSended = 1;
//     trasmitState = radio.transmit(msgToSink);
//     if (trasmitState == RADIOLIB_ERR_NONE) {
//       Serial.print("send: ");
//       Serial.println(msgToSink);
//       Serial.println("transmission finished!");
//     } else {
//       Serial.print("failed, code ");
//       Serial.println(trasmitState);
//     }
//     delay(5);
//     // delay(5 / portTICK_PERIOD_MS);
//     state = radio.startReceive();
//   }
// }
