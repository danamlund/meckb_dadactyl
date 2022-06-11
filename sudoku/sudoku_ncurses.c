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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <curses.h>
#include <sys/time.h>

#include "sudoku.c"

static WINDOW *mainwin;

static int idx = 0;
static char area[10000] = { 0 };
static bool insert_enabled = false;

void initlog() {
  FILE *f = fopen("log.txt", "w");
  fclose(f);
}
void d(char *format, ...) {
  FILE *f = fopen("log.txt", "a");

  va_list arglist;
  va_start( arglist, format );
  vfprintf( f, format, arglist );
  va_end( arglist );
  
  fprintf(f, "\n");
  fclose(f);
}
void drow(char *msg, const char row[5]) {
  FILE *f = fopen("log.txt", "a");
  fprintf(f, "%s: %d%d%d%d%d%d%d\n", msg,
          row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
  fclose(f);
}
void dpiece(char piece[7][7]) {
  FILE *f = fopen("log.txt", "a");
  for (int y = 0; y < 7; y++) {
    fprintf(f, "  ");
    for (int x = 0; x < 7; x++) {
      if (piece[y][x]) {
        fprintf(f, "x");
      } else {
        fprintf(f, ".");
      }
    }
    fprintf(f, "\n");
  }
  fclose(f);
}
void dboard(char board[20][10]) {
  FILE *f = fopen("log.txt", "a");
  for (int y = 0; y < 20; y++) {
    fprintf(f, "  ");
    for (int x = 0; x < 10; x++) {
      if (board[y][x]) {
        fprintf(f, "x");
      } else {
        fprintf(f, ".");
      }
    }
    fprintf(f, "\n");
  }
  fclose(f);
}

static void sleepms(int ms) {
  usleep(ms * 1000);
}

static void die(char *msg) {
  mvaddstr(0, 15, msg);
  refresh();
  while (true);

  delwin(mainwin);
  endwin();
  refresh();
  fprintf(stderr, "%s\n", msg);
  exit(1);
}

static int getx() {
  int x = 0;
  for (int i = idx - 1; i >= 0; i--) {
    if (area[i] == '\n') {
      break;
    }
    x++;
  }
  return x;
}

static int gety() {
  int y = 0;
  for (int i = idx; i >= 0; i--) {
    if (area[i] == '\n') {
      y++;
    }
  }
  return y;
}

static void redraw() {
  sleepms(10);
  /* sleepms(500); */
  /* d("%s<%d %d,%d", area, idx, getx(), gety()); */

  erase();
  int y = 0;
  move(y, 0);
  for (int i = 0; area[i] != 0; i++) {
    if (area[i] == '\n') {
      y++;
      move(y, 0);
    } else {
      addch(area[i]);
    }
  }
  
  move(gety(), getx());
  refresh();
}

static void add_space(int len) {
  for (int i = strlen(area); i >= idx; i--) {
    area[i + len] = area[i];
  }
}
static void remove_space(int len) {
  for (int i = idx; area[i] != 0; i++) {
    area[i] = area[i + len];
  }
}

static void send_up() {
  int x = getx();

  if (idx <= x) {
    die("too much up");
  }
  
  idx -= x; // to end line
  idx--; // to end of prev line
  idx -= getx(); // to start of prev line
  idx += x; // to same x as old line
  redraw();
}

static void send_left() {
  if (idx <= 0) {
    die("too much left");
  }
  idx--;
  redraw();
}

static void send_down() {
  int x = getx();

  while (area[idx] != '\n' && area[idx] != 0) {
    idx++;
  }

  if (area[idx] == 0) {
    die("too much down");
  }

  idx++; // to next line

  // move to same x or end of line
  for (int i = 0; i < x && area[idx] != '\n' && area[idx]; i++) {
    idx++;
  }
  
  redraw();
}

static void send_right() {
  if (area[idx] == 0) {
    die("too much right");
  }
  if (area[idx] == '\n') {
    die("right to move to next line");
  }
  idx++;
  redraw();
}

static void send_delete() {
  remove_space(1);
  redraw();
}
static void send_insert() {
    insert_enabled = !insert_enabled;
}

static void send(char c) {
    if (insert_enabled && area[idx] != '\n') {
        area[idx] = c;
        idx++;
    } else {
        add_space(1);
        area[idx] = c;
        idx++;
    }
  redraw();
}

void send_newline() {
  send('\n');
}

uint32_t timer_read32() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_usec / 1000 + t.tv_sec * 1000;
}

int main(int argc, char **args) {
  initlog();
  if ( (mainwin = initscr()) == NULL ) {
    fprintf(stderr, "Error initializing ncurses.\n");
    exit(EXIT_FAILURE);
  }

  cbreak();
  noecho();
  nodelay(mainwin, 0);
  keypad(mainwin, 1);

  //start(0x9a);
  srand(time(NULL));
  //start(random() & 0xFFFF);
  start(1);

  while (true) {
      bool not_lost = true;
      switch (getch()) {
      case KEY_UP: get_up(); break;
      case KEY_LEFT: get_left(); break;
      case KEY_DOWN: get_down(); break;
      case KEY_RIGHT: get_right(); break;
      case '1': get_number(1); break;
      case '2': get_number(2); break;
      case '3': get_number(3); break;
      case '4': get_number(4); break;
      case '5': get_number(5); break;
      case '6': get_number(6); break;
      case '7': get_number(7); break;
      case '8': get_number(8); break;
      case '9': get_number(9); break;
      case 'q': goto loop; // quit
      }
      if (!not_lost) {
          break;
      }
  }
loop:

  getch();
  
  delwin(mainwin);
  endwin();
  refresh();

  return 0;
}
