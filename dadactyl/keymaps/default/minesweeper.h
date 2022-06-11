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

#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <stdint.h>
#include <stdbool.h>

// call
void start(uint16_t seed);
void quit_prematurely(void);

void get_up(void);
void get_left(void);
void get_down(void);
void get_right(void);
bool get_press(void);
void get_flag_press(void);
void get_print_mines_left(void);

// implement
void send(char c);

void send_newline(void);
void send_insert(void);

void send_up(void);
void send_left(void);
void send_down(void);
void send_right(void);

uint32_t timer_read32(void);

#endif
