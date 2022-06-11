#include QMK_KEYBOARD_H

#include "tinybasic_qmk.h"
#include "tetris_qmk.h"
#include "minesweeper_qmk.h"
#include "dynmacro.h"
#include "sudoku_qmk.h"

enum my_keycodes {
  HELP = SAFE_RANGE,
  FN,
  TETRIS,
  BASIC,
  MINES,
  SUDOKU,
  QUICKCALC,
  DYNMACRO_RECORD,
  DYNMACRO_STOP,
  DYNMACRO_REPLAY,
};

#define ____ KC_NO

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* 
 * ,------------------------,                                ,-----------------------.
 * |ESC | 1 | 2 | 3 | 4 | 5 |                                | 6 | 7 | 8 | 9 | 0 | ] |
 * |------------------------|                                |-----------------------|
 * |TAB | Q | W | E | R | T |                                | Y | U | I | O | P | [ |
 * |------------------------+-------------,    ,-------------+-----------------------|
 * | FN | A | S | D | F | G |  Fn   | WIN |    | APP |  Fn   | H | J | K | L | ; | ' |
 * |--------------------------------------|    |-----------------+-------------------|
 * | Z  | X | C | V | B |     |     |     |    |     |     |     | N | M | , | . | / |
 * `--------------------+ SFT | CTL | ALT |    |BSPC | ENT | SPC +-------------------'
 *                      `-----------------'    `-----------------'
 */
[0] = LAYOUT(
    KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5,                                          KC_6,     KC_7, KC_8, KC_9,    KC_0,    KC_MINS, \
    KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T,                                          KC_Y,     KC_U, KC_I, KC_O,    KC_P,    KC_LBRC, \
    FN,     KC_A, KC_S, KC_D, KC_F, KC_G,    FN,      KC_LGUI,    KC_APP,  FN,     KC_H,     KC_J, KC_K, KC_L,    KC_SCLN, KC_QUOT, \
    KC_Z,   KC_X, KC_C, KC_V, KC_B, KC_LSFT, KC_LCTL, KC_LALT,    KC_BSPC, KC_ENT, KC_SPACE, KC_N, KC_M, KC_COMM, KC_DOT,  KC_SLSH \
),
[1] = LAYOUT(
    KC_GRV, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,                                         KC_F6,    KC_F7,   KC_F8,   KC_F9,   KC_F10, KC_EQL, \
    ____,   ____,  ____,  ____,  ____,  ____,                                          ____,     ____,    KC_UP,   ____,    ____,   KC_RBRC, \
    FN,     ____,  ____,  ____,  ____,  ____,    FN,      KC_APP,     KC_RALT,  FN,    ____,     KC_LEFT, KC_DOWN, KC_RGHT, ____,   KC_BSLS, \
    ____,   ____,  ____,  ____,  ____,  KC_LSFT, KC_LCTL, KC_LALT,    KC_BSPC, KC_ENT, KC_SPACE, ____,    ____,    ____,    ____,   ____ \
),
[2] = LAYOUT(
    ____, KC_F11,    KC_F12,          KC_F13,          KC_F14,          KC_F15,                                         KC_F16,   KC_F17,  KC_F18,  KC_F19, KC_F20,  ____, \
    ____, QUICKCALC, DYNMACRO_RECORD, DYNMACRO_RECORD, DYNMACRO_REPLAY, TETRIS,                                         ____,     ____,    KC_PGUP, KC_INS, KC_PSCR, ____, \
    FN,   ____,      SUDOKU,           ____,           ____,            ____,    FN,      KC_APP,      KC_RALT,  FN,    ____,     KC_HOME, KC_PGDN, KC_END, ____,    ____, \
    ____, ____,      ____,             ____,           BASIC,           KC_LSFT, KC_LCTL, KC_LALT,     KC_BSPC, KC_ENT, KC_SPACE, ____,    MINES,   ____,   ____,    ____ \
),
};

const uint16_t PROGMEM fn_actions[] = {
};

static char layerstack = 0;

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == FN) {
        if (layerstack >= 0 && layerstack <= 2) {
            if (record->event.pressed) {
                layerstack++;
                layer_on(layerstack);
            } else {
                layer_off(layerstack);
                layerstack--;
            }
        }
    }
    if (record->event.pressed) {
        if (keycode == TETRIS && !basic_is_running()) {
            tetris_qmk_start();
            layer_clear();
        }
        if (keycode == HELP) {
            SEND_STRING("http://github.com/danamlund/meckb_da64");
        }
        if (keycode == BASIC && !basic_is_running()) {
            basic_start();
        }
        if (keycode == MINES && !basic_is_running()) {
            minesweeper_start();
            layer_clear();
        }
        if (keycode == QUICKCALC && !basic_is_running()) {
            basic_quickcalc();
        }
        if (keycode == DYNMACRO_RECORD) {
            dynmacro_record();
        }
        if (keycode == DYNMACRO_STOP) {
            dynmacro_stop();
        }
        if (keycode == DYNMACRO_REPLAY) {
            dynmacro_replay();
        }
        if (keycode == SUDOKU && !basic_is_running()) {
            sudoku_start();
            layer_clear();
            //return false;
        }
    }
    if (!tetris_process_record_user(keycode, record)) {
        return false;
    }
    if (!minesweeper_process_record_user(keycode, record)) {
        return false;
    }
    return true;
}

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
    layerstack = 0;
}

/* Called when sending a keycode press. Do not send code if false. */
bool register_code_user(uint8_t code) {
    if (!basic_register_code_user(code)) {
        return false;
    }
    if (!dynmacro_register_code_user(code)) {
        return false;
    }
    if (!sudoku_register_code_user(code)) {
        return false;
    }
    return true;
}

/* Called when sending a keycode release. Do not send code if false. */
bool unregister_code_user(uint8_t code) {
    return true;
}

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
    da_util_matrix_scan_user();
    tetris_matrix_scan_user();
    basic_matrix_scan_user();
    dynmacro_matrix_scan_user();
}
