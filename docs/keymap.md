# Keymap: LK401 → USB HID → veetee

elkay sends the PC key **in the same position** as each LK401 key, so veetee's stock positional
keymap (`crates/vt-keyboard/src/default.toml`) turns it back into the right DEC key. Other
software sees an ordinary PC keyboard.

LK401 keycodes come from `lkkbd.c`, with the LK401 labels added. Every row is **provisional**
until Phase 3 checks it on the real keyboard.

## Top row

| LK | LK401 key | HID usage | veetee |
|----|-----------|-----------|--------|
| `56` | F1 Hold Screen | F1 `3A` | hold-screen |
| `57` | F2 Print Screen | F2 `3B` | print-screen |
| `58` | F3 Set-Up | F3 `3C` | set-up |
| `59` | F4 Data/Talk | F4 `3D` | session |
| `5A` | F5 Break | F5 `3E` | break |
| `64`–`68` | F6–F10 | F6–F10 `3F`–`43` | f6–f10 |
| `71` | F11 (Esc) | F11 `44` | f11 |
| `72` | F12 (BS) | F12 `45` | f12 |
| `73` | F13 (LF) | F13 `68` | f13 ⚠ |
| `74` | F14 | F14 `69` | f14 ⚠ |
| `7C` | Help | F15 `6A` | help ⚠ |
| `7D` | Do | F16 `6B` | do ⚠ |
| `80`–`83` | F17–F20 | F17–F20 `6C`–`6F` | f17–f20 ⚠ |

⚠ On Linux these need the xkb option `elkay:fkeys`; see
[host/linux/README.md](../host/linux/README.md). Windows needs nothing.

## Editing and cursor keys

| LK | LK401 key | HID usage | veetee |
|----|-----------|-----------|--------|
| `8A` | Find | Insert `49` | find |
| `8B` | Insert Here | Home `4A` | insert-here |
| `8C` | Remove | Page Up `4B` | remove |
| `8D` | Select | Delete `4C` | select |
| `8E` | Prev Screen | End `4D` | prev-screen |
| `8F` | Next Screen | Page Down `4E` | next-screen |
| `A7` | ← | Left `50` | left |
| `A8` | → | Right `4F` | right |
| `A9` | ↓ | Down `51` | down |
| `AA` | ↑ | Up `52` | up |

## Numeric keypad

| LK | LK401 key | HID usage | veetee |
|----|-----------|-----------|--------|
| `A1` | PF1 | Num Lock `53` | pf1 |
| `A2` | PF2 | KP / `54` | pf2 |
| `A3` | PF3 | KP * `55` | pf3 |
| `A4` | PF4 | KP − `56` | pf4 |
| `A0` | KP − | KP = `67` | kp-minus (veetee after 1.7.0) |
| `9C` | KP , | KP + `57` | kp-comma |
| `95` | Enter | KP Enter `58` | kp-enter |
| `94` | KP . | KP . `63` | kp-period |
| `92` | KP 0 | KP 0 `62` | kp0 |
| `96`–`98` | KP 1–3 | KP 1–3 `59`–`5B` | kp1–kp3 |
| `99`–`9B` | KP 4–6 | KP 4–6 `5C`–`5E` | kp4–kp6 |
| `9D`–`9F` | KP 7–9 | KP 7–9 `5F`–`61` | kp7–kp9 |

The HID Keypad Comma usage (`85`) would be the obvious choice for KP `,`, but xkb gives it
`KP_Decimal`, which collides with KP `.`. KP + reaches veetee's existing kp-comma binding.

## Modifiers and special keys

| LK | LK401 key | HID usage | Notes |
|----|-----------|-----------|-------|
| `AE` | Shift (left) | Left Shift `E1` | |
| `AB` | Shift (right) | Right Shift `E5` | LK401 mode only |
| `AF` | Ctrl | Left Ctrl `E0` | |
| `B0` | Lock | Caps Lock `39` | Lock LED follows the host's Caps Lock |
| `AC` | Alt Function (left) | Left Alt `E2` | LK401 mode only |
| `B2` | Alt Function (right) | Right Alt `E6` | LK401 mode only |
| `B1` | Compose Character (left) | Menu `65` | proposed; GNOME Compose Key = Menu |
| `AD` | Compose Character (right) | Menu `65` | LK401 mode only |
| `BC` | `<X]` | Backspace `2A` | veetee: delete |
| `BD` | Return | Enter `28` | |
| `BE` | Tab | Tab `2B` | |
| `D4` | Space | Space `2C` | |

## Main typing keys (US layout)

| LK | Key | HID | LK | Key | HID | LK | Key | HID |
|----|-----|-----|----|-----|-----|----|-----|-----|
| `BF` | `` ` ~ `` ? | `35` | `C0` | 1 | `1E` | `C5` | 2 | `1F` |
| `CB` | 3 | `20` | `D0` | 4 | `21` | `D6` | 5 | `22` |
| `DB` | 6 | `23` | `E0` | 7 | `24` | `E5` | 8 | `25` |
| `EA` | 9 | `26` | `EF` | 0 | `27` | `F9` | - _ | `2D` |
| `F5` | = + | `2E` | `C1` | Q | `14` | `C6` | W | `1A` |
| `CC` | E | `08` | `D1` | R | `15` | `D7` | T | `17` |
| `DC` | Y | `1C` | `E1` | U | `18` | `E6` | I | `0C` |
| `EB` | O | `12` | `F0` | P | `13` | `FA` | [ { | `2F` |
| `F6` | ] } | `30` | `F7` | \ \| | `31` | `C2` | A | `04` |
| `C7` | S | `16` | `CD` | D | `07` | `D2` | F | `09` |
| `D8` | G | `0A` | `DD` | H | `0B` | `E2` | J | `0D` |
| `E7` | K | `0E` | `EC` | L | `0F` | `F2` | ; : | `33` |
| `FB` | ' " | `34` | `C9` | < > | `64` | `C3` | Z | `1D` |
| `C8` | X | `1B` | `CE` | C | `06` | `D3` | V | `19` |
| `D9` | B | `05` | `DE` | N | `11` | `E3` | M | `10` |
| `E8` | , < | `36` | `ED` | . > | `37` | `F3` | / ? | `38` |

`lkkbd.c` calls `BF` KEY_ESC, but the LK401 has no Esc key; it is probably the `` ` ~ `` key.
**Verify**.

## Open questions

1. **Compose Character.** Proposed: Menu (`65`). veetee's default Compose setting is Local
   Compose, which on a Linux desktop means the input method's compose. GNOME can use Menu as
   its Compose key (Settings → Keyboard → Compose Key), so no veetee change is needed.
2. **No Esc key.** DEC-faithful behaviour is F11. Optionally, a converter layer could make
   F11 send Esc for desktop use.
3. **PF1 is Num Lock.** Pressing it toggles the host's Num Lock state. veetee binds both keypad
   states, so it is harmless there, but other applications will see the keypad change.
