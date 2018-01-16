//#include <Arduino.h>
#include <analog.h>
#include <pincontrol.h>
#include "pro_micro.h"

#include "analog_stick.h"

#define StickThreshold 10
#define StickCenterThreshold 10

#define CenterX 512;
#define CenterY CenterX
#define MaxX 1023
#define MaxY MaxX

#define MaxButtonCount 255


// from Arduino.h
#define INPUT_PULLUP 0x2
#define LOW 0
#define abs(x) ((x)>0?(x):-(x))


static void analog_stick_button_check(AnalogStick *stick);

void analog_stick_setup(AnalogStick *stick, int pin_x, int pin_y, int pin_button) {
    stick->pin_x = pin_x;
    stick->pin_y = pin_y;
    stick->pin_button = pin_button;

    stick->is_button_pressed = false;
    stick->is_button_upped = false;

    pinMode(pin_button, INPUT_PULLUP);

    analog_stick_calibrate(stick);
}


void analog_stick_calibrate(AnalogStick *stick) {
    stick->center.x = analogRead(stick->pin_x);
    stick->center.y = analogRead(stick->pin_y);
    stick->pos.x = stick->pos.y = 0;
    stick->old = stick->pos;
}


void analog_stick_update(AnalogStick *stick) {
    stick->old = stick->pos;

    // アナログスティックのX,Y座標
    stick->pos.x = analogRead(stick->pin_x) - stick->center.x;
    stick->pos.y = analogRead(stick->pin_y) - stick->center.y;

    stick->delta.x = stick->pos.x - stick->old.x;
    stick->delta.y = stick->pos.y - stick->old.y;

    analog_stick_button_check(stick);
}


bool analog_stick_is_center(AnalogStick *stick) {
  return (abs(stick->pos.x) <= StickCenterThreshold)
    && (abs(stick->pos.y) <= StickCenterThreshold);
}


bool analog_stick_is_position_changed(AnalogStick *stick) {
  return (StickThreshold <= abs(stick->delta.x))
    || (StickThreshold <= abs(stick->delta.y));
}


static void analog_stick_button_check(AnalogStick *stick) {
  stick->is_button_pressed = false;
  stick->is_button_upped = false;

  // ボタン押下チェック
  if (digitalRead(stick->pin_button) == LOW) {
    if (stick->button_count == 0) {
      stick->is_button_pressed = true;
    }
    if (stick->button_count < MaxButtonCount) {
      stick->button_count++;
    }
  } else {
    if (0 < stick->button_count) {
      stick->is_button_upped = true;
    }
    stick->button_count = 0;
  }
}

#ifdef SERIAL
void analog_stick_show_positions_to_serial(AnalogStick *stick) {
  Serial.print("AnalogStick(x,y): ");
  Serial.print(stick->pos.x); Serial.print(", "); Serial.print(stick->pos.y);
  Serial.print(" ("); Serial.print(stick->delta.x); Serial.print(", "); Serial.print(stick->delta.y); Serial.print(")");
  Serial.println("");
}
#endif

