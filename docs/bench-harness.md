# C5 Cluster Bench Harness (1997-2000, Class 2 VPW)

## Scope

This harness is for bench testing only. It powers a C5 instrument cluster and provides a Class 2 VPW bus so an MCU-based EBCM emulator can keep the cluster from flagging EBCM communication faults.

## Required Hardware

- 12 V bench supply, current limited (start at 2 A max)
- Fused 12 V feed (3 A recommended for cluster bench use)
- C5 cluster connector pigtail or breakout
- MCU board (Arduino-compatible)
- J1850 VPW transceiver/interface board (automotive capable)
- USB-to-Class-2 sniffer interface (for reverse engineering and validation)
- Shared ground rail (star-ground preferred)
- Optional inline TVS and reverse-polarity protection on 12 V input

## Power Topology

- Bench supply `+12 V` -> inline fuse -> cluster ignition/battery feeds as required by your connector pinout.
- Bench supply `GND` -> cluster ground pins.
- Same `GND` rail -> MCU ground.
- Same `GND` rail -> VPW transceiver ground.
- Keep all grounds common. Do not float the MCU or sniffer grounds.

## Class 2 Bus Topology

- Single-wire Class 2 bus from cluster Class 2 pin to:
  - VPW transceiver bus pin
  - Sniffer/Class 2 monitor input
- Keep bus stubs short.
- Use one bus line only. Do not add CAN transceivers on this wire.
- Preserve known-good pull-up/pull-down behavior from your VPW interface design.

## Suggested Signal Path

1. MCU UART/SPI/GPIO (as required by your VPW interface) talks to VPW transceiver logic side.
2. VPW transceiver drives/receives the Class 2 physical bus.
3. Sniffer listens passively on the same bus line.
4. Cluster sends requests and receives EBCM emulation replies.

## Bring-up Sequence

1. Power off bench supply.
2. Confirm continuity and no short from Class 2 to 12 V or GND.
3. Power supply on with current limit set.
4. Confirm cluster wakes and bus is active.
5. Start sniffer capture.
6. Start emulator firmware.
7. Observe cluster indicators and message activity.

## Safety Notes

- Never connect direct MCU GPIO to vehicle-level Class 2 bus without a proper VPW transceiver/front-end.
- Keep supply current-limited during first power-up.
- If bus is stuck dominant/recessive, power down and inspect wiring before retrying.
- This setup is not intended for road use or in-vehicle safety system operation.
