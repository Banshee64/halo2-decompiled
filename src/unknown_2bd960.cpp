#include <string.h>
#include <math.h>
#include "cseries.h"
#include "globals.h"
#include "engine_peer.h"
#include "unknown_2bd960.h"

// @flags /O2 /arch:SSE /Gr

/* ---- local views ---- */

/* the game options (g_4e6948): the word at 0x22c is also a set of flags */
struct s_options_2bd
{
	byte unknown00[0x22c];
	union
	{
		struct
		{
			dword bit0 : 1;
			dword bit1 : 1;
			dword bit2 : 1;
			dword bit3 : 1;
			dword bit4 : 1;
			dword unknown : 27;
		} flags22c;
		struct
		{
			word w22c;
			short s22e;
		};
	};
	short s230;
};

/* a player (g_4e8c24 elements, 0x21c bytes) */
struct s_player_2bd
{
	byte unknown00[0x2c];
	long object_index;
	byte unknown30[0x1b8 - 0x30];
	short s1b8;
	byte unknown1ba[0x21c - 0x1ba];
};

/* the engine state (g_51ecc8): four points are set to a default */
struct s_state_2bd
{
	byte unknown00[0x1a0];
	long l1a0;
	byte unknown1a4[4];
	short s1a8;
	byte unknown1aa[2];
	real_point3d p1ac;
	real_point3d p1b8;
	real_point3d p1c4;
	real_point3d p1d0;
};

/* the engine state in the multiplayer globals (g_51eccc, 0x118 bytes at
   g_4e9ae8 + 0xfc) */
struct s_state_2bf
{
	real f0[8];
	real f20[8];
	real f40[8];
	union
	{
		word w60[8];
		long l60[4];
	};
	long l70[8];
	byte unknown90[0x110 - 0x90];
	word w110;
	word w112;
	word w114;
	byte unknown116[2];
};

struct s_spline_2bd
{
	long indices[24];
	long count;
};

/* a polygon of up to 32 vertices inside a circle and a height range */
struct s_polygon_2be
{
	byte unknown00[0x60];
	long count;
	real_point2d vertices[32];
	real center_x;
	real center_y;
	byte unknown16c[4];
	real radius;
	real z_min;
	real z_max;
};

/* the settings an engine update copies, as the peer sees them */
struct s_settings_2bd
{
	byte unknown00[0x24];
	short s24;
	short s26;
};

struct s_stats_a
{
	long l[10];
};

struct s_stats_b
{
	long l[9];
	long b[8];
	long unknown44[8];
};

s_state_2bd *g_51ecc8;
s_state_2bf *g_51eccc;
real_point3d *g_468710;

/* the entries of the points table (g_4e0350 + 0x11c, 32 bytes each) */
struct s_point_entry_2bd
{
	real_point3d position;
	byte unknown0c[0x14];
};

struct s_point_globals_2bd
{
	byte unknown00[0x11c];
	s_point_entry_2bd *entries;
};

/* callees */
real_point3d *function_b9dd0(long object_index, real_point3d *result);
long function_19ec40(real_point3d const *, real, short, short, short, long, long *, real);

static inline s_options_2bd *options()
{
	return (s_options_2bd *)g_4e6948;
}

static inline s_point_globals_2bd *point_globals()
{
	return (s_point_globals_2bd *)g_4e0350;
}

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

/* ---- the engine classes at 0x45c8f0 and 0x45c9c0 ---- */

class c_game_engine_a : public c_game_engine
{
public:
	virtual bool v5(long, long);
	virtual bool v23();
	virtual void v34();
};

class c_game_engine_b : public c_game_engine
{
public:
	virtual bool v5(long, long);
};

/* slots 14 and 15 of the first, slot 14 of the second: wrappers that run on
   the peer object (see unknown_2bcdd0.cpp) */
class c_engine_peer_a : public c_engine_peer
{
public:
	virtual void q0(long, s_stats_a *);
	virtual void q1(dword *, long, s_settings_2bd *);
};

class c_engine_peer_b : public c_engine_peer
{
public:
	virtual void q0(long, s_stats_b *);
};

// @retail 0x2bd960
void function_2bd960()
{
	s_state_2bd *state = g_51ecc8;

	state->p1ac = *g_468710;
	state->p1b8 = *g_468710;
	state->p1c4 = *g_468710;
	state->p1d0 = *g_468710;
}

// @retail 0x2bd9d0
bool c_game_engine_a::v5(long a, long b)
{
	s_player_2bd *player = (s_player_2bd *)(g_4e8c24->data + (a & 0xffff) * 0x21c);
	bool result = false;

	if (player->s1b8 > 0)
	{
		if (b == 2)
			result = options()->flags22c.bit2;
		else if (b == 3)
			result = options()->flags22c.bit3;
		else if (b == 1)
			result = options()->flags22c.bit4;
	}
	else
		result = c_game_engine::v5(a, b);
	return result;
}

// @retail 0x2bdb40
void c_engine_peer_a::q0(long, s_stats_a *stats)
{
	memset(stats, 0, sizeof(s_stats_a));
	p41((s_stats *)stats);
}

// @retail 0x2bdb80
void c_engine_peer_a::q1(dword *value, long, s_settings_2bd *settings)
{
	long result = 0;
	dword m = *value & 0x1f;

	if (m)
		p42(m, &result, (long)settings);
	s_state_2bd *state = g_51ecc8;
	if (*value & 0x20)
	{
		if (settings->s24 != state->l1a0)
		{
			settings->s24 = (short)state->l1a0;
			result |= 0x20;
		}
	}
	if (*value & 0x40)
	{
		if (settings->s26 != state->s1a8)
		{
			settings->s26 = state->s1a8;
			result |= 0x40;
		}
	}
	*value = result;
}

// @retail 0x2bdd20
void function_2bdd20(s_spline_2bd *spline, real_point3d *points)
{
	for (long i = 0; i < spline->count; i++, points++)
	{
		s_point_globals_2bd *globals = (s_point_globals_2bd *)g_4e0350;
		s_point_entry_2bd *entry = &globals->entries[spline->indices[i]];

		if (points)
			*points = entry->position;
	}
}

// @retail 0x2bdd70
void function_2bdd70(long n, real_point3d *points, real_point3d *out, s_spline_2bd *spline)
{
	long i = n * 2;
	long previous = i - 1;
	long next = i + 2;

	if (previous < 0)
		previous += spline->count;
	if (next >= spline->count)
		next -= spline->count;

	real_point3d *a = &points[previous];
	real_point3d *b = &points[i];
	real_point3d *c = &points[i + 1];
	real_point3d *d = &points[next];
	out[0].x = b->x + a->x;
	out[0].y = b->y + a->y;
	out[0].z = b->z + a->z;
	out[0].x *= 0.5f;
	out[0].y *= 0.5f;
	out[0].z *= 0.5f;
	out[1] = *b;
	out[2] = *c;
	out[3].x = d->x + c->x;
	out[3].y = d->y + c->y;
	out[3].z = d->z + c->z;
	out[3].x *= 0.5f;
	out[3].y *= 0.5f;
	out[3].z *= 0.5f;
}

// @retail 0x2bde90
void function_2bde90(real t, real_point3d *out, real_point3d *points)
{
	real t2 = t * t;
	real t3 = t2 * t;
	real m[4][4] =
	{
		{ -1.0f, 3.0f, -3.0f, 1.0f },
		{ 3.0f, -6.0f, 3.0f, 0.0f },
		{ -3.0f, 3.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f, 0.0f }
	};
	real r[4];
	long i = 0;

	do
	{
		r[i] = m[i][0] * t3 + m[i][2] * t + m[i][1] * t2 + m[i][3];
		i++;
	}
	while (i < 4);

	*out = *g_468788;
	out->x = points[3].x * r[3] + points[2].x * r[2] + points[1].x * r[1] + points[0].x * r[0];
	out->y = points[3].y * r[3] + points[2].y * r[2] + points[1].y * r[1] + points[0].y * r[0];
	out->z = points[3].z * r[3] + points[2].z * r[2] + points[1].z * r[1] + points[0].z * r[0];
}

// @retail 0x2be880
bool function_2be880(long player_index, s_polygon_2be *polygon)
{
	bool result = false;

	if (player_index != NONE && polygon->count >= 4 && !(polygon->count & 1))
	{
		s_player_2bd *player = (s_player_2bd *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c);

		if (player->object_index != NONE)
		{
			real_point3d position;

			function_b9dd0(player->object_index, &position);
			if (position.z >= polygon->z_min && polygon->z_max >= position.z)
			{
				real dx = polygon->center_x - position.x;
				real dy = polygon->center_y - position.y;

				if (dx * dx + dy * dy < polygon->radius * polygon->radius)
				{
					long crossings = 0;

					for (long i = 0; i < 32; i++)
					{
						long j = (i != 0) ? i - 1 : 31;
						real d = (polygon->vertices[i].y - polygon->vertices[j].y) * -100.0f;

						if (fabs(d) > 1e-4f)
						{
							real a = polygon->vertices[j].y - position.y;
							real t = a * 100.0f;
							real inv = 1.0f / d;
							t *= inv;
							real s =((polygon->vertices[i].x - polygon->vertices[j].x) * a - (polygon->vertices[i].y - polygon->vertices[j].y) * (polygon->vertices[j].x - position.x)) * inv;

							if (t >= 0.0f && t < 1.0f && s >= 0.0f && s < 1.0f)
								crossings++;
						}
					}
					result = crossings & 1;
				}
			}
		}
	}
	return result;
}

// @retail 0x2bf310
bool c_game_engine_a::v23()
{
	s_state_2bf *state = (s_state_2bf *)((byte *)g_4e9ae8 + 0xfc);

	memset(state, 0, 0x118);
	for (long i = 0; i < 4; i++)
		state->l60[i] = NONE;
	for (long j = 0; j < 8; j++)
		state->l70[j] = NONE;
	g_51eccc = state;

	short n = options()->s22e;
	if (n < 1)
		n = 1;
	state->w110 = g_510c54->ticks_per_second * n;

	short m = options()->s230;
	if (m < 1)
		m = 1;
	state->w112 = m * g_510c54->ticks_per_second;
	state->w114 = options()->w22c;
	return true;
}

// @retail 0x2bf5b0
void c_game_engine_a::v34()
{
	s_state_2bf *state = g_51eccc;
	s_point_globals_2bd *globals = point_globals();

	for (long i = 0; i < 8; i++)
	{
		short n = options()->w22c;

		if (n == 0 || n > i)
		{
			long ids[8];
			long count = function_19ec40(0, 0.0f, 10, NONE, i, 8, ids, 0.0f);

			if (count >= 1)
			{
				state->w60[i] = (word)ids[0];
				real_point3d p0 = globals->entries[ids[0]].position;
				state->f0[i] = 1.0f;
				state->f20[i] = 0.1f;
				state->f40[i] = 0.9f;
				for (long j = 1; j < count; j++)
				{
					real_point3d p = globals->entries[ids[j]].position;
					real d = (real)sqrt((p.x - p0.x) * (p.x - p0.x) + (p.y - p0.y) * (p.y - p0.y));
					real below = p0.z - p.z;
					real above = p.z - p0.z + 0.8f;

					state->f0[i] = MAX(state->f0[i], d);
					state->f20[i] = MAX(state->f20[i], below);
					state->f40[i] = MAX(state->f40[i], above);
				}
			}
		}
	}
}

// @retail 0x2c02e0
bool c_game_engine_b::v5(long a, long b)
{
	return c_game_engine::v5(a, b);
}

// @retail 0x2c0410
void c_engine_peer_b::q0(long, s_stats_b *stats)
{
	memset(stats, 0, sizeof(s_stats_b));
	memset(stats->b, 0xff, sizeof(stats->b));
	p41((s_stats *)stats);
}
