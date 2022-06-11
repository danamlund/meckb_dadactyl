MCU = atmega32u4
BOOTLOADER = caterina

BOOTMAGIC_ENABLE ?= no      # Virtual DIP switch configuration(+1000)
MOUSEKEY_ENABLE ?= no       # Mouse keys(+4700)
EXTRAKEY_ENABLE ?= no       # Audio control and System control(+450)
CONSOLE_ENABLE ?= no        # Console for debug(+400)
COMMAND_ENABLE ?= no        # Commands for debug and configuration
SLEEP_LED_ENABLE ?= no       # Breathing sleep LED during USB suspend
# if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
NKRO_ENABLE ?= no            # USB Nkey Rollover
BACKLIGHT_ENABLE ?= no       # Enable keyboard backlight functionality on B7 by default
MIDI_ENABLE ?= no            # MIDI support (+2400 to 4200, depending on config)
UNICODE_ENABLE ?= no         # Unicode
BLUETOOTH_ENABLE ?= no       # Enable Bluetooth with the Adafruit EZ-Key HID
AUDIO_ENABLE ?= no           # Audio output on port C6
FAUXCLICKY_ENABLE ?= no      # Use buzzer to emulate clicky switches
RGBLIGHT_ENABLE ?= no      # Enable RGB light

SPLIT_KEYBOARD = yes
