#pragma once
#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6061
#define DEVICE_VER      0x0002
#define MANUFACTURER    danamlund
#define PRODUCT         dadactyl
#define DESCRIPTION     dactyl

/* key matrix size */
#define MATRIX_ROWS (4*2)
#define MATRIX_COLS 8

// https://cdn.sparkfun.com/datasheets/Dev/Arduino/Boards/ProMicro16MHzv1.pdf
// wiring of each half
#define MATRIX_ROW_PINS { B5, B4, E6, D7 } // 9, 8, 7, 6
#define MATRIX_COL_PINS { B6, B2, B3, B1, F7, F6, F5, F4 } // 10, 16, 14, 15, A0, A1, A2, A3

/* COL2ROW, ROW2COL, or CUSTOM_MATRIX */
#define DIODE_DIRECTION COL2ROW
 
#define DEBOUNCE 5

/* serial.c configuration for split keyboard */
#define SOFT_SERIAL_PIN D0 // 3

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE
