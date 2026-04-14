# GM Class 2 Serial Traffic Simulator

Arduino Nano project for simulating/sending GM Class 2 (J1850 VPW) traffic over the serial bus.

This repository is focused on the `EBCM-status` sketch used for simulating EBCM traffic on the serial bus.  Useful for those who have removed the ABS/EBCM entirely or for those who have switched over to an aftermarket ABS system (mk60, Delta, etc.) as it suppresses the cluster warnings.

NOTE: As of this writing, I have NOT driven the car with all of this all wired up.  I can only confirm that - with the factory EBCM unplugged - the ABS and traction control warnings no longer appear on start up.  This was tested on a 1999 C5.  Unsure if it will work on any other model years or how it'll interact with magnetic ride.

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