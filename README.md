# GM Class 2 Serial Traffic Simulator

This repository builds on the <a href="https://github.com/garnerm91/J1850" target="_blank" rel="noopener noreferrer">J1850 VPW Arduino module project</a> by <a href="https://github.com/garnerm91" target="_blank" rel="noopener noreferrer">garnerm91</a>. It uses the same XC68HC58-based Class 2 transceiver; you can buy that hardware from the author’s <a href="https://www.ebay.com/usr/mattus.i8wvojtcv" target="_blank" rel="noopener noreferrer">eBay store</a>.

Arduino (Nano or Pro Mini) project for simulating/sending GM Class 2 (J1850 VPW) traffic over the serial bus. Both boards use hardware SPI on **D11–D13**; **CS** and **RST** differ—use the matching pinout table and `csPIN` / `rstPIN` in `EBCM-status.ino`.

This project targets C5 Corvette model years `1997-2004` and focuses on the `EBCM-status` sketch for simulating EBCM traffic on the Class 2 bus. It is intended for setups where the factory ABS/EBCM is removed or replaced (for example, MK60 or Delta conversions) and suppresses ABS/traction warnings at startup.

## Compatibility and Test Status

- Intended platform: `1997-2004 Corvette (C5)`
- Verified platform: `1999 Corvette z51` only
- Current validation: bench and startup-warning suppression checks
- Not yet validated: long-term road use, all C5 model years, interactions with options such as magnetic ride, or interactions with other modules
- Use at your own risk; behavior may vary across trims, years, and module configurations!

## Pinout

SPI clock is always **D13** on these boards (not D8). Pick **one** table and set `csPIN` / `rstPIN` at the top of `EBCM-status.ino` to match.

### Arduino Nano

| Nano | Transceiver |
| --- | --- |
| D8 | RST |
| D10 | CS |
| D11 | MOSI |
| D12 | MISO |
| D13 | CLK |
| 5V | 5V |
| GND | GND |

Sketch: `#define csPIN 10` and `#define rstPIN 8`. CS on D10 satisfies the ATmega328 SPI master **SS** (D10) requirement.

### Arduino Pro Mini

| Pro Mini | Transceiver |
| --- | --- |
| D14 / A0 | CS |
| D15 / A1 | RST |
| D11 | MOSI |
| D12 | MISO |
| D13 | CLK |
| VCC | 5V |
| GND | GND |

Sketch (default in this repo): `#define csPIN A0` and `#define rstPIN A1`. **Do not** connect the transceiver CS to D10—leave **D10** unconnected; the sketch drives D10 as SPI **SS** (output high) because CS is on A0.

**Pro Mini wiring tip:** **CLK** must go to **D13**. D8 is not SPI clock (easy to confuse with Nano, where D8 is RST).

## Sketch Location

Primary sketch path:

- `C5-J1850VPW-comms/EBCM-status/EBCM-status.ino`

## Usage

1. Wire your Nano or Pro Mini per the matching pinout table and matching `csPIN` / `rstPIN` in the sketch
2. Open the sketch in Arduino IDE
3. Select the correct board (Arduino Nano, or Pro Mini ATmega328 5V / 16 MHz) and port
4. Build and upload the sketch
5. Connect transceiver to `pin 2` (J1850 bus+) on the OBD2 connector and the ground wire to either of the `grounds (pins 4 or 5)`
6. Connect the Arduino to an `ignition ON` source.  This is very important as the boot sequence runs shortly after the Arduino powers up.
7. Unplug factory EBCM connector or remove the ABS fuse - this should be `fuse 52` in the underhood fuses box
8. Turn key to on
9. ABS / TC warnings should not appear