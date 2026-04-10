#include <Arduino.h>

#include "class2_vpw_driver.h"
#include "ebcm_profiles/c5_97_00.h"

namespace {

using class2::Frame;
using class2::VpwDriver;

VpwDriver g_bus;

uint32_t g_last_presence_ms = 0;
uint32_t g_last_health_ms = 0;
uint32_t g_last_diag_ms = 0;

bool sendTemplateFrame(uint8_t id, const uint8_t* payload, uint8_t len) {
  Frame tx;
  tx.id = id;
  tx.len = len > class2::kMaxDataBytes ? class2::kMaxDataBytes : len;
  for (uint8_t i = 0; i < tx.len; ++i) {
    tx.data[i] = payload[i];
  }
  return g_bus.send(tx);
}

void publishPresenceIfDue(uint32_t now_ms) {
  using namespace ebcm_profile::c5_97_00;
  if (now_ms - g_last_presence_ms < kPresencePeriodMs) {
    return;
  }
  g_last_presence_ms = now_ms;
  sendTemplateFrame(kResponses.broadcast_presence_id, kPresencePayload, sizeof(kPresencePayload));
}

void publishHealthIfDue(uint32_t now_ms) {
  using namespace ebcm_profile::c5_97_00;
  if (now_ms - g_last_health_ms < kHealthPeriodMs) {
    return;
  }
  g_last_health_ms = now_ms;
  sendTemplateFrame(kResponses.broadcast_health_id, kHealthPayload, sizeof(kHealthPayload));
}

void handleRxFrame(const Frame& rx) {
  using namespace ebcm_profile::c5_97_00;

  if (rx.len == 0) {
    return;
  }

  if (rx.id == kRequests.dtc_status_id && rx.data[0] == kSvcDtcStatus) {
    (void)sendTemplateFrame(kResponses.dtc_status_id, kNoActiveDtcPayload, sizeof(kNoActiveDtcPayload));
    return;
  }

  if (rx.id == kRequests.health_status_id && rx.data[0] == kSvcHealthStatus) {
    (void)sendTemplateFrame(kResponses.broadcast_health_id, kHealthPayload, sizeof(kHealthPayload));
    return;
  }
}

void pumpBusRx() {
  Frame rx;
  while (g_bus.available()) {
    if (g_bus.read(rx)) {
      handleRxFrame(rx);
    } else {
      break;
    }
  }
}

void printDiagIfDue(uint32_t now_ms) {
  using namespace ebcm_profile::c5_97_00;
  if (now_ms - g_last_diag_ms < kDiagPrintPeriodMs) {
    return;
  }
  g_last_diag_ms = now_ms;

  Serial.print(F("idsConfigured="));
  Serial.print(idsConfigured() ? F("yes") : F("no"));
  Serial.print(F(" tx="));
  Serial.print(g_bus.txCount());
  Serial.print(F(" rx="));
  Serial.println(g_bus.rxCount());

  if (!idsConfigured()) {
    Serial.println(F("Update placeholders in ebcm_profiles/c5_97_00.h from captured Class 2 traffic."));
  }
}

}  // namespace

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }

  (void)g_bus.begin();
  g_bus.setLoopbackForBench(false);
  Serial.println(F("C5 EBCM bench emulator started."));
}

void loop() {
  const uint32_t now_ms = millis();
  g_bus.poll();
  pumpBusRx();
  publishPresenceIfDue(now_ms);
  publishHealthIfDue(now_ms);
  printDiagIfDue(now_ms);
}
