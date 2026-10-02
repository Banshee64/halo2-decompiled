// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1E67A0.CPP: character physics update input datum setters */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"

extern real_vector3d *g_4687b0;

struct s_character_physics_component
{
	byte unknown00[0x10];
	real_point3d position;
	byte unknown1c;
	byte has_position;
};

struct s_character_physics_update_input_datum
{
	long unknown00;
	byte unknown04;
	byte unknown05[0x27];
	real_point3d point2c;
	real_point3d point38;
	real_point3d point44;
	long unknown50;
	byte unknown54[0x58];
	real_point3d pointac;
	byte byteb8;
	byte byteb9;
	byte unknownba[2];
	byte bytebc;
	byte unknownbd[3];
	real_point3d pointc0;
	real_point3d pointcc;
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
	real_point3d point1c;
	real_point3d point28;
	real_point3d point34;
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
void function_1e67a0(s_character_physics_component *component, s_character_physics_update_input_datum *datum, byte a, byte b)
{
	real_point3d *point = &component->position;
	if (!component->has_position)
	{
		point = (real_point3d *)g_4687b0;
	}
	datum->pointac = *point;
	datum->byteb8 = a;
	datum->byteb9 = b;
	datum->unknown04 = 1;
}

// @retail 0x1e67f0
void character_physics_update_input_datum_initialize_sentinel(s_character_physics_update_input_datum *datum, s_character_physics_component *component, long animation_id, real_point3d *p1, real_point3d *p2, real_point3d *p3)
{
	datum->unknown50 = animation_id;
	datum->point2c = *p1;
	datum->point38 = *p2;
	datum->point44 = *p3;
	datum->unknown04 = 1;
}

// @retail 0x1e6850
void function_1e6850(s_character_physics_update_input_datum *datum, byte a, real_point3d *p1, real_point3d *p2, real v)
{
	datum->bytebc = a;
	datum->pointc0 = *p1;
	datum->pointcc = *p2;
	datum->valued8 = v;
	datum->unknown04 = 1;
}

// @retail 0x1e68a0
void function_1e68a0(s_character_physics_update_input_datum_a *datum, s_source_a *source, long a1, long a2, long a3, bool b0, bool b1, bool b2, bool b3, bool b4, real_point3d *p1, real_point3d *p2, real_point3d *p3)
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
