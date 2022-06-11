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

#include "sudoku.h"

#define SEEDS_NUMBERS 26
/* #define SEEDS_SIZE 151 */
/* uint16_t seeds[] = {111, 249, 1394, 1421, 1883, 2280, 2589, 3049, */
/*                     3789, 4653, 4684, 5080, 5228, 5429, 5488, 6036, 6095, 6217, 7051, */
/*                     8007, 8591, 8900, 8962, 9359, 9414, 9952, 10137, 10287, 10601, 11437, */
/*                     11783, 11953, 12393, 12465, 12913, 13909, 14207, 14466, 14835, 14912, */
/*                     15330, 15578, 16279, 16717, 17655, 17703, 18078, 18112, 20203, 20553, */
/*                     21185, 21627, 22139, 22758, 23048, 23207, 23915, 24729, 24784, 25367, */
/*                     25418, 25746, 25857, 26450, 27669, 27718, 27819, 28592, 28798, 28799, */
/*                     28881, 29141, 30176, 31191, 31531, 32707, 32949, 33588, 34323, 34752, */
/*                     34860, 35402, 35499, 35732, 36870, 37031, 37121, 37172, 37318, 38155, */
/*                     38386, 38415, 38613, 38735, 39150, 39486, 39649, 41323, 41589, 43275, */
/*                     43464, 44887, 44983, 45276, 46531, 46928, 47298, 47473, 48365, 48834, */
/*                     50912, 51046, 51969, 52726, 52758, 53578, 53705, 53757, 53920, 53964, */
/*                     54264, 55451, 55643, 56627, 56859, 56862, 57010, 57622, 57954, 58606, */
/*                     58889, 59397, 59470, 60056, 60188, 60311, 61012, 61825, 61871, 62959, */
/*                     63141, 63344, 63412, 64049, 64127, 64188, 64189, 64533, 64914, 65108, */
/*                     65219}; */
#define SEEDS_SIZE 10
uint16_t seeds[] = {111, 249, 1394, 1421, 1883, 2280, 2589, 3049, 3789, 4653};

static uint8_t board[81];

static uint32_t start_time = 0;

static uint8_t curx = 0;
static uint8_t cury = 0;

static uint16_t xorshift16(uint16_t seed) {
    if (seed == 0)
        seed = 1;
    seed ^= seed << 7;
    seed ^= seed >> 9;
    seed ^= seed << 8;
    return seed;
}

static uint8_t get_board_index(uint8_t i) {
    return board[i] >> 1;
}

static bool is_valid_row(uint8_t i) {
    uint8_t row = i / 9;
    bool seen[10] = { false };
    for (uint8_t j = 0; j < 9; j++) {
        uint8_t number = get_board_index(row * 9 + j);
        if (seen[number]) return false; // duplicate number
        if (number >= 1) seen[number] = true;
    }
    return true;
}

static bool is_valid_column(uint8_t i) {
    uint8_t column = i % 9;
    bool seen[10] = { false };
    for (uint8_t j = 0; j < 9; j++) {
        uint8_t number = get_board_index(column + j * 9);
        if (seen[number]) return false; // duplicate number
        if (number >= 1) seen[number] = true;
    }
    return true;
}

static bool is_valid_subgrid(uint8_t i) {
    uint8_t first = (i%9)/3*3 + i/9/3*3*9;
    bool seen[10] = { false };
    for (uint8_t j = 0; j < 9; j++) {
        uint8_t number = get_board_index(first + j%3 + j/3*9);
        if (seen[number]) return false; // duplicate number
        if (number >= 1) seen[number] = true;
    }
    return true;
}

static bool is_valid(void) {
    if (!is_valid_row(0)) return false;
    if (!is_valid_row(9)) return false;
    if (!is_valid_row(18)) return false;
    if (!is_valid_row(27)) return false;
    if (!is_valid_row(36)) return false;
    if (!is_valid_row(45)) return false;
    if (!is_valid_row(54)) return false;
    if (!is_valid_row(63)) return false;
    if (!is_valid_row(72)) return false;
    if (!is_valid_column(0)) return false;
    if (!is_valid_column(1)) return false;
    if (!is_valid_column(2)) return false;
    if (!is_valid_column(3)) return false;
    if (!is_valid_column(4)) return false;
    if (!is_valid_column(5)) return false;
    if (!is_valid_column(6)) return false;
    if (!is_valid_column(7)) return false;
    if (!is_valid_column(8)) return false;
    if (!is_valid_row(0)) return false;
    if (!is_valid_row(3)) return false;
    if (!is_valid_row(6)) return false;
    if (!is_valid_row(28)) return false;
    if (!is_valid_row(31)) return false;
    if (!is_valid_row(34)) return false;
    if (!is_valid_row(54)) return false;
    if (!is_valid_row(57)) return false;
    if (!is_valid_row(60)) return false;
    return true;
}

static void random_board(uint8_t numbers, uint16_t seed) {
    uint16_t random = seed;
    for (uint8_t i = 0; i < 81; i++)
        board[i] = 0;
    for (uint8_t i = 0; i < numbers; i++) {
        while (true) {
            uint8_t ii = random % 81;
            random = xorshift16(random);
            if (board[ii] != 0) continue;
            uint8_t number = 1 + (random % 9);
            random = xorshift16(random);
            board[ii] = (number << 1) + 1; // set number and mark as fixed
            if (is_valid_row(ii)
                && is_valid_column(ii)
                && is_valid_subgrid(ii)) {
                break;
            }
            board[ii] = 0;
        }
    }
}

static void send_number(uint8_t number) {
    if (number == 0) send('.'); else send('0' + number);
}

static void start(uint16_t seed) {
    uint16_t board_seed = seed % SEEDS_SIZE;
    random_board(SEEDS_NUMBERS, seeds[board_seed]);

    for (uint8_t i = 0; i < 81; i++) {
        send_number(get_board_index(i));
        send(' ');
        if (i % 3 == 2) send(' ');
        if (i % 9 == 8) {
            send_newline();
        }
        if (i % 27 == 26) {
            for (uint8_t j = 0; j < 18; j++)
                send(' ');
            send_newline();
        }
    }
    send_insert();
    send_up();
    send_up();
    curx = 0;
    cury = 8;

    start_time = timer_read32();
}

static void quit_prematurely(void) {
    send_insert();
}

static void get_up(void) {
    if (cury > 0) {
        send_up();
        if (cury % 3 == 0) send_up();
        cury--;
    } else {
        for (uint8_t i = 0; i < 10; i++)
            send_down();
        cury = 8;
    }
}
static void get_left(void) {
    if (curx > 0) {
        send_left();
        send_left();
        if (curx % 3 == 0) send_left();
        curx--;
    } else {
        for (uint8_t i = 0; i < 18; i++)
            send_right();
        curx = 8;
    }
}
static void get_down(void) {
    if (cury < 8) {
        send_down();
        if (cury % 3 == 2) send_down();
        cury++;
    } else {
        for (uint8_t i = 0; i < 10; i++)
            send_up();
        cury = 0;
    }
}
static void get_right(void) {
    if (curx < 8) {
        send_right();
        send_right();
        if (curx % 3 == 2) send_right();
        curx++;
    } else {
        for (uint8_t i = 0; i < 18; i++)
            send_left();
        curx = 0;
    }
}

static bool get_number(uint8_t number) {
    if (board[cury*9+curx] & 1) {
        // dont change fixed numbers
        return true;
    }
    board[cury*9+curx] = number << 1;
    send_number(number);
    send_left();

    for (uint8_t i = 0; i < 81; i++) {
        if (board[i] == 0) return true;
    }
    // grid filled out, validate
    if (!is_valid()) {
        return true;
    }

    while (curx > 0) {
        get_left();
    }
    while (cury < 8) {
        get_down();
    }
    send_down();

    const char *winmsg = "WIN  ";
    for (uint8_t i = 0; winmsg[i]; i++)
        send(winmsg[i]);
    
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
