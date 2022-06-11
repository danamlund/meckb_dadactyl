/* Copyright 2020 Dan Amlund Thomsen
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

#include "da_util.h"
#include "sudoku_qmk.h"
#include "sudoku.c"

static bool sudoku_running = false;

static void sudoku_quit(void) {
    sudoku_running = false;
}

bool sudoku_register_code_user(uint8_t code) {
    if (sudoku_running) {
        if (keycodes_buffer_is_sending()) {
            return true;
        }
        switch (code) {
        case KC_UP: get_up(); break;
        case KC_LEFT: get_left(); break;
        case KC_DOWN: get_down(); break;
        case KC_RIGHT: get_right(); break;
        case KC_0: get_number(0); break;
        case KC_ESC: quit_prematurely(); sudoku_quit(); break;
        }
        if (code >= KC_1 && code <= KC_9) {
            if (!get_number(code - KC_1 + 1)) {
                sudoku_quit();
            }
        }
        return false;
    }
    return true;
}

void sudoku_start(void) {
    sudoku_running = true;
    start(random_counter);
}

static void send(char c) {
    send_keycode(ascii_to_keycode(c));
}

static void send_newline(void) {
    send_keycode(KC_ENT);
}
static void send_insert(void) {
    send_keycode(KC_INS);
}

static void send_up(void) {
    send_keycode(KC_UP);
}
static void send_left(void) {
    send_keycode(KC_LEFT);
}
static void send_down(void) {
    send_keycode(KC_DOWN);
}
static void send_right(void) {
    send_keycode(KC_RGHT);
}
