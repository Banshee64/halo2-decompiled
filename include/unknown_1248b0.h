/* UNKNOWN_1248B0.H: the gamepad state unknown_1248b0.cpp keeps for each gamepad */

#ifndef INPUT_XBOX_H
#define INPUT_XBOX_H

#include "unknown_11c920.h"

enum
{
	k_gamepad_analog_button_count = 8,
	k_gamepad_button_count = 8,
	k_gamepad_thumbstick_axis_count = 4
};

struct s_type_ff3a2a
{
	byte analog_buttons[k_gamepad_analog_button_count];
	byte analog_button_thresholds[k_gamepad_analog_button_count];
	byte analog_button_frames_down[k_gamepad_analog_button_count];
	byte button_frames_down[k_gamepad_button_count];
	word analog_button_msec_down[k_gamepad_analog_button_count];
	word button_msec_down[k_gamepad_button_count];
	short thumbsticks[k_gamepad_thumbstick_axis_count];
};

s_type_ff3a2a const *function_1249a0(short gamepad_index);

#endif
