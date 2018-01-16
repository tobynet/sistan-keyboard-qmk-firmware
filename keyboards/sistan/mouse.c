#include "mousekey.h"
#include "quantum.h"

#include "mouse.h"


// Dynamic wait_ms for AVR
static void wait_ms_(uint32_t wait) {
  while (0 < wait) {
    wait_ms(1);
    --wait;
  }
}

// Emulate mouse click
void send_mouse_click_with_wait(uint32_t wait) {
  mousekey_on(KC_MS_BTN1);
  mousekey_send();
  wait_ms_(wait);

  mousekey_off(KC_MS_BTN1);
  mousekey_send();
  wait_ms_(wait);
}

// Emulate mouse click
void send_mouse_click(void) {
  mousekey_on(KC_MS_BTN1);
  mousekey_send();
  wait_ms(100);

  mousekey_off(KC_MS_BTN1);
  mousekey_send();
  wait_ms(100);
}

// Emulate mouse down
void send_mouse_down(uint8_t code) {
  mousekey_on(code);
  mousekey_send();
}

// Emulate mouse up
void send_mouse_up(uint8_t code) {
  mousekey_off(code);
  mousekey_send();
}


// Emulate mouse move
// ...mousekey is not suitable for this application
void send_mouse_move(int8_t delta_x, int8_t delta_y) {
  report_mouse_t m = {};
  m.x = delta_x;
  m.y = delta_y;
  host_mouse_send(&m);
}
