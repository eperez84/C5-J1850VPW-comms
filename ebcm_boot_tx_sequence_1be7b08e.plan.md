---
name: EBCM boot TX sequence
overview: Add a one-time J1850 transmit sequence in `setup()` of [EBCM-status.ino](firmware/src/EBCM-status/EBCM-status.ino) using the existing `transmitJ1850Bytes()` path, with spacing that matches the sketch’s inter-payload rules and timer baselines reset after boot so periodic frames behave predictably.
todos:
  - id: define-frames
    content: Add const byte arrays (or frame table) for all 11 boot payloads in EBCM-status.ino
    status: completed
  - id: boot-sequence
    content: "Implement runEcbmBootSequence(): Serial + transmitJ1850Bytes + gap wait per frame"
    status: completed
  - id: setup-order
    content: Call boot sequence after SPIcycleRead; set g_last_ms_495 / g_next_ms_1964 after boot completes
    status: completed
isProject: false
---

# EBCM (0x28) boot-time message sequence

## Context

[`EBCM-status.ino`](firmware/src/EBCM-status/EBCM-status.ino) already sends J1850 frames by iterating bytes through `SPIcycleWrite()` with command bytes `0x14` / `0x04` / `0x0C` (first / middle / last) via [`transmitJ1850Bytes()`](firmware/src/EBCM-status/EBCM-status.ino). Each full frame should be one contiguous call to `transmitJ1850Bytes(data, len)` — no changes to the SPI layer are required.

## Message list (order preserved)

Define eleven `const byte` arrays (or one table of `{ptr, len}`) matching your capture, in this order:

| # | Bytes (hex) |
|---|-------------|
| 1 | `AC FE 28 60 CE` |
| 2 | `68 33 28 22 2D` |
| 3 | `68 85 28 02 00 7B` |
| 4 | `88 25 28 03 00 35` |
| 5 | `8A 25 28 05 00 00 91` |
| 6–11 | `8A EA 28 20 95 00`, `…96 00`, `…99 00`, `…9A 00`, `…9B 00`, `…AE 00` |

## `setup()` flow

1. Keep existing reset, SPI init, and `SPIcycleRead(0x12, 0x18, …)` as-is.
2. **Run boot sequence** before initializing periodic timer globals:
   - For each frame: optional `Serial.println(F("TX: …"))` (hex string) for parity with the periodic loop, then `transmitJ1850Bytes(frame, sizeof(frame))`.
   - **Inter-frame spacing:** After each transmission, wait until `canSendPayloadNow(millis())` is true (or `delay(kMinGapBetweenPayloadMs)`), so boot traffic respects the same `kMinGapBetweenPayloadMs` (250 ms) rule as [`loop()`](firmware/src/EBCM-status/EBCM-status.ino) and avoids back-to-back collisions on the wire.
3. **Re-base periodic timers after boot** (important): Today `g_last_ms_495` / `g_next_ms_1964` are set from `millis()` immediately after init. If boot TX runs first, that `t` becomes stale and the first `loop()` can fire both periodic frames immediately or with odd phasing. Fix: set  
   `g_last_ms_495 = millis() - kPeriod495Ms` and  
   `g_next_ms_1964 = millis() + kStagger1964AfterSetupMs`  
   **after** the boot sequence completes.

## “Burst” lines (6–11)

You described those as bus bursts; the sketch goal is **one boot execution**. The minimal implementation is **one `transmitJ1850Bytes` per row** (eleven TX events total). If you later want closer OEM timing, add a small constant (e.g. repeat count 3–5) only for the `8A EA 28 20 …` frames, with the same inter-payload gap between repeats — that can be a follow-up without restructuring the rest.

## Files to touch

- Only [`firmware/src/EBCM-status/EBCM-status.ino`](firmware/src/EBCM-status/EBCM-status.ino): new frame arrays, a `runEcbmBootSequence()` (or inline loop in `setup`), and moved timer initialization after boot.

## Verification (manual)

- Serial monitor: eleven boot `TX:` lines in order, then existing periodic messages.
- On-vehicle or bus sniffer: confirm frames match bytes above and spacing is at least ~250 ms between distinct payloads.
