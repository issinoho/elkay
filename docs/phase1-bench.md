# Phase 1: characterising the keyboard

Goal: before building anything, find which connector pin is which, how much current the
keyboard draws, and what voltages its data line uses. Only a multimeter and a 12 V supply are
needed; no microcontroller yet.

Record every result in the table at the end and commit it.

## What you need

- LK401. Its cable is hardwired at the keyboard and ends in a 4P4C (RJ10) plug, about 7.7 mm
  wide. Don't cut it.
- Multimeter
- USB adjustable power module with a display (3 W, short-circuit protected) as the bench
  supply. The MT3608 is kept for the final box.
- An RJ10 (4P4C) female-to-female coupler, 1:1 wiring
- An RJ10 plug-to-plug flat cable with one end cut off and the four wires stripped
- A breadboard or terminal block for the flat cable's wires; jumper wires

The setup is: LK401 plug → coupler → flat cable → four bare wires.

## 1. Set the supply

1. Plug the power module into USB, ideally on an extension cable, with **nothing** connected
   to its screw terminal output except the multimeter.
2. Find the + terminal: a positive reading means the red probe is on +.
3. Turn the trimmer (clockwise raises it) until the **multimeter** reads **12.0 V**. The
   module's own display can be a few tenths out. Too much voltage can damage the keyboard.
4. Unplug it again.

## 2. Find ground and +12 V (keyboard unpowered)

The keyboard is not opened and its cable is not touched: its plug sits in the coupler exactly
as it would in a terminal. Everything is measured on the cut end of our own flat cable.

1. Plug the LK401's plug into the coupler and the flat cable into its other side. Push the four
   bare wires into a breadboard or terminal block so none can touch, and label them A–D by
   colour. Don't assume pin numbers: the flat cable is probably reversed.
2. Per `lkkbd.c` the order is `1 RX-in, 2 GND, 3 +12 V, 4 TX-out`. A reversed cable only flips
   it end to end, so the **two middle wires are the supply** and the two outer wires are data.
   Check this in the next steps rather than trusting it.
3. Meter in **diode-test** mode. It drives about 1 mA at 2–3 V, harmless to the keyboard.
   Measure between the two middle wires, then swap the probes:
   - **red on +12 V, black on GND:** the reading climbs and ends at OL, as the meter charges
     the keyboard's supply capacitor;
   - **red on GND, black on +12 V:** a steady 0.4–0.7 V through the protection diodes of the
     keyboard's chips.
4. Red probe on GND, black probe on each outer wire in turn: each should show a diode drop
   (a signal line), not 0 (a short) or OL (not connected).
5. If the middle wires don't behave like this, stop and record every pair's readings in both
   directions before powering anything.

## 3. Power up and measure current

1. Put the multimeter in **DC current (mA)** mode, in series between the supply's +12 V
   output and the +12 V pin. Connect the supply's ground to the GND pin.
2. Power up. The keyboard should beep or click and flash its four LEDs, which is its self-test.
3. Record the current at idle, the **peak** as the LEDs flash, and the current with keys held.
4. If the supply's display blanks or the keyboard keeps restarting, the supply is hitting its
   3 W limit (about 250 mA at 12 V). Note it; the final MT3608 has more headroom.

## 4. Identify the data pins and their levels

Keyboard powered, multimeter in DC volts, black probe on GND:

1. Measure each of the two data pins at idle. The keyboard's **output** (TX) is driven, so it
   will show a steady voltage. Its **input** (RX) will read near 0 V or float.
2. On the TX pin, hold a key with auto-repeat, such as a letter, and watch the reading. A
   multimeter can't show the serial data, but the average voltage will shift while bytes are
   being sent.
3. Record the idle voltage of TX. It tells us:
   - idle near **0 V**, active high: RS-232 polarity, and the receive path needs an inverter
     (the expected case);
   - idle near **+12 V** or +5 V: TTL polarity, and a resistor divider is enough.

## Results

| Measurement | Value |
|-------------|-------|
| GND wire (colour) | |
| +12 V wire | |
| Keyboard TX wire | |
| Keyboard RX wire | |
| Current, idle | |
| Current, LED flash peak | |
| Current, keys held | |
| TX idle voltage | |
| TX voltage with a key held | |
| Diode test, +12 V → GND (red → black) | |
| Diode test, GND → +12 V | |
| Diode test, GND → each data wire | |

With these, the interface circuit in [hardware/README.md](../hardware/README.md) can be
confirmed and the power budget checked: USB 2.0 gives 500 mA at 5 V, and the boost module turns
roughly 1 mA at 12 V into 3 mA at 5 V.
