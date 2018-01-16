#ifndef __ANALOG_STICK_INCLUDED__
#define __ANALOG_STICK_INCLUDED__

typedef struct {
    int x;
    int y;
} AnalogStickPosition;


typedef struct {
  AnalogStickPosition old;
  AnalogStickPosition delta;
  AnalogStickPosition pos;
  AnalogStickPosition center;

  int pin_y;
  int pin_x;
  int pin_button;

  int button_count;
  bool is_button_pressed;
  bool is_button_upped;
} AnalogStick;

#ifdef __cplusplus
extern "C" {
#endif

void analog_stick_setup(AnalogStick *stick, int pin_x, int pin_y, int pin_button);
void analog_stick_calibrate(AnalogStick *stick);

void analog_stick_update(AnalogStick *stick);

bool analog_stick_is_center(AnalogStick *stick);
bool analog_stick_is_position_changed(AnalogStick *stick);

bool inline analog_stick_is_button_pressed(AnalogStick *stick) { return stick->is_button_pressed; }
bool inline analog_stick_is_button_upped(AnalogStick *stick) { return stick->is_button_upped; }

#ifdef __cplusplus
}
#endif

#ifdef SERIAL
// After Serial.begin(9600); in initialization like a setup()
void analog_stick_show_positions_to_serial(AnalogStick *stick);
#endif

#endif
