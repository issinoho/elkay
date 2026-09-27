# Linux host setup

elkay sends the LK401's F13–F20 as the standard USB keys F13–F20. The kernel reports them as
`KEY_F13`–`KEY_F20`, but xkb's default symbols then turn them into other keys:

| LK401 key | Default keysym | GNOME does |
|-----------|----------------|------------|
| F13 | XF86Tools | opens Settings |
| F14 | XF86Launch5 | |
| Help (F15) | XF86Launch6 | |
| Do (F16) | XF86Launch7 | |
| F17 | XF86Launch8 | |
| F18 | XF86Launch9 | |
| F19 | none | |
| F20 | XF86AudioMicMute | mutes the microphone |

GNOME acts on F13 and F20 before any application sees them, so veetee can't fix this on its
own. The xkb option `elkay:fkeys` gives all eight keys the keysyms F13–F20, and veetee's default
keymap already binds those.

## Install

```sh
host/linux/install-xkb.sh
```

This copies `xkb/` to `~/.config/xkb/`, where libxkbcommon looks for user rules, and adds
`elkay:fkeys` to GNOME's `xkb-options`, keeping any options already set. On other desktops, run
it and then enable the option in the keyboard settings.

The option applies to every keyboard. That's harmless: few PC keyboards have F13–F20, and the
ones that do then send F13–F20 too.

## Remove

```sh
gsettings reset org.gnome.desktop.input-sources xkb-options   # or remove just elkay:fkeys
rm ~/.config/xkb/symbols/elkay ~/.config/xkb/rules/evdev
```

## Testing without the keyboard

`xkb/` was checked by compiling `evdev / pc105 / us` keymaps with libxkbcommon, with and
without the option. Keycodes FK13–FK20 resolve to F13–F20 only with it. The rules file includes
the system rules *first*: rules apply in file order, and the option has to come after
`inet(evdev)` to override it.
