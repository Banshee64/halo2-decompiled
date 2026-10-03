// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_157450.CPP: the game engine globals: the lifecycle callbacks of
   entry 22 and the team bookkeeping */

#include "cseries.h"
#include "game_state.h"
#include "globals.h"
#include "engine_peer.h"
#include "unknown_157450.h"
#include "input_record.h"
#include <string.h>

/* the players (g_4e8c24, 0x21c bytes each) */
/* a player's recent marks: an identifier, its code and when it was made */
struct s_engine_player_mark
{
	short identifier;
	byte code;
	byte unknown03;
	long time;
};

struct s_engine_player
{
	short salt;
	word flags;
	byte unknown04[0x28 - 4];
	short local_index;
	byte unknown2a[2];
	long unit_index;
	byte unknown30[0xc0 - 0x30];
	char team;
	char value_c1;
	byte unknownc2[0x164 - 0xc2];
	long value164;
	byte unknown168[0x170 - 0x168];
	long value170;
	byte unknown174[0x194 - 0x174];
	long value194;
	byte unknown198[0x1a7 - 0x198];
	bool flag1a7;
	char value1a8;
	byte unknown1a9[0x1ac - 0x1a9];
	short value1ac;
	byte unknown1ae[0x1d8 - 0x1ae];
	s_engine_player_mark marks[8];
	byte unknown218[0x21c - 0x218];
};

/* the iterator over the players of 0x19f240 */
struct s_engine_player_iterator
{
	s_engine_player *player;
	s_data_array *data;
	long index;
	long absolute_index;
};

/* the tag the engine needs before it can run (g_4e034c->index) */
struct s_engine_tag
{
	byte unknown00[8];
	void *data;
};

/* the scenario's netgame entries (g_4e0350, the count at +0x120) */
struct s_scenario_netgame_view
{
	byte unknown00[0x120];
	long count;
};

/* the game options' view of the players (16 of 0xe4 bytes at +0x2dc) */
struct s_options_player
{
	bool valid;
	byte unknown01[0x98 - 1];
	char team;
	byte unknown99[0xe4 - 0x99];
};

struct s_engine_options
{
	byte unknown00[0x184];
	dword flags_bit0 : 1;
	dword flags_bit1 : 1;
	dword flags_bit2 : 1;
	dword flags_bit3 : 1;
	dword flags_bit4 : 1;
	dword flags_bit5 : 1;
	dword flags_bit6 : 1;
	dword flags_bit7 : 1;
	dword flags_bit8 : 1;
	dword flags_bit9 : 1;
	dword flags_bit10 : 1;
	dword : 21;
	byte unknown188[0x190 - 0x188];
	long value190;
	byte unknown194[0x1bc - 0x194];
	long value1bc;
	byte unknown1c0[4];
	long value1c4;
	byte unknown1c8[0x1f0 - 0x1c8];
	long maximum_teams;
	byte unknown1f4[0x2dc - 0x1f4];
	s_options_player players[16];
};

dword g_502258[0x27];
s_local_engine_state g_4e9af0;

bool function_19f240(long *iterator);
void function_b58c0(long index, dword mask);
void function_1523c0();
void function_23aea0(void);
void function_19cad0(void);
void function_23f0a0(void);
void function_157790(void);
void function_157670(void);
void function_158140(void);

static inline s_engine_options *engine_options()
{
	return (s_engine_options *)g_4e6948;
}

static inline long local_player_next(long index)
{
	long result = NONE;

	for (long i = index == NONE ? 0 : index + 1; i < 4; i++)
	{
		if (g_4e8c20->entries[i] != NONE)
		{
			result = i;
			break;
		}
	}
	return result;
}

// @retail 0x157450
void function_157450(void)
{
	s_mp_globals *data = (s_mp_globals *)game_state_malloc("unknown", "unknown", sizeof(s_mp_globals));

	memset(data, 0, sizeof(*data));
	memset(g_502258, 0, sizeof(g_502258));
	g_4e9ae8 = data;
	data->engine_index = NONE;
}

// @retail 0x15ace0
void function_15ace0(s_netgame_entry_state *entries)
{
	if (g_4e6948->mode != 4)
	{
		s_scenario_netgame_view *scenario = (s_scenario_netgame_view *)g_4e0350;

		memset(entries, 0, sizeof(s_netgame_entry_state) * 100);
		for (short i = 0; i < scenario->count; i++)
		{
			entries[i].index = NONE;
		}
	}
}

// @retail 0x157970
void function_157970(void)
{
	if (game_engine_has_teams())
	{
		word teams = 0;

		for (long team = 0; team < 8; team++)
		{
			c_engine_peer *engine = game_engine_get();

			if (!engine || engine->p4(team))
			{
				if (!(teams & (1 << team)))
				{
					teams |= 1 << team;
				}
			}
		}
		game_engine_globals()->present_teams = teams;
	}
}

// @retail 0x1579f0
void function_1579f0(void)
{
	if (g_4e6948->mode != 4 && game_engine_has_teams())
	{
		if (game_engine_globals()->active_teams == 0)
		{
			function_157790();
		}
		function_157670();
		function_158140();
	}
}

// @retail 0x1574c0
void function_1574c0(void)
{
	s_game_engine_globals *globals = game_engine_globals();
	s_statborg *statborg;
	long index;

	memset(globals, 0, sizeof(*globals));
	memset(&g_4e9af0, 0, sizeof(g_4e9af0));
	globals->engine_index = g_4e6948->mode_180;
	globals->index24 = NONE;
	globals->index28 = NONE;
	memset(globals->slots, NONE, sizeof(globals->slots));
	memset(globals->team_designators, NONE, sizeof(globals->team_designators));
	statborg = game_engine_statborg_inline();
	if (statborg)
	{
		memset(statborg, 0, sizeof(*statborg));
	}
	if (game_engine_get())
	{
		long tag_index = g_4e034c->index;

		if (tag_index == NONE || !((s_engine_tag *)g_4e3b44[tag_index & 0xffff].bytes)->data)
		{
			globals->engine_index = 0;
		}
	}
	if (game_engine_get())
	{
		function_23aea0();
		function_19cad0();
		function_23f0a0();
		function_15ace0(globals->netgame_entries);
		if (!game_engine_get()->p1())
		{
			game_engine_globals()->engine_index = NONE;
		}
		function_157970();
		function_1579f0();
	}
	statborg = game_engine_statborg_inline();
	if (statborg)
	{
		statborg->valid = true;
	}
	for (index = local_player_next(NONE); index != NONE; index = local_player_next(index))
	{
		if (index != NONE && g_4e8c20->entries[index] != NONE)
		{
			g_4e9af0.players[index].index = NONE;
		}
	}
}

// @retail 0x157670
void function_157670(void)
{
	long team = game_engine_get()->p26();
	s_game_engine_globals *globals;

	team = team == NONE ? 0 : team;
	globals = game_engine_globals();
	globals->assigned_teams = 0;
	globals->team_flags = 0;
	memset(globals->team_designators, NONE, sizeof(globals->team_designators));
	for (long count = 0; count < 8; count++, team++)
	{
		if (team == 8)
		{
			team = 0;
		}
		if (globals->active_teams & (1 << team))
		{
			long i;

			for (i = 0; i < 9; i++)
			{
				if ((globals->present_teams & (1 << i)) && !(globals->assigned_teams & (1 << i)))
				{
					break;
				}
			}
			if (i != 9)
			{
				globals->assigned_teams |= 1 << i;
				globals->team_flags |= 1 << team;
				globals->team_designators[i] = (short)team;
			}
		}
	}
	function_1523c0();
	if (game_engine_get() && game_engine_globals()->index24 != NONE)
	{
		function_b58c0(game_engine_globals()->index24, 1);
	}
}

// @retail 0x157790
void function_157790(void)
{
	s_engine_options *options = engine_options();
	s_game_engine_globals *globals;
	word teams = 0;
	long count = 0;
	long maximum;
	long present;
	long i;

	for (i = 0; i < 16; i++)
	{
		if (options->players[i].valid)
		{
			short team = options->players[i].team;

			if (team != -1 && !(teams & (1 << team)))
			{
				teams |= 1 << team;
				count++;
			}
		}
	}
	maximum = engine_options()->maximum_teams;
	if (!maximum)
	{
		maximum = 8;
	}
	globals = game_engine_globals();
	present = 0;
	for (i = 0; i < 8; i++)
	{
		if (globals->present_teams & (1 << i))
		{
			present++;
		}
	}
	if (maximum > present)
	{
		maximum = present;
	}
	if (count > 0 && count < maximum)
	{
		for (i = 0; i < 8; i++)
		{
			if (!(teams & (1 << i)))
			{
				teams |= 1 << i;
				count++;
				if (count == maximum)
				{
					break;
				}
			}
		}
	}
	globals->active_teams = teams;
	if (game_engine_get() && globals->index24 != NONE)
	{
		function_b58c0(globals->index24, 1);
	}
}

// @retail 0x157a40
word function_157a40(void)
{
	word result = 0;

	if (game_engine_has_teams())
	{
		result = game_engine_globals()->team_flags;
	}
	return result;
}

// @retail 0x157a80
void function_157a80(void)
{
	s_engine_player_iterator iterator;

	iterator.data = g_4e8c24;
	iterator.absolute_index = NONE;
	iterator.index = NONE;
	while (function_19f240((long *)&iterator))
	{
		iterator.player->flag1a7 = false;
		iterator.player->value1a8 = NONE;
	}
}

// @retail 0x157bb0
void function_157bb0(void)
{
	c_engine_peer *object = g_55e4d0[g_4e9ae8->engine_index];

	if (object)
	{
		object->p2();
		g_4e9ae8->engine_index = NONE;
	}
}

void function_196780(void);

/* unknown_1967d0.cpp's 0x1968b0 (sets an input counter, clamped to its
   range), which retail inlines here; its own file is built /Ob1 */
static inline void input_counter_set_inlined(long c, long a, long b, long value)
{
	if (g_510ca0 && !g_510ca1)
	{
		long minimum = g_46ddc0[b].minimum;
		long maximum = g_46ddc0[b].maximum;
		if (a != NONE)
		{
			long clamped = value;
			if (clamped < minimum)
				clamped = minimum;
			else if (clamped > maximum)
				clamped = maximum;
			g_511bf4.all[a * 0x1b5 + b].value = clamped;
		}
		if (c != NONE)
		{
			long clamped = value;
			if (clamped < minimum)
				clamped = minimum;
			else if (clamped > maximum)
				clamped = maximum;
			g_511bf4.counters[0][c * 0x2d + b].value = clamped;
		}
	}
}

/* recomputes the teams that have players */
// @retail 0x158140
void function_158140(void)
{
	s_game_engine_globals *globals = game_engine_globals();
	dword old_playing_teams = globals->playing_teams;
	word old_team_mask = globals->team_mask;
	dword playing_teams = 0;
	dword present_teams = 0;
	s_data_iterator iterator;
	s_engine_player *player;

	iterator.data = g_4e8c24;
	iterator.index = NONE;
	while ((player = (s_engine_player *)data_iterator_next_inlined(&iterator)) != 0)
	{
		long team = player->team;

		if (team != NONE)
		{
			present_teams |= 1 << team;
			if (!(player->flags & 2))
			{
				playing_teams |= 1 << team;
			}
		}
	}
	if (playing_teams != old_playing_teams || present_teams != old_team_mask)
	{
		dword added_teams = ~old_playing_teams & playing_teams;
		long team;

		globals->playing_teams = (word)playing_teams;
		globals->team_mask = old_team_mask | (word)present_teams;
		if (game_engine_get() && globals->index24 != NONE)
		{
			function_b58c0(globals->index24, 1);
		}
		function_196780();
		for (team = 0; team < 8; team++)
		{
			if (added_teams & (1 << team))
			{
				input_counter_set_inlined(team, NONE, 0, 1);
			}
		}
	}
}

// @retail 0x1584c0
void function_1584c0(long index)
{
	long player_index = NONE;
	s_game_engine_globals *globals;

	if (index != NONE)
	{
		player_index = g_4e8c20->entries[index];
	}
	globals = game_engine_globals();
	globals->timers[index] = 1.0f;
	globals->timer_flags |= 1 << index;
	if (player_index != NONE)
	{
		((s_engine_player *)g_4e8c24->data)[player_index & 0xffff].value170 = engine_options()->value1bc;
	}
}

// @retail 0x1587b0
long function_1587b0(long player_index)
{
	long result = 0;
	s_statborg *statborg = game_engine_statborg_inline();

	if (statborg)
	{
		result = statborg->players[player_index & 0xffff].score;
	}
	return result;
}

// @retail 0x1587f0
long function_1587f0(long team)
{
	long result = NONE;

	if (game_engine_statborg_inline() && game_engine_has_teams() && game_engine_team_is_active(team))
	{
		result = game_engine_get_statborg()->teams[team].score;
	}
	return result;
}

// @retail 0x158850
void function_158850(long *type, long *count)
{
	if (engine_options()->value190)
	{
		*type = 2;
	}
	else
	{
		*type = 1;
	}
	if (game_engine_globals()->value_e0 > 0)
	{
		*count = game_engine_globals()->value_e0;
	}
	else
	{
		*count = 0;
	}
}

#define PIN(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))

/* the juggernauts (juggernaut.cpp) */
struct s_juggernaut_globals
{
	word players;
	word teams;
};

extern s_juggernaut_globals *g_510c9c;

bool juggernaut_is(short player_index);
bool function_15db30(long player_index);
long unit_seat_get_occupant(long unit_index, short seat_index);

static inline s_engine_player *engine_player_get(long player_index)
{
	return (s_engine_player *)g_4e8c24->data + (player_index & 0xffff);
}

static inline bool engine_team_is_active(long team)
{
	bool result = false;

	if (game_engine_has_teams() && team >= 0 && team < 8)
	{
		result = (game_engine_globals()->team_mask & (1 << team)) != 0;
	}
	return result;
}

// @retail 0x1588b0
real function_1588b0(long player_index, long type)
{
	long index = player_index & 0xffff;
	s_engine_player *player = (s_engine_player *)g_4e8c24->data + index;
	real result = 1.0f;

	switch (type)
	{
	case 2:
		if (g_4e6948->mode_180 == 7 && juggernaut_is((short)index))
		{
			if (g_4e6948->flags22c & 2)
			{
				result = 3.0f;
			}
			else
			{
				switch (engine_options()->value1c4)
				{
				case 0:
					result = 1.0f;
					break;
				case 1:
					result = 0.0f;
					break;
				case 2:
					result = 1.0f;
					break;
				}
			}
		}
		else
		{
			switch (engine_options()->value1c4)
			{
			case 0:
				result = 1.0f;
				break;
			case 1:
				result = 0.0f;
				break;
			case 2:
				result = 3.0f;
				break;
			}
		}
		switch (player->value_c1)
		{
		case 0:
			result *= 1.0f;
			break;
		case 1:
			result *= 0.75f;
			break;
		case 2:
			result *= 0.5f;
			break;
		case 3:
			result *= 0.25f;
			break;
		}
		break;
	}
	return result;
}

static inline bool team_designator_is_assigned(long index)
{
	bool result = false;

	if (index >= 0 && index < 9)
	{
		result = (game_engine_globals()->assigned_teams & (1 << index)) != 0;
	}
	return result;
}

// @retail 0x158990
short function_158990(long team)
{
	long i;

	for (i = 0; i < 9; i++)
	{
		if (team_designator_is_assigned(i) && game_engine_globals()->team_designators[i] == team)
		{
			break;
		}
	}
	if (i == 9)
	{
		i = NONE;
	}
	return i;
}

// @retail 0x158a50
bool function_158a50(long player_index)
{
	s_engine_player *player = engine_player_get(player_index);
	word flags = player->flags;
	bool result = false;

	if ((flags & 1) && !(flags & 2) && player->team != NONE)
	{
		if (player->unit_index != NONE)
		{
			return true;
		}
		if (!(flags & 0x4000) && !function_15db30(player_index))
		{
			result = true;
		}
	}
	return result;
}

// @retail 0x158ab0
long function_158ab0(void)
{
	long count = 0;

	if (game_engine_has_teams())
	{
		for (long team = 0; team < 8; team++)
		{
			if (engine_team_is_active(team))
			{
				count++;
			}
		}
	}
	else
	{
		s_data_iterator iterator;

		iterator.data = g_4e8c24;
		iterator.index = NONE;
		while (data_iterator_next_inlined(&iterator))
		{
			count++;
		}
	}
	return count;
}

/* data_datum_iterator_next (data_iterator.cpp), which retail inlines here */
static inline bool data_datum_iterator_next_inlined(s_data_datum_iterator *iterator)
{
	s_data_array *data = iterator->data;
	long index = data_next_absolute_index_inlined(data, iterator->index + 1);
	byte *datum;

	if (index != NONE)
	{
		datum = data->data + data->size * index;
		iterator->index = index;
		iterator->datum_index = (*(short *)datum << 16) | index;
	}
	else
	{
		iterator->index = data->maximum_count;
		iterator->datum_index = NONE;
		datum = 0;
	}
	iterator->datum = datum;
	return datum != 0;
}

// @retail 0x158c00
void function_158c00(long *player_index, long *active_count, long *team_count)
{
	dword active_teams = 0;
	dword teams = 0;
	long teams_counted = 0;
	long active_counted = 0;
	s_data_datum_iterator iterator;

	iterator.data = g_4e8c24;
	iterator.index = NONE;
	while (data_datum_iterator_next_inlined(&iterator))
	{
		s_engine_player *player = (s_engine_player *)iterator.datum;
		long team = player->team;
		long datum_index = iterator.datum_index;

		if (!(player->flags & 2) && team >= 0 && team < 16 && team != NONE)
		{
			dword bit = 1 << team;

			if (!(teams & bit))
			{
				teams |= bit;
				teams_counted++;
			}
			if (function_158a50(datum_index) || !(player->flags & 3))
			{
				*player_index = datum_index;
				if (!(active_teams & bit))
				{
					active_teams |= bit;
					active_counted++;
				}
			}
		}
	}
	if (team_count)
	{
		*team_count = teams_counted;
	}
	if (active_count)
	{
		*active_count = active_counted;
	}
}

// @retail 0x158e20
void function_158e20(void)
{
	s_engine_player_iterator iterator;
	long count = 0;

	iterator.data = g_4e8c24;
	iterator.absolute_index = NONE;
	iterator.index = NONE;
	while (function_19f240((long *)&iterator))
	{
		count++;
	}
	if (count > 6)
	{
		game_engine_globals()->flags |= 2;
	}
	if (count >= 5)
	{
		game_engine_globals()->flags |= 4;
	}
	if (count >= 9)
	{
		game_engine_globals()->flags |= 8;
	}
}

/* the object header data's view of a unit (g_4e0300) */
struct s_engine_unit_header
{
	byte unknown00[8];
	long *object;
};

struct s_engine_unit_definition
{
	byte unknown00[0x1c8];
	long seat_count;
};

// @retail 0x158ff0
bool function_158ff0(long unit_index)
{
	long *unit = ((s_engine_unit_header *)g_4e0300->data)[unit_index & 0xffff].object;
	s_engine_unit_definition *definition = (s_engine_unit_definition *)g_4e3b44[*unit & 0xffff].bytes;
	long seat_count = definition->seat_count;
	long seat;

	for (seat = 0; seat < seat_count; seat++)
	{
		if (unit_seat_get_occupant(unit_index, (short)seat) != NONE)
		{
			break;
		}
	}
	return seat != seat_count;
}

static inline real local_player_fraction(long local_index)
{
	real fraction = (real)g_4e9af0.timers[local_index] * g_510c54->rate * 4.0f;

	return PIN(fraction, 0.0f, 1.0f);
}

// @retail 0x159170
bool function_159170(long player_index)
{
	bool result = true;

	if (game_engine_get())
	{
		if (local_player_fraction(engine_player_get(player_index)->local_index) >= 1.0f)
		{
			result = false;
		}
	}
	return result;
}

// @retail 0x1591e0
real function_1591e0(long local_index)
{
	real result = 1.0f;

	if (game_engine_get())
	{
		result = 1.0f - local_player_fraction(local_index);
	}
	return result;
}

extern long g_4b9ed8;

// @retail 0x159610
bool function_159610(void)
{
	bool result = false;

	if (game_engine_get())
	{
		real timer = 1.0f;

		if (g_4b9ed8 != NONE)
		{
			timer = game_engine_globals()->timers[g_4b9ed8];
		}
		if (timer >= 1.0f)
		{
			long player_index = NONE;

			if (g_4b9ed8 != NONE)
			{
				player_index = g_4e8c20->entries[g_4b9ed8];
			}
			result = player_index == NONE || engine_player_get(player_index)->unit_index == NONE;
		}
	}
	return result;
}

static inline bool game_engine_respawn_flag()
{
	bool result = false;
	long mode = g_4e6948->mode_180;

	if (mode == 1 || mode == 9)
	{
		result = !g_4e6948->flags22c_bits.bit4;
	}
	else if (mode == 3)
	{
		result = !g_4e6948->flags22c_bits.bit0;
	}
	return result;
}

// @retail 0x159d40
bool function_159d40(void)
{
	return game_engine_respawn_flag();
}

// @retail 0x159d80
bool function_159d80(void)
{
	return !game_engine_respawn_flag();
}

// @retail 0x159dd0
bool function_159dd0(long player_index)
{
	bool result = false;

	if (game_engine_get() && g_4e6948->mode_180 == 7 && juggernaut_is((short)player_index) && (g_4e6948->flags22c & 0x20))
	{
		result = true;
	}
	return result;
}

// @retail 0x15dea0
bool function_15dea0(void)
{
	return game_engine_get() ? true : false;
}

// @retail 0x15fe50
void function_15fe50(long value)
{
	if (g_4e6948->mode != 4)
	{
		game_engine_globals()->value_c04 = value;
	}
}

// @retail 0x15ff50
bool function_15ff50(long a, long b)
{
	bool result = false;
	c_engine_peer *engine = game_engine_get();

	if (engine)
	{
		result = engine->p35(a, b);
	}
	return result;
}

// @retail 0x15fef0
bool function_15fef0(long player_index)
{
	bool result = true;

	if (game_engine_get())
	{
		if (g_4e6948->mode_180 == 7 && juggernaut_is((short)player_index))
		{
			result = g_4e6948->flags22c_bits.bit0;
		}
		else
		{
			result = g_4e6948->flags184.bit1;
		}
	}
	return result;
}

/* the multiplayer globals tag (g_4e034c->index): the tag references the
   engine picks its default equipment from (each reference's index at +4) */
struct s_tag_reference
{
	dword group_tag;
	long index;
};

struct s_multiplayer_equipment
{
	s_tag_reference references[12];
};

struct s_multiplayer_globals_data
{
	byte unknown00[0x74];
	s_multiplayer_equipment *equipment;
	byte unknown78[4];
	s_tag_reference *objects7c;
	byte unknown80[4];
	s_tag_reference *objects84;
};

struct s_multiplayer_globals_tag
{
	byte unknown00[0xc];
	s_multiplayer_globals_data *data;
};

static inline s_multiplayer_globals_data *multiplayer_globals_data()
{
	return ((s_multiplayer_globals_tag *)g_4e3b44[g_4e034c->index & 0xffff].bytes)->data;
}

static inline char random_option(short range)
{
	return (char)(random_index((dword *)&g_4e7408->unknown0, range) + 1);
}

/* the chooser of the engine's default equipment (its methods ignore this) */
class c_engine_equipment
{
public:
	long choose_15a140(long default_value, char option, long *name);
	long choose_15a250(long default_value, char option, long *name);
	long choose_15a310(long default_value, char option, long *name);
	long choose_15a3d0(long default_value, char option, long *name);
	long choose(long default_value, long *name, char kind);
};

// @retail 0x15a140
long c_engine_equipment::choose_15a140(long default_value, char option, long *name)
{
	s_multiplayer_globals_data *data = multiplayer_globals_data();
	long result = default_value;

	switch (option)
	{
	case 1:
		result = data->equipment->references[0].index;
		*name = 0;
		break;
	case 2:
		result = data->equipment->references[0].index;
		*name = 0x5000694;
		break;
	case 3:
		result = data->equipment->references[1].index;
		*name = 0;
		break;
	case 4:
		result = data->equipment->references[6].index;
		*name = 0;
		break;
	case 5:
		result = data->equipment->references[5].index;
		*name = 0;
		break;
	case 6:
		do
		{
			result = choose_15a140(default_value, random_option(5), name);
		}
		while (result == NONE);
		break;
	case 7:
		result = NONE;
		break;
	}
	return result;
}

// @retail 0x15a250
long c_engine_equipment::choose_15a250(long default_value, char option, long *name)
{
	s_multiplayer_globals_data *data = multiplayer_globals_data();
	long result = default_value;

	switch (option)
	{
	case 1:
		result = data->equipment->references[2].index;
		*name = 0xb00078b;
		break;
	case 2:
		result = data->equipment->references[3].index;
		*name = 0xb00078b;
		break;
	case 3:
		do
		{
			result = choose_15a250(default_value, random_option(2), name);
		}
		while (result == NONE);
		break;
	case 4:
		result = NONE;
		break;
	}
	return result;
}

// @retail 0x15a310
long c_engine_equipment::choose_15a310(long default_value, char option, long *name)
{
	s_multiplayer_globals_data *data = multiplayer_globals_data();
	long result = default_value;

	switch (option)
	{
	case 1:
		result = data->equipment->references[4].index;
		*name = 0;
		break;
	case 2:
		result = data->equipment->references[4].index;
		*name = 0;
		break;
	case 3:
		do
		{
			result = choose_15a310(default_value, random_option(2), name);
		}
		while (result == NONE);
		break;
	case 4:
		result = NONE;
		break;
	}
	return result;
}

// @retail 0x15a3d0
long c_engine_equipment::choose_15a3d0(long default_value, char option, long *name)
{
	s_multiplayer_globals_data *data = multiplayer_globals_data();
	long result = default_value;

	switch (option)
	{
	case 1:
		result = data->equipment->references[7].index;
		*name = 0;
		break;
	case 2:
		result = data->equipment->references[9].index;
		*name = 0;
		break;
	case 3:
		result = data->equipment->references[8].index;
		*name = 0;
		break;
	case 4:
		result = data->equipment->references[10].index;
		*name = 0;
		break;
	case 5:
		do
		{
			result = choose_15a3d0(default_value, random_option(4), name);
		}
		while (result == NONE);
		break;
	case 6:
		result = NONE;
		break;
	}
	return result;
}

/* the game options' equipment choices (+0x209..+0x20f) */
struct s_options_equipment
{
	byte unknown00[0x209];
	char options[7];
};

// @retail 0x15a4d0
long c_engine_equipment::choose(long default_value, long *name, char kind)
{
	s_options_equipment *options = (s_options_equipment *)g_4e6948;

	switch (kind)
	{
	case 1:
		return choose_15a140(default_value, options->options[0], name);
	case 2:
		return choose_15a140(default_value, options->options[1], name);
	case 3:
		return choose_15a250(default_value, options->options[2], name);
	case 4:
		return choose_15a310(default_value, options->options[3], name);
	case 5:
		return choose_15a250(default_value, options->options[4], name);
	case 6:
		return choose_15a3d0(default_value, options->options[5], name);
	case 7:
		return choose_15a3d0(default_value, options->options[6], name);
	default:
		__assume(0);
	}
}

// @retail 0x15a5b0
long function_15a5b0(long tag_index)
{
	s_multiplayer_globals_data *data = multiplayer_globals_data();
	s_engine_options *options = engine_options();

	if (tag_index == data->objects7c[0].index && !TEST_FIELD_BIT(options->flags_bit10) ||
		tag_index == data->objects7c[1].index && !TEST_FIELD_BIT(options->flags_bit10) ||
		tag_index == data->objects84[0].index && !TEST_FIELD_BIT(options->flags_bit9) ||
		tag_index == data->objects84[1].index && (!TEST_FIELD_BIT(options->flags_bit8) || options->value1c4 == 1))
	{
		return NONE;
	}
	return tag_index;
}

/* the objects (g_4e0300) as the game engine sees them */
struct s_engine_object
{
	long definition_index;
	byte unknown04[0xaa - 4];
	byte type;
	byte unknownab[0xaf - 0xab];
	char netgame_entry;
	byte unknownb0[0xd4 - 0xb0];
	long simulation_index;
	byte unknownd8[0x13c - 0xd8];
	long value13c;
	byte unknown140[0x150 - 0x140];
	long value150;
	long value154;
	byte unknown158[0x16c - 0x158];
	union
	{
		byte flags16c;
		struct
		{
			word flag16c_bit0_5 : 6;
			word flag16c_bit6 : 1;
			word flag16c_bit7 : 1;
		};
	};
	byte unknown16e[0x17e - 0x16e];
	short value17e;
};

struct s_engine_object_header
{
	byte unknown00[8];
	s_engine_object *object;
};

static inline s_engine_object *engine_object_get(long object_index)
{
	return ((s_engine_object_header *)g_4e0300->data)[object_index & 0xffff].object;
}

/* marks an object's simulation entity dirty */
static inline void engine_object_mark_dirty(long object_index, dword mask)
{
	s_engine_object *object = engine_object_get(object_index);

	if (object->simulation_index != NONE)
	{
		function_b58c0(object->simulation_index, mask);
	}
}

static inline s_game_engine_object_entry *game_engine_object_entry(long object_index)
{
	s_game_engine_globals *globals = game_engine_globals();
	s_game_engine_object_entry *result = 0;

	for (long i = 0; i < globals->object_count; i++)
	{
		if (globals->objects[i].object_index == object_index)
		{
			result = &globals->objects[i];
		}
	}
	return result;
}

/* the scenario's netgame equipment (g_4e0350 + 0x124, 0x90 bytes each) */
struct s_scenario_netgame_equipment
{
	byte unknown00[0xe];
	short spawn_time;
	byte unknown10[0x58 - 0x10];
	dword group_tag;
	long tag_index;
	byte unknown60[0x90 - 0x60];
};

struct s_scenario_netgame_equipment_view
{
	byte unknown00[0x124];
	s_scenario_netgame_equipment *equipment;
};

struct s_item_collection
{
	byte unknown00[8];
	short spawn_time;
};

// @retail 0x15ac50
long function_15ac50(long index)
{
	s_scenario_netgame_equipment *equipment = &((s_scenario_netgame_equipment_view *)g_4e0350)->equipment[index];
	real seconds = 30.0f;
	short spawn_time = 0;
	long result;

	if (equipment->spawn_time)
	{
		spawn_time = equipment->spawn_time;
	}
	else if ((equipment->group_tag == 'itmc' || equipment->group_tag == 'vehc') && equipment->tag_index != NONE)
	{
		spawn_time = ((s_item_collection *)g_4e3b44[equipment->tag_index & 0xffff].bytes)->spawn_time;
	}
	if (spawn_time)
	{
		seconds = (real)spawn_time;
	}
	__asm
	{
		fld seconds
		fistp result
	}
	return result;
}

// @retail 0x15b1e0
void function_15b1e0(long object_index, s_netgame_entry_state *entries, long index)
{
	s_engine_object *object = engine_object_get(object_index);

	if (g_4e6948->mode != 4)
	{
		entries[index].index = NONE;
	}
	object->netgame_entry = NONE;
}

// @retail 0x15b220
void function_15b220(long object_index, long index)
{
	s_engine_object *object = engine_object_get(object_index);

	if (g_4e6948->mode != 4)
	{
		game_engine_globals()->netgame_entries[index].index = NONE;
	}
	object->netgame_entry = NONE;
}

void function_1967d0(long a, long b, long c, long delta);

// @retail 0x15b930
void function_15b930(long player_index, bool by_team, long counter, long delta)
{
	long index = player_index & 0xffff;

	if (by_team)
	{
		function_1967d0(index, counter, engine_player_get(index)->team, delta);
	}
	else
	{
		function_1967d0(index, counter, NONE, delta);
	}
}

long function_196ef0(byte code);
void function_196e60(long b, long a, long c, long delta);

/* the identifier of the next player mark */
short g_47ff84 = 42;

// @retail 0x15cca0
short function_15cca0(long player_index, byte code)
{
	short result = NONE;

	if (game_engine_get() && player_index != NONE)
	{
		long maximum_age = g_510c54->ticks_per_second * 60;
		long time = g_510c54->game_time;
		s_engine_player *player = engine_player_get(player_index);
		long oldest_age = 0;
		long slot = 0;
		long i;

		for (i = 0; i < 8; i++)
		{
			s_engine_player_mark *mark = &player->marks[i];
			long age = time - mark->time;

			if (mark->identifier == NONE || age > maximum_age)
			{
				slot = i;
				break;
			}
			if (age > oldest_age)
			{
				slot = i;
				oldest_age = age;
			}
		}
		result = g_47ff84++;
		player->marks[slot].identifier = result;
		player->marks[slot].code = code;
		player->marks[slot].time = time;
		function_196e60(player_index & 0xffff, 4, function_196ef0(code), 1);
	}
	return result;
}

// @retail 0x15cd90
void function_15cd90(long player_index, short identifier, long other_player_index)
{
	s_engine_player *player = (s_engine_player *)datum_get_inlined(g_4e8c24, player_index);

	if (identifier != NONE && player && game_engine_get() &&
		game_engine_get()->p27(engine_player_get(other_player_index)->team, player->team))
	{
		s_engine_player_mark *mark = player->marks;
		long i;

		for (i = 8; i != 0; i--, mark++)
		{
			if (mark->identifier == identifier)
			{
				function_196e60(player_index & 0xffff, 5, function_196ef0(mark->code), 1);
				mark->identifier = NONE;
			}
		}
	}
}

// @retail 0x15db80
bool function_15db80(long team)
{
	bool result = false;

	if (game_engine_has_teams() && g_4e6948->value1b8 > 0)
	{
		s_engine_player_iterator iterator;

		iterator.data = g_4e8c24;
		iterator.absolute_index = NONE;
		iterator.index = NONE;
		result = true;
		while (function_19f240((long *)&iterator))
		{
			s_engine_player *player = iterator.player;

			if (player->team == team && (player->unit_index != NONE || player->value1ac != 0))
			{
				return false;
			}
		}
	}
	return result;
}

static inline void game_engine_player_mark_dirty(short absolute_index, dword mask)
{
	if (game_engine_get())
	{
		long index = game_engine_globals()->slots[absolute_index];

		if (index != NONE)
		{
			function_b58c0(index, mask);
		}
	}
}

// @retail 0x15dc40
void function_15dc40(void)
{
	s_engine_player_iterator iterator;

	iterator.data = g_4e8c24;
	iterator.absolute_index = NONE;
	iterator.index = NONE;
	while (function_19f240((long *)&iterator))
	{
		long absolute_index = iterator.index & 0xffff;

		iterator.player->value164 = 0;
		game_engine_globals()->players[absolute_index].value0 = 0;
		game_engine_player_mark_dirty((short)absolute_index, 1);
	}
}

// @retail 0x15e300
void function_15e300(long object_index)
{
	if (game_engine_get())
	{
		s_game_engine_object_entry *entry = game_engine_object_entry(object_index);

		if (entry->other_index != NONE)
		{
			entry->value0c = engine_object_get(entry->other_index)->value13c;
		}
	}
}

// @retail 0x15e250
void function_15e250(long object_index, long other_index)
{
	if (game_engine_get())
	{
		s_engine_object *object = engine_object_get(object_index);
		s_engine_object *other = engine_object_get(other_index);
		s_game_engine_object_entry *entry = game_engine_object_entry(object_index);

		entry->other_index = other_index;
		entry->value0c = other->value13c;
		object->flags16c |= 0x80;
		game_engine_get()->p23(object_index, other_index);
	}
}

// @retail 0x15e460
void function_15e460(long object_index, long value)
{
	s_engine_object *object = engine_object_get(object_index);
	s_game_engine_object_entry *entry = game_engine_object_entry(object_index);

	object->flag16c_bit7 = false;
	entry->other_index = NONE;
	entry->value0c = NONE;
	game_engine_get()->p24(object_index, value);
}

// @retail 0x15eb20
bool function_15eb20(long player_index)
{
	bool result = true;

	if (game_engine_get() && player_index != NONE)
	{
		if (g_4e6948->mode_180 == 7 && juggernaut_is((short)player_index) && (g_4e6948->flags22c & 2))
		{
			return true;
		}
		result = engine_options()->value1c4 != 1;
	}
	return result;
}

/* the multiplayer globals' block at +0x68 (8 bytes per element, a tag index at +4) */
struct s_multiplayer_globals_blocks
{
	byte unknown00[0x68];
	long count;
	s_tag_reference *elements;
};

// @retail 0x15eb80
long function_15eb80(long tag_index)
{
	s_multiplayer_globals_blocks *data = (s_multiplayer_globals_blocks *)multiplayer_globals_data();
	long result = NONE;

	if (data->count > 0)
	{
		for (long i = 0; i < data->count; i++)
		{
			if (tag_index == data->elements[i].index)
			{
				result = i;
				break;
			}
		}
	}
	return result;
}

// @retail 0x1595d0
void function_1595d0(void)
{
	if (g_4b9ed8 != NONE)
	{
		long player_index = g_4e8c20->entries[g_4b9ed8];

		if (player_index != NONE)
		{
			c_engine_peer *engine = game_engine_get();

			if (engine)
			{
				engine->p13(player_index);
			}
		}
	}
}

// @retail 0x15ea50
void function_15ea50(long player_index)
{
	c_engine_peer *engine = game_engine_get();

	if (engine && player_index != NONE)
	{
		engine->p14(player_index);
	}
}

/* the weapon definitions (the type at +0x290) */
struct s_engine_weapon_definition
{
	byte unknown00[0x290];
	short value290;
};

// @retail 0x15e8e0
void function_15e8e0(long object_index, long value)
{
	c_engine_peer *engine = game_engine_get();

	if (engine)
	{
		s_engine_object *object = engine_object_get(object_index);

		if ((1 << object->type) & 4)
		{
			if (((s_engine_weapon_definition *)g_4e3b44[object->definition_index & 0xffff].bytes)->value290)
			{
				engine->p17(value, object_index);
			}
			else
			{
				object->value150 = g_510c54->game_time;
			}
		}
	}
}

// @retail 0x15e050
void function_15e050(long object_index, short value)
{
	s_game_engine_globals *globals = game_engine_globals();

	if (g_55e4d0[globals->engine_index])
	{
		s_engine_object *object = engine_object_get(object_index);
		long count = globals->object_count;

		if (count < sizeof(globals->objects) / sizeof(globals->objects[0]))
		{
			s_game_engine_object_entry *entry;

			globals->object_count = count + 1;
			object->flag16c_bit6 = true;
			entry = &globals->objects[count];
			object->value17e = value;
			engine_object_mark_dirty(object_index, 0x1000);
			entry->value04 = value;
			entry->object_index = object_index;
			entry->other_index = NONE;
			entry->value0c = NONE;
			g_55e4d0[globals->engine_index]->p21(object_index);
			if (object->value154 != NONE)
			{
				function_15e250(object_index, object->value154);
			}
		}
	}
}

// @retail 0x15e130
void function_15e130(long object_index)
{
	if (game_engine_get())
	{
		s_engine_object *object = engine_object_get(object_index);
		s_game_engine_object_entry *entry = game_engine_object_entry(object_index);
		long index;

		if (entry->other_index != NONE)
		{
			function_15e460(object_index, object->value154);
		}
		game_engine_get()->p22(object_index);
		object->flag16c_bit6 = false;
		object->value17e = NONE;
		engine_object_mark_dirty(object_index, 0x1000);
		index = entry - game_engine_globals()->objects;
		if (index < game_engine_globals()->object_count - 1)
		{
			game_engine_globals()->objects[index] = game_engine_globals()->objects[game_engine_globals()->object_count - 1];
		}
		game_engine_globals()->object_count--;
	}
}

void __stdcall function_b8540(long a);

// @retail 0x15e4d0
void function_15e4d0(void)
{
	s_game_engine_globals *globals = game_engine_globals();
	long objects[8];
	long count = 0;
	long i;

	for (i = 0; i < globals->object_count; i++)
	{
		objects[count++] = globals->objects[i].object_index;
	}
	for (i = 0; i < count; i++)
	{
		long object_index = objects[i];

		function_15e130(object_index);
		function_b8540(object_index);
	}
}

// @retail 0x15e7a0
void __stdcall function_15e7a0(long object_index)
{
	s_engine_object *object = engine_object_get(object_index);

	if (((1 << object->type) & 4) && TEST_FIELD_BIT(object->flag16c_bit6))
	{
		function_15e130(object_index);
	}
}

/* the object deletion callbacks: each is called with the index of an object
   being deleted */
void __stdcall function_bb880(long object_index);
void __stdcall function_c1670(long object_index);
void __stdcall function_1c9f30(long object_index);
void __stdcall function_152cf0(long object_index);
void __stdcall function_16651c(long object_index);
void __stdcall function_2095e0(long object_index);
void __stdcall function_17b3c0(long object_index);

void (__stdcall *g_468664[])(long object_index) =
{
	function_bb880,
	function_c1670,
	function_1c9f30,
	function_152cf0,
	function_16651c,
	function_15e7a0,
	function_2095e0,
	function_17b3c0,
};

// @retail 0x15ece0
void function_15ece0(void)
{
	if (g_4e6948->mode != 4)
	{
		s_game_time_globals *time = g_510c54;
		s_game_engine_globals *globals = game_engine_globals();

		if (time->game_time >= globals->update_time + time->ticks_per_second * 3)
		{
			for (long i = 0; i < 16; i++)
			{
				game_engine_player_mark_dirty((short)i, 0x10);
			}
			globals->update_time = time->game_time;
		}
	}
}

// @retail 0x159460
void function_159460(long player_index, bool *leading, bool *tied)
{
	long absolute_index = player_index & 0xffff;
	s_engine_player *player = (s_engine_player *)g_4e8c24->data + absolute_index;
	bool is_tied = false;

	if (game_engine_has_teams())
	{
		long score = function_1587f0(player->team);

		for (long team = 0; team < 8; team++)
		{
			if (team != player->team && game_engine_team_is_active(team))
			{
				long difference = function_1587f0(team) - score;

				if (difference == 0)
				{
					is_tied = true;
				}
				else if (difference > 0)
				{
					*leading = false;
					*tied = false;
					return;
				}
			}
		}
	}
	else
	{
		long score = function_1587b0(player_index);
		s_engine_player_iterator iterator;

		iterator.data = g_4e8c24;
		iterator.absolute_index = NONE;
		iterator.index = NONE;
		while (function_19f240((long *)&iterator))
		{
			if (iterator.index != player_index)
			{
				long difference = function_1587b0(iterator.index) - score;

				if (difference == 0)
				{
					is_tied = true;
				}
				if (difference > 0)
				{
					*leading = false;
					*tied = false;
					return;
				}
			}
		}
	}
	*leading = true;
	*tied = is_tied;
}

#include "object_iterator.h"

// @retail 0x15adb0
void function_15adb0(s_netgame_entry_state *entries)
{
	s_object_iterator iterator;
	s_engine_object *object;

	function_15ace0(entries);
	function_bae80(&iterator, 0x1e, 0);
	while ((object = (s_engine_object *)function_baeb0(&iterator)) != 0)
	{
		long index = object->netgame_entry;

		if (index >= 0 && index < ((s_scenario_netgame_view *)g_4e0350)->count)
		{
			if (entries[index].index != NONE)
			{
				function_15b1e0(entries[index].index, entries, index);
			}
			entries[index].value04 = 0;
			entries[index].index = iterator.object_index;
		}
		else
		{
			object->netgame_entry = NONE;
		}
	}
}

bool function_15e020(short a, short b);

// @retail 0x15dec0
real function_15dec0(long player_index, long other_index)
{
	real result = 1.0f;

	if (game_engine_get() && player_index != other_index && player_index != NONE)
	{
		s_engine_player *player = engine_player_get(player_index);

		switch (player->value_c1)
		{
		case 0:
			break;
		case 1:
			result = 0.75f;
			break;
		case 2:
			result = 0.5f;
			break;
		case 3:
			result = 0.25f;
			break;
		}
		if (other_index != NONE)
		{
			s_engine_player *other;

			if (function_15ff50(player_index, 2))
			{
				result *= 1.5f;
			}
			if (function_15ff50(other_index, 3))
			{
				result *= 0.5f;
			}
			other = engine_player_get(other_index);
			if (other->value194 > 0)
			{
				result *= 0.1f;
			}
			if (!TEST_FIELD_BIT(engine_options()->flags_bit7) && !function_15e020(player->team, other->team))
			{
				return 0.0f;
			}
		}
	}
	return result;
}

// @retail 0x15d6c0
void function_15d6c0(long player_index, long maximum, long minimum)
{
	s_engine_player *player = engine_player_get(player_index);
	s_engine_player_iterator iterator;

	iterator.data = g_4e8c24;
	iterator.absolute_index = NONE;
	iterator.index = NONE;
	while (function_19f240((long *)&iterator))
	{
		s_engine_player *other = iterator.player;

		if (other->value164 > 0 &&
			!game_engine_get()->p27(other->team, player->team) &&
			other->value164 >= minimum && other->value164 < maximum)
		{
			maximum = other->value164;
		}
	}
	player->value164 = maximum;
}

// @retail 0x15a090
long function_15a090(short const *types, long type, long count)
{
	long result = NONE;
	long i;

	if (game_engine_get())
	{
		for (i = 0; i < count; i++)
		{
			short entry = types[i];
			bool match = entry == type;

			if ((type == 9 || type == 1) && (entry == 9 || entry == 1))
			{
				match = true;
			}
			if (entry == 12)
			{
				match |= true;
			}
			else if (entry == 13 || entry == 14)
			{
				match |= type != 1 && type != 9;
			}
			if (match)
			{
				return i;
			}
		}
	}
	else
	{
		for (i = 0; i < count; i++)
		{
			if (types[i] == 0)
			{
				return i;
			}
		}
	}
	return result;
}

// @retail 0x15ad30
void function_15ad30(s_netgame_entry_state *entries)
{
	if (g_4e6948->mode != 4)
	{
		s_scenario_netgame_view *scenario = (s_scenario_netgame_view *)g_4e0350;

		if (TEST_FIELD_BIT(engine_options()->flags_bit3) || game_engine_globals()->value6e == 0)
		{
			for (short i = 0; i < scenario->count; i++)
			{
				if (entries[i].index != NONE)
				{
					function_b8540(entries[i].index);
					entries[i].index = NONE;
				}
				entries[i].value04 = 1;
				entries[i].value06 = 0;
			}
		}
	}
}
