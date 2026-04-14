#include <SPI.h>

#define csPIN  10
#define rstPIN  8

static constexpr uint32_t kPeriod495Ms = 495;
static constexpr uint32_t kPeriod1964Ms = 1964;
static constexpr uint32_t kMinGapRunMs = 8;
static constexpr uint32_t kStagger1964AfterSetupMs = 250;

// Boot spacing from capture session 20260410_172629 (wallTime deltas between matching raw payloads).
// Ignition proxy: first line A8FF400264 @ 7324 ms, ACFE2860CE @ 7399 ms → 75 ms before presence.
static constexpr uint16_t kBootDelayBeforePresenceMs = 75;
// After each boot frame, delay before starting the next (ms).
static constexpr uint16_t kBootDelayAfterFrameMs[] = {
    17,   // presence → early init (7399 → 7416)
    14,   // early init → ABS/TC init (7416 → 7430)
    8,    // ABS/TC init → DIC sync (7430 → 7438)
    26,   // DIC sync → DIC state (7438 → 7464)
    200,  // DIC state → warn 95 (7464 → 7664)
    8,    // 95 → 96 (no matching rows in snippet; tight burst placeholder)
    8,
    8,
    8,
    8,  // traction → sys state
};

uint32_t g_last_ms_495 = 0;
uint32_t g_next_ms_1964 = 0;
uint32_t g_last_payload_ms = 0;

void SPIcycle(int firstbyte, int secondbyte);
void markPayloadSent();
bool canSendPayloadNow(uint32_t now_ms, uint32_t min_gap_ms);

void sendFrame495ms();
void sendFrame1964ms();
void sendBootPresence();
void sendBootEarlyInit();
void sendBootAbsTcInit();
void sendBootDicSync();
void sendBootDicState();
void sendBootWarn95();
void sendBootWarn96();
void sendBootWarn99();
void sendBootWarn9A();
void sendBootTraction();
void sendBootSysState();
void runBootSequenceOnce();

void setup() {
  pinMode(rstPIN, OUTPUT);
  pinMode(csPIN, OUTPUT);
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE3));
  SPI.begin();
  digitalWrite(rstPIN, LOW);
  delay(1000);
  digitalWrite(rstPIN, HIGH);
  SPIcycle(0x12, 0x18);
  delay(1000);

  runBootSequenceOnce();

  const uint32_t t = millis();
  g_last_ms_495 = t - kPeriod495Ms;
  g_next_ms_1964 = t + kStagger1964AfterSetupMs;
}

void SPIcycle(int firstbyte, int secondbyte) {
  digitalWrite(csPIN, LOW);
  delay(1);
  SPI.transfer(firstbyte);
  SPI.transfer(secondbyte);
  digitalWrite(csPIN, HIGH);
  delay(1);
}

void markPayloadSent() {
  g_last_payload_ms = millis();
}

bool canSendPayloadNow(uint32_t now_ms, uint32_t min_gap_ms) {
  return (g_last_payload_ms == 0) || (now_ms - g_last_payload_ms >= min_gap_ms);
}

// Heartbeat
void sendFrame495ms() {
  SPIcycle(0x88, 0x14);  // priority
  SPIcycle(0x25, 0x04);  // destination
  SPIcycle(0x28, 0x04);  // source
  SPIcycle(0x07, 0x04);  // command
  SPIcycle(0x00, 0x04);  // sub-command
  SPIcycle(0x18, 0x0C);  // data
  markPayloadSent();
}

// Heartbeat
void sendFrame1964ms() {
  SPIcycle(0xE8, 0x14);  // priority
  SPIcycle(0xFF, 0x04);  // destination
  SPIcycle(0x28, 0x04);  // source
  SPIcycle(0x03, 0x04);  // command
  SPIcycle(0x28, 0x0C);  // data (last byte of 5-byte frame)
  markPayloadSent();
}

void sendBootPresence() {
  SPIcycle(0xAC, 0x14);  // priority
  SPIcycle(0xFE, 0x04);  // destination
  SPIcycle(0x28, 0x04);  // source
  SPIcycle(0x60, 0x04);  // command
  SPIcycle(0xCE, 0x0C);  // data
  markPayloadSent();
}

void sendBootEarlyInit() {
  SPIcycle(0x68, 0x14);  // priority
  SPIcycle(0x33, 0x04);  // destination
  SPIcycle(0x28, 0x04);  // source
  SPIcycle(0x22, 0x04);  // command
  SPIcycle(0x2D, 0x0C);  // data
  markPayloadSent();
}

void sendBootAbsTcInit() {
  SPIcycle(0x68, 0x14);  // priority
  SPIcycle(0x85, 0x04);  // destination
  SPIcycle(0x28, 0x04);  // source
  SPIcycle(0x02, 0x04);  // command
  SPIcycle(0x00, 0x04);  // sub-command
  SPIcycle(0x7B, 0x0C);  // data
  markPayloadSent();
}

void sendBootDicSync() {
  SPIcycle(0x88, 0x14);  // priority
  SPIcycle(0x25, 0x04);  // destination
  SPIcycle(0x28, 0x04);  // source
  SPIcycle(0x03, 0x04);  // command
  SPIcycle(0x00, 0x04);  // sub-command
  SPIcycle(0x35, 0x0C);  // data
  markPayloadSent();
}

void sendBootDicState() {
  SPIcycle(0x8A, 0x14);  // priority
  SPIcycle(0x25, 0x04);  // destination
  SPIcycle(0x28, 0x04);  // source
  SPIcycle(0x05, 0x04);  // command
  SPIcycle(0x00, 0x04);  // sub-command
  SPIcycle(0x00, 0x04);  // body
  SPIcycle(0x91, 0x0C);  // data
  markPayloadSent();
}

void sendBootWarn95() {
  SPIcycle(0x8A, 0x14);  // priority
  SPIcycle(0xEA, 0x04);  // destination
  SPIcycle(0x28, 0x04);  // source
  SPIcycle(0x20, 0x04);  // command
  SPIcycle(0x95, 0x04);  // sub-command
  SPIcycle(0x00, 0x0C);  // data
  markPayloadSent();
}

void sendBootWarn96() {
  SPIcycle(0x8A, 0x14);  // priority
  SPIcycle(0xEA, 0x04);  // destination
  SPIcycle(0x28, 0x04);  // source
  SPIcycle(0x20, 0x04);  // command
  SPIcycle(0x96, 0x04);  // sub-command
  SPIcycle(0x00, 0x0C);  // data
  markPayloadSent();
}

void sendBootWarn99() {
  SPIcycle(0x8A, 0x14);  // priority
  SPIcycle(0xEA, 0x04);  // destination
  SPIcycle(0x28, 0x04);  // source
  SPIcycle(0x20, 0x04);  // command
  SPIcycle(0x99, 0x04);  // sub-command
  SPIcycle(0x00, 0x0C);  // data
  markPayloadSent();
}

void sendBootWarn9A() {
  SPIcycle(0x8A, 0x14);  // priority
  SPIcycle(0xEA, 0x04);  // destination
  SPIcycle(0x28, 0x04);  // source
  SPIcycle(0x20, 0x04);  // command
  SPIcycle(0x9A, 0x04);  // sub-command
  SPIcycle(0x00, 0x0C);  // data
  markPayloadSent();
}

void sendBootTraction() {
  SPIcycle(0x8A, 0x14);  // priority
  SPIcycle(0xEA, 0x04);  // destination
  SPIcycle(0x28, 0x04);  // source
  SPIcycle(0x20, 0x04);  // command
  SPIcycle(0x9B, 0x04);  // sub-command
  SPIcycle(0x00, 0x0C);  // data
  markPayloadSent();
}

void sendBootSysState() {
  SPIcycle(0x8A, 0x14);  // priority
  SPIcycle(0xEA, 0x04);  // destination
  SPIcycle(0x28, 0x04);  // source
  SPIcycle(0x20, 0x04);  // command
  SPIcycle(0xAE, 0x04);  // sub-command
  SPIcycle(0x00, 0x0C);  // data
  markPayloadSent();
}

void runBootSequenceOnce() {
  delay(kBootDelayBeforePresenceMs);
  sendBootPresence();
  delay(kBootDelayAfterFrameMs[0]);
  sendBootEarlyInit();
  delay(kBootDelayAfterFrameMs[1]);
  sendBootAbsTcInit();
  delay(kBootDelayAfterFrameMs[2]);
  sendBootDicSync();
  delay(kBootDelayAfterFrameMs[3]);
  sendBootDicState();
  delay(kBootDelayAfterFrameMs[4]);
  sendBootWarn95();
  delay(kBootDelayAfterFrameMs[5]);
  sendBootWarn96();
  delay(kBootDelayAfterFrameMs[6]);
  sendBootWarn99();
  delay(kBootDelayAfterFrameMs[7]);
  sendBootWarn9A();
  delay(kBootDelayAfterFrameMs[8]);
  sendBootTraction();
  delay(kBootDelayAfterFrameMs[9]);
  sendBootSysState();
}

void loop() {
  uint32_t now_ms = millis();

  if (now_ms - g_last_ms_495 >= kPeriod495Ms && canSendPayloadNow(now_ms, kMinGapRunMs)) {
    g_last_ms_495 += kPeriod495Ms;
    sendFrame495ms();
  }

  now_ms = millis();
  if (now_ms >= g_next_ms_1964 && canSendPayloadNow(now_ms, kMinGapRunMs)) {
    g_next_ms_1964 += kPeriod1964Ms;
    sendFrame1964ms();
  }
}
