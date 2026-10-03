#include <string.h>
#include <math.h>
#include "cseries.h"
#include "globals.h"
#include "unknown_19ec40.h"
#include "engine_peer.h"
#include "game_engine.h"

// @flags /O2 /arch:SSE /Gr

/* The table at 0x45c750 is the vtable of a game engine class; the other
   engine objects (reached through g_55e4d0) are viewed through c_engine_peer. */

/* ---- globals (pointers to the data they name) ---- */

struct s_event
{
	long type;
	long subtype;
	long a;
	long b;
	long c;
	long d;
	long e;
	long f;
	short g;
};

/* the settings an engine update copies (0x24 bytes) */
struct s_engine_settings
{
	word w0;
	word w2;
	word w4;
	word w6;
	word w8;
	s_name18 name;
	byte b1c;
	byte b1d;
	word w1e;
	word w20;
	word unknown22;
};

/* the player update (0x28 bytes, see function_24e0e0) */
struct s_player_update
{
	byte b0;
	byte unknown01[3];
	long l4;
	real_point3d v8;
	long l14;
	byte b18;
	byte unknown19;
	short s1a;
	long l1c;
	word w20;
	byte b22;
	byte b23;
	byte b24;
	byte unknown25;
	word w26[8];
};

struct s_player
{
	short identifier;
	struct
	{
		byte bit0 : 1;
		byte bit1 : 1;
		byte bit2 : 1;
		byte unknown : 5;
	} flags2;
	struct
	{
		byte unknown : 3;
		byte bit3 : 1;
		byte unknown2 : 2;
		byte bit6 : 1;
		byte unknown3 : 1;
	} flags3;
	byte unknown04[0x2c - 4];
	long l2c;
	byte unknown30[0xc0 - 0x30];
	char c0;
	byte unknownc1[0x164 - 0xc1];
	long l164;
	byte unknown168[0x170 - 0x168];
	long l170;
	byte unknown174[0x19c - 0x174];
	long l19c;
	byte unknown1a0[0x1ac - 0x1a0];
	short s1ac;
	byte unknown1ae[2];
	long l1b0;
	byte unknown1b4[0x21c - 0x1b4];
};

struct s_object
{
	byte unknown00[0x13c];
	long owner;
	byte unknown140[0x17e - 0x140];
	short s17e;
};

struct s_object_header
{
	byte unknown00[8];
	s_object *object;
};

struct s_stats_state
{
	byte unknown00[0xc];
	long l0c[1];
};

struct s_game_engine_data
{
	byte unknown00[0x18];
	byte bytes18[3];
};

struct s_tag_b
{
	byte unknown00[0x1c];
	long value;
};

struct s_tag_a
{
	byte unknown00[4];
	s_tag_b *b;
};

s_game_engine_data *g_51ecc4;
real_point3d g_468d18 = { 0.0f, 0.0f, 500.0f };

/* callees not decompiled yet (stubs in src/stubs/game_engine.cpp) */
void function_15b7c0(long, long);
bool function_15eaf0();
long function_23f260(long, long, long);
void function_1523c0();
void function_196780();
void function_15cba0();
void function_1389c0();
void function_a7c50(s_event *);
void function_19eb30(s_event *);
long function_19f3c0(long, long);
void function_19eb90(s_event *);
s_stats_state *function_15e410();
bool function_162550(long);
bool function_19f240(long *);
void function_24e59f(long *);
void function_2bc5c0(long, long *);
void function_2bcf10(long *, long *);
bool function_2bcf90(long *, long *, long);
void function_15fe70(long);
void function_2bc1f0();
void function_2bc990(long);
void unicode_string_list_get_string(long tag_index, long string_id, word *buffer);

static inline s_game_options_view *options()
{
	return g_4e6948;
}

static inline s_player *player_try_get(long index)
{
	s_player *player = 0;

	if (index != NONE && index >= 0 && index < g_4e8c24->high_water_index)
	{
		s_player *p = (s_player *)(g_4e8c24->data + g_4e8c24->size * index);
		if (p->identifier != 0)
			player = p;
	}
	return player;
}

static inline s_player *player_get(long index)
{
	return (s_player *)(g_4e8c24->data + (index & 0xffff) * 0x21c);
}

/* ---- the game engine class ---- */
class c_game_engine_derived : public c_game_engine
{
public:
	virtual void v0(long, long, bool, long);
	virtual bool v4(long);
	virtual bool v23();
	virtual void v28(long);
	virtual void v36(long);
	virtual void v37(long);
	virtual void v40();
	virtual real v41(long);
	virtual void v45(long, long);
	virtual void v46(long, long);
	virtual void v50(long);
};

/* ---- the small default handlers ---- */

// @retail 0x72c70
void c_game_engine::v6(long)
{
}

// @retail 0x99810
long c_game_engine::v9()
{
	return 0;
}

// @retail 0x9a280
long c_game_engine::v22()
{
	return 3;
}

// @retail 0x175f40
void c_game_engine::v10()
{
}

// @retail 0x241510
long c_game_engine::v20(long, long, long, long, long)
{
	return 0;
}

// @retail 0x24dc40
bool c_game_engine::v25()
{
	return true;
}

// @retail 0x24dc50
void c_game_engine::v2(long, long)
{
}

// @retail 0x24dd40
long c_game_engine::v47(long a, long, long)
{
	return a;
}

// @retail 0x24dd60
long c_game_engine::v42(long)
{
	return NONE;
}

// @retail 0x24dd70
bool c_game_engine::v49(short a, short b)
{
	return a != b;
}

// @retail 0x24dd90
long c_game_engine::v1(long, long, long)
{
	return NONE;
}

// @retail 0x24dda0
bool c_game_engine::v5(long, long b)
{
	bool result = false;
	if (b == 2)
		result = options()->flags184.bit12;
	else if (b == 3)
		result = options()->flags184.bit13;
	else if (b == 1)
		result = options()->flags184.bit2;
	return result;
}

// @retail 0x24de40
long c_game_engine::v7(long, byte *b)
{
	*b = 1;
	return NONE;
}

// @retail 0x24de50
void c_game_engine::v17(long, long, real *out)
{
	memset(out, 0, 0x38);
	out[5] = 1.0f;
	((long *)out)[7] = NONE;
}

// @retail 0x24e320
void c_game_engine::v11(s_engine_settings *settings)
{
	memset(settings, 0, 0x24);
	memset(&settings->name, 0xff, sizeof(settings->name));
}

// @retail 0x24e360
void c_game_engine::v12(byte mask, dword *changed, s_engine_settings *settings)
{
	s_mp_globals *g = g_4e9ae8;

	if (mask & 1)
	{
		if (settings->w0 != g->w6 || settings->w2 != g->w8 || settings->w4 != g->wa ||
			settings->w6 != g->wc || settings->w8 != g->we ||
			memcmp(&settings->name, &g->name, sizeof(s_name18)) != 0)
		{
			settings->w0 = g->w6;
			settings->w2 = g->w8;
			settings->w4 = g->wa;
			settings->w6 = g->wc;
			settings->w8 = g->we;
			settings->name = g->name;
			*changed |= 1;
		}
	}
	if (mask & 2)
	{
		if (settings->b1c != g->w6c)
		{
			settings->b1c = (byte)g->w6c;
			*changed |= 2;
		}
	}
	if (mask & 4)
	{
		if ((bool)settings->b1d != g->bc08)
		{
			settings->b1d = g->bc08 != 0;
			*changed |= 4;
		}
	}
	if (mask & 8)
	{
		if (settings->w1e != g->w6e)
		{
			settings->w1e = g->w6e;
			*changed |= 8;
		}
	}
	if (mask & 0x10)
	{
		if (settings->w20 != g->we0)
		{
			settings->w20 = g->we0;
			*changed |= 0x10;
		}
	}
}

// @retail 0x24e490
bool c_game_engine::v13(byte mask, s_engine_settings *settings)
{
	if (mask & 1)
	{
		s_mp_globals *g = g_4e9ae8;
		g->w6 = settings->w0;
		g->w8 = settings->w2;
		g->wa = settings->w4;
		g->wc = settings->w6;
		g->we = settings->w8;
		g->name = settings->name;
		function_1523c0();
		function_196780();
	}
	s_mp_globals *g = g_4e9ae8;
	if (mask & 2)
	{
		if (settings->b1c != g->w6c)
		{
			function_15cba0();
			g = g_4e9ae8;
			g->w6c = settings->b1c;
		}
	}
	if (mask & 4)
		function_1389c0();
	if (mask & 8)
		g->w6e = settings->w1e;
	if (mask & 0x10)
		g->we0 = settings->w20;
	return true;
}

// @retail 0x24e0e0
bool c_game_engine::v19(short player_index, dword mask, long, s_player_update *update)
{
	long index = player_index;
	s_player *player = player_try_get(index);
	bool result = false;

	if (player)
	{
		s_mp_globals *g = g_4e9ae8;
		if (mask & 1)
		{
			player->l164 = update->l4;
			g->players[index].w10 = (word)update->l4;
			g->players[index].w12 = (word)update->l4;
			g->players[index].v = update->v8;
			if (fabs(g->players[index].v.x - g_468d18.x) < 0.0001f &&
				fabs(g->players[index].v.y - g_468d18.y) < 0.0001f &&
				fabs(g->players[index].v.z - g_468d18.z) < 0.0001f)
				g->players[index].b0 = 0;
			else
				g->players[index].b0 = 1;
		}
		if (mask & 2)
			player->l19c = update->l14;
		if (mask & 4)
			g->players[index].b14 = update->b0;
		if (mask & 8)
		{
			player->flags2.bit2 = update->b18;
		}
		if (mask & 0x10)
			memcpy(&g->l6dc[index * 4], update->w26, 16);
		if (mask & 0x20)
			player->s1ac = update->s1a;
		if (mask & 0x40)
		{
			long owner = update->l1c;
			s_player *op = player_try_get(owner);
			long value = NONE;
			if (op)
			{
				value = NONE;
				if (owner != NONE)
					value = (player_get(owner)->identifier << 16) | owner;
			}
			player->l1b0 = value;
		}
		if (mask & 0x80)
			player->l170 = update->w20;
		if (mask & 0x100)
		{
			player->flags3.bit3 = update->b22;
		}
		if (mask & 0x200)
		{
			player->flags2.bit0 = update->b23;
		}
		if (mask & 0x400)
		{
			player->flags3.bit6 = update->b24;
		}
		result = true;
	}
	return result;
}
/* ---- the peer helpers (slots 14..16 of the retail table) ---- */

// @retail 0x2bc1a0
bool c_game_engine_derived::v4(long a)
{
	bool result = false;

	switch (a)
	{
	case 1:
		result = true;
		break;
	}
	return result;
}

// @retail 0x2bc000
void c_game_engine_derived::v0(long killer, long victim, bool suicide_or_betrayal, long weapon)
{
	if (options()->mode != 4)
	{
		if (killer != NONE && !player_get(victim)->flags2.bit1)
		{
			long delta;
			if (!suicide_or_betrayal)
				delta = 1;
			else
			{
				if (victim == killer && !(options()->flags22c & 2))
					goto skip;
				delta = NONE;
			}
			function_15b7c0(delta, killer);
		}
skip:
		if (options()->flags22c & 4)
			function_15b7c0(NONE, victim);
		if (options()->flags22c & 1)
		{
			long valid = 0;
			if (weapon != NONE && weapon >= 0 && weapon <= 0xe)
				valid = 1;
			if (killer != NONE && killer != victim && !suicide_or_betrayal)
			{
				long team;
				if (function_15eaf0())
					team = player_get(killer)->c0;
				else
					team = NONE;
				long r = function_23f260(0, victim, team);
				if (r / 2 == 0)
					valid++;
			}
			function_15b7c0(valid, killer);
		}
	}
}

// @retail 0x2bc2e0
bool c_game_engine_derived::v23()
{
	s_stats *stats = &g_4e9ae8->stats;
	memset(stats, 0, sizeof(s_stats));
	g_51ecc4 = (s_game_engine_data *)stats;
	long *p = (long *)((byte *)stats + 0xc);
	p[0] = NONE;
	p[1] = NONE;
	p[2] = NONE;
	function_19ec40(0, 0.0f, 4, 8, NONE, 3, p, 0.0f);
	return true;
}

// @retail 0x2bc370
void c_game_engine_derived::v28(long a)
{
	if (options()->mode != 4)
	{
		s_event e;
		e.type = 4;
		e.subtype = 0;
		e.a = a;
		e.b = NONE;
		e.c = NONE;
		e.d = NONE;
		e.e = NONE;
		e.f = 0;
		e.g = NONE;
		function_a7c50(&e);
		function_19eb30(&e);
	}
}

// @retail 0x2bc3d0
real c_game_engine_derived::v41(long a)
{
	real result = 1.0f;

	if (function_19f3c0(a, 2) != NONE)
	{
		switch (options()->s234)
		{
		case 1:
			break;
		case 2:
			result = 1.25f;
			break;
		default:
			result = 0.75f;
			break;
		}
	}
	return result;
}

// @retail 0x2bc420
void c_game_engine_derived::v45(long a, long b)
{
	s_data_array *objects = g_4e0300;
	long index = ((s_object_header *)objects->data)[a & 0xffff].object->s17e;

	if (index >= 0 && index < 3)
	{
		g_51ecc4->bytes18[index] = ((byte *)g_510c54)[2];
		if (b != NONE)
		{
			long owner = ((s_object_header *)objects->data)[b & 0xffff].object->owner;
			if (owner != NONE)
			{
				s_event e;
				e.type = 4;
				e.subtype = 2;
				e.a = NONE;
				e.b = owner;
				e.c = player_get(owner)->c0;
				e.d = NONE;
				e.e = NONE;
				e.f = 0;
				e.g = NONE;
				function_19eb90(&e);
			}
		}
	}
}

// @retail 0x2bc4f0
void c_game_engine_derived::v46(long a, long b)
{
	s_data_array *objects = g_4e0300;
	long index = ((s_object_header *)objects->data)[a & 0xffff].object->s17e;

	if (index >= 0 && index < 3)
	{
		if (b != NONE)
		{
			long owner = ((s_object_header *)objects->data)[b & 0xffff].object->owner;
			if (owner != NONE)
			{
				s_event e;
				e.type = 4;
				e.subtype = 3;
				e.a = NONE;
				e.b = owner;
				e.c = player_get(owner)->c0;
				e.d = NONE;
				e.e = NONE;
				e.f = 0;
				e.g = NONE;
				function_19eb90(&e);
			}
		}
		g_51ecc4->bytes18[index] = NONE;
	}
}

// @retail 0x2bc920
void c_game_engine_derived::v37(long a)
{
	if (player_get(a)->l2c != NONE)
	{
		if (function_19f3c0(a, 2) != NONE)
		{
			c_engine_peer *engine = g_55e4d0[g_4e9ae8->engine_index];
			if (!engine->p35(a, 1))
			{
				if (options()->mode != 4)
					function_15fe70(a);
			}
		}
	}
}

// @retail 0x2bcbb0
void c_game_engine_derived::v40()
{
	if (options()->mode != 4)
	{
		for (long i = 0; i < options()->s230; i++)
		{
			if (((s_stats_state *)g_51ecc4)->l0c[i] != NONE)
			{
				if (function_15e410())
					function_2bc990(i);
				else
					function_2bc1f0();
			}
		}
	}
}

// @retail 0x2bd910
void c_game_engine_derived::v50(long a)
{
	s_tag_header_globals *view = g_4e034c;

	if (view && view->index != NONE)
	{
		s_tag_a *tag = (s_tag_a *)g_4e3b44[view->index & 0xffff].data;
		long value = tag->b->value;
		if (value != NONE)
			unicode_string_list_get_string(value, 0x7000232, (word *)a);
	}
}

// @retail 0x2bc6e0
void c_game_engine_derived::v36(long a)
{
	long p = NONE;
	if (a != NONE)
		p = g_4e8c20->entries[a];
	s_player *player = player_get(p);
	long mode = options()->s236;
	long count = options()->s230;

	for (long i = 0; i < count; i++)
	{
		long buf30[4];
		long buf34[3];
		long buf40[3];
		s_stats_state *s = function_15e410();
		function_2bc5c0(i, buf30);
		if (s)
		{
			bool go = false;
			if (mode == 0)
			{
				long other = s->l0c[0];
				if (other == NONE)
					go = true;
				else
				{
					c_engine_peer *engine = g_55e4d0[g_4e9ae8->engine_index];
					if (engine && engine->p27(player_get(other)->c0, player->c0))
						go = true;
				}
			}
			else if (mode == 1)
			{
				if (s->l0c[0] == NONE)
					go = true;
			}
			if (go && function_2bcf90(buf34, buf40, *(long *)s))
				function_24e59f(buf40);
		}
	}

	c_engine_peer *engine = g_55e4d0[g_4e9ae8->engine_index];
	if (p != NONE && engine && options()->flags184.bit0 && player->l2c != NONE)
	{
		struct
		{
			long object;
			s_data_array *array;
			long index;
			long next;
		} it;
		long buf30[4];
		long buf3c[3];

		it.array = g_4e8c24;
		it.next = NONE;
		it.index = NONE;
		while (function_19f240((long *)&it))
		{
			long other = it.index;
			if (other != p)
			{
				if (engine && engine->p27(((s_player *)it.object)->c0, player->c0))
					continue;
				if (function_162550(other))
				{
					long object = function_19f3c0(other, 2);
					if (object != NONE && mode != 3)
					{
						s_data_array *objects = g_4e0300;
						function_2bc5c0(((s_object_header *)objects->data)[object & 0xffff].object->s17e, buf30);
						function_2bcf10(buf3c, buf30);
					}
					function_24e59f(buf3c);
				}
			}
		}
	}
}
