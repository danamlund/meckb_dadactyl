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

#ifndef TETRIS_QMK_H
#define TETRIS_QMK_H

#include "da_util.h"
void tetris_qmk_start(void);

bool tetris_is_running(void);

bool tetris_process_record_user(uint16_t keycode, keyrecord_t *record);

void tetris_matrix_scan_user(void);

#endif
