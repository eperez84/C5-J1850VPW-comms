# GM Class 2 Serial Traffic Simulator

Arduino Nano project for simulating/sending GM Class 2 (J1850 VPW) traffic over the serial bus.

This project targets C5 Corvette model years `1997-2004` and focuses on the `EBCM-status` sketch for simulating EBCM traffic on the Class 2 bus. It is intended for setups where the factory ABS/EBCM is removed or replaced (for example, MK60 or Delta conversions) and suppresses ABS/traction warnings at startup.

## Compatibility and Test Status

- Intended platform: `1997-2004 Corvette (C5)`
- Verified platform: `1999 Corvette (C5)` only
- Current validation: bench and startup-warning suppression checks
- Not yet validated: long-term road use, all C5 model years, and interactions with options such as magnetic ride
- Use at your own risk; behavior may vary across trims, years, and module configurations

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

1. Wire Nano to the transceiver using the pinout table above
2. Open the sketch in Arduino IDE
3. Select the correct Arduino Nano board/port
4. Build and upload the sketch
5. Connect transceiver to `pin 2` (J1850 bus+) on the OBD2 connector and the ground wire to either of the `grounds (pins 4 or 5)`
6. Connect the Arduino to an `ignition ON` source.  This is very important as the boot sequence runs shortly after the Arduino powers up.
7. Unplug factory EBCM connector or remove the ABS fuse - this should be `fuse 52` in the underhood fuses box
8. Turn key to on
9. ABS / TC warnings should not appear