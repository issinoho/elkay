# Hardware

A separate inline box: a 4P4C socket for the LK401's own cable, and USB out to the host.

The interface is designed around the [Phase 1](../docs/phase1-bench.md) measurements. The
keyboard's output is true RS-423: it idles at −5.49 V, from a negative rail it makes itself. Its
input sits at +2.37 V when nothing drives it. A MAX3232 RS-232 transceiver matches this in both
directions. Whether the keyboard accepts the MAX3232's levels is confirmed by the first command
that gets a reply (**verify**).

## Block diagram

```
 USB 5V ──┬──────────────────────────── Arduino Micro 5V ──── MAX3232 VCC
          │
        polyfuse 500 mA
          │
     12 V boost ──┬── 470 µF ── GND
                  │
 4P4C pin 3 (+12 V)    ◀──┘
 4P4C pin 1 (data in)  ◀── T1OUT  MAX3232  T1IN  ◀── D1 / TX1
 4P4C pin 4 (data out) ──▶ R1IN   MAX3232  R1OUT ──▶ D0 / RX1
 4P4C pin 2 (GND)      ◀── GND, common to the boost, MAX3232 and Micro
```

Pins are the keyboard's, confirmed in Phase 1: 1 data in (yellow), 2 GND (green), 3 +12 V (red),
4 data out (black). On the breakout board the terminal labels match.

## Interface: MAX3232

The MAX3232 runs from the Micro's 5 V and makes about ±5.5 V with a charge pump, so its
transmitter swings about ±5.4 V, the same as the keyboard's own output. Its receiver takes up to
±25 V. Both directions invert, which is exactly what a UART needs, so no other parts are
involved. The 12 V rail only powers the keyboard.

DIP-16 pinout, viewed from above with the notch at the top:

```
          ┌───┬─┬───┐
  C1+   1 │   └─┘   │ 16  VCC
  V+    2 │         │ 15  GND
  C1−   3 │         │ 14  T1OUT
  C2+   4 │ MAX3232 │ 13  R1IN
  C2−   5 │         │ 12  R1OUT
  V−    6 │         │ 11  T1IN
  T2OUT 7 │         │ 10  T2IN
  R2IN  8 │         │  9  R2OUT
          └─────────┘
```

| MAX3232 pin | Connect to |
|-------------|------------|
| 1 C1+, 3 C1− | 100 nF between them |
| 4 C2+, 5 C2− | 100 nF between them |
| 2 V+ | 100 nF to GND |
| 6 V− | 100 nF to GND |
| 16 VCC | Micro 5V, with 100 nF to GND right at the pin |
| 15 GND | GND |
| 11 T1IN | Micro D1 (TX1) |
| 14 T1OUT | keyboard pin 1 (data in) |
| 13 R1IN | keyboard pin 4 (data out) |
| 12 R1OUT | Micro D0 (RX1) |
| 10 T2IN | GND (unused input, don't leave it floating) |
| 7, 8, 9 | not connected |

Ceramic capacitors have no polarity. A MAX232 (not 3232) has the same pinout but needs 1 µF
capacitors; mind the polarity if they are electrolytic.

Levels at idle:

| Point | Idle |
|-------|------|
| Micro TX1 → T1IN | 5 V |
| T1OUT → keyboard pin 1 | about −5.4 V (mark) |
| keyboard pin 4 → R1IN | −5.49 V (mark) |
| R1OUT → Micro RX1 | 5 V |

R1IN has an internal 5 kΩ pull-down, so with no keyboard connected R1OUT still reads idle.

## Bench setup

Until the box is built, the keyboard keeps its 12 V from the USB display supply, through the
breakout board, as in Phase 1. Its ground must be joined to the Micro's GND, or the data lines
have no reference.

Check the MAX3232 before the keyboard goes near it:

1. Build the circuit on a breadboard with the keyboard unplugged and the 12 V supply off.
2. Plug the Micro into USB. Meter on 20 V DC, black probe on GND:
   - pin 2 (V+) about +5.5 V, pin 6 (V−) about −5.5 V; if either is near 0, check the four
     charge-pump capacitors;
   - pin 14 (T1OUT) about −5.4 V, and pin 12 (R1OUT) about 5 V.
3. Unplug the Micro. Wire T1OUT to breakout terminal 1, R1IN to terminal 4, and GND to
   terminal 2, then plug the keyboard into the breakout board.
4. Power the keyboard from the 12 V supply, then plug in the Micro and open the console. The
   `id` command should get two bytes back, which confirms pin 1.

## Power budget

USB 2.0 supplies 500 mA at 5 V. The keyboard draws 33.5 mA at 12 V, rising to 34.6 mA while a
key auto-repeats, and it showed no LED-flash surge at power-up. With a boost module about 80 %
efficient that's about 105 mA at 5 V. The Micro needs about 40 mA and the MAX3232 a few mA, so
the total is about 150 mA. A 500 mA polyfuse and a 470 µF bulk capacitor give plenty of margin.
The LEDs haven't been lit yet; measure again once `13 8F` turns them all on.

## Bill of materials

| Qty | Part | Notes |
|-----|------|-------|
| 1 | Arduino Micro (A000053) | Teensy 2.0 is a drop-in alternative |
| 1 | MAX3232, DIP-16 | buy a spare; from a distributor, as fakes are common |
| 5 | 100 nF ceramic capacitor | 4 for the charge pump, 1 to decouple VCC |
| 1 | 16-pin DIP socket | optional |
| 1 | MT3608 5 V → 12 V boost module | set to 12.0 V before connecting |
| 1 | 4P4C (RJ9/RJ10) PCB socket or breakout board | takes the LK401's cable |
| 1 | 470 µF 25 V electrolytic | 12 V rail bulk capacitor |
| 1 | 100 nF ceramic | 12 V rail, at the socket |
| 1 | 500 mA polyfuse | on USB 5 V into the boost |
| 1 | Enclosure | about 80 × 50 × 25 mm |
| — | 24 MHz 8-channel USB logic analyser | test equipment, recommended; use with PulseView |

## Arduino Micro pins

| Pin | Use |
|-----|-----|
| D0 (RX1) | From the keyboard, via MAX3232 R1OUT |
| D1 (TX1) | To the keyboard, via MAX3232 T1IN |
| 5V, GND | MAX3232 VCC and GND |
| D13 (LED) | Blinks on each byte received |
| USB | HID keyboard and debug console |
