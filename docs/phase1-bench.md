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
5. **Before the first power-up, do the load test.** Put a 1 kΩ resistor across the output for
   5 minutes. If it holds steady, re-set it to 12.0 V with the resistor in place. If it keeps
   climbing, don't use it on the keyboard until that is understood.

A multimeter with a weak battery can read high and drift. An apparent upward creep of about
0.8 V turned out to be exactly that. If readings wander, change the meter battery first.

## 2. Map the flat cable (no keyboard needed)

This finds which bare wire goes to which pin, so the colours found in section 3 can be checked
against the pin order in `lkkbd.c`. The keyboard is not involved.

Pins are numbered as in [protocol.md](protocol.md): hold the plug with the cable downwards, the
latch away from you and the contacts facing you. Pins 1–4 then run from left to right.

1. Meter on **continuity** (beeper), or the lowest Ω range.
2. For each plug contact 1–4 in turn, hold a probe tip on the contact's gold edge and touch the
   other probe to each bare wire until it beeps. Record the colour. Each contact should connect
   to exactly one wire, reading under about 1 Ω.
3. Check for shorts: probe each of the six pairs of bare wires. All should read OL.

Once the coupler has arrived, map through it too, before the keyboard is plugged in:

4. Plug the flat cable into one side of the coupler. Look into the empty side with the latch
   slot at the top. The contacts are along the bottom, and pin 1 is on the **right**, because a
   socket is the mirror image of a plug. The keyboard's pin 1 will land on that contact.
5. Meter probes are too thick for the socket, so hold a jumper wire's male pin against one
   probe and use it as a fine tip. Touch each socket contact and find its bare wire, as in
   step 2. Record the colours. This maps each keyboard pin to a colour, and shows whether the
   coupler is straight or crossed.

## 3. Find ground and +12 V (keyboard unpowered)

The keyboard is not opened and its cable is not touched: its plug sits in the coupler exactly
as it would in a terminal. Everything is measured on the cut end of our own flat cable.

1. Plug the LK401's plug into the coupler and the flat cable into its other side. Push the four
   bare wires into a breadboard or terminal block so none can touch. Label each wire with its
   colour and the keyboard pin section 2 mapped it to.
2. Per `lkkbd.c` the order is `1 RX-in, 2 GND, 3 +12 V, 4 TX-out`. A reversed cable only flips
   it end to end, so the **two middle wires are the supply** and the two outer wires are data.
   Check this in the next steps rather than trusting it.
3. Meter in **diode-test** mode. It drives about 1 mA at 2–3 V, harmless to the keyboard.
   Measure between the two middle wires, then swap the probes:
   - **red on +12 V, black on GND:** the reading climbs and ends at OL, as the meter charges
     the keyboard's supply capacitor;
   - **red on GND, black on +12 V:** a steady 0.4–0.7 V through the protection diodes of the
     keyboard's chips.
4. Red probe on GND, black probe on each outer wire in turn. The keyboard's output (pin 4)
   should show a diode drop. Its input (pin 1) may read OL: on our keyboard it read OL to every
   other pin in both directions, in diode test and on the 20 MΩ range. Whether that is a
   high-impedance input or an open wire is settled when the keyboard is first sent a command
   (**verify**). A reading of 0 on any data wire is a short: stop.
5. If the middle wires don't behave as in step 3, stop and record every pair's readings in both
   directions before powering anything.

## 4. Power up and measure current

1. Put the multimeter in **DC current (mA)** mode, in series between the supply's +12 V
   output and the +12 V pin. Connect the supply's ground to the GND pin.
2. Power up. The keyboard should beep or click and flash its four LEDs, which is its self-test.
3. Record the current at idle, the **peak** as the LEDs flash, and the current with keys held.
4. If the supply's display blanks or the keyboard keeps restarting, the supply is hitting its
   3 W limit (about 250 mA at 12 V). Note it; the final MT3608 has more headroom.

## 5. Identify the data pins and their levels

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
| Supply display offset | display 12.4 V = meter 12.0 V (1 kΩ load) |
| Supply drift, 1 kΩ load | holds 12.0 V (step 5 passed) |
| Flat cable plug, pins 1–4 (colours) | 1 yellow, 2 green, 3 red, 4 black; no shorts between wires |
| Through coupler, keyboard pins 1–4 (colours) | 1 yellow, 2 green, 3 red, 4 black |
| Coupler wiring (straight / crossed) | straight |
| Pin 1 (yellow) resistance | OL to every other pin, both directions, on 20 MΩ |
| GND wire (colour) | green (keyboard pin 2) |
| +12 V wire | red (keyboard pin 3) |
| Keyboard TX wire | |
| Keyboard RX wire | |
| Current, idle | |
| Current, LED flash peak | |
| Current, keys held | |
| TX idle voltage | |
| TX voltage with a key held | |
| Diode test, +12 V → GND (red → black) | OL |
| Diode test, GND → +12 V | 771 mV |
| Diode test, GND → each data wire | yellow (pin 1) OL; black (pin 4) 1027 mV |

With these, the interface circuit in [hardware/README.md](../hardware/README.md) can be
confirmed and the power budget checked: USB 2.0 gives 500 mA at 5 V, and the boost module turns
roughly 1 mA at 12 V into 3 mA at 5 V.
