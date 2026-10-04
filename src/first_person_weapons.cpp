// @flags /O1 /Oi /arch:SSE /Gr
/* FIRST_PERSON_WEAPONS.CPP: the first person weapons of the four local
   users (0x165ce5..0x1689a6, built for size; the same object as
   unknown_165cc3.cpp, unknown_16658d.cpp and lane R's unknown_166244.cpp).
   Each user holds the unit it views from and two weapons (the second for
   dual wielding); each weapon has its own animation state, two more
   channels, the node maps of the weapon and of the arms, and the node
   matrices last built for it. */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "unknown_1c62f0.h"

#include <string.h>

#define MAXIMUM_FIRST_PERSON_USERS 4
#define MAXIMUM_FIRST_PERSON_WEAPONS 2
#define MAXIMUM_FIRST_PERSON_NODES 64

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define NUMBEROF(array) (sizeof(array) / sizeof((array)[0]))
#define FLAG(bit) (1 << (bit))
#define TEST_FLAG(flags, bit) (((flags) & FLAG(bit)) != 0)
#define SET_FLAG(flags, bit, value) ((value) ? ((flags) |= FLAG(bit)) : ((flags) &= ~FLAG(bit)))

/* the flags of a first person weapon */
enum
{
	_first_person_weapon_active_bit = 0,
	_first_person_weapon_animated_bit,
	_first_person_weapon_arms_animated_bit
};

/* the flags of a first person user */
enum
{
	_first_person_user_active_bit = 0,
	_first_person_user_animated_bit,
	_first_person_user_adjusted_bit
};

/* the interpolator at +0x64 of the animation state (src/unknown_1d9240.cpp) */
struct s_1d9240
{
	char value;
	char count;
	byte unknown02;
	byte flag;
};

/* the animation state of src/unknown_1cafc0.cpp, as this file sees it */
struct s_animation_state
{
	c_animation_channel channels[3];
	byte unknown60[4];
	s_1d9240 unknown64;
	long graph_tag_index;
	word flags;
	word unknown6e;
	long unknown70;
	long unknown74;
	long unknown78;
	long animation_name;
	real unknown80;
	real_vector3d unknown84;

	s_animation_state();
	bool animation_set(long mode, long weapon_class, long weapon_type, long set, long state_flags, long unknown);
	void reset();
	bool initialize(long graph_tag_index, long model_tag_index, bool flag);
	void channels_clear_partial();
	s_graph_entry *entry_get(long index);
};

/* the weapon's indices and timer (at +0xd8, 0x14 bytes) */
struct s_first_person_indices
{
	short unknown0;
	short unknown2;
	short unknown4;
	short unknown6;
	short unknown8;
	short unknowna;
	byte unknownc[4];
	real unknown10;
};

static inline void first_person_indices_reset(s_first_person_indices *indices)
{
	indices->unknown4 = NONE;
	indices->unknown6 = NONE;
	indices->unknown8 = NONE;
	indices->unknowna = NONE;
	indices->unknown0 = NONE;
	indices->unknown2 = NONE;
}

/* the first person weapon (0x1010 bytes) */
struct s_first_person_weapon
{
	dword flags;
	long weapon_index;
	s_animation_state animation;
	c_animation_channel channel98;
	c_animation_channel channelb8;
	s_first_person_indices indices;
	byte unknownec[4];
	short unknownf0;
	short unknownf2;
	long weapon_model_index;
	long arms_model_index;
	long weapon_node_map[MAXIMUM_FIRST_PERSON_NODES];
	long arms_node_map[MAXIMUM_FIRST_PERSON_NODES];
	byte unknown2fc[4];
	long orientation_count;
	long node_count;
	real_matrix4x3 nodes[MAXIMUM_FIRST_PERSON_NODES];
	long sound_index;
	short sound_animation;
	byte unknown100e[2];
};

struct s_first_person_bits
{
	byte unknown0;
	byte unknown1;
	byte unknown2;
	byte unknown3;
};

/* a local user's first person state (0x20cc bytes) */
struct s_first_person_user
{
	dword flags;
	long unit_index;
	long character_index;
	s_first_person_weapon weapons[MAXIMUM_FIRST_PERSON_WEAPONS];
	s_first_person_bits unknown202c;
	byte unknown2030[0x2060 - 0x2030];
	real_matrix4x3 matrix;
	long unknown2094;
	real_matrix4x3 adjustment;
};

/* the node matrices of one model built for rendering (0xd0c bytes) */
struct s_first_person_model
{
	long render_model_index;
	long object_index;
	long unknown08;
	real_matrix4x3 nodes[MAXIMUM_FIRST_PERSON_NODES];
};

/* an object marker (0x70 bytes): its node matrix, then the marker's own */
struct s_first_person_marker
{
	short node_index;
	short unknown02;
	real_matrix4x3 node_matrix;
	real_matrix4x3 matrix;
	byte unknown6c[4];
};

/* an animation event (the sound events 166d50 and 166d62 receive) */
struct s_first_person_event
{
	short type;
	short unknown02;
	long sound_tag_index;
	byte unknown08[4];
	byte flags;
};

/* the objects, as this file reads them */
struct s_first_person_object
{
	long definition_index;
	byte unknown004[0x13c - 0x4];
	long player_index;
	byte unknown140[0x154 - 0x140];
	long unit_index;
};

struct s_first_person_object_header
{
	byte unknown00[8];
	s_first_person_object *object;
};

/* the weapon's first person block entries (16 bytes) */
struct s_first_person_interface
{
	byte unknown00[4];
	long render_model_index;
	byte unknown08[4];
	long animation_graph_index;
};

struct s_first_person_weapon_definition
{
	byte unknown000[0x12c];
	dword unknown12c_bits0 : 17;
	dword unknown12c_bit17 : 1;
	dword unknown12c_bits18 : 14;
	byte unknown130[0x292 - 0x130];
	short unknown292;
	byte unknown294[0x2a8 - 0x294];
	long interface_count;
	s_first_person_interface *interfaces;
	byte unknown2b0[0x2b8 - 0x2b0];
	byte unknown2b8[4];
};

/* the render model (only the node count at +0x48 is read here) */
struct s_first_person_render_model
{
	byte unknown00[0x48];
	long node_count;
};

/* the globals' player representations (0xbc bytes each) */
struct s_player_representation
{
	byte unknown00[4];
	long arms_render_model_index;
	byte unknown08[4];
	long arms_animation_graph_index;
	byte unknown10[0xbc - 0x10];
};

struct s_first_person_globals_view
{
	byte unknown000[0x138];
	long representation_count;
	s_player_representation *representations;
};

/* the players (0x21c bytes) */
struct s_first_person_player
{
	byte unknown000[0x28];
	short local_user_index;
	byte unknown02a[2];
	long unit_index;
	byte unknown030[0x88 - 0x30];
	char character_type;
	byte unknown089[0x21c - 0x89];
};

/* the users (defined in unknown_16658d.cpp) and the orientation buffers, 0x1000
   bytes per weapon (unknown_165cc3.cpp allocates both) */
struct s_16658d_group;
extern s_16658d_group *g_4e9bc8;
#define first_person_users ((s_first_person_user *)g_4e9bc8)
extern void *g_165cc3_aligned_data;
#define first_person_orientations ((byte *)g_165cc3_aligned_data)

extern real_matrix4x3 *g_4687d0;
extern long g_4b9ed8;
extern real_point3d g_4b9da0;
extern real_vector3d g_4b9dac;
extern real_vector3d g_4b9db8;
real g_4b9db4;

/* callees in other files */
int __fastcall function_142a60(real_matrix4x3 const *a, real_matrix4x3 const *b, real_matrix4x3 *result);
void function_141590(real_matrix4x3 const *in, real_matrix4x3 *out);
void matrix4x3_from_point_and_vectors(real_matrix4x3 *out, real_point3d const *position, real_vector3d const *forward, real_vector3d const *up);
long function_14de70(long local_player_index);
long function_155760(long index);
struct s_object;
s_object *function_badc0(long object_index, dword type_mask);
long function_baf80(long object_index);
long function_cbd50(long unit_index, short weapon_index);
bool function_cd660(long unit_index);
long function_1469f0(real seconds);
long function_189060(long object_index, short value, real scale, real_point3d const *position, real_vector3d const *direction, long tag_index);
void function_1d9240(s_1d9240 *p, char flag, real x);
long function_16658d(long group_index, long key);
real function_1d9430(s_1d9240 const *p);
long unit_get_player_index(long unit_index);
/* lane R's unknown_166244.cpp */
long function_166244(long key);

/* 0x1d9430 is in unknown_1d9240.cpp and 0x14de90 in unknown_14b560.cpp; 0x1cba80 is in
   unknown_1cafc0.cpp (state, weapon_class, weapon_type, set) */
real function_1d9430(s_1d9240 const *p);
s_animation const *function_1cba80(s_animation_state *state, long mode, long weapon_class, long name);
long unit_get_player_index(long unit_index);
void function_1776e0(long user_index, long object_index, bool add); /* unknown_175bd0.cpp */
short function_1d90b0(long render_model_index, long marker_name, long unknown0, long model_index, long const *node_map,
	long node_map_count, real_matrix4x3 const *nodes, long unknown1, s_first_person_marker *markers, long marker_count);

static inline s_first_person_object *first_person_object_get(long object_index)
{
	return ((s_first_person_object_header *)g_4e0300->data)[object_index & 0xffff].object;
}

static inline s_first_person_weapon_definition *first_person_object_definition_get(s_first_person_object *object)
{
	return (s_first_person_weapon_definition *)g_4e3b44[object->definition_index & 0xffff].bytes;
}

static inline s_first_person_player *first_person_player_get(long player_index)
{
	return (s_first_person_player *)(g_4e8c24->data + (player_index & 0xffff) * sizeof(s_first_person_player));
}

// @retail 0x16896f
long first_person_character_from_player(long character)
{
	if (g_4e6948->state == 2)
	{
		if (character == 0)
		{
			character = 2;
		}
		if (character == 1)
		{
			character = 3;
		}
	}
	return character;
}

// @retail 0x16898b
long first_person_character_to_interface(long character)
{
	if (g_4e6948->state == 2)
	{
		if (character == 2)
		{
			character = 0;
		}
		if (character == 3)
		{
			character = 1;
		}
	}
	return character;
}

// @retail 0x168395
void first_person_nodes_remap(real_matrix4x3 const *base, real_matrix4x3 *out, long out_count,
	real_matrix4x3 const *nodes, long const *node_map, long render_model_index)
{
	s_first_person_render_model *model = (s_first_person_render_model *)g_4e3b44[render_model_index & 0xffff].bytes;
	long count = MIN(model->node_count, out_count);
	long i;

	for (i = 0; i < count; i++, out++)
	{
		long node_index = node_map[i];

		if (node_index != NONE)
		{
			if (base)
			{
				function_142a60(base, &nodes[node_index], out);
			}
			else
			{
				*out = nodes[node_index];
			}
		}
	}
}

// @retail 0x165e9f
void first_person_model_build(long render_model_index, long node_count, real_matrix4x3 const *nodes, long object_index, long unknown08,
	real_matrix4x3 const *base, long const *node_map, s_first_person_model *model)
{
	model->object_index = object_index;
	model->render_model_index = render_model_index;
	model->unknown08 = unknown08;
	if (node_map)
	{
		first_person_nodes_remap(base, model->nodes, NUMBEROF(model->nodes), nodes, node_map, render_model_index);
	}
	else
	{
		node_count = MIN((dword)node_count, NUMBEROF(model->nodes));
		if (base)
		{
			long i;

			for (i = 0; i < node_count; i++)
			{
				function_142a60(base, &nodes[i], &model->nodes[i]);
			}
		}
		else
		{
			memcpy(model->nodes, nodes, node_count * sizeof(real_matrix4x3));
		}
	}
}

// @retail 0x1662c1
short first_person_weapon_get_markers_internal(long weapon_index, long marker_name, s_first_person_marker *markers, short marker_count)
{
	short result = 0;
	s_object *weapon = function_badc0(weapon_index, 4);

	if (weapon)
	{
		long user_index = function_166244(weapon_index);

		if (user_index != NONE && !function_155760(user_index))
		{
			long weapon_slot = function_16658d(user_index, weapon_index);

			if (weapon_slot != NONE)
			{
				s_first_person_user *user = &first_person_users[user_index];
				s_first_person_weapon *fp_weapon = &user->weapons[weapon_slot];

				if (marker_name != 0xa0000b6)
				{
					s_first_person_interface *interface = &first_person_object_definition_get((s_first_person_object *)weapon)->interfaces[first_person_character_to_interface(user->character_index)];
					long render_model_index = interface->render_model_index;

					if (TEST_FLAG(fp_weapon->flags, _first_person_weapon_animated_bit) && render_model_index != NONE && interface->animation_graph_index != NONE)
					{
						result = function_1d90b0(render_model_index, marker_name, 0, fp_weapon->weapon_model_index, fp_weapon->weapon_node_map,
							MAXIMUM_FIRST_PERSON_NODES, fp_weapon->nodes, 0, markers, marker_count);
					}
				}
				else
				{
					long render_model_index = ((s_first_person_globals_view *)g_4e034c)->representations[user->character_index].arms_render_model_index;

					if (TEST_FLAG(fp_weapon->flags, _first_person_weapon_arms_animated_bit) && render_model_index != NONE)
					{
						result = function_1d90b0(render_model_index, marker_name, 0, fp_weapon->arms_model_index, fp_weapon->arms_node_map,
							MAXIMUM_FIRST_PERSON_NODES, fp_weapon->nodes, 0, markers, marker_count);
					}
				}

				{
					long i;

					for (i = 0; i < result; i++)
					{
						function_142a60(&user->matrix, &markers[i].matrix, &markers[i].matrix);
					}
				}
			}
		}
	}
	return result;
}

// @retail 0x1662a1
short first_person_weapon_get_markers(long weapon_index, long marker_name, s_first_person_marker *markers, short marker_count)
{
	if (g_4b9ed8 == function_166244(weapon_index))
	{
		return first_person_weapon_get_markers_internal(weapon_index, marker_name, markers, marker_count);
	}
	return 0;
}

// @retail 0x166561
long first_person_weapon_mode_name(long user_index)
{
	s_first_person_user *user = &first_person_users[user_index];
	long result = 0x30000d9;

	if (user->unit_index != NONE && function_cd660(user->unit_index))
	{
		result = 0x400054b;
	}
	return result;
}

// @retail 0x1665c2
long first_person_weapon_state_animation(short state)
{
	long result = NONE;

	switch (state)
	{
	case 0: result = 0x400000c; break;
	case 1: result = 0x6000006; break;
	case 2: result = 0x6000007; break;
	case 3: result = 0x900006a; break;
	case 4: result = 0x900006b; break;
	case 5: result = 0x8000063; break;
	case 6: result = 0x9000062; break;
	case 7: result = 0xc000064; break;
	case 8: result = 0xb000065; break;
	case 13: result = 0xb000067; break;
	case 14: result = 0xb00006d; break;
	case 15: result = 0x8000071; break;
	case 9: result = 0xc0005ae; break;
	case 10: result = 0x150005af; break;
	case 11: result = 0x140005b0; break;
	case 12: result = 0xb0005b1; break;
	case 17: result = 0x500000a; break;
	case 18:
	case 19: result = 0x8000025; break;
	case 20:
	case 21: result = 0x5000024; break;
	case 22:
	case 23: result = 0x1000006e; break;
	case 26: result = 0xd000021; break;
	case 27: result = 0x1000006c; break;
	case 28: result = 0xc000073; break;
	case 29: result = 0x11000074; break;
	case 30: result = 0xc000075; break;
	case 31: result = 0x11000076; break;
	case 32: result = 0xc000077; break;
	case 33: result = 0xe000607; break;
	case 34: result = 0xe000608; break;
	case 35: result = 0xe000609; break;
	case 36: result = 0xe00060a; break;
	case 37: result = 0xa0005bb; break;
	case 38: result = 0xb0005b2; break;
	case 39: result = 0x130005bc; break;
	case 40: result = 0x140005b3; break;
	}
	return result;
}

// @retail 0x166c39
void first_person_weapon_save_orientations(long user_index, long weapon_slot, real blend_time)
{
	s_first_person_user *user = &first_person_users[user_index];
	s_first_person_weapon *weapon = &user->weapons[weapon_slot];

	if (TEST_FLAG(user->flags, _first_person_user_animated_bit))
	{
		byte *orientations = first_person_orientations +(weapon_slot + user_index * MAXIMUM_FIRST_PERSON_WEAPONS) * 0x1000;

		memcpy(orientations + 0x800, orientations, weapon->orientation_count * 0x20);
		if (blend_time >= function_1d9430(&weapon->animation.unknown64))
		{
			function_1d9240(&weapon->animation.unknown64, true, blend_time);
		}
	}
}

// @retail 0x166cb0
void first_person_weapon_reset_blend(long user_index, long weapon_slot)
{
	s_1d9240 *blend = &first_person_users[user_index].weapons[weapon_slot].animation.unknown64;

	blend->count = 0;
	blend->value = 0;
	blend->flag = 0;
}

static inline s_first_person_user *first_person_user_get(long user_index)
{
	return &first_person_users[user_index];
}

// @retail 0x166cd1
void first_person_weapon_sound_event(long user_index, long weapon_slot, s_first_person_event const *event)
{
	s_first_person_user *user = first_person_user_get(user_index);

	if (user && event->type == 1)
	{
		long sound_tag_index = event->sound_tag_index;

		if (sound_tag_index != NONE && (!function_155760(user_index) || !(event->flags & 8)))
		{
			s_first_person_weapon *weapon = &user->weapons[weapon_slot];

			weapon->sound_index = function_189060(weapon->weapon_index, NONE, 1.0f, g_468788, g_4687a8, sound_tag_index);
			weapon->sound_animation = (short)weapon->animation.animation_name;
		}
	}
}

// @retail 0x166d50
void __stdcall first_person_weapon_sound_event_right(long user_index, long unused, s_first_person_event const *event)
{
	first_person_weapon_sound_event(user_index, 0, event);
}

// @retail 0x166d62
void __stdcall first_person_weapon_sound_event_left(long user_index, long unused, s_first_person_event const *event)
{
	first_person_weapon_sound_event(user_index, 1, event);
}

// @retail 0x168311
void first_person_weapon_set_active(long user_index, long weapon_slot, bool active)
{
	s_first_person_weapon *weapon = &first_person_users[user_index].weapons[weapon_slot];

	if (active != TEST_FLAG(weapon->flags, _first_person_weapon_active_bit))
	{
		SET_FLAG(weapon->flags, _first_person_weapon_active_bit, active);
	}
}

// @retail 0x16834a
void first_person_weapon_set_object(long user_index, long weapon_slot, long weapon_index)
{
	s_first_person_weapon *weapon = &first_person_users[user_index].weapons[weapon_slot];

	if (weapon_index != weapon->weapon_index)
	{
		if (weapon->weapon_index != NONE)
		{
			function_1776e0(user_index, weapon->weapon_index, false);
		}
		weapon->weapon_index = weapon_index;
		if (weapon_index != NONE)
		{
			function_1776e0(user_index, weapon_index, true);
		}
	}
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))

// @retail 0x168470
s_animation const *first_person_weapon_animation_get(long weapon_index, long animation_name, long *frame_out)
{
	s_animation const *result = NULL;
	s_first_person_object *weapon = first_person_object_get(weapon_index);
	s_first_person_weapon_definition *definition = first_person_object_definition_get(weapon);

	if (weapon->unit_index != NONE)
	{
		s_first_person_object *unit = first_person_object_get(weapon->unit_index);

		if (unit->player_index != NONE)
		{
			long character = first_person_player_get(unit->player_index)->character_type;

			if (character >= 0 && character < definition->interface_count)
			{
				s_first_person_interface *interface = &definition->interfaces[first_person_character_to_interface(character)];

				if (interface->animation_graph_index != NONE)
				{
					s_animation_state state;

					if (state.initialize(interface->animation_graph_index, NONE, true))
					{
						long mode = 0x7000101;
						long weapon_class = mode;

						if (function_cd660(weapon->unit_index))
						{
							weapon_class = 0x400054b;
						}
						result = function_1cba80(&state, mode, weapon_class, animation_name);
						if (result && (short)function_1dae80(result) != NONE && frame_out)
						{
							*frame_out = *(long *)((byte *)state.entry_get((short)function_1dae80(result)) + 4);
						}
					}
					state.channels_clear_partial();
				}
			}
		}
	}
	return result;
}

// @retail 0x1685a6
short first_person_weapon_animation_ticks(long weapon_index, long animation_name, short type)
{
	short result = 0;
	s_animation const *animation = first_person_weapon_animation_get(weapon_index, animation_name, NULL);

	if (animation)
	{
		long frame_count = *(short const *)((byte const *)animation + 0x14);
		long event_frame = function_1dadb0(animation, 4);
		long frame;

		switch (type)
		{
		case 0:
			frame = frame_count;
			break;
		case 1:
			frame = event_frame != NONE ? MIN(frame_count, event_frame) : frame_count;
			break;
		case 2:
			frame = function_1dae20(animation);
			break;
		default:
			frame = function_1dae20(animation);
			if (frame <= 0)
			{
				frame = frame_count;
			}
			break;
		}

		if (frame == NONE)
		{
			result = NONE;
		}
		else if (frame > 0)
		{
			result = (short)function_1469f0((real)frame * (1.0f / 30.0f));
			result = MAX(result, 1);
		}
	}
	return result;
}

// @retail 0x16640f
bool first_person_weapon_get_marker(long object_index, long marker_name, real_point3d *position, real_vector3d *forward, real_vector3d *up)
{
	s_first_person_marker marker;
	long unit_index = function_baf80(object_index);
	long user_index = unit_get_player_index(unit_index) == NONE ? NONE : first_person_player_get(unit_get_player_index(unit_index))->local_user_index;
	bool result = false;

	if (user_index != NONE)
	{
		s_first_person_weapon *weapon = &first_person_users[user_index].weapons[0];

		if (TEST_FLAG(weapon->flags, _first_person_weapon_active_bit) &&
			first_person_weapon_get_markers_internal(weapon->weapon_index, marker_name, &marker, 1))
		{
			*position = marker.matrix.position;
			*forward = marker.matrix.forward;
			*up = marker.matrix.up;
			result = true;
		}
	}
	return result;
}
static inline void first_person_bits_reset(s_first_person_bits *bits)
{
	bits->unknown1 = 0;
	bits->unknown0 = 0;
	bits->unknown3 = 0;
}

// @retail 0x165ce5
void first_person_weapons_initialize_for_new_map(void)
{
	long user_index;

	for (user_index = 0; user_index < MAXIMUM_FIRST_PERSON_USERS; user_index++)
	{
		s_first_person_user *user = &first_person_users[user_index];
		long weapon_slot;

		memset(user, 0, sizeof(s_first_person_user));
		user->unit_index = NONE;
		user->character_index = NONE;
		user->unknown2094 = NONE;
		user->matrix = *g_4687d0;
		for (weapon_slot = 0; weapon_slot < MAXIMUM_FIRST_PERSON_WEAPONS; weapon_slot++)
		{
			s_first_person_weapon *weapon = &user->weapons[weapon_slot];

			weapon->weapon_index = NONE;
			weapon->sound_index = NONE;
			weapon->sound_animation = NONE;
			weapon->animation.reset();
			weapon->channel98.reset();
			weapon->channelb8.reset();
			first_person_indices_reset(&weapon->indices);
		}
		first_person_bits_reset(&user->unknown202c);
		user->unknown202c.unknown2 = 1;
	}
}

/* not decompiled yet (src/stubs/lane_t.cpp) */
void __stdcall function_167e86(long user_index, long weapon_slot);

/* in its own file (unknown_1682bf.cpp): retail calls it out of line */
void function_1682bf(long unit_index, long user_index, long character_index);

// @retail 0x1682af
void function_1682af(long user_index)
{
	function_1682bf(NONE, user_index, NONE);
}

// @retail 0x165db0
void function_165db0(void)
{
	long user_index;

	for (user_index = 0; user_index < MAXIMUM_FIRST_PERSON_USERS; user_index++)
	{
		function_1682af(user_index);
	}
}

// @retail 0x16651c
void __stdcall function_16651c(long weapon_index)
{
	long user_index;

	for (user_index = 0; user_index < MAXIMUM_FIRST_PERSON_USERS; user_index++)
	{
		s_first_person_user *user = &first_person_users[user_index];
		long weapon_slot;

		for (weapon_slot = 0; weapon_slot < MAXIMUM_FIRST_PERSON_WEAPONS; weapon_slot++)
		{
			if (user->weapons[weapon_slot].weapon_index == weapon_index)
			{
				function_167e86(user_index, weapon_slot);
			}
		}
	}
}

struct s_predicted_resource_block;
bool function_16e5e0(s_predicted_resource_block const *block, short mode);

// @retail 0x16840e
void function_16840e(long user_index, long weapon_slot)
{
	s_first_person_weapon *weapon = &first_person_users[user_index].weapons[weapon_slot];

	if (weapon->weapon_index != NONE)
	{
		s_first_person_weapon_definition *definition = first_person_object_definition_get(first_person_object_get(weapon->weapon_index));

		function_16e5e0((s_predicted_resource_block const *)definition->unknown2b8, 0);
	}
	weapon->unknownf0 = 30;
}

/* the units' weapon slots, as read here */
struct s_first_person_unit_view
{
	byte unknown000[0x212];
	char weapon_slots[MAXIMUM_FIRST_PERSON_WEAPONS];
};

/* not decompiled yet (src/stubs/lane_t.cpp) */
void __stdcall function_166d75(long user_index);

// @retail 0x165dc1
void first_person_weapons_update(void)
{
	long user_index;

	for (user_index = 0; user_index < MAXIMUM_FIRST_PERSON_USERS; user_index++)
	{
		long player_index = function_14de70(user_index);

		if (player_index != NONE)
		{
			s_first_person_user *user = &first_person_users[user_index];
			s_first_person_player *player = first_person_player_get(player_index);
			long character_index = first_person_character_from_player(player->character_type);

			if (user->unit_index != player->unit_index || user->character_index != character_index)
			{
				function_1682bf(player->unit_index, user_index, character_index);
			}
			if (user->unit_index != NONE)
			{
				long weapon_slot;

				for (weapon_slot = 0; weapon_slot < MAXIMUM_FIRST_PERSON_WEAPONS; weapon_slot++)
				{
					s_first_person_unit_view *unit = (s_first_person_unit_view *)first_person_object_get(user->unit_index);

					if (user->weapons[weapon_slot].weapon_index != function_cbd50(user->unit_index, unit->weapon_slots[weapon_slot]))
					{
						function_167e86(user_index, weapon_slot);
					}
				}
				function_166d75(user_index);
			}
		}
	}
}

void first_person_weapon_set_animation(long user_index, long weapon_slot, long animation_name, bool restart);
/* not decompiled yet (src/stubs/lane_t.cpp) */
void __stdcall function_105c20(long weapon_index, long animation_name);

// @retail 0x168896
void first_person_weapon_set_state(long user_index, long weapon_index, long weapon_slot, long state)
{
	long animation_name = first_person_weapon_state_animation(state);

	if (user_index != NONE)
	{
		s_first_person_weapon *weapon = &first_person_users[user_index].weapons[weapon_slot];
		bool restart;

		switch (state)
		{
		case 1:
			weapon->unknownf2 = 4;
			weapon->indices.unknown10 += 0.05f;
			break;
		case 20:
		case 21:
		case 24:
		case 25:
			{
				long slot;

				for (slot = 0; slot < MAXIMUM_FIRST_PERSON_WEAPONS; slot++)
				{
					function_167e86(user_index, slot);
				}
			}
			break;
		}
		restart = weapon->sound_index != NONE && weapon->sound_animation != animation_name;
		if (state >= 10 && state <= 12)
		{
			restart = false;
		}
		if (animation_name != NONE)
		{
			first_person_weapon_set_animation(user_index, weapon_slot, animation_name, restart);
		}
	}
	else if (weapon_index != NONE)
	{
		function_105c20(weapon_index, animation_name);
	}
}

/* not decompiled yet (src/stubs/lane_t.cpp) */
void function_126360(long sound_index);

// @retail 0x166992
void first_person_weapon_set_animation(long user_index, long weapon_slot, long animation_name, bool restart)
{
	s_first_person_user *user = &first_person_users[user_index];
	s_first_person_weapon *weapon = &user->weapons[weapon_slot];

	if (user->unit_index != NONE && weapon->weapon_index != NONE)
	{
		s_first_person_weapon_definition *definition = first_person_object_definition_get(first_person_object_get(weapon->weapon_index));
		long current_animation = weapon->animation.animation_name;
		long mode_name = first_person_weapon_mode_name(user_index);
		real blend_time;
		long flags;

		switch (animation_name)
		{
		case 0x5000024:
			if (current_animation == 0x5000024)
			{
				animation_name = NONE;
			}
			break;
		case 0x6000006:
			if (current_animation == 0x6000006 && TEST_FIELD_BIT(definition->unknown12c_bit17))
			{
				return;
			}
			break;
		case 0x8000063:
		case 0x9000062:
			if (current_animation != 0x400000c && current_animation != 0x600005f)
			{
				animation_name = NONE;
			}
			break;
		case 0x1000006e:
			if (current_animation == 0x1000006e)
			{
				animation_name = NONE;
			}
			break;
		}
		if (animation_name == NONE)
		{
			return;
		}

		blend_time = 0.267f;
		switch (animation_name)
		{
		case 0x500000a:
		case 0xb000065:
		case 0xc000064:
		case 0xc000073:
		case 0xc0005ae:
		case 0xe000607:
		case 0xe000608:
		case 0xe000609:
		case 0xe00060a:
			blend_time = 0.1335f;
			break;
		case 0x5000024:
		case 0x6000006:
		case 0x6000007:
		case 0x900006a:
		case 0x900006b:
		case 0xa000066:
		case 0xb0005b1:
		case 0x1000006e:
			blend_time = 0.0f;
			break;
		}
		switch (animation_name)
		{
		case 0x500000a:
		case 0x5000024:
		case 0xc000073:
		case 0xc000075:
		case 0xc000077:
		case 0xe000607:
		case 0xe000608:
		case 0xe000609:
		case 0xe00060a:
		case 0x1000006e:
			first_person_weapon_reset_blend(user_index, weapon_slot);
			break;
		}
		if (blend_time > 0.0f)
		{
			first_person_weapon_save_orientations(user_index, weapon_slot, blend_time);
		}

		flags = 0x3f;
		if (mode_name == 0x400054b)
		{
			flags = weapon_slot ? 0x203f : 0x103f;
		}
		if (weapon->animation.animation_set(0x7000101, mode_name, 0x7000001, animation_name, 0x82, flags) && restart &&
			weapon->sound_index != NONE && weapon->sound_animation != 0xb00006d)
		{
			function_126360(weapon->sound_index);
			weapon->sound_index = NONE;
			weapon->sound_animation = NONE;
		}
	}
}

bool function_ee8a0(long unit_index, long weapon_slot);

// @retail 0x16674e
void first_person_weapon_animation_finished(long user_index, long weapon_slot)
{
	s_first_person_user *user = &first_person_users[user_index];
	s_first_person_weapon *weapon = &user->weapons[weapon_slot];
	s_first_person_weapon_definition *definition = first_person_object_definition_get(first_person_object_get(weapon->weapon_index));
	long animation_name = weapon->animation.animation_name;
	long next_animation = NONE;

	switch (animation_name)
	{
	case 0x400000c:
	case 0x500000a:
	case 0x5000024:
	case 0x600005f:
	case 0x8000063:
	case 0x8000071:
	case 0x9000062:
	case 0x900006a:
	case 0x900006b:
	case 0xb000065:
	case 0xb0005b1:
	case 0xb0005b2:
	case 0xc000064:
	case 0xc000077:
	case 0xd000021:
	case 0xe000607:
	case 0xe000608:
	case 0xe000609:
	case 0xe00060a:
	case 0x11000074:
	case 0x11000076:
	case 0x140005b3:
		next_animation = 0x400000c;
		break;
	case 0x6000006:
	case 0x6000007:
		if (!TEST_FIELD_BIT(definition->unknown12c_bit17) || weapon->unknownf2 <= 0)
		{
			next_animation = 0x400000c;
		}
		if (definition->unknown292 == 3 && animation_name == 0x6000007)
		{
			next_animation = 0xa000066;
		}
		break;
	case 0x8000025:
		weapon->animation.flags |= 1;
		break;
	case 0xa000066:
		if (!function_ee8a0(user->unit_index, weapon_slot))
		{
			next_animation = 0x8000071;
		}
		break;
	case 0xa0005bb:
		next_animation = 0xb0005b2;
		break;
	case 0xb00006d:
	case 0x1000006c:
	case 0x1000006e:
		next_animation = 0xa000066;
		break;
	case 0xc000073:
		next_animation = 0x11000074;
		break;
	case 0xc000075:
		next_animation = 0x11000076;
		break;
	case 0xc0005ae:
	case 0x140005b0:
	case 0x150005af:
		next_animation = 0xb0005b1;
		break;
	case 0x130005bc:
		next_animation = 0x140005b3;
		break;
	}
	if (next_animation != NONE)
	{
		first_person_weapon_set_animation(user_index, weapon_slot, next_animation, false);
	}
}