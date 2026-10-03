/* INPUT_XBOX.H: the gamepad state input_xbox.cpp keeps for each gamepad */

#ifndef INPUT_XBOX_H
#define INPUT_XBOX_H

#include "cseries.h"

enum
{
	k_gamepad_analog_button_count = 8,
	k_gamepad_button_count = 8,
	k_gamepad_thumbstick_axis_count = 4
};

struct gamepad_state
{
	byte analog_buttons[k_gamepad_analog_button_count];
	byte analog_button_thresholds[k_gamepad_analog_button_count];
	byte analog_button_frames_down[k_gamepad_analog_button_count];
	byte button_frames_down[k_gamepad_button_count];
	word analog_button_msec_down[k_gamepad_analog_button_count];
	word button_msec_down[k_gamepad_button_count];
	short thumbsticks[k_gamepad_thumbstick_axis_count];
};

gamepad_state const *input_get_gamepad_state(short gamepad_index);

#endif
