#!/usr/bin/env bash

CURRENT=$(cd $(dirname "${BASH_SOURCE[0]}") && pwd)

# create symlink
rm -rf "$HOME/qmk_firmware/keyboards/crkbd/keymaps/fjmoralesp"
ln -s "$CURRENT/keymaps/fjmoralesp" "$HOME/qmk_firmware/keyboards/crkbd/keymaps"
