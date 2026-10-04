// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1E67A0.CPP: character physics update input datum setters */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_0259d0.h"


struct s_character_physics_component
{
	byte unknown00[0x10];
	point3f position;
	byte unknown1c;
	byte has_position;
};

struct s_type_94656b
{
	long unknown00;
	byte unknown04;
	byte unknown05[0x27];
	point3f point2c;
	point3f point38;
	point3f point44;
	long unknown50;
	byte unknown54[0x58];
	point3f pointac;
	byte byteb8;
	byte byteb9;
	byte unknownba[2];
	byte bytebc;
	byte unknownbd[3];
	point3f pointc0;
	point3f pointcc;
	real valued8;
};

struct s_character_physics_update_input_datum_a
{
	long unknown00;
	long unknown04;
	long unknown08;
	long unknown0c;
	long unknown10;
	byte unknown14;
	byte unknown15[3];
	unsigned long flags;
	point3f point1c;
	point3f point28;
	point3f point34;
	long unknown40;
};

struct s_source_a
{
	byte unknown00;
	byte unknown01[7];
	long unknown08;
	long unknown0c;
};

struct s_time_entry
{
	long time;
	short a;
	short b;
};

// @retail 0x1e67a0
void function_1e67a0(s_character_physics_component *component, s_type_94656b *datum, byte a, byte b)
{
	point3f *point = &component->position;
	if (!component->has_position)
	{
		point = (point3f *)g_4687b0;
	}
	datum->pointac = *point;
	datum->byteb8 = a;
	datum->byteb9 = b;
	datum->unknown04 = 1;
}

// @retail 0x1e67f0
void function_1e67f0(s_type_94656b *datum, s_character_physics_component *component, long animation_id, point3f *p1, point3f *p2, point3f *p3)
{
	datum->unknown50 = animation_id;
	datum->point2c = *p1;
	datum->point38 = *p2;
	datum->point44 = *p3;
	datum->unknown04 = 1;
}

// @retail 0x1e6850
void function_1e6850(s_type_94656b *datum, byte a, point3f *p1, point3f *p2, real v)
{
	datum->bytebc = a;
	datum->pointc0 = *p1;
	datum->pointcc = *p2;
	datum->valued8 = v;
	datum->unknown04 = 1;
}

// @retail 0x1e68a0
void function_1e68a0(s_character_physics_update_input_datum_a *datum, s_source_a *source, long a1, long a2, long a3, bool b0, bool b1, bool b2, bool b3, bool b4, point3f *p1, point3f *p2, point3f *p3)
{
	datum->unknown04 = source->unknown08;
	datum->unknown08 = source->unknown0c;
	datum->unknown0c = a3;
	datum->unknown10 = source->unknown00;
	datum->unknown00 = a1;
	datum->unknown40 = a2;
	datum->flags = 0;
	datum->unknown14 = 0;
	datum->flags = b0 ? 1 : 0;
	if (b1) datum->flags |= 2; else datum->flags &= ~2;
	if (b2) datum->flags |= 4; else datum->flags &= ~4;
	if (b3) datum->flags |= 8; else datum->flags &= ~8;
	if (b4) datum->flags |= 16; else datum->flags &= ~16;
	datum->point1c = *p1;
	datum->point28 = *p2;
	datum->point34 = *p3;
}

// @retail 0x1e6980
void function_1e6980(s_time_entry *entries, short a, byte b)
{
	long index = 0;
	long oldest = entries[0].time;
	if (entries[1].time < oldest)
	{
		index = 1;
		oldest = entries[1].time;
	}
	if (entries[2].time < oldest)
	{
		index = 2;
		oldest = entries[2].time;
	}
	if (entries[3].time < oldest)
	{
		index = 3;
	}
	entries[index].time = g_510c54->game_time;
	entries[index].a = a;
	entries[index].b = b;
}
