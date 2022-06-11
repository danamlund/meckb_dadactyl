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

#include "minesweeper.h"

// Beginner
/* #define GRID_SIZE 8 */
/* #define MINES 10 */

// Intermediate
#define GRID_SIZE 16
#define MINES 40

// Expert 30x16_99 (~= 22x22_99)
/* #define GRID_SIZE 22 */
/* #define MINES 99 */

static int curx = 0;
static int cury = 0;

static uint8_t grid_mines[GRID_SIZE * GRID_SIZE / 8];
static uint8_t grid_cleared[GRID_SIZE * GRID_SIZE / 8];
static uint8_t grid_flagged[GRID_SIZE * GRID_SIZE / 8];

static uint32_t start_time = 0;

static uint16_t xorshift16(uint16_t seed) {
    if (seed == 0)
        seed = 1;
    seed ^= seed << 7;
    seed ^= seed >> 9;
    seed ^= seed << 8;
    return seed;
}

static void goto_xy(int x, int y) {
    while (curx < x) {
        send_right();
        curx++;
    }
    while (curx > x) {
        send_left();
        curx--;
    }
    while (cury < y) {
        send_down();
        cury++;
    }
    while (cury > y) {
        send_up();
        cury--;
    }
}

static bool has_bit(uint8_t *grid, uint8_t x, uint8_t y) {
    uint8_t index = y * GRID_SIZE + x;
    return grid[index / 8] & (1 << (index % 8));
}

static void set_bit(uint8_t *grid, uint8_t x, uint8_t y, bool value) {
    uint8_t index = y * GRID_SIZE + x;
    if (value) {
        grid[index / 8] |= (1 << (index % 8));
    } else {
        grid[index / 8] &= ~(1 << (index % 8));
    }
}

static bool is_mine(uint8_t x, uint8_t y) {
    if (x < 0 || y < 0 || x >= GRID_SIZE || y >= GRID_SIZE) {
        return false;
    }
    return has_bit(grid_mines, x, y);
}
static void set_mine(uint8_t x, uint8_t y, bool value) {
    set_bit(grid_mines, x, y, value);
}
static bool is_cleared(uint8_t x, uint8_t y) {
    return has_bit(grid_cleared, x, y);
}
static void set_cleared(uint8_t x, uint8_t y, bool value) {
    set_bit(grid_cleared, x, y, value);
}
static bool is_flagged(uint8_t x, uint8_t y) {
    return has_bit(grid_flagged, x, y);
}
static void set_flagged(uint8_t x, uint8_t y, bool value) {
    set_bit(grid_flagged, x, y, value);
}

static void draw(void) {
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            send('.');
        }
        send_newline();
    }
    curx = 0;
    cury = GRID_SIZE - 1;
    send_up();
}

static uint8_t number(uint8_t x, uint8_t y) {
    uint8_t n = 0;
    for (int xadd = -1; xadd <= 1; xadd++) {
        for (int yadd = -1; yadd <= 1; yadd++) {
            if (is_mine(x + xadd, y + yadd)) {
                n++;
            }
        }
    }
    return n;
}

void start(uint16_t seed) {
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            set_mine(x, y, false);
            set_cleared(x, y, false);
            set_flagged(x, y, false);
        }
    }

    int attempts = 255;
    for (int i = 0; i < MINES; i++) {
        while (attempts-- > 0) {
            seed = xorshift16(seed);
            uint8_t x = seed % GRID_SIZE;
            seed = xorshift16(seed);
            uint8_t y = seed % GRID_SIZE;
            if (!is_mine(x, y)) {
                set_mine(x, y, true);
                break;
            }
        }
    }

    draw();
    send_insert();

    for (int y = GRID_SIZE - 1; y >= 0; y--) {
        for (int x = 0; x < GRID_SIZE; x++) {
            if (number(x, y) == 0) {
                goto_xy(x, y);
                get_press();
                start_time = timer_read32();
                return;
            }
        }
    }
}

void quit_prematurely(void) {
    send_insert();
}

void get_up(void) {
    if (cury > 0) {
        cury--;
        send_up();
    }
}
void get_left(void) {
    if (curx > 0) {
        curx--;
        send_left();
    }
}
void get_down(void) {
    if (cury < GRID_SIZE - 1) {
        cury++;
        send_down();
    }
}
void get_right(void) {
    if (curx < GRID_SIZE - 1) {
        curx++;
        send_right();
    }
}

static bool clear_square(uint8_t x, uint8_t y) {
    if (x < 0 || y < 0 || x >= GRID_SIZE || y >= GRID_SIZE) {
        return false;
    }
    if (is_cleared(x, y) || is_flagged(x, y)) {
        return false;
    }
    goto_xy(x, y);
    set_cleared(x, y, true);

    uint8_t n = number(x, y);
    if (n == 0) {
        send(' ');
        send_left();
    } else {
        send('0' + n);
        send_left();
    }
    return true;
}

static void clear_squares_auto(void) {
    // auto clear squares around a '0' square.
    // slow and complicated to reduce needed call stack
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            if (is_cleared(x, y) && number(x, y) == 0) {
                bool cleared_square = false;
                for (int xadd = -1; xadd <= 1; xadd++) {
                    for (int yadd = -1; yadd <= 1; yadd++) {
                        if (clear_square(x + xadd, y + yadd)) {
                            cleared_square = true;
                        }
                    }
                }
                if (cleared_square) {
                    y = 0;
                    x = 0;
                }
            }
        }
    }
}

static void print_string(const char *msg) {
    goto_xy(GRID_SIZE, GRID_SIZE / 2);
    send(' ');
    curx++;
    while (msg[0] != '\0') {
        send(msg[0]);
        msg++;
        curx++;
    }
}

bool get_press(void) {
    if (is_cleared(curx, cury) || is_flagged(curx, cury)) {
        return true;
    }

    if (is_mine(curx, cury)) {
        send('X');
        quit_prematurely();
        return false;
    }

    clear_square(curx, cury);
    clear_squares_auto();

    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            if (!is_cleared(x, y) && !is_flagged(x, y) && !is_mine(x, y)) {
                return true;
            }
            if (is_flagged(x, y) && !is_mine(x, y)) {
                return true;
            }
        }
    }

    print_string("WIN!   "); // extra spaces to clear "left: xx"
    send_left();
    send_left();
    quit_prematurely();
    start_time = (timer_read32() - start_time) / 1000;
    send('s');
    send_left();
    while (start_time > 0) {
        send('0' + (start_time % 10));
        send_left();
        start_time = start_time / 10;
    }
    return false;
}

void get_flag_press(void) {
    if (is_cleared(curx, cury)) {
        return;
    }
    if (is_flagged(curx, cury)) {
        set_flagged(curx, cury, false);
        send('.');
    } else {
        set_flagged(curx, cury, true);
        send('*');
    }
    send_left();
}


void get_print_mines_left(void) {
    uint8_t oldx = curx;
    uint8_t oldy = cury;

    int8_t mines_left = MINES;
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            if (is_flagged(x, y)) {
                mines_left--;
            }
        }
    }

    print_string("left: ");
    if (mines_left < 0) {
        send('-');
        send(' ');
    } else {
        send('0' + (mines_left / 10));
        send('0' + (mines_left % 10));
    }
    curx += 2;
    goto_xy(oldx, oldy);
}
