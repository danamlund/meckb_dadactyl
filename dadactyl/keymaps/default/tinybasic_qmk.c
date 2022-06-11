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

#include QMK_KEYBOARD_H
#include "da_util.h"
#include "TinyBASIC.cpp"

static bool basic_inited = false;
static bool basic_running = false;
static bool dobreak = false;
static bool getln_started = false;
static bool getln_ready = false;
static uint8_t quickcalc = 0;
static uint8_t no_repeat_code = 0;

void basic_start(void) {
    if (!basic_inited) {
        basic_inited = true;
        init();
    }
    timer = timer_read();
    basic_running = true;
}

void basic_quickcalc(void) {
    quickcalc = 1;
    basic_start();
    loop();
    // store result in variable a
    txtpos[0] = 'a';
    txtpos++;
    txtpos[0] = '=';
    txtpos++;
}

bool basic_is_running(void) {
    return basic_running;
}

bool basic_register_code_user(uint8_t code) {
    if (keycodes_buffer_is_sending()) {
        return true;
    }

    /* do basic checks here and not process_record_user so we dont have
       to parse layers to see which keycode is actually being typed. */
    if (basic_running) {
        if (isrunning()) {
            if (code == KC_ESC) {
                dobreak = true;
            }
            return false;
        }
        if (getln_isstarted() && !getln_isready()) {
            if (code == KC_LSFT || code == KC_RSFT) {
                return true;
            }
            if (code == KC_ENT) {
                txtpos[0] = NL;
                getln_ready = true;

                if (quickcalc) {
                    do {
                        loop(); // run calc
                    } while (!getln_isstarted());
                    // then print the result which we stored in 'a'
                    txtpos[0] = '?';
                    txtpos++;
                    txtpos[0] = 'a';
                    txtpos++;
                    txtpos[0] = NL;
                    getln_ready = true;

                    SEND_STRING(" = ");
                    quickcalc = 2;
                    do {
                        loop(); // run calc
                    } while (!getln_isstarted());
                    quickcalc = 0;
                    basic_running = false;
                    txtpos[0] = NL;
                    getln_ready = true;
                    return false;
                }

                return true;
            } else if (code == KC_BSPC) {
                if (txtpos > program_end+sizeof(LINENUM)) {
                    txtpos--;
                    return true;
                } else {
                    return false;
                }
                return false;
            } else if (txtpos == variables_begin - 2) {
                return false;
            } else {
                uint16_t keycode;
                if (get_mods() & (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))) {
                    keycode = LSFT(code);
                } else {
                    keycode = code;
                }
                char c = keycode_to_ascii(keycode);
                if (c != 0) {
                    txtpos[0] = c;
                    txtpos++;
                    no_repeat_code = code;
                    return true;
                } else {
                    return false;
                }
            }
        } else {
            // in basic mode dont allow keys to move the cursor
            if (keycode_to_ascii(code) == 0) {
                return false;
            }
        }
    }
    return true;
}

void basic_matrix_scan_user(void) {
    if (basic_running) {
        if (no_repeat_code != 0) {
            // keyboard cannot detect auto-repeats because that is OS-defined.
            // So we disable auto-repeat by immediately basic keypresses.
            unregister_code(no_repeat_code);
            no_repeat_code = 0;
        }

        if (timer_elapsed(timer) > 50) {
            timer = timer_read();
            // execute 1 instruction every 50ms
            // so we notice ESC (break) presses
            if (!loop()) {
                // bye
                basic_running = false;
            }
        }
    }
}

void outchar(unsigned char c) {
    if (!quickcalc || (quickcalc == 2 && (c != '\n' && c != '>'))) {
        send_keycode(ascii_to_keycode(c));
    }
}

unsigned char breakcheck(void) {
    if (dobreak) {
        dobreak = false;
        return 1;
    } else {
        return 0;
    }
}

bool getln_isstarted(void) {
    return getln_started;
}
bool getln_isready(void) {
    return getln_ready;
}

void getln_start(char prompt) {
    outchar(prompt);
    txtpos = program_end+sizeof(LINENUM);
    getln_started = true;
    getln_ready = false;
}
void getln_consume(void) {
    getln_started = false;
}
