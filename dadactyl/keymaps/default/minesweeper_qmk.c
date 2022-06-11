/* Copyright 2018 Dan Amlund Thomsen
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
#include "minesweeper_qmk.h"
#include "minesweeper.c"

static bool minesweeper_running = false;

static void minesweeper_quit(void) {
    minesweeper_running = false;
    layer_clear();
}

bool minesweeper_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (minesweeper_running) {
        if (keycodes_buffer_is_sending()) {
            // dont accept input while digging a bunch a mines to avoid misclicks
            return true;
        }
        if (record->event.pressed) {
            switch (keycode) {
            case KC_UP: get_up(); break;
            case KC_LEFT: get_left(); break;
            case KC_DOWN: get_down(); break;
            case KC_RIGHT: get_right(); break;
            case KC_D: if (!get_press()) { minesweeper_quit(); } break;
            case KC_F: get_flag_press(); break;
            case KC_L: get_print_mines_left(); break;
            case KC_ESC: quit_prematurely(); minesweeper_quit(); break;
            }
        }
        return false;
    }
    return true;
}

void minesweeper_start(void) {
    minesweeper_running = true;
    start(random_counter);
}

void send(char c) {
    send_keycode(ascii_to_keycode(c));
}

void send_newline(void) {
    send_keycode(KC_ENT);
}
void send_insert(void) {
    send_keycode(KC_INS);
}

void send_up(void) {
    send_keycode(KC_UP);
}
void send_left(void) {
    send_keycode(KC_LEFT);
}
void send_down(void) {
    send_keycode(KC_DOWN);
}
void send_right(void) {
    send_keycode(KC_RGHT);
}
