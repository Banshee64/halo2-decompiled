// @flags /O2 /Gr /arch:SSE2
#include "cseries.h"
#include "unknown_26b230.h"

s_data_array *g_502420;
s_data_array *g_50241c;
s_data_array *g_4f55f0;
s_game_time_globals *g_510c54;

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
long clump_get_clump_prop(long clump_index, long prop_index)
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
