# elkay

DEC LK401 keyboard → USB HID converter, in an inline box. The primary host application is
veetee (github.com/issinoho/veetee, same author), whose positional PC→LK401 keymap the converter
targets; see `docs/keymap.md`.

- Firmware: PlatformIO in `firmware/`, envs `micro` (default, Arduino Micro) and `teensy2`.
  Build with `pio run` (and `-e teensy2`) from `firmware/`.
- Protocol facts live in `docs/protocol.md`; mark anything not yet confirmed on the real
  keyboard as **verify**, and remove the mark once it has been confirmed.
- The keyboard talks on `Serial1` at 4800 8N1 through external inverters; `Serial` (USB CDC) is
  the debug console.
