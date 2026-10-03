/* EFFECTS.H: the effects, their locations and the particle systems they
   start (src/unknown_175bd0.cpp, src/unknown_173b90.cpp) */

#ifndef EFFECTS_H
#define EFFECTS_H

#include "cseries.h"
#include "real_math.h"
#include "data_array.h"
#include "globals.h"
#include "object_queries.h"

/* a marker an effect is placed at: a point, a direction and the marker's
   name (a string id) */
struct s_effect_marker
{
	real_point3d position;
	real_vector3d forward;
	dword name;
};

/* who caused an effect */
struct s_effect_owner
{
	long unknown0;
	long unknown4;
	short unknown8;
};

/* what an effect is started near (s_effect_parameters::source) */
struct s_effect_source
{
	byte unknown00[8];
	real_point3d position;
	byte unknown14[0xc];
	short index;
};

/* what a new effect is made from (0x60 bytes) */
struct s_effect_parameters
{
	union
	{
		dword flags;
		struct
		{
			dword attached : 1;
			dword flag1 : 1;
			dword flag2 : 1;
			dword colors_set : 1;
			dword : 28;
		};
	};
	long tag_index;
	long object_index;
	s_effect_owner owner;
	short unknown18;
	s_effect_marker *markers;
	long marker_count;
	real_vector3d velocity;
	long unknown30;
	long unknown34;
	long unknown38;
	short unknown3c;
	real scale_a;
	real scale_b;
	real_point3d const *origin;
	real_vector3d const *direction;
	long const *unknown50;
	dword color_a;
	dword color_b;
	s_effect_source *source;
};

/* a particle system of an effect event (0x38 bytes) */
struct s_effect_particle_system_definition
{
	byte unknown00[4];
	long tag_index;
	long location_index;
	short unknown0c;
	short placement;
	short unknown10;
	word location_mode;
	byte unknown14[2];
	byte flag0 : 1;
	byte flag1 : 1;
	byte flag2 : 1;
	byte flag3 : 1;
	byte : 4;
	byte unknown17[0x30 - 0x17];
	long unknown30;
	byte unknown34[4];
};

/* the effect tag ('effe') */
struct s_effect_part
{
	byte unknown00[0xc];
	dword group_tag;
	long tag_index;
	byte unknown14[0x38 - 0x14];
};

struct s_effect_event
{
	byte unknown00[4];
	real skip_chance;
	real delay_lower;
	real delay_upper;
	real duration_lower;
	real duration_upper;
	long part_count;
	s_effect_part *parts;
	byte unknown20[0x10];
	long particle_system_count;
	s_effect_particle_system_definition *particle_systems;
};

struct s_effect_definition
{
	dword flag0 : 1;
	dword flag1 : 1;
	dword flag2 : 1;
	dword flag3 : 1;
	dword flag4 : 1;
	dword flag5 : 1;
	dword : 26;
	short restart_event_index;
	byte unknown06[2];
	real unknown08;
	long location_count;
	dword *locations;
	long event_count;
	s_effect_event *events;
	byte unknown1c[4];
	long looping_sound_tag_index;
	short looping_sound_location;
	byte unknown26[2];
	real distance_lower;
	real distance_upper;
};

struct s_effect_event_slot
{
	long unknown0;
	long unknown4;
};

/* the effects (g_4ea93c, 0x190 bytes each) */
struct s_effect_datum
{
	short salt;
	union
	{
		word flags;
		struct
		{
			word flag0 : 1;
			word flag1 : 1;
			word flag2 : 1;
			word flag3 : 1;
			word flag4 : 1;
			word flag5 : 1;
			word flag6 : 1;
			word flag7 : 1;
			word flag8 : 1;
			word flag9 : 1;
			word flag10 : 1;
			word : 5;
		};
	};
	long tag_index;
	long looping_sound_index;
	long unknown0c;
	long unknown10;
	long unknown14;
	short unknown18;
	short unknown1a;
	s_location location;
	real_point3d origin;
	real_vector3d velocity;
	union
	{
		real_vector3d direction;
		struct
		{
			real unknown3c;
			long unknown40;
			long unknown44;
		};
	};
	long object_index;
	s_effect_owner owner;
	long unknown58;
	short event_index;
	short unknown5e;
	real unknown60;
	real event_delay;
	real unknown68;
	real scale_a;
	real scale_b;
	real unknown74;
	real unknown78;
	long location_indices[32];
	long first_particle_system_index;
	long last_particle_system_index;
	dword color_a;
	dword color_b;
	byte unknown10c[4];
	s_effect_event_slot event_slots[16];
};

/* the effect locations (g_4ea938, 0x3c bytes each): chained per marker of
   an effect */
struct s_effect_location_datum
{
	short salt;
	short node_index;
	long next_index;
	real_matrix4x3 matrix;
};

/* the particle systems (g_510c74, 0x54 bytes each) */
struct s_particle_system_datum
{
	short salt;
	short unknown02;
	real unknown04;
	real unknown08;
	word flag0 : 1;
	word flag1 : 1;
	word flag2 : 1;
	word flag3 : 1;
	word flag4 : 1;
	word flag5 : 1;
	word flag6 : 1;
	word flag7 : 1;
	word flag8 : 1;
	word flag9 : 1;
	word flag10 : 1;
	word : 5;
	word definition_index;
	long tag_index;
	long event_index;
	long effect_index;
	s_location location;
	long unknown24;
	real random_a;
	real random_b;
	long location_index;
	long unknown34;
	long next_index;
	long previous_index;
	long first_child_index;
	long last_child_index;
	s_particle_system_datum *parent;
	long unknown4c;
	dword color;

	void set_location(s_location const *location);
	struct s_effect_particle_system_definition *get_definition();
};

/* the particle locations (g_51ec8c, 0x34 bytes each) */
struct s_particle_location_datum
{
	short salt;
	short unknown02;
	byte unknown04[8];
	long next_index;
	real_point3d position;
	byte unknown1c[0x34 - 0x1c];
};

extern s_data_array *g_4ea93c;
extern s_data_array *g_4ea938;
extern s_data_array *g_510c74;
extern s_data_array *g_51ec84;
extern s_data_array *g_51ec88;
extern s_data_array *g_51ec8c;

#define DATUM(array, type, index) ((type *)((array)->data) + ((index) & 0xffff))

/* the definition (and the group) of a tag */
#define TAG_GET(type, index) ((type *)g_4e3b44[(index) & 0xffff].bytes)
#define TAG_GROUP(index) (*(dword *)&g_4e3b44[(short)(index)])

/* the datum of an index that may be stale (the salt is checked) */
static inline byte *datum_try_and_get(s_data_array *data, long datum_index)
{
	byte *result = 0;

	if (datum_index != NONE)
	{
		long index = datum_index & 0xffff;

		if (index < data->high_water_index)
		{
			byte *datum = data->data + data->size * index;

			if (*(short *)datum != 0 && *(short *)datum == (datum_index >> 16))
				result = datum;
		}
	}
	return result;
}

/* data_make_valid (0x16b790), which retail inlines into the lifecycle
   callbacks */
static inline void data_make_valid_inlined(s_data_array *data)
{
	data->valid = true;
	data_delete_all(data);
}

/* the particle systems' lifecycle steps, which retail inlines into the
   effects' */
static inline void particle_systems_initialize_for_new_map(void)
{
	data_make_valid_inlined(g_510c74);
	data_make_valid_inlined(g_51ec84);
	data_make_valid_inlined(g_51ec88);
	data_make_valid_inlined(g_51ec8c);
}

static inline void particle_systems_dispose_from_old_map(void)
{
	g_51ec8c->valid = false;
	g_51ec88->valid = false;
	g_51ec84->valid = false;
	g_510c74->valid = false;
}

/* the particle system tags: the objects at 0x479868 and 0x479874 that
   function_137bd0 (unknown_0e4050.cpp) picks by the group of a tag */
class c_particle_system
{
public:
	virtual void v0() {}
	virtual void v1() {}
	virtual void v2() {}
	virtual void v3() {}
	virtual void v4() {}
	virtual void v5() {}
	virtual void v6() {}
	virtual struct s_effect_particle_system_definition *get_definition(word index) { return 0; }
	virtual void v8() {}
	virtual void v9() {}
	virtual void v10() {}
	virtual void v11() {}
	virtual void v12() {}
	virtual void v13() {}
	virtual void v14() {}
	virtual void v15() {}
	virtual void v16() {}
	virtual bool multiplied() { return false; }
	virtual bool tinted() { return false; }
	virtual void v19() {}
	virtual void initialize(long tag_index) {}
};

c_particle_system *function_137bd0(long tag_index);

/* what a particle system spawns from (function_178c80) */
struct s_particle_system_spawn
{
	real scale;
	real unknown;
	long location_index;
};

/* particles (src/unknown_173b90.cpp) */
void particle_systems_initialize(void);
void particle_systems_update_locations(void);
void __stdcall particle_system_delete(long particle_system_index);
void particle_system_unlink(s_particle_system_datum *particle_system, long *first_index, long *last_index);
void particle_system_link(s_particle_system_datum *particle_system, long *last_index, long *first_index);
long function_173fd0(struct s_effect_particle_system_definition *definition, long effect_index, long tag_index, short definition_index, long event_index);
void function_175a80(bool tinted, dword color_a, dword color_b, s_particle_system_datum *particle_system, bool multiplied);
void function_175270(s_particle_system_datum *particle_system, s_particle_system_spawn *spawn, real_matrix4x3 const *matrix, bool first_person);

/* effects (src/unknown_175bd0.cpp) */
void effect_delete(long effect_index);
void effect_remove_event_slot(long effect_index, long value);
void function_176780(long object_index, real_vector3d const *velocity, real scale_a, long tag_index, real scale_b, real_point3d const *origin, real_vector3d const *direction);

#endif
