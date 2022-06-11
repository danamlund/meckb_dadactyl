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

#ifndef TINYBASIC_QMK_H
#define TINYBASIC_QMK_H

#include QMK_KEYBOARD_H

bool basic_register_code_user(uint8_t code);

void basic_matrix_scan_user(void);

void basic_start(void);

bool basic_is_running(void);

void basic_quickcalc(void);

#endif
