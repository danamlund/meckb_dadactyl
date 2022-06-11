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
#include "da_util.h"

#define BUFFER_SIZE 16
static uint16_t keycodes_buffer[BUFFER_SIZE];
static uint8_t keycodes_buffer_index = 0;
static bool keycodes_buffer_sending = false;

bool keycodes_buffer_is_sending(void) {
    return keycodes_buffer_sending;
}

uint16_t keycode_without_shift(uint16_t keycode) {
    return keycode & ~QK_LSFT;
}
bool keycode_has_shift(uint16_t keycode) {
    return keycode & QK_LSFT;
}

void send_keycode_now(uint16_t keycode) {
    if (keycode_has_shift(keycode)) {
        keycode = keycode_without_shift(keycode);
        register_code(KC_LSFT);
        register_code(keycode);
        unregister_code(keycode);
        unregister_code(KC_LSFT);
    } else {
        register_code(keycode);
        unregister_code(keycode);
    }
}

void send_next_buffer(void) {
    if (keycodes_buffer_index > 0) {
        keycodes_buffer_sending = true;
        send_keycode_now(keycodes_buffer[0]);
        keycodes_buffer_sending = false;
        
        for (int i = 0; i < BUFFER_SIZE - 1; i++) {
            keycodes_buffer[i] = keycodes_buffer[i+1];
        }
        keycodes_buffer_index--;
    }
}

void da_util_matrix_scan_user(void) {
    random_counter++;
    // send one keycode per matrix scan to limit the time we are sending keycodes.
    // Because we cannot detect new keypresses while sending keycodes
    send_next_buffer();
}

void send_keycode(uint16_t keycode) {
    // if buffer full, send first keycode
    if (keycodes_buffer_index >= BUFFER_SIZE) {
        send_next_buffer();
    }
    
    keycodes_buffer[keycodes_buffer_index] = keycode;
    keycodes_buffer_index++;
}

void send_keycode_shift(uint16_t keycode) {
    send_keycode(LSFT(keycode));
}

// from quantum.c
#define PGM_LOADBIT(mem, pos) ((pgm_read_byte(&((mem)[(pos) / 8])) >> ((pos) % 8)) & 0x01)

uint16_t ascii_to_keycode(char ascii_code) {
    uint8_t keycode = pgm_read_byte(&ascii_to_keycode_lut[(uint8_t)ascii_code]);
    bool is_shifted = PGM_LOADBIT(ascii_to_shift_lut, (uint8_t)ascii_code);
    if (is_shifted) {
        return LSFT(keycode);
    } else{
        return keycode;
    }
}

char keycode_to_ascii(uint16_t keycode) {
    for (char c = 0; c <= 126; c++) {
        if (ascii_to_keycode(c) == keycode) {
            return c;
        }
    }
    return 0;
}

void send_i(int i) {
    if (i == 0) {
        send_keycode_now(KC_0);
    } else if (i <= 9) {
        send_keycode_now(KC_1 + i - 1);
    } else {
        send_keycode_now(KC_A + (i - 10));
    }
}
void send_integer(uint16_t i) {
    if (i >= 16*16*16) {
        send_i((i / (16*16*16)) % 16);
    } else {
        send_i(0);
    }
    if (i >= 16*16) {
        send_i((i / (16*16)) % 16);
    } else {
        send_i(0);
    }
    if (i >= 16) {
        send_i((i / 16) % 16);
    } else {
        send_i(0);
    }
    send_i(i % 16);
}
