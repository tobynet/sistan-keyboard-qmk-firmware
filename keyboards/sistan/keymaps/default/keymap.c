/* Copyright 2018 tobynet
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
#include "sistan.h"
#include "mousekey.h"

// SEND_STRING with delay
#define SEND_STRING_DELAY(str, interval) send_string_with_delay_P(PSTR(str), interval)

// Define custom keycods for special keys
enum custom_keycodes {
   MACRO_PLUS_X = SAFE_RANGE,
   MACRO_SISTAN,
   MACRO_X_TODAY,
   MACRO_DEL_ALL,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[0] = KEYMAP( /* Base */
  MACRO_DEL_ALL,    KC_MS_BTN1,     RESET, \
  MACRO_PLUS_X,     MACRO_SISTAN,   MACRO_X_TODAY \
),
};


void matrix_init_user(void) {

}

void matrix_scan_user(void) {

}


// Emulate mouse click
static void send_mouse_click(void) {
  mousekey_on(KC_MS_BTN1);
  mousekey_send();
  wait_ms(100);

  mousekey_off(KC_MS_BTN1);
  mousekey_send();
  wait_ms(100);
}

// Emulate mouse move
// ...mousekey is not suitable for this application
static void send_mouse_move(int8_t delta_x, int8_t delta_y) {
  report_mouse_t m = {};
  m.x = delta_x;
  m.y = delta_y;
  host_mouse_send(&m);
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case MACRO_PLUS_X:
      if (record->event.pressed) {
        // Input "あなたと"
        // X_GRAVE is an alternative code of X_ZKHK(Zenkaku/Hankaku)
        SEND_STRING (SS_TAP(X_GRAVE)
          "anatato" SS_TAP(X_F6)
          SS_TAP(X_ENTER) SS_TAP(X_GRAVE));
      }
      return false;
      break;


    case MACRO_SISTAN:
      if (record->event.pressed) {
        // Input "ジスたん、"
        // X_GRAVE is an alternative code of X_ZKHK(Zenkaku/Hankaku)
        SEND_STRING (SS_TAP(X_GRAVE)
          "jisu" SS_TAP(X_F7)
          "tan," SS_TAP(X_F6)
          SS_TAP(X_ENTER) SS_TAP(X_GRAVE)
          SS_TAP(X_ENTER)
        );
      }
      return false;
      break;


    case MACRO_X_TODAY:
      if (record->event.pressed) {
        // Input "今すぐダ"
        // X_GRAVE is an alternative code of X_ZKHK(Zenkaku/Hankaku)
        SEND_STRING (SS_TAP(X_GRAVE)
          "imasugu "
          "dau" SS_TAP(X_F7)
          SS_TAP(X_ENTER) SS_TAP(X_GRAVE));

        wait_ms(2400);

        // Erase strings...
        SEND_STRING_DELAY (SS_TAP(X_BSPACE) SS_TAP(X_BSPACE), 200);

        wait_ms(800);

        // Input "アッパー"
        // X_GRAVE is an alternative code of X_ZKHK(Zenkaku/Hankaku)
        SEND_STRING_DELAY (SS_TAP(X_GRAVE)
          "appa-" SS_TAP(X_F7)
          SS_TAP(X_ENTER) SS_TAP(X_GRAVE)
          SS_TAP(X_ENTER),
          160);
      }
      return false;
      break;


    case MACRO_DEL_ALL:
      if (record->event.pressed) {
        const int delta_x = 20;
        const int short_wait = 100;

        // Test for mouse motion
        send_mouse_move(delta_x, delta_x);

        // Select all text (for VSCode)
        send_mouse_click();
        send_mouse_click();
        send_mouse_click();
        send_mouse_click();

        wait_ms(short_wait);

        // Test for mouse motion
        send_mouse_move(-delta_x, 0);
        wait_ms(short_wait);
        send_mouse_move(0, -delta_x);

        // Select all text (insurance for notepad.exe)
        SEND_STRING (SS_LCTRL("a"));

        wait_ms(short_wait);

        // Delete text
        SEND_STRING (SS_TAP(X_DELETE));
      }
      return false;
      break;
  }

  return true;
}

void led_set_user(uint8_t usb_led) {

}

