#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <Arduino.h>
#include <math.h>

/* ====== Serial ports ====== */
SoftwareSerial mySerial(2, 3);         // RX=D2, TX=D3 tới thiết bị NH4
const uint32_t USB_BAUD = 115200;      // Serial USB ↔ PC
const uint32_t DEV_BAUD = 9600;        // mySerial ↔ thiết bị NH4

/* ====== Defaults & settings ====== */
float   C1 = 1.0f;                     // mg/L as N (chuẩn thấp)
float   C2 = 100.0f;                   // mg/L as N (chuẩn cao)
uint8_t CAPTURE_SAMPLES = 8;           // số mẫu khi capture
uint8_t MEAS_SAMPLES    = 6;           // số mẫu khi đo
const  uint16_t REQ_TIMEOUT_MS = 1000; // ↑ tăng timeout để thiết bị trả lời kịp
const  uint16_t REQ_INTERVAL_MS= 60;   // nghỉ giữa các request

/* ====== Calibration store (EEPROM) ====== */
struct CalData { float m; float e0; uint16_t sig; };
const int      EEPROM_ADDR = 0;
const uint16_t CAL_SIG     = 0x4E48;   // 'N''H'
bool   hasCal = false;
float  M = NAN, E0 = NAN;              // E = E0 + M * ln(C)

/* ====== Captured points ====== */
float  E_low  = NAN, E_high = NAN;
bool   haveLow=false, haveHigh=false;
unsigned long tCapHigh=0, tCapLow=0;   // để check thứ tự High -> Low

/* ====== Looping measurement ====== */
bool   loopOn = false;
unsigned long loopIntMs = 1000;
unsigned long lastLoopTs = 0;

/* ====== Wizard state ====== */
bool wizardOn = false;
uint8_t wizardStep = 0; // 0=off, 1..5 = các bước

/* -------- Helpers: EEPROM -------- */
void saveCal(float m, float e0) {
  CalData c{m, e0, CAL_SIG};
  EEPROM.put(EEPROM_ADDR, c);
}
bool loadCal(float &m, float &e0) {
  CalData c; EEPROM.get(EEPROM_ADDR, c);
  if (c.sig == CAL_SIG && isfinite(c.m) && isfinite(c.e0)) { m=c.m; e0=c.e0; return true; }
  return false;
}
void clearCal() {
  CalData c{NAN, NAN, 0}; EEPROM.put(EEPROM_ADDR, c);
}

/* -------- Request raw -> voltage (0..5V) --------
   Gửi lệnh kèm CR, đọc ưu tiên LF, rồi CR, rồi toàn buffer; tăng timeout. */
int requestRaw(const char* cmd, uint16_t timeout_ms) {
  while (mySerial.available()) mySerial.read();   // flush cũ
  mySerial.setTimeout(timeout_ms);

  // Nhiều thiết bị chỉ cần CR:
  mySerial.print(cmd);
  mySerial.print('\r');

  // Thử đọc tới LF
  String line = mySerial.readStringUntil('\n');

  // Nếu không có LF, thử tới CR
  if (!line.length()) {
    line = mySerial.readStringUntil('\r');
  }

  // Nếu vẫn rỗng, đọc hết buffer đến timeout
  if (!line.length()) {
    line = mySerial.readString();
  }

  line.trim();
  if (!line.length()) return -1;

  // Chấp nhận trả số nguyên thuần (0..1023)
  long v = line.toInt();
  if (v < 0) return -1;
  return (int)v;
}

float captureVoltageNH4(uint8_t nSamples) {
  float acc = 0; uint8_t k=0;
  for (uint8_t i=0;i<nSamples;i++) {
    int raw = requestRaw("NH4_2_1", REQ_TIMEOUT_MS);
    if (raw >= 0) { acc += raw * (5.0f/1023.0f); k++; }
    delay(REQ_INTERVAL_MS);
  }
  return (k>0)? (acc/k) : NAN;
}

/* -------- Nernst 2-point -------- */
bool computeTwoPoint(float E1, float C1_, float E2, float C2_, float &m, float &e0) {
  if (C1_<=0 || C2_<=0 || !isfinite(E1) || !isfinite(E2)) return false;
  float dln = logf(C2_) - logf(C1_);
  if (fabs(dln) < 1e-6) return false;
  m  = (E2 - E1) / dln;                // M: V per ln(C)
  e0 = E1 - m * logf(C1_);             // E0: V
  return isfinite(m) && isfinite(e0) && fabs(m)>1e-6;
}

/* -------- Status/Help -------- */
void printHelp() {
  Serial.println(F("Commands:"));
  Serial.println(F("  help"));
  Serial.println(F("  status"));
  Serial.println(F("  cal                (print E0 & M)"));
  Serial.println(F("  c1 <val>           (set low std, mg/L as N)"));
  Serial.println(F("  c2 <val>           (set high std, mg/L as N)"));
  Serial.println(F("  capture high [n]   (capture E2 @ high std, avg n, default 8)"));
  Serial.println(F("  capture low [n]    (capture E1 @ low  std, avg n, default 8)"));
  Serial.println(F("  compute            (solve m,E0 and save EEPROM; order HIGH -> LOW)"));
  Serial.println(F("  clear              (erase calibration)"));
  Serial.println(F("  read [n]           (single measurement, avg n)"));
  Serial.println(F("  loop on [ms]       (start periodic measurement)"));
  Serial.println(F("  loop off           (stop periodic measurement)"));
  Serial.println(F("  wizard             (start guided calibration)"));
  Serial.println(F("  abort              (abort wizard)"));
  Serial.println(F("  diag               (send NH4_2_1 and dump raw ASCII/HEX for ~1s)"));
}
void printStatus() {
  Serial.println(F("=== STATUS ==="));
  Serial.print(F("C1=")); Serial.print(C1, 6); Serial.println(F(" mg/L as N"));
  Serial.print(F("C2=")); Serial.print(C2, 6); Serial.println(F(" mg/L as N"));
  Serial.print(F("E1=")); Serial.print(E_low, 6);  Serial.print(F(" V  (have="));  Serial.print(haveLow);  Serial.println(')');
  Serial.print(F("E2=")); Serial.print(E_high, 6); Serial.print(F(" V  (have="));  Serial.print(haveHigh); Serial.println(')');
  Serial.print(F("hasCal=")); Serial.print(hasCal);
  if (hasCal) {
    float S_mV_dec = 1000.0f * (2.303f * M); // mV/decade (tham khảo)
    Serial.print(F("  m=")); Serial.print(M,6);
    Serial.print(F(" V/ln, E0=")); Serial.print(E0,6);
    Serial.print(F(" V, S≈")); Serial.print(S_mV_dec,1); Serial.println(F(" mV/dec"));
  } else Serial.println();
  Serial.print(F("loop=")); Serial.print(loopOn ? F("ON") : F("OFF"));
  Serial.print(F(", interval=")); Serial.print(loopIntMs); Serial.println(F(" ms"));
  Serial.print(F("wizard=")); Serial.print(wizardOn ? F("ON") : F("OFF"));
  if (wizardOn) { Serial.print(F(", step=")); Serial.println(wizardStep); } else Serial.println();
}
void printCal() {
  if (hasCal) {
    float S_mV_dec = 1000.0f * (2.303f * M);
    Serial.print(F("E0=")); Serial.print(E0, 6); Serial.print(F(" V,  "));
    Serial.print(F("M="));  Serial.print(M, 6);  Serial.print(F(" V/ln,  "));
    Serial.print(F("S≈"));  Serial.print(S_mV_dec, 1); Serial.println(F(" mV/dec"));
  } else {
    Serial.println(F("No calibration yet. Do 'wizard' or 'capture high/low' then 'compute'."));
  }
}

/* -------- Wizard UI -------- */
void wizardShowStep() {
  Serial.println();
  switch (wizardStep) {
    case 1:
      Serial.println(F("=== CALIB WIZARD: Step 1/5 - CHUAN BI ==="));
      Serial.println(F("- Co 2 chuan: C1 (Low) = 1 mg/L as N, C2 (High) = 100 mg/L as N"));
      Serial.println(F("- Neu co ISA: them cung ti le vao CHUAN & MAU (vd 1 mL/50 mL)."));
      Serial.println(F("- Giu pH ~4–7.5; tranh bong bong tai dau do."));
      Serial.print  (F("  Hien tai: C1=")); Serial.print(C1); Serial.print(F(", C2=")); Serial.println(C2);
      Serial.println(F("Lenh: 'c1 <val>'/ 'c2 <val>' de doi, sau do go 'next' de qua buoc 2."));
      break;
    case 2:
      Serial.println(F("=== CALIB WIZARD: Step 2/5 - HIGH STANDARD ==="));
      Serial.println(F("- Dat dau do vao chuan HIGH (vd 100 mg/L), ngam/khuay nhe den khi on dinh (20–30 phut lan dau)."));
      Serial.println(F("Lenh: go 'cap' (hoac 'cap 12') de ghi E2 @ High. Sau do go 'next'."));
      break;
    case 3:
      Serial.println(F("=== CALIB WIZARD: Step 3/5 - LOW STANDARD ==="));
      Serial.println(F("- Rua DI, tham kho, chuyen sang chuan LOW (vd 1 mg/L), doi on dinh."));
      Serial.println(F("Lenh: go 'cap' (hoac 'cap 12') de ghi E1 @ Low. Sau do go 'next'."));
      break;
    case 4:
      Serial.println(F("=== CALIB WIZARD: Step 4/5 - TINH & LUU ==="));
      Serial.println(F("- Tinh M, E0 theo E=E0+M*ln(C) va luu EEPROM."));
      Serial.println(F("Lenh: 'save' de tinh & luu; 'redo' de lam lai; 'abort' de thoat."));
      break;
    case 5:
      Serial.println(F("=== CALIB WIZARD: Step 5/5 - KIEM TRA NHANH ==="));
      Serial.println(F("- Dat dau do trong chuan LOW (vd 1 mg/L)."));
      Serial.println(F("Lenh: 'test' (hoac 'test 10') de do thu; 'finish' de ket thuc wizard."));
      break;
    default:
      Serial.println(F("Wizard step khong hop le. Dung 'abort' de thoat."));
  }
  Serial.println();
}
void wizardStart() {
  wizardOn = true; wizardStep = 1;
  loopOn = false; // tạm dừng do định kỳ
  haveLow = haveHigh = false; E_low=E_high=NAN; tCapHigh=tCapLow=0;
  Serial.println(F("[WIZARD] Bat dau che do calib huong dan (HIGH -> LOW)."));
  wizardShowStep();
}
void wizardStop() {
  wizardOn = false; wizardStep = 0;
  Serial.println(F("[WIZARD] Da thoat che do calib."));
}
void wizardHandleCommand(String low) {
  if (low == F("status")) { printStatus(); return; }
  if (low.startsWith(F("c1 "))) { float v = low.substring(3).toFloat();
    if (v>0) { C1=v; Serial.print(F("Set C1=")); Serial.println(C1,6); }
    else Serial.println(F("ERR: C1 must be >0")); return; }
  if (low.startsWith(F("c2 "))) { float v = low.substring(3).toFloat();
    if (v>0) { C2=v; Serial.print(F("Set C2=")); Serial.println(C2,6); }
    else Serial.println(F("ERR: C2 must be >0")); return; }

  if (low == F("abort")) { wizardStop(); return; }

  if (low == F("next")) {
    if (wizardStep==1) { wizardStep=2; wizardShowStep(); return; }
    if (wizardStep==2) { if (!haveHigh) { Serial.println(F("[TIP] Chua 'cap' High. Hay go 'cap' truoc.")); return; }
                         wizardStep=3; wizardShowStep(); return; }
    if (wizardStep==3) { if (!haveLow)  { Serial.println(F("[TIP] Chua 'cap' Low. Hay go 'cap' truoc."));  return; }
                         wizardStep=4; wizardShowStep(); return; }
    Serial.println(F("[TIP] Khong co buoc tiep theo. Dung 'save' hoac 'finish'."));
    return;
  }

  if (low.startsWith(F("cap"))) {
    int n = CAPTURE_SAMPLES;
    int sp = low.lastIndexOf(' ');
    if (sp>0 && sp != (int)low.indexOf(' ')) { int nTry = low.substring(sp+1).toInt(); if (nTry>0) n=nTry; }

    if (wizardStep==2) { // High
      E_high = captureVoltageNH4(n); haveHigh = isfinite(E_high); tCapHigh = millis();
      Serial.print(F("[CAPTURE] HIGH E2=")); Serial.print(E_high,6);
      Serial.print(F(" V  avg=")); Serial.println(n);
      Serial.println(F("Goi 'next' de qua buoc 3."));
      return;
    }
    if (wizardStep==3) { // Low
      E_low = captureVoltageNH4(n); haveLow = isfinite(E_low); tCapLow = millis();
      Serial.print(F("[CAPTURE] LOW  E1=")); Serial.print(E_low,6);
      Serial.print(F(" V  avg=")); Serial.println(n);
      Serial.println(F("Goi 'next' de qua buoc 4."));
      return;
    }
    Serial.println(F("[TIP] 'cap' chi dung o buoc 2 (High) hoac 3 (Low)."));
    return;
  }

  if (low == F("save")) {
    if (wizardStep!=4) { Serial.println(F("[TIP] Chi dung 'save' o buoc 4.")); return; }
    if (tCapHigh && tCapLow && tCapLow < tCapHigh) {
      Serial.println(F("[WARN] Khuyen nghi thu tu HIGH -> LOW truoc khi luu."));
    }
    float m,e0;
    if (haveLow && haveHigh && computeTwoPoint(E_low,C1,E_high,C2,m,e0)) {
      M=m; E0=e0; hasCal=true; saveCal(M,E0);
      float S_mV_dec = 1000.0f * (2.303f * M);
      Serial.print(F("[CAL] Saved  m=")); Serial.print(M,6);
      Serial.print(F(" V/ln, E0="));      Serial.print(E0,6);
      Serial.print(F(" V, S≈"));          Serial.print(S_mV_dec,1);
      Serial.println(F(" mV/dec"));
      wizardStep=5; wizardShowStep();
    } else {
      Serial.println(F("[CAL][ERR] Thieu E1/E2 hop le. Dung 'redo' de lam lai."));
    }
    return;
  }

  if (low == F("redo")) {
    haveLow=haveHigh=false; E_low=E_high=NAN; tCapHigh=tCapLow=0;
    wizardStep=2; wizardShowStep();
    return;
  }

  if (low.startsWith(F("test"))) {
    if (wizardStep!=5) { Serial.println(F("[TIP] 'test' dung o buoc 5.")); return; }
    int n = MEAS_SAMPLES;
    int sp = low.indexOf(' ');
    if (sp>0) { int nTry = low.substring(sp+1).toInt(); if (nTry>0) n=nTry; }
    float E = captureVoltageNH4(n);
    if (!isfinite(E)) { Serial.println(F("[MEAS][ERR] read failed")); return; }
    if (hasCal) {
      float C = expf((E - E0) / M);
      float err = (C1>0) ? ( (C - C1)/C1 * 100.0f ) : NAN;
      Serial.print(F("[TEST] E=")); Serial.print(E,6);
      Serial.print(F(" V -> NH4=")); Serial.print(C,3); Serial.println(F(" mg/L as N"));
      if (isfinite(err)) { Serial.print(F("Sai so vs C1: ")); Serial.print(err,1); Serial.println(F(" %")); }
      else Serial.println(F("Khong tinh duoc sai so (C1<=0)."));
    } else {
      Serial.println(F("Chua co calib."));
    }
    return;
  }

  if (low == F("finish")) { wizardStop(); return; }

  Serial.println(F("Lenh khong hop le trong wizard. Dung: next | cap [n] | save | redo | test [n] | finish | abort"));
}

/* -------- DIAG: sniff raw bytes for ~1s -------- */
void runDiag() {
  while (mySerial.available()) mySerial.read();
  mySerial.setTimeout(1000);

  Serial.println(F("[DIAG] Send 'NH4_2_1\\r' and sniff for ~1s"));
  mySerial.print("NH4_2_1\r");

  unsigned long t0 = millis();
  while (millis() - t0 < 1000) {
    if (mySerial.available()) {
      int c = mySerial.read();
      Serial.print(F(" ["));
      if (c >= 32 && c <= 126) { Serial.write((char)c); } else { Serial.print(F(".")); }
      Serial.print(F("]0x"));
      if (c < 16) Serial.print('0');
      Serial.print(c, HEX);
    }
  }
  Serial.println();
  Serial.println(F("[DIAG] Done."));
}

/* -------- CLI tổng -------- */
void handleCommand(String cmd) {
  cmd.trim(); if (!cmd.length()) return;
  String low = cmd; low.toLowerCase();

  if (wizardOn) { wizardHandleCommand(low); return; }

  if (low == F("help"))   { printHelp();   return; }
  if (low == F("status")) { printStatus(); return; }
  if (low == F("cal") || low == F("getcal")) { printCal(); return; }
  if (low == F("wizard")) { wizardStart(); return; }
  if (low == F("abort"))  { wizardStop();  return; }

  if (low == F("diag"))   { runDiag();     return; }

  if (low.startsWith(F("c1 "))) {
    float v = low.substring(3).toFloat();
    if (v>0) { C1=v; Serial.print(F("Set C1=")); Serial.println(C1,6); }
    else Serial.println(F("ERR: C1 must be >0"));
    return;
  }
  if (low.startsWith(F("c2 "))) {
    float v = low.substring(3).toFloat();
    if (v>0) { C2=v; Serial.print(F("Set C2=")); Serial.println(C2,6); }
    else Serial.println(F("ERR: C2 must be >0"));
    return;
  }

  if (low.startsWith(F("capture high"))) {
    int n = CAPTURE_SAMPLES;
    int sp = low.lastIndexOf(' ');
    if (sp > 0 && sp != (int)low.indexOf(' ')) { int nTry = low.substring(sp+1).toInt(); if (nTry>0) n=nTry; }
    E_high = captureVoltageNH4(n); haveHigh = isfinite(E_high); tCapHigh = millis();
    Serial.print(F("[CAPTURE] HIGH E2=")); Serial.print(E_high,6);
    Serial.print(F(" V  avg=")); Serial.println(n);
    return;
  }

  if (low.startsWith(F("capture low"))) {
    int n = CAPTURE_SAMPLES;
    int sp = low.lastIndexOf(' ');
    if (sp > 0 && sp != (int)low.indexOf(' ')) { int nTry = low.substring(sp+1).toInt(); if (nTry>0) n=nTry; }
    E_low = captureVoltageNH4(n); haveLow = isfinite(E_low); tCapLow = millis();
    Serial.print(F("[CAPTURE] LOW  E1=")); Serial.print(E_low,6);
    Serial.print(F(" V  avg=")); Serial.println(n);
    return;
  }

  if (low == F("compute")) {
    if (tCapHigh && tCapLow && tCapLow < tCapHigh) {
      Serial.println(F("[WARN] Recommended sequence is HIGH -> LOW before compute."));
    }
    float m,e0;
    if (haveLow && haveHigh && computeTwoPoint(E_low,C1,E_high,C2,m,e0)) {
      M=m; E0=e0; hasCal=true; saveCal(M,E0);
      float S_mV_dec = 1000.0f * (2.303f * M);
      Serial.print(F("[CAL] Saved  m=")); Serial.print(M,6);
      Serial.print(F(" V/ln, E0="));      Serial.print(E0,6);
      Serial.print(F(" V, S≈"));          Serial.print(S_mV_dec,1);
      Serial.println(F(" mV/dec"));
    } else {
      Serial.println(F("[CAL][ERR] Need valid E1/E2 and C1/C2 before compute."));
    }
    return;
  }

  if (low == F("clear")) {
    clearCal(); hasCal=false; M=NAN; E0=NAN; haveLow=haveHigh=false; E_low=E_high=NAN;
    tCapHigh=tCapLow=0;
    Serial.println(F("[CAL] Cleared."));
    return;
  }

  if (low.startsWith(F("read"))) {
    int n = MEAS_SAMPLES;
    int sp = low.indexOf(' ');
    if (sp>0) { int nTry = low.substring(sp+1).toInt(); if (nTry>0) n=nTry; }
    float E = captureVoltageNH4(n);
    if (!isfinite(E)) { Serial.println(F("[MEAS][ERR] read failed")); return; }
    if (hasCal) {
      float C = expf((E - E0) / M);      // mg/L as N
      Serial.print(F("[MEAS] E=")); Serial.print(E,6);
      Serial.print(F(" V -> NH4=")); Serial.print(C,3);
      Serial.println(F(" mg/L as N"));
    } else {
      Serial.print(F("[MEAS] E=")); Serial.print(E,6);
      Serial.println(F(" V (no calibration)"));
    }
    return;
  }

  if (low.startsWith(F("loop on"))) {
    unsigned long ms = 1000;
    int sp = low.lastIndexOf(' ');
    if (sp>0 && low.substring(sp+1).length()) { unsigned long v = low.substring(sp+1).toInt(); if (v>0) ms=v; }
    loopOn = true; loopIntMs = ms; lastLoopTs = 0;
    Serial.print(F("Loop ON, interval=")); Serial.print(loopIntMs); Serial.println(F(" ms"));
    return;
  }

  if (low == F("loop off")) { loopOn=false; Serial.println(F("Loop OFF")); return; }

  Serial.println(F("Unknown command. Type: help"));
}

/* ====== Setup/Loop ====== */
void setup() {
  Serial.begin(USB_BAUD);
  mySerial.begin(DEV_BAUD);
  mySerial.setTimeout(REQ_TIMEOUT_MS);

  hasCal = loadCal(M, E0);
  Serial.println(F("NH4 Nernst 2-Point Calibration (Serial CLI + Wizard)"));
  if (hasCal) {
    float S_mV_dec = 1000.0f * (2.303f * M);
    Serial.print(F("[CAL] Loaded m=")); Serial.print(M,6);
    Serial.print(F(" V/ln, E0="));    Serial.print(E0,6);
    Serial.print(F(" V, S≈"));        Serial.print(S_mV_dec,1);
    Serial.println(F(" mV/dec"));
  } else {
    Serial.println(F("[CAL] No data. Type 'wizard' for guided calibration."));
  }
  Serial.println(F("Type 'help' for commands."));
}

void loop() {
  // CLI: đọc 1 dòng nếu có
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    handleCommand(line);
  }

  // Loop đo định kỳ (tắt khi wizard đang bật)
  if (!wizardOn && loopOn && (millis() - lastLoopTs >= loopIntMs)) {
    lastLoopTs = millis();
    float E = captureVoltageNH4(MEAS_SAMPLES);
    if (!isfinite(E)) { Serial.println(F("[MEAS][ERR] read failed")); return; }
    if (hasCal) {
      float C = expf((E - E0) / M);     // mg/L as N
      Serial.print(F("[MEAS] E="));  Serial.print(E,6);
      Serial.print(F(" V -> NH4=")); Serial.print(C,3);
      Serial.println(F(" mg/L as N"));
    } else {
      Serial.print(F("[MEAS] E=")); Serial.print(E,6);
      Serial.println(F(" V (no calibration)"));
    }
  }
}
