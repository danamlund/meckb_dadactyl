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

/*
 * dynamic_macro.h stores keyrecord_t (5 bytes) per press and per release event.
 * This stores 2 bytes per press+release event.
 * So this should be able to have a buffer 5 times bigger using the same memory.
 */

#ifndef DYNMACRO_H
#define DYNMACRO_H

#define DYNMACRO_BUFFER 128

void dynmacro_record(void);

void dynmacro_stop(void);

void dynmacro_replay(void);

bool dynmacro_register_code_user(uint8_t code);

void dynmacro_matrix_scan_user(void);

#endif
