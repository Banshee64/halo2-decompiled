// @flags /O2 /Ob1 /arch:SSE /Gr
/* GAME_ALLEGIANCE.CPP: alliances between teams

LTCG dropped the last argument of function_1df6c0 (always false) and
the bool * of function_1df820.

Two team-by-team bit vectors hold the alliances: ally_bits (read by
function_1df5d0) and peace_bits (function_1df560 is its inverse). A
broken alliance keeps its ally bit but loses its peace bit until enough
time passes without incidents. */

#include "cseries.h"
#include "globals.h"
#include "game_state.h"
#include <string.h>

enum
{
	k_maximum_game_teams = 16,
	k_maximum_game_allegiances = 8
};

enum
{
	_game_mode_campaign = 1,
	_game_mode_multiplayer = 2
};

struct s_type_e695f2
{
	short team_a;
	short team_b;
	short incident_threshold;
	short incident_decay_ticks;
	bool team_b_provokes;
	bool team_a_provokes;
	bool broken;
	bool changed;
	short incidents;
	short decay_timer;
	long last_incident_time;
};

struct s_game_allegiance_globals
{
	short allegiance_count;
	byte unknown02[2];
	s_type_e695f2 allegiances[k_maximum_game_allegiances];
	dword ally_bits[k_maximum_game_teams * k_maximum_game_teams / 32];
	dword peace_bits[k_maximum_game_teams * k_maximum_game_teams / 32];
};

s_game_allegiance_globals *g_4f55ec;

bool function_0bfe60(const dword *flags, long bit);
bool function_15e020(short a, short b);
void __stdcall function_1c9830(short team_a, short team_b, bool broken, bool removed);

PRIVATE void function_1df9c0(s_type_e695f2 *allegiance, bool broken, bool removed);

// @retail 0x1df460
void function_1df460(void)
{
	g_4f55ec = (s_game_allegiance_globals *)function_123d40("game allegiance globals", "game_allegiance_globals",
		sizeof(s_game_allegiance_globals));
	memset(g_4f55ec, 0, sizeof(s_game_allegiance_globals));
}

// @retail 0x1df4b0
void function_1df4b0(void)
{
	memset(g_4f55ec, 0, sizeof(s_game_allegiance_globals));
	for (short team = 0; team < k_maximum_game_teams; team++)
	{
		long bit = team * k_maximum_game_teams + team;
		g_4f55ec->peace_bits[bit >> 5] |= 1 << (bit & 31);
	}
}

// @retail 0x1df500
void function_1df500(void)
{
	s_type_e695f2 *allegiance = g_4f55ec->allegiances;

	for (short i = 0; i < g_4f55ec->allegiance_count; i++, allegiance++)
	{
		if (allegiance->decay_timer > 0 && --allegiance->decay_timer == 0)
		{
			if (--allegiance->incidents == 0)
				function_1df9c0(allegiance, false, false);
			else
				allegiance->decay_timer = allegiance->incident_decay_ticks;
		}
	}
}

// @retail 0x1df560
bool function_1df560(short team_a, short team_b)
{
	bool result = true;

	if (team_a == NONE || team_b == NONE)
		return true;

	long mode = g_4e6948->state;

	if (mode == _game_mode_campaign)
	{
		if (team_a >= 0 && team_a < k_maximum_game_teams && team_b >= 0 && team_b < k_maximum_game_teams)
		{
			long bit = team_a * k_maximum_game_teams + team_b;
			result = !function_0bfe60(g_4f55ec->peace_bits, bit);
		}
	}
	else if (mode == _game_mode_multiplayer)
	{
		result = function_15e020(team_a, team_b);
	}
	else
	{
		result = team_a != team_b;
	}
	return result;
}

// @retail 0x1df5d0
bool function_1df5d0(short team_a, short team_b)
{
	bool result = false;

	if (team_a == NONE || team_b == NONE)
		return false;

	long mode = g_4e6948->state;

	if (mode == _game_mode_campaign)
	{
		if (team_a >= 0 && team_a < k_maximum_game_teams && team_b >= 0 && team_b < k_maximum_game_teams)
		{
			long bit = team_a * k_maximum_game_teams + team_b;
			result = function_0bfe60(g_4f55ec->ally_bits, bit);
		}
	}
	else if (mode == _game_mode_multiplayer)
	{
		result = !function_15e020(team_a, team_b);
	}
	else
	{
		result = team_a == team_b;
	}
	return result;
}

/* allies whose alliance is broken */
// @retail 0x1df640
bool function_1df640(short team_a, short team_b)
{
	bool result = false;

	if (team_a == NONE || team_b == NONE || team_a == team_b)
		return false;

	if (g_4e6948->state == _game_mode_campaign &&
		team_a >= 0 && team_a < k_maximum_game_teams && team_b >= 0 && team_b < k_maximum_game_teams)
	{
		long bit = team_a * k_maximum_game_teams + team_b;

		result = function_0bfe60(g_4f55ec->ally_bits, bit) && !function_0bfe60(g_4f55ec->peace_bits, bit);
	}
	return result;
}

// @retail 0x1df6c0
void function_1df6c0(short team_a, short team_b, bool team_b_provokes, bool team_a_provokes,
	short incident_threshold, short incident_decay_ticks)
{
	if (g_4e6948->state != _game_mode_campaign)
		return;

	s_game_allegiance_globals *globals = g_4f55ec;
	short count = globals->allegiance_count;
	short i;

	s_type_e695f2 *allegiance = globals->allegiances;

	for (i = 0; i < globals->allegiance_count; i++, allegiance++)
	{
		if (allegiance->team_a == team_a && allegiance->team_b == team_b ||
			allegiance->team_b == team_a && allegiance->team_a == team_b)
			break;
	}

	if (i >= count && count < k_maximum_game_allegiances)
		i = count++, globals->allegiance_count = count;

	if (i < globals->allegiance_count)
	{
		allegiance = &globals->allegiances[i];

		allegiance->team_a_provokes = team_a_provokes;
		allegiance->team_b_provokes = team_b_provokes;
		allegiance->team_a = team_a;
		allegiance->team_b = team_b;
		allegiance->incident_threshold = incident_threshold;
		allegiance->incident_decay_ticks = incident_decay_ticks;
		allegiance->incidents = 0;
		allegiance->decay_timer = 0;
		allegiance->broken = true;
		function_1df9c0(allegiance, false, false);
		allegiance->changed = false;
	}
}

// @retail 0x1df770
bool function_1df770(short team_a, short team_b)
{
	bool result = false;

	if (g_4e6948->state == _game_mode_campaign)
	{
		s_type_e695f2 *allegiance = g_4f55ec->allegiances;

		for (short i = 0; i < g_4f55ec->allegiance_count; i++, allegiance++)
		{
			if (allegiance->team_a == team_a && allegiance->team_b == team_b ||
				allegiance->team_b == team_a && allegiance->team_a == team_b)
			{
				function_1df9c0(allegiance, true, true);
				g_4f55ec->allegiance_count--;
				if (g_4f55ec->allegiance_count > i)
					g_4f55ec->allegiances[i] = g_4f55ec->allegiances[g_4f55ec->allegiance_count];
				return true;
			}
		}
	}
	return result;
}

// @retail 0x1df820
bool function_1df820(short team_a, short team_b, short incident_type)
{
	if (g_4e6948->state != _game_mode_campaign)
		return false;

	s_game_allegiance_globals *globals = g_4f55ec;
	s_type_e695f2 *allegiance = globals->allegiances;

	for (short i = 0; i < globals->allegiance_count; i++, allegiance++)
	{

		if (allegiance->team_a == team_a && allegiance->team_b == team_b && allegiance->team_a_provokes ||
			allegiance->team_b == team_a && allegiance->team_a == team_b && allegiance->team_b_provokes)
		{
			s_game_time_globals *game_time = g_510c54;
			real seconds = game_time->field_2_3 * 0.2f;
			long ticks;

			__asm
			{
				fld seconds
				fistp ticks
			}

			if (game_time->game_time - allegiance->last_incident_time > ticks)
			{
				short delta = 0;

				allegiance->last_incident_time = game_time->game_time;
				switch (incident_type)
				{
				case 0: delta = 1; break;
				case 1: delta = 3; break;
				case 2: delta = -1; break;
				}
				allegiance->incidents += delta;
				if (allegiance->incident_decay_ticks != NONE)
					allegiance->decay_timer = allegiance->incident_decay_ticks;
				if (allegiance->incident_threshold != NONE && allegiance->incidents >= allegiance->incident_threshold)
				{
					function_1df9c0(allegiance, true, false);
					function_1c9830(allegiance->team_a, allegiance->team_b, true, false);
					return true;
				}
				return false;
			}
		}
	}
	return false;
}

/* an incident between the teams restarts the decay of their grudge */
// @retail 0x1df950
void function_1df950(short team_a, short team_b)
{
	if (g_4e6948->state != _game_mode_campaign)
		return;

	s_type_e695f2 *allegiance = g_4f55ec->allegiances;

	for (short i = 0; i < g_4f55ec->allegiance_count; i++, allegiance++)
	{
		if (allegiance->team_a == team_a && allegiance->team_b == team_b && allegiance->team_a_provokes ||
			allegiance->team_b == team_a && allegiance->team_a == team_b && allegiance->team_b_provokes)
		{
			if (allegiance->incidents > 0 && allegiance->incident_decay_ticks != NONE)
				allegiance->decay_timer = allegiance->incident_decay_ticks;
			return;
		}
	}
}

// @retail 0x1dfaa0
PRIVATE void function_1dfaa0(dword *vector, long bit, bool value)
{
	if (value)
		vector[bit >> 5] |= 1 << (bit & 31);
	else
		vector[bit >> 5] &= ~(1 << (bit & 31));
}

// @retail 0x1df9c0
PRIVATE void function_1df9c0(s_type_e695f2 *allegiance, bool broken, bool removed)
{
	if (!removed && allegiance->broken == broken)
		return;

	allegiance->broken = broken;
	if (allegiance->team_a < k_maximum_game_teams && allegiance->team_b < k_maximum_game_teams)
	{
		function_1dfaa0(g_4f55ec->ally_bits, allegiance->team_a * k_maximum_game_teams + allegiance->team_b, !removed);
		function_1dfaa0(g_4f55ec->ally_bits, allegiance->team_b * k_maximum_game_teams + allegiance->team_a, !removed);
		function_1dfaa0(g_4f55ec->peace_bits, allegiance->team_a * k_maximum_game_teams + allegiance->team_b, !broken);
		function_1dfaa0(g_4f55ec->peace_bits, allegiance->team_b * k_maximum_game_teams + allegiance->team_a, !broken);
	}
	allegiance->changed = true;
	function_1c9830(allegiance->team_a, allegiance->team_b, broken, removed);
}
