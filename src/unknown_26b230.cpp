// @flags /O2 /Gr /arch:SSE2
#include "cseries.h"
#include "unknown_26b230.h"
#include "globals.h"
#include "data_array.h"
#include "slot_handler.h"
#include "unknown_20f040.h"


typedef bool (__stdcall *t_transition_test)(long, real *, real *);
typedef short (__stdcall *t_state_update)(long, real *, real *);

struct s_clump_state_entry
{
	t_state_update update;
	dword unknown4;
	t_transition_test test;
};

short __stdcall function_26b630(long clump_index, real *b, real *a);
short __stdcall function_26b660(long clump_index, real *b, real *a);
short __stdcall function_26b6b0(long clump_index, real *b, real *a);
bool __stdcall function_26b6e0(long clump_index, real *b, real *a);

s_clump_state_entry g_470fb4[4] =
{
	{function_26b630, 0, 0},
	{function_26b660, 0, 0},
	{function_26b6b0, 0, 0},
	{0, 0, function_26b6e0},
};

// @retail 0x26b230
long function_26b230(long clump_index, long prop_index)
{
	s_clump *clump = (s_clump *)(g_502420->data + (clump_index & 0xffff) * sizeof(s_clump));
	long result = NONE;
	long index = clump->first_prop;

	while (index != NONE)
	{
		s_clump_prop *prop = (s_clump_prop *)(g_50241c->data + (index & 0xffff) * sizeof(s_clump_prop));
		if (prop->type == prop_index)
		{
			result = index;
			break;
		}
		index = prop->next;
	}

	return result;
}

// @retail 0x26b290
void function_26b290(long clump_index)
{
	s_clump *clump = (s_clump *)(g_502420->data + (clump_index & 0xffff) * sizeof(s_clump));
	real a[13];
	real b[13];
	long object_index = clump->first_object;
	long i;

	for (i = 0; i < 13; i++)
	{
		a[i] = 0.0f;
		b[i] = 0.0f;
	}

	for (;;)
	{
		if (object_index == NONE)
		{
			break;
		}
		s_clump_object *object = (s_clump_object *)(g_4f55f0->data + (object_index & 0xffff) * sizeof(s_clump_object));
		object_index = object->next;
		short count = object->count;

		for (i = 0; i <= count; i++)
		{
			a[i] += 1.0;
		}
		b[count] += 1.0;
	}

	if (clump->divisor > 0)
	{
		for (i = 0; i < 13; i++)
		{
			b[i] /= clump->divisor;
			a[i] /= clump->divisor;
		}
	}

	short state;
	short new_state = NONE;
	for (state = 3; clump->state < state; state--)
	{
		t_transition_test test = g_470fb4[state].test;
		if (test && test(clump_index, b, a))
		{
			new_state = state;
			break;
		}
	}
	if (new_state == NONE)
	{
		new_state = g_470fb4[clump->state].update(clump_index, b, a);
	}

	if (new_state != NONE && new_state != clump->state)
	{
		clump->state_time = g_510c54->game_time;
		clump->state = new_state;
	}
}

// @retail 0x26b630
short __stdcall function_26b630(long clump_index, real *b, real *a)
{
	s_clump *clump = (s_clump *)(g_502420->data + (clump_index & 0xffff) * sizeof(s_clump));
	clump->unknown3e = 0;
	return NONE;
}

// @retail 0x26b660
short __stdcall function_26b660(long clump_index, real *b, real *a)
{
	s_clump *clump = (s_clump *)(g_502420->data + (clump_index & 0xffff) * sizeof(s_clump));
	if ((real)(g_510c54->game_time - clump->state_time) * g_510c54->rate > 1.0f)
	{
		clump->unknown30 = 0;
		return 0;
	}
	return NONE;
}

// @retail 0x26b6b0
short __stdcall function_26b6b0(long clump_index, real *b, real *a)
{
	short result = NONE;
	if (a[2] == 0.0f)
	{
		result = 1;
	}
	return result;
}

// @retail 0x26b6e0
bool __stdcall function_26b6e0(long clump_index, real *b, real *a)
{
	return a[3] > 0.5;
}

/* ---- whether a clump may use a prop ---- */
struct s_clump_prop_object
{
	byte unknown000[0x10a];
	word unknown10a_0 : 2;
	word flag10a_2 : 1;
	word unknown10a_3 : 13;
};

struct s_clump_prop_object_header
{
	byte unknown00[8];
	s_clump_prop_object *object;
};

s_ai_player *ai_player_get(long player_index);

// @retail 0x26b8a0
bool function_26b8a0(long prop_index, long actor_index)
{
	s_clump_prop *prop = (s_clump_prop *)(g_50241c->data + (prop_index & 0xffff) * sizeof(s_clump_prop));
	long node_index = prop->first_node;
	bool result = false;

	while (node_index != NONE)
	{
		s_clump_node *node = (s_clump_node *)(g_502418->data + (node_index & 0xffff) * sizeof(s_clump_node));
		node_index = node->next;
		if (node->state < 1 || node->state > 2 || node->actor_index == actor_index)
			continue;

		result = true;
		break;
	}

	return result;
}

// @retail 0x26b900
long function_26b900(long prop_index, long actor_index, long clump_index)
{
	bool result = function_26b8a0(prop_index, actor_index);

	if (!result)
	{
		s_clump *clump = (s_clump *)(g_502420->data + (clump_index & 0xffff) * sizeof(s_clump));
		long side = function_20f040(clump->team);
		s_clump_prop *prop = (s_clump_prop *)(g_50241c->data + (prop_index & 0xffff) * sizeof(s_clump_prop));
		s_record_pool_iterator iterator;
		s_clump *other;

		if (g_4f55d0->active)
		{
			iterator.data = g_502420;
			iterator.index = NONE;
		}

		while (g_4f55d0->active && (other = (s_clump *)data_iterator_next_inlined(&iterator)) != NULL)
		{
			if (clump_index != iterator.datum_index && function_20f040(other->team) == side)
			{
				long other_prop_index = function_26b230(iterator.datum_index, prop->type);
				if (other_prop_index != NONE && function_26b8a0(other_prop_index, NONE))
					return true;
			}
		}
	}

	return result;
}

// @retail 0x26ba60
bool function_26ba60(long prop_index, long actor_index, long clump_index)
{
	s_clump_prop *prop = (s_clump_prop *)(g_50241c->data + (prop_index & 0xffff) * sizeof(s_clump_prop));
	bool result = false;

	if (prop->unknown34)
		return result;

	if (TEST_FIELD_BIT(((s_clump_prop_object_header *)g_4e0300->data)[prop->type & 0xffff].object->flag10a_2))
		return result;

	if (clump_index == NONE)
		return result;

	s_clump *clump = (s_clump *)(g_502420->data + (clump_index & 0xffff) * sizeof(s_clump));
	if (!team_is_enemy(clump->team, 1))
	{
		s_game_time_globals *game_time_globals = g_510c54;
		long game_time = game_time_globals->game_time;
		s_data_datum_iterator players;
		bool ready = true;

		players.data = g_4e8c24;
		players.index = NONE;
		players.datum_index = NONE;
		while (data_datum_iterator_next(&players))
		{
			s_ai_player *player = ai_player_get(players.datum_index);
			if (player && game_time - player->unknown0c < game_time_globals->field_2_3)
				ready = false;
		}

		if (ready)
		{
			s_iterator actors;

			function_26bda0(clump_index, &actors);
			while (actors.next != NONE)
			{
				s_clump_object *actor = (s_clump_object *)(g_4f55f0->data + (actors.next & 0xffff) * sizeof(s_clump_object));
				actors.next = actor->next;
				if (actor->node_index != NONE)
				{
					s_clump_node *node = (s_clump_node *)(g_502418->data + (actor->node_index & 0xffff) * sizeof(s_clump_node));
					if (node->state < 1 || node->state > 2)
						continue;

					return false;
				}
			}
		}

		return ready;
	}

	return !function_26b900(prop_index, actor_index, clump_index);
}
