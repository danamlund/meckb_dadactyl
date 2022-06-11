/* Copyright 2018 dan amlund
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "dynmacro.h"

#define DYNMACRO_STATE_IDLE 0
#define DYNMACRO_STATE_RECORD 1
#define DYNMACRO_STATE_REPLAY 2
#define DYNMACRO_STATE_OUT_OF_MEMORY 3
#define DYNMACRO_STATE_REPLAY_REPEAT 4

static uint8_t codes[DYNMACRO_BUFFER] = { KC_NO };
static uint8_t mods[DYNMACRO_BUFFER];

static uint8_t index = 0;
static uint8_t state = 0;

void dynmacro_record() {
    state = DYNMACRO_STATE_RECORD;
    index = 0;
}

void dynmacro_stop() {
    if (state == DYNMACRO_STATE_RECORD) {
        codes[index] = KC_NO;
    }
    state = DYNMACRO_STATE_IDLE;
    index = 0;
}

void dynmacro_replay() {
    state = DYNMACRO_STATE_REPLAY;
    index = 0;
}

bool dynmacro_register_code_user(uint8_t code) {
    if (state == DYNMACRO_STATE_RECORD) {
        if (IS_MOD(code) || code == KC_NO) {
            return true;
        }
        if (index >= DYNMACRO_BUFFER - 1) {
            state = DYNMACRO_STATE_OUT_OF_MEMORY;
            return true;
        }
        codes[index] = code;
        mods[index] = get_mods();
        index++;
    }

    return true;
}

void dynmacro_matrix_scan_user(void) {
    if (state == DYNMACRO_STATE_OUT_OF_MEMORY) {
        SEND_STRING(" OOM! ");
        state = DYNMACRO_STATE_IDLE;
    }
    if (state == DYNMACRO_STATE_REPLAY) {
        if (codes[index] == KC_NO) {
            state = DYNMACRO_STATE_IDLE;
            index = 0;
        } else {
            set_mods(mods[index]);
            send_keyboard_report();
            register_code(codes[index]);
            unregister_code(codes[index]);
            set_mods(0);
            send_keyboard_report();
            index++;
        }
    }
}
