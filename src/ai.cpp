// @flags /O2 /Ob1 /arch:SSE /Gr
/* AI.CPP: the ai globals, the ai's view of the players and units, and small
   ai helpers (0x1c7790..0x1caxxx; the atlas puts ai_get_responsible_unit,
   0x1c9580, in ai.obj) */

#include "cseries.h"
#include "globals.h"
#include "slot_handler.h"
#include "game_state.h"
#include "data_array.h"
#include <string.h>
#include <math.h>

/* what the ai tracks of each local player (2 entries of 0x1c bytes in the
   game state) */
struct s_ai_player
{
	long player_index;
	long unit_index;
	short unknown08;
	short unknown0a;
	byte unknown0c[0x1c - 0xc];
};

#define MAXIMUM_AI_PLAYERS 2

s_ai_player *g_4f55cc;

// @retail 0x1c7fe0
inline void ai_players_reset(void)
{
	long i;

	for (i = 0; i < MAXIMUM_AI_PLAYERS; i++)
	{
		g_4f55cc[i].player_index = NONE;
		g_4f55cc[i].unit_index = NONE;
		g_4f55cc[i].unknown0a = 0;
	}
}

// @retail 0x1c7fa0
void ai_globals_clear(void)
{
	memset(g_4f55d0, 0, sizeof(s_ai_globals));
	ai_players_reset();
}

// @retail 0x1c80f0
void ai_player_add(long player_index)
{
	if (g_4e6948->state == 1)
	{
		bool added = false;
		long i;

		for (i = 0; i < MAXIMUM_AI_PLAYERS; i++)
		{
			s_ai_player *player = &g_4f55cc[i];

			if (player->player_index == NONE && !added)
			{
				memset(player, 0, sizeof(s_ai_player));
				player->player_index = player_index;
				player->unit_index = NONE;
				player->unknown08 = NONE;
				player->unknown0a = 0;
				added = true;
			}
		}
	}
}

// @retail 0x1c8010
void ai_globals_initialize_for_new_map(void)
{
	s_ai_globals *globals = g_4f55d0;
	s_data_iterator iterator;

	memset(globals, 0, sizeof(s_ai_globals));
	globals->enabled = true;
	globals->unknown02 = true;
	globals->unknown14 = NONE;
	globals->unknown340 = true;
	globals->unknown20 = true;
	globals->unknown364 = NONE;
	globals->unknown36c = NONE;
	globals->unknown24.clear();
	globals->unknown2c.clear();
	globals->unknown34.clear();
	ai_players_reset();
	iterator.data = g_4e8c24;
	iterator.index = NONE;
	iterator.datum_index = NONE;
	while (data_iterator_next_inlined(&iterator))
	{
		ai_player_add(iterator.datum_index);
	}
}

// @retail 0x1c8150
inline short ai_player_index_get(long player_index)
{
	short index = NONE;
	long i;

	for (i = 0; i < MAXIMUM_AI_PLAYERS; i++)
	{
		if (g_4f55cc[i].player_index == player_index)
		{
			index = (short)i;
			break;
		}
	}
	return index;
}

// @retail 0x1c8180
s_ai_player *ai_player_get(long player_index)
{
	s_ai_player *player = NULL;
	long index = ai_player_index_get(player_index);

	if (index != NONE)
	{
		player = &g_4f55cc[index];
	}
	return player;
}

// @retail 0x1c8390
void ai_players_unit_deleted(long unit_index)
{
	long i = 0;

	do
	{
		s_ai_player *player = &g_4f55cc[i];

		if (player->unit_index == unit_index)
		{
			player->unit_index = NONE;
			player->unknown08 = NONE;
			player->unknown0a = 0;
		}
		i++;
	}
	while (i < MAXIMUM_AI_PLAYERS);
}

/* the objects (0bad50.cpp) */
struct s_object;
s_object *function_badc0(long object_index, dword type_mask);
long function_baf40(long object_index);

struct s_ai_object_header
{
	short identifier;
	byte flags;
	byte type;
	byte unknown04[4];
	s_object *object;
};

struct s_ai_unit
{
	byte unknown000[0x248];
	long unknown248;
	long unknown24c;
};

// @retail 0x1c9580
long ai_get_responsible_unit(long object_index, bool a)
{
	long result = NONE;

	if (object_index != NONE)
	{
		s_ai_unit *unit = (s_ai_unit *)function_badc0(object_index, 3);

		if (unit)
		{
			if (a && unit->unknown24c != NONE)
			{
				result = unit->unknown24c;
			}
			else if (unit->unknown248 != NONE)
			{
				result = unit->unknown248;
			}
			else
			{
				result = object_index;
			}
		}
	}
	return result;
}

/* the ai globals tag, a block at +0xc8 of the tag header globals */
struct s_ai_globals_definition
{
	real unknown00;
	byte unknown04[4];
	real unknown08;
	byte unknown0c[4];
	real unknown10;
	byte unknown14[4];
	real unknown18;
	byte unknown1c[4];
	real unknown20;
	real unknown24;
	real unknown28;
	real unknown2c;
};

struct s_ai_tag_header_globals
{
	byte unknown00[0xc8];
	long ai_globals_count;
	s_ai_globals_definition *ai_globals;
};

// @retail 0x1c9e50
real function_1c9e50(short index)
{
	s_ai_tag_header_globals *globals = (s_ai_tag_header_globals *)g_4e034c;
	real result = 0.0f;

	if (globals && globals->ai_globals_count > 0)
	{
		s_ai_globals_definition *definition = globals->ai_globals;

		switch (index)
		{
		case 0:
			result = 0.0f;
			break;
		case 1:
			result = definition->unknown00;
			break;
		case 2:
			result = definition->unknown08;
			break;
		case 3:
			result = definition->unknown10;
			break;
		case 4:
			result = definition->unknown18;
			break;
		case 5:
			result = definition->unknown20;
			break;
		case 6:
			result = definition->unknown24;
			break;
		case 7:
			result = definition->unknown28;
			break;
		case 8:
			result = definition->unknown2c;
			break;
		}
	}
	return result;
}

// @retail 0x1c9ee0
real function_1c9ee0(real fraction)
{
	if (fraction < 0.0f)
	{
		fraction = 0.0f;
	}
	else if (fraction > 1.0f)
	{
		fraction = 1.0f;
	}
	return 1.0f - (real)pow(1.0f - fraction, g_510c54->rate);
}

// @retail 0x1caa10
long function_1caa10(long object_index)
{
	long parent_index = function_baf40(object_index);

	if (((s_ai_object_header *)g_4e0300->data)[parent_index & 0xffff].type != 1)
	{
		parent_index = object_index;
	}
	return parent_index;
}

/* what 0x1c8440 scales (its flags at +4) */
struct s_ai_scale_source
{
	byte unknown00[4];
	byte flags;
};

// @retail 0x1c8440
bool function_1c8440(long actor_index, real *value, s_ai_scale_source const *source)
{
	bool result = false;

	if (actor_index != NONE)
	{
		s_actor_view *actor = actor_get(actor_index);

		if ((source->flags & 8) && actor->unknown7c0 > 0.0f)
		{
			*value *= actor->unknown7c0;
			result = true;
		}
	}
	return result;
}
/* the ai's data arrays and game state (0x1c7790 builds them) */
s_data_array *g_51e9dc;
s_data_array *g_502404;
s_data_array *g_51ecb4;
void *g_5044cc;
void *g_5044d0;
void *g_5047f4;
short g_4f5768;

void function_1dfae0(void);
void function_28d930(void);
void function_25c170(void);
void function_200930(void);
void function_257d00(void);
void function_20b930(void);
void function_292130(void);
void function_1a6d80(void);
void function_28d9d0(void);
void function_292e00(void);
void function_292f60(void);

// @retail 0x1c7790
void ai_initialize(void)
{
	g_4f55d0 = (s_ai_globals *)game_state_malloc("ai globals", NULL, sizeof(s_ai_globals));
	g_4f55cc = (s_ai_player *)game_state_malloc("ai players", NULL, MAXIMUM_AI_PLAYERS * sizeof(s_ai_player));
	ai_globals_clear();
	function_1dfae0();
	function_28d930();
	function_25c170();
	function_200930();
	g_5044cc = game_state_malloc("ai 5044cc", NULL, 0x20);
	g_502420 = data_new_inlined("clump", 20, 0x50, 0, g_510c2c);
	g_502424 = data_new_inlined("joint state", 20, 0xbc, 0, g_510c2c);
	g_4f5768 = NONE;
	g_51eca4 = data_new_inlined("dynamic firing points", 15, 0x484, 0, g_510c2c);
	function_257d00();
	function_20b930();
	g_5044d0 = game_state_malloc("ai 5044d0", NULL, 0x10);
	function_292130();
	function_1a6d80();
	g_51ecb4 = data_new_inlined("flocks", 10, 0x28, 0, g_510c2c);
	g_5047f4 = game_state_malloc("ai 5047f4", NULL, 0x24);
}

// @retail 0x1c7b20
void ai_dispose_from_old_map(void)
{
	if (g_4f55d0->active)
	{
		g_51e9d8->valid = false;
		g_51e9dc->valid = false;
		g_502420->valid = false;
		g_502424->valid = false;
		g_502408->valid = false;
		g_502404->valid = false;
		g_51eca4->valid = false;
		g_50241c->valid = false;
		g_502418->valid = false;
		g_502414->valid = false;
		g_4f55f0->valid = false;
		function_28d9d0();
		g_4f9398->valid = false;
		function_292e00();
		g_4f55d0->active = false;
	}
}

// @retail 0x1c7f60
void function_1c7f60(void)
{
	if (g_4f55d0->active)
	{
		function_292f60();
	}
}
/* two scratch buffers the ai borrows (0x22974 bytes each, carved from
   g_510c44); while one is out, g_510c48 is set, and the physics work list
   (0x146de0/0x146b80) is paused if it was running */
struct s_ai_scratch_buffer
{
	bool used;
	byte unknown01[3];
	byte *address;
	long size;
};

#define AI_SCRATCH_BUFFER_COUNT 2
#define AI_SCRATCH_BUFFER_SIZE 0x22974

s_ai_scratch_buffer g_4f55b4[AI_SCRATCH_BUFFER_COUNT];
bool g_51e9b4;
long g_51e9b0;
byte *g_510c44;
bool g_510c48;

void function_146de0(void);
void function_146b80(void);

inline bool ai_physics_work_list_running(void)
{
	return g_47989c != NULL;
}

// @retail 0x1caae0
byte *ai_scratch_buffer_get(void)
{
	byte *result = NULL;
	long i;

	if (!g_51e9b4)
	{
		byte *address;

		g_51e9b0 = ai_physics_work_list_running();
		if (g_51e9b0)
		{
			function_146de0();
		}
		address = g_510c44;
		g_510c48 = true;
		for (i = 0; i < AI_SCRATCH_BUFFER_COUNT; i++)
		{
			g_4f55b4[i].used = false;
			g_4f55b4[i].size = AI_SCRATCH_BUFFER_SIZE;
			g_4f55b4[i].address = address;
			address += g_4f55b4[i].size;
		}
		g_51e9b4 = true;
	}
	for (i = 0; i < AI_SCRATCH_BUFFER_COUNT; i++)
	{
		if (!g_4f55b4[i].used)
		{
			g_4f55b4[i].used = true;
			result = g_4f55b4[i].address;
			break;
		}
	}
	return result;
}

// @retail 0x1cab80
void ai_scratch_buffer_release(byte *address)
{
	long used_count = 0;
	long i;

	for (i = 0; i < AI_SCRATCH_BUFFER_COUNT; i++)
	{
		if (g_4f55b4[i].used)
		{
			if (g_4f55b4[i].address == address)
			{
				g_4f55b4[i].used = false;
			}
			else
			{
				used_count++;
			}
		}
	}
	if (!used_count)
	{
		g_51e9b4 = false;
		g_510c48 = false;
		if (g_51e9b0)
		{
			function_146b80();
		}
	}
}

/* the ai's list of actors and squads by importance (0x1c8700) */
struct s_ai_importance_entry
{
	byte kind;
	long index;
	long importance;
};

#define MAXIMUM_AI_IMPORTANCE_ENTRIES 0x100

struct s_ai_importance_list
{
	short count;
	short unknown2;
	s_ai_importance_entry entries[MAXIMUM_AI_IMPORTANCE_ENTRIES];
};

/* the actor (g_4f55f0) and the squad (g_51e9d8) as the list reads them */
struct s_ai_importance_actor
{
	byte unknown00[9];
	bool unknown09;
	byte unknown0a[0x10 - 0xa];
	long importance;
	byte unknown14[0x20 - 0x14];
	long next_index;
};

struct s_ai_importance_squad
{
	byte unknown00[2];
	byte flags;
	byte unknown03[0xa - 0x3];
	short unknown0a;
	byte unknown0c[0x78 - 0xc];
	long importance;
};

typedef bool (__stdcall *t_sort_compare_function)(void const *a, void const *b, void const *context);
void function_13da70(void *elements, long count, long element_size, t_sort_compare_function compare, void const *context);

// @retail 0x1c86d0
bool __stdcall ai_importance_compare(void const *a, void const *b, void const *context)
{
	s_ai_importance_entry const *entry_a = (s_ai_importance_entry const *)a;
	s_ai_importance_entry const *entry_b = (s_ai_importance_entry const *)b;
	long importance_a = entry_a->importance;
	long importance_b = entry_b->importance;
	bool result;

	if (importance_b < importance_a)
	{
		return true;
	}
	if (importance_b > importance_a)
	{
		return false;
	}
	result = entry_a->kind < entry_b->kind;
	return result;
}

// @retail 0x1c8700
void ai_importance_list_build(long unused, s_ai_importance_list *list, long unused2)
{
	long actor_index;
	s_data_iterator iterator;

	list->count = 0;
	list->unknown2 = 0;
	if (g_4f55d0->active)
	{
		actor_index = g_4f55d0->unknown14;
	}
	while (g_4f55d0->active && actor_index != NONE)
	{
		s_ai_importance_actor *actor = (s_ai_importance_actor *)(g_4f55f0->data + (actor_index & 0xffff) * 0x888);
		long index = actor_index;

		if (list->count >= MAXIMUM_AI_IMPORTANCE_ENTRIES)
		{
			break;
		}
		actor_index = actor->next_index;
		if (!actor->unknown09 && actor->importance != NONE)
		{
			list->entries[list->count].kind = 1;
			list->entries[list->count].index = index;
			list->entries[list->count].importance = actor->importance;
			list->count++;
		}
	}
	if (g_4f55d0->active)
	{
		iterator.data = g_51e9d8;
		iterator.index = NONE;
	}
	while (g_4f55d0->active)
	{
		s_ai_importance_squad *squad = (s_ai_importance_squad *)data_iterator_next_inlined(&iterator);

		if (!squad || list->count >= MAXIMUM_AI_IMPORTANCE_ENTRIES)
		{
			break;
		}
		if (!(squad->flags & 0x80) && squad->unknown0a > 0 && squad->importance != NONE)
		{
			list->entries[list->count].kind = 0;
			list->entries[list->count].index = iterator.datum_index;
			list->entries[list->count].importance = squad->importance;
			list->count++;
		}
	}
	if (list->count > 0)
	{
		function_13da70(list->entries, list->count, sizeof(s_ai_importance_entry), ai_importance_compare, NULL);
	}
}
