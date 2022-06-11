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

#ifndef DA_UTIL_H
#define DA_UTIL_H

#include QMK_KEYBOARD_H

#define BUFFER_SIZE 16

uint16_t timer;
uint16_t random_counter;

// call this
void da_util_matrix_scan_user(void);

bool keycodes_buffer_is_sending(void);

uint16_t keycode_without_shift(uint16_t keycode);
bool keycode_has_shift(uint16_t keycode);

void send_keycode_now(uint16_t keycode);

void send_next_buffer(void);

void send_keycode(uint16_t keycode);

void send_keycode_shift(uint16_t keycode);

// ascii to keycodes (US layout)
uint16_t ascii_to_keycode(char c);

char keycode_to_ascii(uint16_t keycode);

// debug
void send_i(int i);
void send_integer(uint16_t i);
#endif
