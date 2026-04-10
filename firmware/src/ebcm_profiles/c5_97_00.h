#pragma once

#include <Arduino.h>

namespace ebcm_profile {
namespace c5_97_00 {

constexpr uint8_t kNoId = 0x00;
constexpr uint8_t kEBCMSourceId = 0x29;  // Placeholder source; verify from captures.

struct RequestMap {
  uint8_t dtc_status_id;
  uint8_t health_status_id;
};

struct ResponseMap {
  uint8_t dtc_status_id;
  uint8_t broadcast_presence_id;
  uint8_t broadcast_health_id;
};

constexpr RequestMap kRequests = {
    .dtc_status_id = kNoId,      // TODO: capture-derived
    .health_status_id = kNoId,   // TODO: capture-derived
};

constexpr ResponseMap kResponses = {
    .dtc_status_id = kNoId,          // TODO: capture-derived
    .broadcast_presence_id = kNoId,  // TODO: capture-derived
    .broadcast_health_id = kNoId,    // TODO: capture-derived
};

// Service or mode bytes inside payload (placeholders until capture-derived).
constexpr uint8_t kSvcDtcStatus = kNoId;
constexpr uint8_t kSvcHealthStatus = kNoId;

constexpr uint32_t kPresencePeriodMs = 100;
constexpr uint32_t kHealthPeriodMs = 250;
constexpr uint32_t kDiagPrintPeriodMs = 1000;

// Payload templates (replace with actual capture-derived bytes).
constexpr uint8_t kPresencePayload[] = {0x01, 0x00, 0x00, 0x00};
constexpr uint8_t kHealthPayload[] = {0x02, 0x00, 0x00, 0x00};
constexpr uint8_t kNoActiveDtcPayload[] = {0x03, 0x00, 0x00, 0x00};

inline bool idsConfigured() {
  return kRequests.dtc_status_id != kNoId && kRequests.health_status_id != kNoId &&
         kResponses.dtc_status_id != kNoId &&
         kResponses.broadcast_presence_id != kNoId &&
         kResponses.broadcast_health_id != kNoId;
}

}  // namespace c5_97_00
}  // namespace ebcm_profile
