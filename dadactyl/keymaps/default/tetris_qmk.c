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

#include "da_util.h"
#include "tetris_text.h"
#include "tetris_text.c"

static bool tetris_running = false;
static int tetris_keypress = 0;

void tetris_qmk_start(void) {
    timer = timer_read();
    tetris_keypress = 0;
    // set randomness using number of keys pressed
    tetris_start(random_counter);
    tetris_running = true;
}

bool tetris_is_running(void) {
    return tetris_running;
}

bool tetris_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (tetris_running) {
        if (record->event.pressed) {
            tetris_keypress = 0;
            switch (keycode) {
            case KC_UP: tetris_keypress = 1; break;
            case KC_LEFT: tetris_keypress = 2; break;
            case KC_DOWN: tetris_keypress = 3; break;
            case KC_RIGHT: tetris_keypress = 4; break;
            case KC_ESC: tetris_stop(); tetris_running = false; layer_clear(); break;
            }
        }
        return false;
    }

    return true;
}

void tetris_matrix_scan_user(void) {
    if (tetris_running) {
        if (timer_elapsed(timer) > 200) {
            timer = timer_read();
            if (!tetris_tick(100)) {
                // game over
                tetris_running = false;
                layer_clear();
            }
        }
    }
}

void tetris_send_up(void) {
    send_keycode(KC_UP);
}
void tetris_send_left(void) {
    send_keycode(KC_LEFT);
}
void tetris_send_down(void) {
    send_keycode(KC_DOWN);
}
void tetris_send_right(void) {
    send_keycode(KC_RGHT);
}

void tetris_send_backspace(void) {
    send_keycode(KC_BSPC);
}
void tetris_send_delete(void) {
    send_keycode(KC_DEL);
}

void tetris_send_string(const char *s) {
    for (int i = 0; s[i] != 0; i++) {
        send_keycode(ascii_to_keycode(s[i]));
    }
}

void tetris_send_newline(void) {
    send_keycode(KC_ENT);
}

void tetris_send_insert(void) {
    send_keycode(KC_INS);
}

int tetris_get_keypress(void) {
    int out = tetris_keypress;
    tetris_keypress = 0;
    return out;
}
