#!/bin/sh
# Installs the elkay xkb option for the current user and turns it on in GNOME.
#
# It makes the LK401's F13-F20 produce the keysyms F13-F20. Without it, xkb
# sends XF86Tools for F13 (GNOME opens Settings) and XF86AudioMicMute for F20
# (GNOME mutes the microphone). See README.md.
set -eu

here=$(cd "$(dirname "$0")" && pwd)
xkb="${XDG_CONFIG_HOME:-$HOME/.config}/xkb"

mkdir -p "$xkb/symbols" "$xkb/rules"
cp "$here/xkb/symbols/elkay" "$xkb/symbols/elkay"

if [ -e "$xkb/rules/evdev" ] && ! cmp -s "$here/xkb/rules/evdev" "$xkb/rules/evdev"; then
    echo "$xkb/rules/evdev already exists; add these lines to it by hand:" >&2
    sed -n '/^! option/,$p' "$here/xkb/rules/evdev" >&2
    exit 1
fi
cp "$here/xkb/rules/evdev" "$xkb/rules/evdev"

if command -v gsettings >/dev/null; then
    options=$(gsettings get org.gnome.desktop.input-sources xkb-options)
    case "$options" in
        *elkay:fkeys*) ;;
        "@as []") gsettings set org.gnome.desktop.input-sources xkb-options "['elkay:fkeys']" ;;
        *) gsettings set org.gnome.desktop.input-sources xkb-options "${options%]}, 'elkay:fkeys']" ;;
    esac
    echo "elkay:fkeys is on. Log out and back in if F13 still opens Settings."
else
    echo "Installed to $xkb. Turn on the xkb option elkay:fkeys in your desktop's keyboard settings."
fi
