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

#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdint.h>
#include <stdbool.h>

// call
static void start(uint16_t seed);
static void quit_prematurely(void);

static void get_up(void);
static void get_left(void);
static void get_down(void);
static void get_right(void);
static bool get_number(uint8_t number);

// implement
static void send(char c);

static void send_newline(void);
static void send_insert(void);

static void send_up(void);
static void send_left(void);
static void send_down(void);
static void send_right(void);

uint32_t timer_read32(void);

#endif
