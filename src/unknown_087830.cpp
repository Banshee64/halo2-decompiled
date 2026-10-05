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
