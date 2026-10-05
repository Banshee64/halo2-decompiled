// @flags /O2 /Ob1 /Gr
#include "unknown_11c920.h"
#include "network_message_types.h"

struct s_player_action_update
{
	long unknown00;
	long unknown04;
	dword player_mask;
	s_player_action actions[16];
};

// @retail 0x88060
bool function_88060(void *a, void *b)
{
	s_player_action_update *state = (s_player_action_update *)a;
	s_player_action_update *update = (s_player_action_update *)b;
	bool result = true;
	if (state->player_mask == update->player_mask)
	{
		for (long i = 0; i < 16; i++)
		{
			if (state->player_mask & (1 << i))
				result = result && function_87830(&state->actions[i], &update->actions[i]);
		}
	}
	else
	{
		result = false;
	}
	return result;
}
