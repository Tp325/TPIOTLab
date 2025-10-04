#include "Communication.h"
SX1278 radio = new Module(Nss, Dio0, Rst, Dio1);
DynamicJsonDocument doc(1024);
DynamicJsonDocument doc1(1024);
int state;
int trasmitState;
volatile bool receiveFlag = false;
bool isSended;
void setReceiveFlag() {
  //Serial.println("flag");
  receiveFlag = true;
  if (isSended == 1) {
    receiveFlag = false;
    isSended = 0;
  }
}
Communication::Communication(uint16_t sensorID,
                             float carrierFrequency, float bandwidth, uint8_t spreadingFactor, uint8_t codingRate, uint8_t syncWord, uint8_t outputPower, uint8_t preambleLength, uint8_t amplifierGain) {
  this->sensorID = sensorID;
  this->carrierFrequency = carrierFrequency;
  this->bandwidth = bandwidth;
  this->spreadingFactor = spreadingFactor;
  this->codingRate = codingRate;
  this->syncWord = syncWord;
  this->outputPower = outputPower;
  this->preambleLength = preambleLength;
  this->amplifierGain = amplifierGain;
}

void Communication::begin() {
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
  stationID = manager.loadFromEEPROM();
}
bool Communication ::scanNewSink() {
  if (isScanningNewSink == 1) {
    isSended = 1;
    trasmitState = radio.transmit(String(String("{\"SS\":0,\"ID\":") + String(sensorID) + String(",\"CM\":\"WTJN\"}")).c_str());
    if (trasmitState == RADIOLIB_ERR_NONE) {
      Serial.print("send to sink: ");
      Serial.println(String(String("{\"SS\":0,\"ID\":") + String(sensorID) + String(",\"CM\":\"WTJN\"}")));
      Serial.println("transmission finished!");
    } else {
      Serial.print("failed, code ");
      Serial.println(state);
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
    state = radio.startReceive();
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
  return isScanningNewSink;
}
void Communication::sendToSinkWithACK() {
  while (!isEmpty(buffDataToSink) && tryToSend < 3) {
    if (isReceiveACK == 1) {
      if (!isEmpty(buffDataToSink)) {
        msgToSink = rear(buffDataToSink);
      }
      isSended = 1;
      trasmitState = radio.transmit(msgToSink);
      if (trasmitState == RADIOLIB_ERR_NONE) {
        Serial.print("send to sink: ");
        Serial.println(msgToSink);
        Serial.println("transmission finished!");
      } else {
        Serial.print("failed, code ");
        Serial.println(state);
      }
      isReceiveACK = 0;
      isWaitingACK = 1;
      timeOutACK = millis();
      vTaskDelay(10 / portTICK_PERIOD_MS);
      state = radio.startReceive();
    }
    if (millis() - timeOutACK >= 10000) {
      Serial.println("retry send to sink");
      tryToSend++;
      isReceiveACK = 1;
      timeOutACK = millis();
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
  if (tryToSend >= 3) {
    isScanningNewSink = 1;
  }
}
void Communication::sendToSink(String msg) {
  if (!isFull(buffDataToSink)) {
    enqueueData(buffDataToSink, msg.c_str());
  }
}
bool Communication::checkingACK(DynamicJsonDocument document) {
  if (!document.containsKey("CM")) return 0;
  if (isScanningNewSink == 1 && document["CM"].as<String>() == "ACK") {
    Serial.println("receive ACK");
    manager.clearUint16();
    if (document.containsKey("SID")) {
      stationID = document["SID"].as<uint16_t>();
      manager.saveUint16(document["SID"].as<uint16_t>());
    }
    isScanningNewSink = 0;
    tryToSend = 0;
    isReceiveACK = 1;
    isWaitingACK = 0;
    timeOutACK = millis();
    return 1;
  } else if (document["CM"].as<String>() == "ACK" && document.containsKey("SS") && document["SS"].as<int>() == 0) {
    Serial.println("receive ACK");
    isScanningNewSink = 0;
    tryToSend = 0;
    isReceiveACK = 1;
    isWaitingACK = 0;
    timeOutACK = millis();
    return 1;
  }
  return 0;
}
void Communication::receiveFromSink() {
  if (receiveFlag) {
    receiveFlag = false;
    state = radio.readData(msgFromSink);
    if (state == RADIOLIB_ERR_NONE) {
      this->RSSI = radio.getRSSI(true);
      this->SNR = radio.getSNR();
      doc.clear();
      deserializeJson(doc, msgFromSink);
      if (isScanningNewSink == 1) {
        checkingACK(doc);
      }
      if (doc.containsKey("SID")) {
        if (doc["SID"].as<uint16_t>() == stationID && doc["ID"].as<uint16_t>() == sensorID) {
          if (doc.containsKey("CM")) {
            if (checkingACK(doc)) {
              if (!isEmpty(buffDataToSink)) {
                dequeue(buffDataToSink);
              }
            }
          }
          Serial.print("receive From sink: ");
          Serial.println(msgFromSink);
          if (!isFull(buffDataFromSink) && doc["CM"].as<String>() != "ACK") {
            enqueueData(buffDataFromSink, msgFromSink.c_str());
          }
        }
      }
      msgFromSink = "";
    }
  }
}
void Communication::installationMode() {
  if (receiveFlag) {
    receiveFlag = false;
    state = radio.readData(msgFromSink);
    if (state == RADIOLIB_ERR_NONE) {
      doc.clear();
      deserializeJson(doc, msgFromSink);
      if (doc.containsKey("SID")) {
        this->RSSI = radio.getRSSI(true);
        this->SNR = radio.getSNR();
        receiveTimeOut = millis();
        msgFromSink = "";
      }
    }
  } else if (millis() - receiveTimeOut >= 2000) {
    this->RSSI = -404;
    this->SNR = -404;
  }
}
void Communication::analize(sensorValue enviromentParameter) {
  while (!isEmpty(buffDataFromSink)) {
    doc1.clear();
    deserializeJson(doc1, dequeue(buffDataFromSink));
    if (doc1["SS"].as<int>() == 1) {
      doc1["SS"] = 0;
      if (doc1.containsKey("CM")) {
        if (doc1["CM"].as<String>() != "ACK") {
          if (doc1["CM"].as<String>() == "RS") {
            haveToReset = 1;
          }
          if (doc1["CM"].as<String>() == "GD") {
          }
          if (doc1["CM"].as<String>() == "CH") {
            state = 0;
            doc1.clear();
            doc1["SS"] = 0;
            doc1["SID"] = stationID;
            doc1["CM"] = "CH";
            doc1["ID"] = sensorID;
            doc1["LRS"] = state;
            if (state == 0) {
              doc1["LRB"] = bandwidth;
              doc1["LRF"] = carrierFrequency;
              doc1["LROP"] = outputPower;
              doc1["RSSI"] = RSSI;
              doc1["SNR"] = SNR;
            }
            doc1["Tem"] = enviromentParameter.Tem;
            doc1["Hum"] = enviromentParameter.Hum;
            doc1["EC"] = enviromentParameter.EC;
            doc1["PH"] = enviromentParameter.PH;
            doc1["Ni"] = enviromentParameter.Ni;
            doc1["Pho"] = enviromentParameter.Photpho;
            doc1["Ka"] = enviromentParameter.Kali;
            doc1["Pin"] = enviromentParameter.pin;
          }
          serializeJson(doc1, buffMsgToSink);
          sendToSink(buffMsgToSink);
        }
      }
    }
  }
}
bool Communication::hasMsgFromSink() {
  return !isEmpty(buffDataFromSink);
}
bool Communication::hasMsgToSink() {
  return !isEmpty(buffDataToSink);
}
