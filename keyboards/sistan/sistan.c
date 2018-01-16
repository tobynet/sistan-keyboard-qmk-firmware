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
#include <stdbool.h>
#include <math.h>
#include "sistan.h"

//#include "analog_stick/analog_stick.h"

#include "pro_micro.h"

#include "analog.h"
#include "print.h"
//#include "pointing_device.h"
//#include "mouse.h"

#define ANALOG_X_PIN A0 //A0
#define ANALOG_Y_PIN A1 //A1
#define ANALOG_BUTTON_IN B1
#define ANALOG_STICK_CENTER_THRESHOLD 10
#define ANALOG_STICK_CHANGED_THRESHOLD 10



// // Scan Analog Stick as mouse
// void scan_analog_stick(void) {
// 	static int counter = ANALOG_STICK_POLLING_INTERVAL;
// 	counter++;

// 	if (0 < counter) {
// 		counter = 0;
// 	}
// }


typedef struct {
    int16_t x;
    int16_t y;
} AnalogStickPosition;

typedef struct {
  AnalogStickPosition raw_pos;
  AnalogStickPosition old;
  AnalogStickPosition delta;
  AnalogStickPosition pos;
  AnalogStickPosition center;

  uint8_t pin_y;
  uint8_t pin_x;
  uint8_t pin_button;

  int16_t button_count;
  bool is_button_pressed;
  bool is_button_upped;

  uint16_t center_threshold;
  uint16_t changed_threshold;
} AnalogStick;


void analog_stick_print_all(AnalogStick *stick) {
  println("AnalogStick:");
  print("  "); print_val_dec(stick->pin_x);
  print("  "); print_val_dec(stick->pin_y);
  print("  "); print_val_dec(stick->pin_button);
  println();
  print("  "); print_val_decs(stick->raw_pos.x);
  print("  "); print_val_decs(stick->raw_pos.y);
  print("  "); print_val_decs(stick->pos.x);
  print("  "); print_val_decs(stick->pos.y);
  print("  "); print_val_decs(stick->delta.x);
  print("  "); print_val_decs(stick->delta.y);
  print("  "); print_val_decs(stick->center.x);
  print("  "); print_val_decs(stick->center.y);
  println();
  print("  "); print_val_dec(stick->center_threshold);
  print("  "); print_val_dec(stick->changed_threshold);
  println();
}

void analog_stick_print_status(AnalogStick *stick) {
  print("AnalogStick(x,y): ");
  print_decs(stick->pos.x); print(", "); print_decs(stick->pos.y);
  print(" ("); print_decs(stick->delta.x); print(", "); print_decs(stick->delta.y); print(")");
  println();
}

void analog_stick_calibrate(AnalogStick *stick) {
    stick->center.x = analogRead(stick->pin_x);
    stick->center.y = analogRead(stick->pin_y);
    stick->pos.x = stick->pos.y = 0;
    stick->old = stick->pos;
}

void analog_stick_setup(AnalogStick *stick, uint8_t pin_x, uint8_t pin_y, uint8_t pin_button,
    uint16_t center_threshold, uint16_t changed_threshold) {
  stick->pin_x = pin_x;
  stick->pin_y = pin_y;
  stick->pin_button = pin_button;
  stick->is_button_pressed = false;
  stick->is_button_upped = false;
  stick->center_threshold = center_threshold;
  stick->changed_threshold = changed_threshold;

  analog_stick_calibrate(stick);
}

void analog_stick_update(AnalogStick *stick) {
  stick->old = stick->pos;

  // Fetch analog position
  stick->raw_pos.x = analogRead(stick->pin_x);
  stick->raw_pos.y = analogRead(stick->pin_y);
  stick->pos.x = stick->raw_pos.x - stick->center.x;
  stick->pos.y = stick->raw_pos.y - stick->center.y;

  stick->delta.x = stick->pos.x - stick->old.x;
  stick->delta.y = stick->pos.y - stick->old.y;

  //print_analog_stick(x, y);
}

bool analog_stick_is_center(AnalogStick *stick) {
  return (abs(stick->pos.x) < stick->center_threshold)
    && (abs(stick->pos.y) < stick->center_threshold);
}

bool analog_stick_is_position_changed(AnalogStick *stick) {
  return (stick->changed_threshold <= abs(stick->delta.x))
    || (stick->changed_threshold <= abs(stick->delta.y));
}



void matrix_init_kb(void) {
	// put your keyboard start-up code here
	// runs once when the firmware starts up
  debug_enable = true;
  debug_matrix = true;
  debug_mouse = true;

  TX_RX_LED_INIT;

//  pointing_device_init();
	matrix_init_user();
}


void matrix_scan_kb(void) {
	// put your looping keyboard code here
	// runs every cycle (a lot)
	matrix_scan_user();

//  pointing_device_task();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
	// put your per-action keyboard code here
	// runs for every action, just before processing by the firmware

	return process_record_user(keycode, record);
}

void led_set_kb(uint8_t usb_led) {
	// put your keyboard LED indicator (ex: Caps Lock LED) toggling code here

	led_set_user(usb_led);
}


// static AnalogStick stick = {};

// // Init Analog Stick for mouse
// void pointing_device_init(void) {
//   RXLED1;
//   println("pointing_device_init...");
//   wait_ms(200);
//   analog_stick_setup(&stick, ANALOG_X_PIN, ANALOG_Y_PIN, ANALOG_BUTTON_IN,
//     ANALOG_STICK_CENTER_THRESHOLD, ANALOG_STICK_CHANGED_THRESHOLD);
//   analog_stick_print_all(&stick);
//   println("pointing_device_init done.");
//   RXLED0;
// }

// // Scan Analog Stick for mouse
// void pointing_device_task(void) {
//   static int counter = ANALOG_STICK_POLLING_INTERVAL;

//   if (counter < ANALOG_STICK_POLLING_INTERVAL) {
//     counter++;
//     return;
//   }
//   counter = 0;

//   analog_stick_update(&stick);

//   //analog_stick_print_all(&stick);

// //  if ((!analog_stick_is_center(&stick)) && analog_stick_is_position_changed(&stick)) {
//   if (!analog_stick_is_center(&stick)) {
//     TXLED1;
//     AnalogStickPosition d = stick.delta;
//     // int8_t dx = 0;
//     // int8_t dy = 0;
//     // if (abs(d.x) > 0) {
//     //   dx = d.x < 0 ? -1 : +1;
//     // }
//     // send_mouse_move(dx, dy);
//     send_mouse_move(d.x, d.y);
//     //analog_stick_print_status(&stick);
//     analog_stick_print_all(&stick);
//   } else {
//     TXLED0;
//   }
// }
