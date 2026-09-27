# elkay

A USB HID converter for the DEC LK401 keyboard, built so that every key and LED works with
[veetee](https://github.com/issinoho/veetee), the DEC VT emulator, and with any other USB host.

The LK401 speaks DEC's LK201-family serial protocol (4800 baud, RS-423 levels, +12 V supply)
through a 4P4C modular cable. elkay is a small inline box: the keyboard's own cable plugs into
it, and a USB cable comes out. The keyboard itself is not modified.

```
LK401 ──4P4C──▶ ┌──────────────── elkay box ────────────────┐ ──USB──▶ host (veetee)
                │ 12 V boost ◀── USB 5 V                     │
                │ level shift/invert ◀─▶ ATmega32U4 UART     │
                │ ATmega32U4 USB ──▶ HID keyboard + LEDs     │
                └────────────────────────────────────────────┘
```

## Status

| Phase | What | State |
|-------|------|-------|
| 1 | Characterise the keyboard: pinout, current draw, signal levels ([procedure](docs/phase1-bench.md)) | next |
| 2 | Power and level-shifting circuit on a breadboard ([hardware](hardware/README.md)) | provisional design |
| 3 | Firmware: serial sniffer and command console, record every keycode | written, untested |
| 4 | Firmware: initialisation (LK401 mode, up/down modes, LEDs, click, bell) | console commands in place |
| 5 | Firmware: USB HID keyboard with LED feedback | — |
| 6 | Keymap for veetee ([keymap](docs/keymap.md)) | proposed |
| 7 | Enclosure, testing, write-up | — |

## Layout

| Path | Contents |
|------|----------|
| `firmware/` | PlatformIO project; Arduino Micro is the main target, Teensy 2.0 the alternative |
| `hardware/` | Interface circuit, bill of materials, wiring notes |
| `docs/protocol.md` | The LK201/LK401 protocol as the firmware uses it |
| `docs/keymap.md` | LK401 keycode → USB HID usage → veetee binding |
| `docs/phase1-bench.md` | Bench procedure for identifying the connector and signals |

## Building

```sh
cd firmware
pio run                  # Arduino Micro
pio run -e teensy2       # Teensy 2.0
pio run -t upload        # flash the Micro
pio device monitor       # console at 115200 baud
```

## References

- *VCB02 Video Subsystem Technical Manual*, EK-104AA-TM-001 (the LK201 protocol, on bitsavers/manx)
- Linux `drivers/input/keyboard/lkkbd.c`, a working LK201/LK401 driver

## Licence

MIT.
