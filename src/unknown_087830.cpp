// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_087830.CPP: whether an update of the simulation's state is within
   one quantum of the state it would replace (lane D) */

#include "unknown_11c920.h"
#include <math.h>
#include "network_message_types.h"

/* the quantized values compared here */
struct s_simulation_quantized_state
{
	long unknown00;
	real angle04;
	real angle08;
	real value0c;
	real value10;
	real value14;
	real value18;
	byte unknown1c[0x50 - 0x1c];
	real value50;
	real value54;
};

/* a player action: takes the update's values when every one of them is within a quantum of
   the state's (the first angle also across the turn) */
// @retail 0x87830
bool function_87830(s_player_action *new_action, s_player_action *action)
{
	s_simulation_quantized_state *state = (s_simulation_quantized_state *)action;
	s_simulation_quantized_state const *update = (s_simulation_quantized_state const *)new_action;
	bool result;
	real difference = (real)fabs(state->angle04 - update->angle04);
	if ((0.00076708407f > difference || 0.00076708407f > (real)fabs(difference - 6.2831855f)) &&
		0.0015343555f > (real)fabs(state->angle08 - update->angle08) &&
		0.06666667f > (real)fabs(state->value0c - update->value0c) &&
		0.06666667f > (real)fabs(state->value10 - update->value10) &&
		0.032258064f > (real)fabs(state->value14 - update->value14) &&
		0.032258064f > (real)fabs(state->value18 - update->value18) &&
		0.06666667f > (real)fabs(state->value50 - update->value50) &&
		0.06666667f > (real)fabs(state->value54 - update->value54))
	{
		result = true;
		state->angle04 = update->angle04;
		state->angle08 = update->angle08;
		state->value0c = update->value0c;
		state->value10 = update->value10;
		state->value14 = update->value14;
		state->value18 = update->value18;
		state->value50 = update->value50;
		state->value54 = update->value54;
	}
	else
	{
		result = false;
	}
	return result;
}

#include "unknown_067e10.h"
#include "bitstream.h"
#include <string.h>

bool function_07ca70(s_bitstream *stream, void *destination);
void function_07c5a0(s_bitstream *stream, const void *source);

// @retail 0x87930
void function_87930(s_bitstream *stream, const s_simulation_player_update *update)
{
	stream_write_checked(stream, update->player_index, 4);
	function_1955d0(stream, update->key, 96);
	stream_write_checked(stream, update->type, 3);
	if (update->type == 3)
	{
		stream_write_bit(stream, update->field_2_2);
		if (!update->field_2_2)
		{
			function_1955d0(stream, &update->machine, 48);
			stream_write_checked(stream, update->controller_index, 2);
			stream_write_checked(stream, update->unknown20, 2);
		}
	}
	if (update->type == 3 || update->type == 4)
		function_07c5a0(stream, update->configuration);
	if (update->type == 1)
	{
		stream_write_checked(stream, update->other_player_index, 4);
		function_1955d0(stream, update->other_key, 96);
	}
}

// @retail 0x87ac0
bool function_87ac0(s_bitstream *stream, s_simulation_player_update *update)
{
	bool result = true;
	update->player_index = function_1959c0(stream, 4);
	function_195820(stream, update->key, 96);
	update->type = function_1959c0(stream, 3);
	if (update->player_index < 0 || update->player_index >= 16 || update->type < 0 || update->type >= 5)
		goto failed;
	if (update->type == 3)
	{
		update->field_2_2 = function_1957d0(stream);
		if (!update->field_2_2)
		{
			function_195820(stream, &update->machine, 48);
			update->controller_index = function_1959c0(stream, 2);
			update->unknown20 = function_1959c0(stream, 2);
			result = update->controller_index >= 0 && update->controller_index < 4 &&
				update->unknown20 >= 0 && update->unknown20 < 4;
		}
		else
		{
			memset(&update->machine, 0, sizeof(update->machine));
			update->controller_index = NONE;
			update->unknown20 = NONE;
		}
	}
	if (update->type == 3 || update->type == 4)
		result = result && function_07ca70(stream, update->configuration);
	if (update->type == 1)
	{
		update->other_player_index = function_1959c0(stream, 4);
		function_195820(stream, update->other_key, 96);
		if (!result || update->other_player_index < 0 || update->other_player_index >= 16 ||
			update->other_player_index == update->player_index)
			goto failed;
		return true;
	}
	goto done;
failed:
	return false;
done:
	return result;
}
