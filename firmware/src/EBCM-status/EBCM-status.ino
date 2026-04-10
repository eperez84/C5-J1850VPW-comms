// Two J1850 payloads on independent timers (SPI transceiver: 0x14 / 0x04 / 0x0C per byte).
#include <SPI.h>

#define csPIN  10
#define rstPIN  8

constexpr uint32_t kPeriod495Ms = 495;
constexpr uint32_t kPeriod1964Ms = 1964;
// Minimum time between any two SPI frame bursts (avoids overlap on the wire).
constexpr uint32_t kMinGapBetweenPayloadMs = 250;
// First 1964ms-frame fires this many ms after setup() completes (stagger vs 495ms stream).
constexpr uint32_t kStagger1964AfterSetupMs = 250;

// Per SPIcycleWrite (j1850_write_example): each J1850 byte = one CS cycle, (data, cmd) with
// cmd = 0x14 first, 0x04 middle, 0x0C last.
const byte kFrame495ms[] = {0x88, 0x25, 0x28, 0x07, 0x00, 0x18};
const byte kFrame1964ms[] = {0xE8, 0xFF, 0x28, 0x03, 0x28};

uint32_t g_last_ms_495 = 0;
uint32_t g_next_ms_1964 = 0;
uint32_t g_last_payload_ms = 0;

void setup() {
  byte statusByte, receivedData;
  Serial.begin(9600);
  pinMode(rstPIN, OUTPUT);
  pinMode(csPIN, OUTPUT);
  SPI.begin();
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE3));
  digitalWrite(rstPIN, LOW);
  delay(1000);
  digitalWrite(rstPIN, HIGH);
  SPIcycleRead(0x12, 0x18, statusByte, receivedData);

  const uint32_t t = millis();
  g_last_ms_495 = t - kPeriod495Ms;
  g_next_ms_1964 = t + kStagger1964AfterSetupMs;
}

void SPIcycleRead(int firstbyte, int secondbyte, byte &statusByte, byte &receivedData) {
  digitalWrite(csPIN, LOW);
  delayMicroseconds(100);
  statusByte = SPI.transfer(firstbyte);
  receivedData = SPI.transfer(secondbyte);
  digitalWrite(csPIN, HIGH);
  delayMicroseconds(100);
}

void SPIcycleWrite(int firstbyte, int secondbyte) {
  digitalWrite(csPIN, LOW);
  delay(1);
  SPI.transfer(firstbyte);
  SPI.transfer(secondbyte);
  digitalWrite(csPIN, HIGH);
  delay(1);
}

void transmitJ1850Bytes(const byte* data, size_t len) {
  for (size_t i = 0; i < len; i++) {
    int cmd = 0x04;
    if (i == 0) {
      cmd = 0x14;
    } else if (i == len - 1) {
      cmd = 0x0C;
    }
    SPIcycleWrite(data[i], cmd);
  }
  g_last_payload_ms = millis();
}

bool canSendPayloadNow(uint32_t now_ms) {
  return (g_last_payload_ms == 0) || (now_ms - g_last_payload_ms >= kMinGapBetweenPayloadMs);
}

void loop() {
  uint32_t now_ms = millis();

  if (now_ms - g_last_ms_495 >= kPeriod495Ms && canSendPayloadNow(now_ms)) {
    g_last_ms_495 = now_ms;
    Serial.println(F("TX: 88 25 28 07 00 18"));
    transmitJ1850Bytes(kFrame495ms, sizeof(kFrame495ms));
  }

  now_ms = millis();
  if (now_ms >= g_next_ms_1964 && canSendPayloadNow(now_ms)) {
    g_next_ms_1964 = now_ms + kPeriod1964Ms;
    Serial.println(F("TX: E8 FF 28 03 28"));
    transmitJ1850Bytes(kFrame1964ms, sizeof(kFrame1964ms));
  }
}
