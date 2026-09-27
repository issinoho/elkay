# Phase 1: characterising the keyboard

Goal: before building anything, find which connector pin is which, how much current the
keyboard draws, and what voltages its data line uses. Only a multimeter and the 12 V boost
module are needed.

Record every result in the table at the end and commit it.

## What you need

- LK401 and its cable
- Multimeter
- 12 V boost module, and a USB power source or phone charger to feed it
- A 4P4C (RJ9/RJ10 handset) breakout socket, or a spare 4P4C cable with one end cut off and
  stripped. Don't cut the LK401's own cable.
- Jumper wires; a 100 Ω resistor

## 1. Set the boost module

1. Power the boost module from USB with **nothing** connected to its output.
2. Adjust its trimmer until the output reads **12.0 V**. (Many modules are sold set to a
   different voltage, and too much voltage can damage the keyboard.)
3. Disconnect it again.

## 2. Find ground and +12 V (keyboard unpowered)

The two supply pins can be found with the keyboard unpowered, by measuring resistance.

1. Plug the cable into the keyboard and into the breakout. Number the breakout's pins 1–4.
2. Open the keyboard (screws on the underside) and find the 4P4C socket and the voltage
   regulator next to it: a three-legged part, probably a 7805 or similar. Note which leg is the
   input, ground and output (for a 7805: input, ground, output, left to right from the front).
3. Continuity mode: find the breakout pin connected to the regulator's **ground** leg. That pin
   is **GND**.
4. Find the pin connected to the regulator's **input** leg, possibly through a diode or fuse,
   so use resistance mode if continuity doesn't beep. That pin is **+12 V**.
5. If the keyboard can't be opened, skip to the fallback in step 3.

The two remaining pins are the data lines. `lkkbd.c` says the order is `1 RX-in, 2 GND,
3 +12 V, 4 TX-out`, but the cable may reverse it.

## 3. Power up and measure current

1. Put the multimeter in **DC current (mA)** mode, in series between the boost module's +12 V
   output and the +12 V pin. Connect the boost module's ground to the GND pin.
2. Power up. The keyboard should beep or click and flash its four LEDs, which is its self-test.
3. Record the current at idle, the **peak** as the LEDs flash, and the current with keys held.

*Fallback if the supply pins are still unknown:* put a 100 Ω resistor in series with the
+12 V feed. It limits the current enough to protect the keyboard while you try pin
combinations. Take it out once the pins are confirmed.

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
| GND pin (breakout numbering) | |
| +12 V pin | |
| Keyboard TX pin | |
| Keyboard RX pin | |
| Current, idle | |
| Current, LED flash peak | |
| Current, keys held | |
| TX idle voltage | |
| TX voltage with a key held | |
| Regulator part number | |
| Anything else seen inside | |

With these, the interface circuit in [hardware/README.md](../hardware/README.md) can be
confirmed and the power budget checked: USB 2.0 gives 500 mA at 5 V, and the boost module turns
roughly 1 mA at 12 V into 3 mA at 5 V.
