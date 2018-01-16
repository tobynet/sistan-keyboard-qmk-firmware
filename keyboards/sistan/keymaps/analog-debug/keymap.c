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
#include "../../sistan.h"

#include "pro_micro.h"
#include "analog.h"
#include "print.h"


// Define custom keycods for special keys
enum custom_keycodes {
  // Joy Buttons
   MACRO_ANALOG_PRINT = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[0] = KEYMAP( /* Base */
  MACRO_ANALOG_PRINT,   KC_MS_UP,    RESET, \
  KC_MS_LEFT,           KC_MS_DOWN,  KC_MS_RIGHT \
),
};

static void print_analog_pins(void);


void matrix_init_user(void) {

}

#define PRINT_INTERVAL 5000
void matrix_scan_user(void) {
  // debugging
  static int16_t counter = PRINT_INTERVAL;

  counter++;
  if (PRINT_INTERVAL < counter) {
    print_val_decs(counter);
    print_analog_pins();
    counter = 0;
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {

    case MACRO_ANALOG_PRINT:
      // For right next button
      if (record->event.pressed) {
        print_analog_pins();
      }
      return false;
      break;
  }

  return true;
}

void led_set_user(uint8_t usb_led) {

}

//#define MAX_ANALOG_PINS 8
#define MAX_ANALOG_PINS NUM_ANALOG_INPUTS
static void print_analog_pins(void) {
//  static const uint8_t PROGMEM pins[MAX_ANALOG_PINS] = { A0 , A1, A2, A3, A4, A5, A6, A7 };
	static const uint8_t PROGMEM pin_to_mux[] = {
		0x00, 0x01, 0x04, 0x05, 0x06, 0x07,
		0x25, 0x24, 0x23, 0x22, 0x21, 0x20};

  println("Analog pins:");

  // readAnalog test A1(x)
  uint8_t pin = 4;
  int16_t x = adc_read(pgm_read_byte(pin_to_mux + pin));
  print("  X: ("); print_dec(pgm_read_byte(pin_to_mux + pin)); print(") "); print_val_decs(x);

  // readAnalog test A0(y)
  pin = 5;
  int16_t y = adc_read(pgm_read_byte(pin_to_mux + pin));
  print("  Y: ("); print_dec(pgm_read_byte(pin_to_mux + pin)); print(") "); print_val_decs(y);
  println();

  // for loop
  for (uint16_t i = 0; i < MAX_ANALOG_PINS; i++) {
//    int16_t val = analogRead(pins[i]);
    int16_t val = analogRead(i);
    print("  "); print_dec(i); print(" ");
    //print_dec(pins[i]); print(" ");
    print_decs(val); println();
  }
  println();

}
