#include "Communication.h"
DynamicJsonDocument doc(1024);
SX1278 radio = new Module(Nss, Dio0, Rst, Dio1);
int state;
int trasmitState = -1;
volatile bool receiveFlag = false;
void setReceiveFlag() {
  // Serial.println("flag");
  receiveFlag = true;
  if (isSended == 1) {
    receiveFlag = false;
    isSended = 0;
  }
}
Communication::Communication() {
}
void Communication::begin() {
  // Serial.println("[SX1278] Initializing ... ");
  state = radio.begin(carrierFrequency, bandwidth, spreadingFactor, codingRate, syncWord, outputPower, preambleLength, amplifierGain);
  // state = radio.begin();
  if (state == RADIOLIB_ERR_NONE) {
    // Serial.println("success!");
  } else {
    // Serial.print("failed, code ");
    // Serial.println(state);
    while (true) { vTaskDelay(100 / portTICK_PERIOD_MS); }
  }
  // sensor.begin();
  radio.setPacketReceivedAction(setReceiveFlag);
  // Serial.print(F("[SX1278] Starting to listen ... "));
  state = radio.startReceive();
  if (state == RADIOLIB_ERR_NONE) {
    // Serial.println(F("success!"));
  } else {
    // Serial.print(F("failed, code "));
    // Serial.println(state);
    while (true) { delay(10); }
  }
}
void Communication::receiveFromSink() {
  if (receiveFlag) {
    receiveFlag = false;
    state = radio.readData(msgFromSink);
    if (state == RADIOLIB_ERR_NONE) {
      if (!isFull(buffDataFromSink)) {
        // Serial.print("receive from sink: ");
        // Serial.println(msgFromSink);
        enqueueData(buffDataFromSink, msgFromSink.c_str());
      }
    }
  }
}
void Communication::analizeData() {
  while (!isEmpty(buffDataFromSink)) {
    doc.clear();
    buffMsgFromSink = dequeue(buffDataFromSink);
    deserializeJson(doc, buffMsgFromSink);
    if (doc.containsKey("SID")) {
      if (doc["SID"].as<String>() == StationID) {
        if (doc.containsKey("ID")) {
          IDOfPool = doc["ID"].as<int>();
          pool[IDOfPool].poolID = IDOfPool;
        }
        if (doc["is"].as<int>() == 0) {
          if (doc.containsKey("mucn")) {
          }
          if (doc.containsKey("ma")) {
            pool[IDOfPool].maxValue = doc["ma"].as<float>();
            pool[IDOfPool].midValue = doc["md"].as<float>();
            pool[IDOfPool].minValue = doc["mn"].as<float>();
          }
          if (doc.containsKey("a")) {
            if (doc["a"].as<int>() == 2 || doc["a"].as<int>() == 3)
              pool[IDOfPool].autoStatus = bool(doc["a"].as<int>() - 2);
            pool[IDOfPool].isSentValStatus = 0;
            pool[IDOfPool].outStatus = 0;
            pool[IDOfPool].inStatus = 0;
            pool[IDOfPool].stepOfAuto = 0;
            pool[IDOfPool].isDoneAutoMode = 0;
            doc["a"] = int(pool[IDOfPool].autoStatus);
            if (pool[IDOfPool].autoStatus == 0) {
              doc["i"] = int(pool[IDOfPool].inStatus);
              doc["o"] = int(pool[IDOfPool].outStatus);
            }
          }
          if (doc.containsKey("i")) {
            if (doc["i"].as<int>() == 3 || doc["i"].as<int>() == 2) {
              pool[IDOfPool].inStatus = bool(doc["i"].as<int>() - 2);
            }
            doc["i"] = int(pool[IDOfPool].inStatus);
          }
          if (doc.containsKey("o")) {
            if (doc["o"].as<int>() == 3 || doc["o"].as<int>() == 2) {
              pool[IDOfPool].outStatus = bool(doc["o"].as<int>() - 2);
            }
            doc["o"] = int(pool[IDOfPool].outStatus);
          }
        } else if (doc["is"].as<int>() == 1) {
          if (doc.containsKey("cm")) {
            if (doc["cm"].as<String>() == "RS") {
              haveToReset = 1;
            } else {
              haveToReset = 0;
            }
          }
          if (doc.containsKey("ma")) {
            pool[IDOfPool].maxValue = doc["ma"].as<float>();
            pool[IDOfPool].midValue = doc["md"].as<float>();
            pool[IDOfPool].minValue = doc["mn"].as<float>();
          }
          if (doc.containsKey("i")) {
            pool[IDOfPool].inStatus = bool(doc["i"].as<int>());
            doc["i"] = int(pool[IDOfPool].inStatus);
          }
          if (doc.containsKey("o")) {
            pool[IDOfPool].outStatus = bool(doc["o"].as<int>());
            doc["o"] = int(pool[IDOfPool].outStatus);
          }
          if (doc.containsKey("a")) {
            pool[IDOfPool].isSentValStatus = 0;
            pool[IDOfPool].stepOfAuto = 0;
            pool[IDOfPool].isDoneAutoMode = 0;
            pool[IDOfPool].outStatus = 0;
            pool[IDOfPool].inStatus = 0;
            pool[IDOfPool].autoStatus = bool(doc["a"].as<int>());
            doc["a"] = int(pool[IDOfPool].autoStatus);
            if (pool[IDOfPool].autoStatus == 0) {
              doc["i"] = int(pool[IDOfPool].inStatus);
              doc["o"] = int(pool[IDOfPool].outStatus);
            }
          }
        }
        buffMsgFromSink = "";
        serializeJson(doc, buffMsgFromSink);
        if (!isFull(buffDataToSink)) {
          enqueueData(buffDataToSink, buffMsgFromSink.c_str());
        }
      }
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
void Communication::sendToSink(String msg) {
  if (!isFull(buffDataToSink)) {
    enqueueData(buffDataToSink, msg.c_str());
  }
}
void Communication::sendToSink() {
  while (!isEmpty(buffDataToSink)) {
    vTaskDelay(30 / portTICK_PERIOD_MS);
    msgToSink = dequeue(buffDataToSink);
    isSended = 1;
    trasmitState = radio.transmit(msgToSink);
    if (trasmitState == RADIOLIB_ERR_NONE) {
      // Serial.print("send to sink: ");
      // Serial.println(msgToSink);
      // Serial.println("transmission finished!");
    } else {
      // Serial.print("failed, code ");
      // Serial.println(trasmitState);
    }
    vTaskDelay(5 / portTICK_PERIOD_MS);
    state = radio.startReceive();
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
