# LK201 / LK401 protocol

What the firmware relies on. Sources: the VCB02 technical manual (EK-104AA-TM-001, LK201
chapter) and Linux `drivers/input/keyboard/lkkbd.c`. Anything marked **verify** comes from
secondary sources or memory and must be confirmed on this keyboard in Phase 3.

## Physical layer

- Asynchronous serial, **4800 baud, 8 data bits, no parity, 1 stop bit**, full duplex.
- RS-423-style single-ended levels. The keyboard has only a +12 V supply, so its output cannot
  swing negative; the real levels are measured in [Phase 1](phase1-bench.md).
- Polarity is RS-232 style: idle (mark) is the *low* voltage, which is the opposite of a
  microcontroller UART, so both directions go through an inverter.
- Connector: 4P4C modular. Per `lkkbd.c`, looking at the plug with the cable downwards and the
  latch away from you, pins 1–4 left to right:

  | Pin | Signal |
  |-----|--------|
  | 1 | Data **to** keyboard (host TX) |
  | 2 | Ground |
  | 3 | +12 V |
  | 4 | Data **from** keyboard (host RX) |

  Modular cables can be straight or reversed, so **verify** at the end of the cable we use.

## Power-up

On power-up, or after the host sends `FD` (power-up reset), the keyboard runs its self-test,
flashes the LEDs, and sends four bytes:

| Byte | Meaning |
|------|---------|
| 1 | Keyboard ID, firmware (`01`) |
| 2 | Keyboard ID, hardware (**verify**: `00`) |
| 3 | Error: `00` none, `3D` key down at power-up, `3E` self-test failed |
| 4 | Keycode of the key held down, or `00` |

The host then asks for the ID (`AB`), which returns two bytes. `lkkbd.c` tells an LK201 (`01`)
from an LK401 (`02`) by the second of them (**verify**).

Because the converter and the keyboard power up together, the power-up bytes are usually sent
before anyone is listening. The console's `reset` command sends `FD` to repeat them.

## Keycodes and special responses

Every key sends a single byte from `56` to `FB`. The bytes `B3`–`BB` are responses, not keys:

| Byte | Name | Meaning |
|------|------|---------|
| `B3` | ALL UPS | The last down/up key was released: every key is now up |
| `B4` | METRONOME | Auto-repeat tick (not used: elkay turns auto-repeat off) |
| `B5` | OUTPUT ERROR | Keyboard's transmit buffer overflowed |
| `B6` | INPUT ERROR | Keyboard received a bad command |
| `B7` | KBD LOCKED | Acknowledges "inhibit transmission" |
| `B8` | TEST MODE ACK | Acknowledges test mode |
| `B9` | PREFIX KEYS DOWN | Precedes a list of held keys after a mode change |
| `BA` | MODE CHANGE ACK | Acknowledges a mode-set command |

The full keycode table is in [keymap.md](keymap.md).

## Divisions and modes

Keys are grouped into 14 **divisions**, and each division is in one of three modes:

| Mode | Bits | Behaviour |
|------|------|-----------|
| Down only | `00` | Keycode on press, nothing on release |
| Auto-repeat down | `01` | Keycode on press, then `B4` repeats while held |
| Down/up | `11` | Keycode on press, and again on release |

USB HID needs releases, so elkay sets **every division to down/up** and lets the host do the
auto-repeat.

A mode-set command is one byte: `1 DDDD MM 0` (division in bits 6–3, mode in bits 2–1), so
down/up for division *d* is `0x86 | d << 3`.

### Releases in down/up mode

A release is reported by sending the same keycode again, **except** when the released key was
the only down/up key still held: then the keyboard sends `B3` (ALL UPS) instead. So the firmware
keeps a set of held keys, toggles a key's state each time its code arrives, and clears the whole
set on `B3`. This is also what `lkkbd.c` does.

## Commands (host → keyboard)

Parameter bytes have bit 7 set; the last byte of every command has bit 7 set, which is how the
keyboard knows the command is complete.

| Bytes | Command |
|-------|---------|
| `FD` | Power-up reset (self-test, then the four power-up bytes) |
| `AB` | Request keyboard ID (two bytes back) |
| `D3` | Reinstate defaults |
| `E9` | Enable LK401 mode: separate codes for the Alt Function keys, right Compose and right Shift |
| `0x86 \| d<<3` | Division *d* to down/up mode (d = 1–14) |
| `13 p` | LEDs on, `p` = `0x80` \| mask |
| `11 p` | LEDs off, `p` = `0x80` \| mask |
| `99` | Disable keyclick |
| `1B v` | Enable keyclick, volume `v` = `0x80` \| 0–7 (0 loudest) |
| `B9` / `BB v` | Disable / enable Ctrl-key click |
| `A1` / `23 v` | Disable / enable bell, with volume |
| `A7` | Sound the bell |

LED mask bits: `01` Wait, `02` Compose, `04` Lock, `08` Hold Screen.

## Initialisation sequence used by elkay

Taken from `lkkbd_reinit()`, with keyclick off:

```
AB                 request ID
D3                 defaults
E9                 LK401 mode
8E 96 9E … F6      divisions 1–14 to down/up
99  B9             keyclick and Ctrl click off
11 8F              all LEDs off
```

The console's `init` command sends exactly this.
