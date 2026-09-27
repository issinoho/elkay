# Hardware

A separate inline box: a 4P4C socket for the LK401's own cable, and USB out to the host.

> **Provisional.** The interface below assumes the keyboard's output idles near 0 V and swings
> towards +12 V (RS-232 polarity, no negative rail). [Phase 1](../docs/phase1-bench.md)
> measures this; revise the circuit before building if the measurements disagree.

## Block diagram

```
          USB 5V ──┬──────────────────────────────── Arduino Micro (5V)
                   │
                 polyfuse 500 mA
                   │
              12 V boost ──┬── 470 µF ── GND
                           │
 4P4C pin 3 (+12 V) ◀──────┘
 4P4C pin 2 (GND)   ◀────────────────────────────── GND
 4P4C pin 4 (kbd TX) ──▶ [RX inverter] ──▶ D0 / RX1
 4P4C pin 1 (kbd RX) ◀── [TX inverter] ◀── D1 / TX1
```

The pin numbers are `lkkbd.c`'s; replace them with the Phase 1 results.

## Receive path (keyboard → Arduino)

An NPN inverter. It also protects the 5 V input from the keyboard's 12 V swing.

```
 kbd TX ──[10k]──┬── B  2N3904
                 │
               [10k]          5V
                 │             │
                GND          [4k7]
                               │
                               ├──▶ D0 (RX1)
                               C
                               E ── GND
```

Keyboard idle (≈0 V) → transistor off → D0 pulled high → UART idle. Correct.

## Transmit path (Arduino → keyboard)

Also an NPN inverter, with its collector pulled up to 12 V so the keyboard sees a full-swing
signal.

```
 D1 (TX1) ──[10k]── B  2N3904       12V
                                     │
                                   [4k7]
                                     │
                                     ├──▶ kbd RX
                                     C
                                     E ── GND
```

UART idle (5 V) → transistor on → keyboard input ≈0 V (mark). Correct.

If an RS-232 transceiver (MAX232/MAX3232) or a 74HC14 turns up in the parts bin, it can replace
both inverters. The transistor version needs nothing unusual.

## Power budget

USB 2.0 supplies 500 mA at 5 V. With a boost module about 80 % efficient, each 1 mA the keyboard
draws at 12 V costs about 3 mA at 5 V, and the Micro needs about 40 mA. The keyboard's current is
measured in Phase 1; if its LED-flash peak goes over about 150 mA, we'll need a bigger bulk
capacitor or a USB 3 / powered port.

## Bill of materials

| Qty | Part | Notes |
|-----|------|-------|
| 1 | Arduino Micro (A000053) | Teensy 2.0 is a drop-in alternative |
| 1 | 5 V → 12 V boost module | on hand; set to 12.0 V before connecting |
| 1 | 4P4C (RJ9/RJ10) PCB socket or breakout | takes the LK401's cable |
| 2 | 2N3904 (or BC547) NPN transistor | |
| 3 | 10 kΩ resistor | |
| 2 | 4.7 kΩ resistor | |
| 1 | 470 µF 25 V electrolytic | 12 V rail bulk capacitor |
| 1 | 100 nF ceramic | 12 V rail, at the socket |
| 1 | 500 mA polyfuse | on USB 5 V into the boost |
| 1 | Enclosure | about 80 × 50 × 25 mm |
| — | 24 MHz 8-channel USB logic analyser | test equipment, optional; use with PulseView |

## Arduino Micro pins

| Pin | Use |
|-----|-----|
| D0 (RX1) | From the keyboard, through the receive inverter |
| D1 (TX1) | To the keyboard, through the transmit inverter |
| D13 (LED) | Blinks on each byte received |
| USB | HID keyboard and debug console |
