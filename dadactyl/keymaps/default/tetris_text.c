/* Copyright 2017 Dan Amlund Thomsen
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
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>

#include "tetris_text.h"

#define HEIGHT 20
#define WIDTH 10
#define BOARDWIDTH 1+WIDTH/8
#define BWORK 0
#define BDRAW 1
#define BDONE 2
static uint8_t boards[3][HEIGHT][BOARDWIDTH];

static uint8_t curx = 0;
static uint8_t cury = 0;

static uint16_t timems;
static uint16_t next_down;
static uint16_t seed;
static uint16_t score;
static uint8_t game_over;
static uint8_t piece;
static uint8_t rotation;
static int8_t piece_x;
static int8_t piece_y;

static int8_t isrowfull(int8_t y) {
    return boards[BDONE][y][0] == 0xFF && boards[BDONE][y][1] == 0x03;
}

static uint16_t xorshift16(uint16_t seed) {
    if (seed == 0)
        seed = 1;
    seed ^= seed << 7;
    seed ^= seed >> 9;
    seed ^= seed << 8;
    return seed;
}

static char get_shape_char(uint8_t piece) {
    switch (piece) {
    case 0: return 'I';
    case 1: return 'O';
    case 2: return 'T';
    case 3: return 'J';
    case 4: return 'L';
    case 5: return 'S';
    case 6: return 'Z';
    }
    return 'Q';
}

static const uint16_t shapes[] = {
    // ....   ..x.
    // ....   ..x.
    // xxxx   ..x.
    // ....   ..x.
    0x00F0, 0x2222, 0x00F0, 0x222,
    // ....
    // ....
    // .xx.
    // .xx.
    0x0066, 0x0066, 0x0066, 0x0066,
    // ....   ....    ....    ....
    // ....   ..x.    ..x.    ..x.
    // .xxx   .xx.    .xxx    ..xx
    // ..x.   ..x.    ....    ..x.
    0x0072, 0x0262, 0x0270, 0x0232,
    // ....   ....   ....    ....
    // ....   ..x.   .x..    ..xx
    // .xxx   ..x.   .xxx    ..x.
    // ...x   .xx.   ....    ..x.
    0x0071, 0x0226, 0x0470, 0x0322,
    // ....    ....    ....    ....
    // ....    .xx.    ...x    ..x.
    // .xxx    ..x.    .xxx    ..x.
    // .x..    ..x.    ....    ..xx
    0x0074, 0x0622, 0x0170, 0x0223,
    // ....   ....
    // ....   .x..
    // ..xx   .xx.
    // .xx.   ..x.
    0x0036, 0x0462, 0x0036, 0x0462,
    // ....   ....
    // ....   ..x.
    // .xx.   .xx.
    // ..xx   .x..
    0x0063, 0x0264, 0x0063, 0x0264
};

static uint16_t get_shape(uint8_t piece, uint8_t rotation) {
    return shapes[piece * 4 + rotation];
}

static void send_goto_xy(uint8_t x, uint8_t y) {
    while (curx < x) {
        tetris_send_right();
        curx++;
    }
    while (curx > x) {
        tetris_send_left();
        curx--;
    }
    while (cury < y) {
        tetris_send_down();
        cury++;
    }
    while (cury > y) {
        tetris_send_up();
        cury--;
    }
}
static void tetris_char(char c) {
    send(c);
    curx++;
}

static uint8_t is_shape(uint8_t x, uint8_t y, uint16_t shape) {
    return (shape & (1 << (y*4+x))) > 0;
}
static uint8_t get_board(uint8_t boardi, uint8_t x, uint8_t y) {
    return (boards[boardi][y][x/8] & (1 << (x % 8))) > 0;
}
static void set_board(uint8_t boardi, uint8_t x, uint8_t y) {
    boards[boardi][y][x/8] |= 1 << (x % 8);
}
static void clear_board(uint8_t boardi) {
    for (uint8_t y = 0; y < HEIGHT; y++) {
        for (uint8_t x = 0; x < BOARDWIDTH; x++) {
            boards[boardi][y][x] = 0;
        }
    }
}
static void copy_board(uint8_t boardi_from, uint8_t boardi_to) {
    for (uint8_t y = 0; y < HEIGHT; y++) {
        for (uint8_t x = 0; x < BOARDWIDTH; x++) {
            boards[boardi_to][y][x] |= boards[boardi_from][y][x];
        }
    }
}

static uint8_t hits(void) {
    for (uint8_t y = 0; y < HEIGHT; y++) {
        for (uint8_t x = 0; x < BOARDWIDTH; x++) {
            if (boards[BWORK][y][x] & boards[BDONE][y][x])
                return 1;
        }
    }
    return 0;
}

static int8_t set_piece(uint8_t boardi, int8_t x, int8_t y, uint16_t piece, uint8_t rotation) {
    int8_t output = 0;
    uint16_t shape = get_shape(piece, rotation);
    for (uint8_t yy = 0; yy < 4; yy++) {
        int8_t yyy = y + yy;
        for (uint8_t xx = 0; xx < 4; xx++) {
            int8_t xxx = x + xx;
            if (is_shape(xx, yy, shape)) {
                if (xxx < 0 || xxx >= WIDTH || yyy >= HEIGHT) {
                    return 0;
                }
                if (yyy >= 0) {
                    set_board(boardi, xxx, yyy);
                    output |= 2; // shape block not above board
                } else {
                    output |= 4; // shape block seen above board
                }
            }
        }
    }
    return output;
}
static void draw(void) {
    char shapechar = get_shape_char(piece);
    for (int8_t y = 0; y < HEIGHT; y++) {
        for (int8_t x = 0; x < WIDTH; x++) {
            uint8_t isold = get_board(BDRAW, x, y);
            uint8_t isnew = get_board(BWORK, x, y);
            if (isold != isnew) {
                send_goto_xy(x+1, y);
                if (isnew) {
                    tetris_char(shapechar);
                } else {
                    tetris_char('.');
                }
            }
        }
        for (uint8_t x = 0; x < BOARDWIDTH; x++) {
            boards[BDRAW][y][x] = boards[BWORK][y][x];
        }
    }
}

static uint8_t move_piece_to(int8_t newx, int8_t newy, uint8_t newpiece, uint8_t newrotation) {
    clear_board(BWORK);
    if (!set_piece(BWORK, newx, newy, newpiece, newrotation)) {
        return 0;
    }
    else if (hits()) {
        return 0;
    } else {
        draw();
        piece_x = newx;
        piece_y = newy;
        piece = newpiece;
        rotation = newrotation;
        return 1;
    }
}

static void draw_board_line(void) {
  tetris_send_string("l..........l");
  tetris_send_newline();
}

static uint8_t clear_lines(void) {
    int8_t fullrows = 0;
    for (int8_t y = HEIGHT-1; y >= 0; y--) {
        if (isrowfull(y)) {
            send_goto_xy(WIDTH+2, y);
            for (int8_t i = 0; i < WIDTH + 3; i++) {
                tetris_send_backspace();
            }
            curx = WIDTH+2;
            cury--;
            fullrows++;
        }
        for (int8_t x = 0; x < BOARDWIDTH; x++) {
            boards[BDONE][y][x] = boards[BDONE][y-fullrows][x];
        }
    }
    if (fullrows > 0) {
        tetris_send_insert();
        send_goto_xy(0, 0);
        for (int8_t y = 0; y < fullrows; y++) {
            for (int8_t x = 0; x < BOARDWIDTH; x++) {
                boards[BDONE][y][x] = 0;
            }
            draw_board_line();
            curx = 0;
            cury++;
        }
        tetris_send_insert();
    }
    return fullrows;
}

void tetris_start(uint16_t initial_seed) {
    tetris_send_insert();
    for (int8_t boardi = 0; boardi < 3; boardi++) {
        for (int8_t y = 0; y < HEIGHT; y++) {
            for (int8_t x = 0; x < BOARDWIDTH; x++) {
                boards[boardi][y][x] = 0;
            }
        }
    }

    for (int i = 0; i < HEIGHT; i++) {
        draw_board_line();
    }
    tetris_send_string("doooooooooob");
    curx = WIDTH+2;
    cury = HEIGHT;

    game_over = 0;
    seed = initial_seed;
    timems = 0;
    next_down = 0;
    score = 0;
    piece = 7;
    rotation = 0;
}

void tetris_stop(void) {
    game_over = 1;
    tetris_send_insert();
}

uint8_t tetris_tick(uint16_t ms_since_previous_tick) {
    if (game_over) {
        return 0;
    }
      
    timems += ms_since_previous_tick;
    uint16_t down_delay = 500 - score * 10;

    if (piece == 7) {
        score += clear_lines();
        seed = xorshift16(seed);
        timems = 0;
        next_down = down_delay;

        piece_x = WIDTH/2-2;
        piece_y = 0;
        piece = seed % 7;
        rotation = 0;
        // move piece up until its 1 above the board
        clear_board(BWORK);
        while ((set_piece(BWORK, piece_x, piece_y, piece, rotation) & 4) == 0) { 
            piece_y--;
        }
        piece_y++; // then move piece down once so the piece is at the top of the board

        if (!move_piece_to(piece_x, piece_y, piece, rotation)) {
            game_over = 1;
            send_goto_xy(WIDTH+2, HEIGHT/2);
            tetris_send_string(" score ");
            for (uint16_t tmp = 10000; tmp >= 10; tmp /= 10) {
                if (score >= tmp) {
                    uint16_t tmp2 = score / tmp;
                    send('0' + tmp2);
                    score -= tmp2 * tmp;
                }
            }
            send('0' + score);
            tetris_stop();
            return 0;
        }
        return 1;
    }

    if (timems > next_down) {
        if (!move_piece_to(piece_x, piece_y+1, piece, rotation)) {
            copy_board(BDRAW, BDONE);
            clear_board(BDRAW);
            piece = 7; // new piece
        }
        next_down += down_delay;
    } else {
        switch (tetris_get_keypress()) {
        case 1: { // up
            move_piece_to(piece_x, piece_y, piece, (rotation + 1) % 4);
            break;
        }
        case 2: // left
            move_piece_to(piece_x - 1, piece_y, piece, rotation);
            break;
        case 3: {// down
            int8_t newy = piece_y;
            clear_board(BWORK);
            do {
                newy++;
            } while (set_piece(BWORK, piece_x, newy, piece, rotation) && !hits());
            newy--;
            if (newy != piece_y) {
                move_piece_to(piece_x, newy, piece, rotation);
                next_down = timems + down_delay;
            }
            break;
        }
        case 4: // right
            move_piece_to(piece_x + 1, piece_y, piece, rotation);
            break;
        }
    }
    return 1;
}
