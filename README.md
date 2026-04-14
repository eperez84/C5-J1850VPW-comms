# GM Class 2 Serial (C5 EBCM Status)

Arduino Nano project for simulating/sending GM Class 2 (J1850 VPW) EBCM status traffic.

This repository is focused on the `EBCM-status` sketch used for bench testing and message replay behavior.

## Pinout

| Nano side | Transceiver side |
| --- | --- |
| D8 | RST |
| D10 | CS |
| D11 | MOSI |
| D12 | MISO |
| D13 | CLK |
| 5V | 5V |
| GND | GND |

## Sketch Location

Primary sketch path:

- `C5-J1850VPW-comms/EBCM-status/EBCM-status.ino`

## Usage

1. Open the sketch in Arduino IDE.
2. Select the correct Arduino Nano board/port.
3. Wire Nano to the transceiver using the pinout table above.
4. Build and upload the sketch.
