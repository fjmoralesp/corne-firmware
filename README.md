# Corne Firmware

Custom keymaps Colemak + Home row mods for the Corne keyboard.

Qmk firmware for the [Corne](https://github.com/qmk/qmk_firmware/blob/master/keyboards/crkbd/readme.md) keyboard.

## Installation

1. Follow the QMK Firmware installation instructions [here](https://docs.qmk.fm/newbs_getting_started).
2. Set qmk config `qmk config user.keyboard=crkbd/rev1`, and `qmk config user.keymap=fjmoralesp`, then, `qmk new-keymap`
3. Run `./install.sh` to set the keymaps.

## Development Resources

1. See the [Basic Keycodes](https://docs.qmk.fm/keycodes_basic) docs.
2. See the [Tap Dance](https://docs.qmk.fm/features/tap_dance) docs.

To compile the firmware run:

```bash
qmk compile
```

To flash the firmware, first set the keyboard into bootloader mode:

- Hold down both shift keys and press Pause
- Hold down both shift keys and press B
- Unplug your keyboard, hold down the Spacebar and B at the same time, plug in your keyboard and wait a second before releasing the keys
- Unplug your keyboard, hold down the top or bottom left key (usually Escape or Left Control) and plug in your keyboard
- Press the physical RESET button, usually located on the underside of the PCB
  Locate header pins on the PCB labeled RESET and GND, and short them together while plugging your PCB in

Then run:

```bash
qmk flash
```
