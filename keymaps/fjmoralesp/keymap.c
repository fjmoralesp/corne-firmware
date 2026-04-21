/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

enum tap_dance_keycodes {
  TD_A_LCTL,
  TD_R_LSFT,
  TD_S_LGUI,
  TD_T_LALT,
  TD_N_RALT,
  TD_E_RGUI,
  TD_I_RSFT,
  TD_O_RCTL,
  TD_ENTER_MO1,
};

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
    bool     is_layer;
} tap_dance_tap_hold_t;

void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {
            if (tap_hold->is_layer) {
                layer_on(tap_hold->hold);
                tap_hold->held = tap_hold->hold;
            } else {
                register_code16(tap_hold->hold);
                tap_hold->held = tap_hold->hold;
            }
        } else {
            register_code16(tap_hold->tap);
            tap_hold->held = tap_hold->tap;
        }
    }
}

void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (tap_hold->held) {
        if (tap_hold->is_layer && tap_hold->held == tap_hold->hold) {
            layer_off(tap_hold->hold);
        } else {
            unregister_code16(tap_hold->held);
        }
        tap_hold->held = 0;
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold, is_layer_hold)                         \
    {                                                                               \
        .fn        = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, \
        .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0, is_layer_hold}),\
    }

tap_dance_action_t tap_dance_actions[] = {
  [TD_A_LCTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_A, KC_LCTL, false),
  [TD_R_LSFT] = ACTION_TAP_DANCE_TAP_HOLD(KC_R, KC_LSFT, false),
  [TD_S_LGUI] = ACTION_TAP_DANCE_TAP_HOLD(KC_S, KC_LGUI, false),
  [TD_T_LALT] = ACTION_TAP_DANCE_TAP_HOLD(KC_T, KC_LALT, false),
  [TD_N_RALT] = ACTION_TAP_DANCE_TAP_HOLD(KC_N, KC_RALT, false),
  [TD_E_RGUI] = ACTION_TAP_DANCE_TAP_HOLD(KC_E, KC_RGUI, false),
  [TD_I_RSFT] = ACTION_TAP_DANCE_TAP_HOLD(KC_I, KC_RSFT, false),
  [TD_O_RCTL] = ACTION_TAP_DANCE_TAP_HOLD(KC_O, KC_RCTL, false),
  [TD_ENTER_MO1] = ACTION_TAP_DANCE_TAP_HOLD(KC_ENTER, 1, true),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    tap_dance_state_t *state;

    switch (keycode) {
        case TD(TD_A_LCTL) ... TD(TD_ENTER_MO1):
            state = tap_dance_get_state(QK_TAP_DANCE_GET_INDEX(keycode));
            if (!record->event.pressed && state != NULL && state->count && !state->finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)tap_dance_actions[QK_TAP_DANCE_GET_INDEX(keycode)].user_data;
                tap_code16(tap_hold->tap);
            }
            break;
    }
    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
    KC_ESCAPE,    KC_Q,    KC_W,    KC_F,    KC_P,   KC_B,                          KC_J,    KC_L,    KC_U,    KC_Y, KC_SCLN, KC_BSLS,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_MINUS, TD(TD_A_LCTL), TD(TD_R_LSFT), TD(TD_S_LGUI), TD(TD_T_LALT), KC_G,     KC_M, TD(TD_N_RALT), TD(TD_E_RGUI), TD(TD_I_RSFT), TD(TD_O_RCTL), KC_QUOTE,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_EQUAL,     KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                         KC_K,    KC_H, KC_COMMA, KC_DOT, KC_SLASH,   MO(3),
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_NO,  KC_TAB,KC_SPACE,   TD(TD_ENTER_MO1), KC_BSPC, KC_NO
                                      //`--------------------------'  `--------------------------'

  ),

    [1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
    KC_GRAVE,     KC_1,    KC_2,    KC_3,   KC_4,     KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,   KC_NO,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_NO,    KC_NO,  KC_NO,  KC_NO, LSFT(KC_9), LSFT(KC_0),                        KC_H,    KC_J,    KC_K,    KC_L,   KC_NO,   KC_NO,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_NO, KC_NO, KC_NO, KC_NO, LSFT(KC_LBRC), LSFT(KC_RBRC),                    KC_LEFT, KC_DOWN,   KC_UP, KC_RIGHT,  KC_NO,   KC_NO,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LBRC, KC_RBRC,   KC_NO,      KC_NO,   KC_NO,   KC_NO
                                      //`--------------------------'  `--------------------------'
  ),

    [2] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                       XXXXXXX,   XXXXXXX,  XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX
                                      //`--------------------------'  `--------------------------'
  ),

    [3] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       RM_ON,  RM_NEXT, RM_HUEU, RM_SATU, RM_VALU, RM_SPDU,                      RM_FLGN, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      RM_OFF,  RM_PREV, RM_HUED, RM_SATD, RM_VALD, RM_SPDD,                      RM_FLGP, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                       XXXXXXX,   XXXXXXX,  XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX
                                      //`--------------------------'  `--------------------------'
  )
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
  [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(KC_RGHT, KC_LEFT), },
  [1] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(KC_RGHT, KC_LEFT), },
  [2] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(KC_RGHT, KC_LEFT), },
  [3] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(KC_RGHT, KC_LEFT), },
};
#endif
