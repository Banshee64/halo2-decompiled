// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_10F1E0.CPP: a unit's animation control: resetting the state at the
   object's offset +0x33e, and starting a unit's base animation, its weapon
   animations and its overlays on the animation state at offset +0x12a
   (unknown_1cafc0.cpp). The neighbouring queries are in unknown_10db60.cpp,
   unknown_10dc70.cpp and unknown_10ee20.cpp. */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_1dacb0.h"
#include "unknown_1cafc0.h"
#include <string.h>
#include "object_markers.h"
#include <xmmintrin.h>

/* the unit (a view of the object data) */
struct s_unit_animation_object
{
	long definition_index;
	byte unknown004[0x14 - 4];
	long parent_index;
	byte unknown018[0x10a - 0x18];
	word flag0 : 1;
	word flag1 : 1;
	word animation_frozen : 1;
	word : 13;
	byte unknown10c[0x116 - 0x10c];
	short node_matrices_offset;
	byte unknown118[0x12a - 0x118];
	short animation_state_offset;
	byte unknown12c[0x134 - 0x12c];
	byte unknown134[3];
	byte flags137;
	byte unknown138[4];
	long field_13c;
	byte unknown140[8];
	dword flags148;
	byte unknown14c[0x1f6 - 0x14c];
	char index1f6;
	char index1f7;
	byte unknown1f8[0x33e - 0x1f8];
	short control_offset;
};

struct s_unit_animation_object_header
{
	byte unknown00[8];
	s_unit_animation_object *object;
};

/* a 4 byte state of the control, five of them at +0x80 */
struct s_unit_animation_slot
{
	byte unknown0;
	byte unknown1;
	byte unknown2;
	byte unknown3;
};

/* the state at the unit's offset +0x33e */
struct s_unit_animation_control
{
	union
	{
		word flags;
		struct
		{
			word flag0 : 1;
			word overlay : 1;
			word : 5;
			word flag7 : 1;
			word flag8 : 1;
			word : 7;
		};
	};
	byte unknown02;
	byte unknown03;
	long unknown04[4];
	long unknown14[4];
	byte unknown24[0x28 - 0x24];
	long unknown28;
	byte unknown2c[0x7c - 0x2c];
	short marker_7c;
	short marker_7e;
	s_unit_animation_slot slots[5];
	long weapon_class;
	long weapon_type;
	c_animation_channel channel_9c;
	c_animation_channel channel_bc;
	c_animation_channel channel_dc;
	c_type_709360 animation_fc;
	c_type_709360 animation_100;
	c_type_709360 animation_104;
	c_type_709360 animation_108;
	c_type_709360 animation_10c;
	c_type_709360 animation_110;
	c_type_709360 animation_114;
	c_type_709360 overlays[3];
};

/* the unit's definition (its model at +0x38) and the model's (its animation
   graph at +0x14) */
struct s_unit_animation_definition
{
	byte unknown00[0x38];
	long model_tag_index;
	byte unknown3c[0xbc - 0x3c];
	dword flags;
};

struct s_unit_animation_model
{
	byte unknown00[0x14];
	long graph_tag_index;
};

#define UNIT_ANIMATION_OBJECT(index) (((s_unit_animation_object_header *)g_4e0300->data)[(index) & 0xffff].object)
#define UNIT_ANIMATION_STATE(unit) ((s_animation_state *)((byte *)(unit) + (unit)->animation_state_offset))
#define UNIT_ANIMATION_CONTROL(unit) ((s_unit_animation_control *)((byte *)(unit) + (unit)->control_offset))
#define TAG_BYTES(index) (g_4e3b44[(index) & 0xffff].bytes)
#define GRAPH_GET(index) ((s_graph_tag *)TAG_BYTES(index))

bool g_5107f8;
bool g_4686a4 = true;

bool function_0c7070(long object_index);

// @retail 0x111010
bool function_111010(long unit_index)
{
	s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
	bool result = true;
	if (function_0c7070(unit_index))
		result = false;
	else
	{
		switch (UNIT_ANIMATION_STATE(unit)->unknown7c)
		{
		case 0x400004a: case 0x400076c: case 0x400076d:
		case 0x500000a: case 0x5000024: case 0x50000c3:
		case 0x5000768: case 0x5000769: case 0x500076a: case 0x500076b:
		case 0x600005f: case 0x600008c: case 0x60000cc: case 0x60000cd: case 0x60006ac:
		case 0x700000d: case 0x700002c: case 0x7000543: case 0x700076e:
		case 0x80000c4: case 0x800061e: case 0x800076f:
		case 0x900000e: case 0x9000011: case 0x9000012: case 0x900001f: case 0x9000020:
		case 0xa00000f: case 0xa000010: case 0xa000013: case 0xa00002d: case 0xa00003e:
		case 0xa000040: case 0xa00022d: case 0xa0005b9: case 0xa000767:
		case 0xb00002e: case 0xb00022e: case 0xb0005b2:
		case 0xc000073: case 0xc000075: case 0xc000077: case 0xc0006b3: case 0xc0006cd:
		case 0xd000021: case 0xd00002b: case 0xd00022c:
		case 0xe00002a: case 0xe000038: case 0xe00003b: case 0xe0000c2: case 0xe0000c3: case 0xe00067d:
		case 0xf00003a: case 0x1000006c: case 0x11000074: case 0x11000076: case 0x140005b3:
			result = false;
		}
	}
	return result;
}

real sound_permutation_reference_duration(long definition_index, s_sound_permutation_reference const *reference);

PRIVATE __forceinline short duration_ticks_1145f0(real seconds)
{
	long ticks;
	seconds *= g_510c54->field_2_3;
	__asm
	{
		fld seconds
		fistp ticks
	}
	return (short)ticks;
}

// @retail 0x1145f0
short function_1145f0(long definition_index, s_sound_permutation_reference const *reference, short type)
{
	if (definition_index != NONE)
		return duration_ticks_1145f0(sound_permutation_reference_duration(definition_index, reference));
	if (type == 12)
		return duration_ticks_1145f0(0.5f);
	return duration_ticks_1145f0(1.5f);
}

// @retail 0x113260
bool function_113260(long unit_index)
{
	s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
	s_unit_animation_definition *definition = (s_unit_animation_definition *)TAG_BYTES(unit->definition_index);
	s_animation_state *state = UNIT_ANIMATION_STATE(unit);
	bool result = false;
	if (!(bool)((definition->flags >> 11) & 1) && state->graph_tag_index != NONE &&
		state->channels[0].graph_tag_index != NONE && state->channels[0].animation_id.index != NONE &&
		g_4686a4 && ((unit->flags148 & 0x8000) || (unit->flags137 & 1)) &&
		(unit->index1f6 != NONE || unit->index1f7 != NONE))
		result = true;
	return result;
}

struct s_object;
s_object *function_badc0(long object_index, dword type_mask);
int __fastcall function_142a60(transform4x3f const *a, transform4x3f const *b, transform4x3f *result);

// @retail 0x112dc0
void __stdcall function_112dc0(long unit_index, transform4x3f *matrix)
{
	s_unit_animation_object_header *headers = (s_unit_animation_object_header *)g_4e0300->data;
	s_unit_animation_object *unit = headers[unit_index & 0xffff].object;
	long parent_index = UNIT_ANIMATION_CONTROL(unit)->unknown28;
	if (parent_index != NONE && function_badc0(parent_index, (dword)NONE))
	{
		s_unit_animation_object *parent = headers[parent_index & 0xffff].object;
		transform4x3f *nodes = (transform4x3f *)((byte *)parent + parent->node_matrices_offset);
		function_142a60(nodes, matrix, matrix);
	}
}

/* Partial view of the callback table slot at 0x467a20. */
struct s_unit_matrix_callbacks
{
	void (__stdcall *update)(long, transform4x3f *);
};
s_unit_matrix_callbacks g_467a20 = { function_112dc0 };

void function_1420f0(transform4x3f *out, point3f const *position, vector3f const *forward, vector3f const *up);
void function_141ce0(real yaw, real pitch, real roll, transform4x3f *out);

PRIVATE __forceinline real angle_radians_112e20(real degrees)
{
	return degrees * 0.017453292f;
}

#if 0
/* Deferred retail 0x112e20: this caller changes 0x1420f0's convention and
   breaks its match and 0x270240. Preserve the candidate for a later pass. */
void function_112e20(transform4x3f *matrix, transform4x3f const *orientation, point3f const *target)
{
	static real yaw_limit = angle_radians_112e20(30.0f);
	static real pitch_limit = angle_radians_112e20(15.0f);
	real scale = matrix->scale;
	transform4x3f frame;
	function_1420f0(&frame, &matrix->position, &orientation->forward, &orientation->up);
	vector3f direction;
	if (frame.scale != 0.0f)
	{
		vector3f relative;
		vector3d_from_points3d(&frame.position, target, &relative);
		if (frame.scale != 1.0f)
		{
			real inverse = 1.0f / frame.scale;
			relative.i *= inverse;
			relative.j *= inverse;
			relative.k *= inverse;
		}
		direction.i = dot3f(&frame.forward, &relative);
		direction.j = dot3f(&frame.left, &relative);
		direction.k = dot3f(&frame.up, &relative);
	}
	else
		direction.i = direction.j = direction.k = 0.0f;
	real squared = direction.k * direction.k + direction.j * direction.j + direction.i * direction.i;
	if (squared != 0.0f)
	{
		real inverse;
		_mm_store_ss(&inverse, _mm_rsqrt_ss(_mm_load_ss(&squared)));
		direction.i *= inverse;
		direction.j *= inverse;
		direction.k *= inverse;
	}
	real yaw = (real)atan2(direction.j, direction.i);
	real pitch = (real)atan2(direction.k, sqrt(direction.i * direction.i + direction.j * direction.j));
	if (0.0f - yaw_limit > yaw)
		yaw = 0.0f - yaw_limit;
	else if (yaw > yaw_limit)
		yaw = yaw_limit;
	if (0.0f - pitch_limit > pitch)
		pitch = 0.0f - pitch_limit;
	else if (pitch > pitch_limit)
		pitch = pitch_limit;
	transform4x3f rotation;
	function_141ce0(yaw, pitch, 0.0f, &rotation);
	function_142a60(&frame, &rotation, matrix);
	matrix->scale = scale;
}
#endif

PRIVATE __forceinline void channel_reset_114240(c_animation_channel *channel)
{
	channel->graph_tag_index = NONE;
	channel->animation_id.graph_index = NONE;
	channel->animation_id.index = NONE;
	channel->frame_position = 0.0f;
	channel->unknown10 = 0;
	channel->unknown11 = 0;
	channel->flags = 0;
	channel->rate = 1.0f;
	channel->unknown14 = 0;
	channel->unknown16 = 0;
	channel->unknown08 = NONE;
	channel->unknown0c = NONE;
	channel->unknown0d = NONE;
	channel->unknown0e = NONE;
}

// @retail 0x114240
void function_114240(long unit_index)
{
	s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
	s_unit_animation_control *control = UNIT_ANIMATION_CONTROL(unit);
	channel_reset_114240(&control->channel_9c);
	channel_reset_114240(&control->channel_bc);
}

// @retail 0x110fc0
bool function_110fc0(long unit_index)
{
	s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
	bool result = false;
	switch (UNIT_ANIMATION_CONTROL(unit)->channel_9c.unknown08)
	{
	case 0x9000008:
	case 0x9000009:
	case 0xa000066:
	case 0xf00067f:
		result = true;
	}
	return result;
}

/* the names whose animations mirror each other */
const long g_468684[4][2] =
{
	{ 0xa000014, 0x9000015 },
	{ 0x9000015, 0xa000014 },
	{ 0x9000016, 0xa000017 },
	{ 0xa000017, 0x9000016 },
};

void function_10f040(long object_index);
void function_10e920(long object_index);
void function_10dbc0(long object_index);
long render_model_find_named_entry(long render_model_index, long name);
void function_11b710(long unit_index, long field_7c);
c_type_709360 function_1dd0b0(s_graph_tag *graph, long name);
s_animation *function_1daea0(s_graph_tag *graph, c_type_709360 animation_id);

// @retail 0x111590
bool function_111590(long unit_index, long bit)
{
	s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
	s_animation_state *state = UNIT_ANIMATION_STATE(unit);
	s_unit_animation_control *control = UNIT_ANIMATION_CONTROL(unit);
	bool result = true;
	if (state->channels[0].animation_id.index != NONE)
	{
		s_graph_tag *graph = GRAPH_GET(state->channels[0].graph_tag_index);
		s_animation *animation = function_1daea0(graph, state->channels[0].animation_id);
		if (animation && ((1 << bit) & *(word *)((byte *)animation + 0x18)))
			result = false;
	}
	if (control->channel_9c.animation_id.index != NONE)
	{
		s_graph_tag *graph = GRAPH_GET(control->channel_9c.graph_tag_index);
		s_animation *animation = function_1daea0(graph, control->channel_9c.animation_id);
		if (animation && ((1 << bit) & *(word *)((byte *)animation + 0x18)))
			return false;
	}
	return result;
}
void function_ba350(long object_index, real seconds);

struct s_1d9240;
void function_1d9240(s_1d9240 *slot, char flag, real seconds);
bool function_1d9320(s_1d9240 *slot);

void function_1d90e0(long render_model_index, transform4x3f *nodes, long node_index, transform4x3f const *marker_matrix,
	transform4x3f const *target_matrix, real weight, long node_count);

// @retail 0x113980
void __stdcall function_113980(long object_index, long target_index, bool alternate, real weight,
	s_animation_state *state, s_graph_pair_iterator *iterator, long node_count, transform4x3f *nodes)
{
	if (object_index != NONE && target_index != NONE && weight > 0.0f)
	{
		s_graph_tag *graph = GRAPH_GET(state->graph_tag_index);
		if (graph && (short)graph->node_count != 0)
		{
			s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(object_index);
			s_unit_animation_definition *definition = (s_unit_animation_definition *)TAG_BYTES(unit->definition_index);
			byte *model = TAG_BYTES(definition->model_tag_index);
			dword visited[8];
			memset(visited, 0, ((state->node_count() + 31) >> 5) * sizeof(dword));
			for (;;)
			{
				graph = GRAPH_GET(state->graph_tag_index);
				iterator->mode = state->unknown70;
				iterator->weapon_class = state->unknown74;
				bool found;
				if (alternate)
					found = function_1dcfa0(graph, iterator);
				else
					found = function_1dcf20(graph, iterator);
				if (!found)
					break;
				if (iterator->b != NONE && iterator->b && iterator->a != NONE && iterator->a)
				{
					s_object_marker marker;
					s_object_marker target;
					if (function_b8d30(object_index, iterator->a, &marker, 1, false) &&
						!(visited[marker.node_index >> 5] & (1 << (marker.node_index & 31))) &&
						function_b8d30(target_index, iterator->b, &target, 1, false))
					{
						function_1d90e0(*(long *)(model + 4), nodes, marker.node_index, &marker.node_matrix,
							&target.matrix, weight, node_count);
						visited[marker.node_index >> 5] |= 1 << (marker.node_index & 31);
					}
				}
			}
		}
	}
}
bool function_10ee20(s_animation_state *state);
bool function_10f630(long object_index, long *first, long *second);
long function_10f720(long object_index, bool first);

// @retail 0x113e40
bool function_113e40(long unit_index)
{
	s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
	s_animation_state *state = UNIT_ANIMATION_STATE(unit);
	return state && state->graph_tag_index != NONE && state->channels[0].graph_tag_index != NONE &&
		state->channels[0].animation_id.index != NONE && !(state->flags & 1) && !function_10ee20(state);
}

// @retail 0x114040
bool function_114040(long unit_index, long name)
{
	s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
	s_unit_animation_control *control = UNIT_ANIMATION_CONTROL(unit);
	bool result = false;
	if (control->channel_9c.unknown08 == name)
		return true;
	if (control->channel_bc.unknown08 == name)
		return true;
	long second, first;
	if (function_10f630(unit_index, &first, &second))
		result = second == name;
	return result;
}

// @retail 0x1143d0
bool function_1143d0(long unit_index)
{
	bool result = true;
	if (unit_index != NONE)
	{
		s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
		s_animation_state *state = UNIT_ANIMATION_STATE(unit);
		long name = state->unknown7c;
		if (state->channels[0].graph_tag_index != NONE && state->channels[0].animation_id.index != NONE &&
			!(state->channels[0].unknown11 & 0xa))
		{
			if (name == 0x700005c)
			{
				if (function_10f720(unit_index, true) != 2)
					result = false;
			}
			else if (name == 0x700005d)
			{
				if (function_10f720(unit_index, true) != 1)
					result = false;
			}
		}
	}
	return result;
}

// @retail 0x113910
bool function_113910(long unit_index)
{
	s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
	s_unit_animation_control *control = UNIT_ANIMATION_CONTROL(unit);
	bool result = function_1d9320((s_1d9240 *)&control->slots[0]);
	result |= function_1d9320((s_1d9240 *)&control->slots[1]);
	result |= function_1d9320((s_1d9240 *)&control->slots[2]);
	result |= function_1d9320((s_1d9240 *)&control->slots[3]);
	result |= function_1d9320((s_1d9240 *)&control->slots[4]);
	return result;
}

// @retail 0x113d20
void function_113d20(long unit_index, real seconds)
{
	if (unit_index != NONE)
	{
		s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
		function_1d9240((s_1d9240 *)&UNIT_ANIMATION_CONTROL(unit)->slots[2], false, seconds);
	}
}

// @retail 0x113d60
void function_113d60(long unit_index, real seconds)
{
	if (unit_index != NONE)
	{
		s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
		function_1d9240((s_1d9240 *)&UNIT_ANIMATION_CONTROL(unit)->slots[2], true, seconds);
	}
}

PRIVATE __forceinline long animation_slot_finished(s_unit_animation_control const *control)
{
	if (control->slots[2].unknown1 && !(control->slots[2].unknown3 & 1) && (control->slots[2].unknown3 & 2))
		return 1;
	return 0;
}

// @retail 0x113da0
long function_113da0(long unit_index)
{
	s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
	s_unit_animation_control *control = UNIT_ANIMATION_CONTROL(unit);
	if (!control->slots[2].unknown1)
		return 1;
	return animation_slot_finished(control);
}

// @retail 0x113df0
long function_113df0(long unit_index)
{
	s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
	s_unit_animation_control *control = UNIT_ANIMATION_CONTROL(unit);
	if (control->slots[2].unknown1 && (control->slots[2].unknown3 & 1) && (control->slots[2].unknown3 & 2))
		return 1;
	return 0;
}

static __forceinline long mirrored_name_get(long name)
{
	for (long i = 0; i < 4; i++)
	{
		if (name == g_468684[i][0])
			return g_468684[i][1];
	}
	return NONE;
}

static inline bool name_is_mirrored(long name)
{
	return name == 0xa000014 || name == 0x9000015 || name == 0x9000016 || name == 0xa000017;
}

static inline void quad_clear(long *quad)
{
	quad[0] = 0;
	quad[1] = 0;
	quad[2] = 0;
	quad[3] = 0;
}

static inline bool channel_refresh_if_valid(s_animation_state *state, c_animation_channel *channel, long weapon_class,
	long weapon_type)
{
	bool result = false;

	if (state->graph_tag_index != NONE)
		result = state->channel_refresh(channel, weapon_class, weapon_type);
	return result;
}

static inline void slot_reset(s_unit_animation_slot *slot, byte value)
{
	slot->unknown0 = 0;
	slot->unknown1 = 0;
	slot->unknown2 = value;
	slot->unknown3 = 0;
}

// @retail 0x113870
void function_113870(long unit_index)
{
	s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
	s_unit_animation_control *control = UNIT_ANIMATION_CONTROL(unit);

	slot_reset(&control->slots[0], 1);
	slot_reset(&control->slots[1], 1);
	slot_reset(&control->slots[2], 2);
	slot_reset(&control->slots[3], 1);
	slot_reset(&control->slots[4], 1);
}

// @retail 0x114330
void function_114330(long unit_index)
{
	s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
	s_animation_state *state = UNIT_ANIMATION_STATE(unit);
	s_unit_animation_control *control = UNIT_ANIMATION_CONTROL(unit);
	long weapon_class = state->unknown74;
	long weapon_type = state->unknown78;

	if (state->graph_tag_index != NONE)
	{
		c_type_709360 animation_id = GRAPH_GET(state->graph_tag_index)->overlay_get(state->unknown70, weapon_class,
			weapon_type, 0x10000551, NULL, NULL, NULL);

		if (animation_id.index != NONE &&
			state->channel_start(&control->channel_dc, animation_id, 0x10000551, NONE, NONE, 1, 0x803f))
		{
			control->flag7 = false;
		}
	}
}

// @retail 0x113410
void function_113410(long unit_index)
{
	s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
	s_unit_animation_control *control = UNIT_ANIMATION_CONTROL(unit);
	s_animation_state *state = UNIT_ANIMATION_STATE(unit);

	if (state)
	{
		long weapon_class = state->unknown74;
		long weapon_type = state->unknown78;
		long other_weapon_type = control->weapon_type;
		long first_set = NONE;
		long second_set = NONE;

		switch (state->unknown7c)
		{
		case 0x400004a:
		case 0x5000049:
		case 0xc000058:
			if (unit->parent_index != NONE)
				break;
		case 0x400000c:
		case 0x700005c:
		case 0x700005d:
		case 0x800001e:
		case 0x900000e:
		case 0x900001f:
		case 0x9000020:
		case 0x90006b2:
		case 0xa00000f:
		case 0xb000059:
		case 0xb00005a:
		case 0xc00005b:
			first_set = 0xc000026;
			second_set = 0xe000028;
			break;
		case 0x9000015:
		case 0x9000016:
		case 0xa000014:
		case 0xa000017:
		case 0xa000019:
		case 0xa00001a:
		case 0xa00002d:
		case 0xb000018:
		case 0xb00001b:
		case 0xb00002e:
		case 0xc000033:
		case 0xc000034:
		case 0xd000032:
		case 0xd000035:
			first_set = 0xb000027;
			second_set = 0xd000029;
			break;
		}

		control->animation_fc = animation_state_overlay_or_animation_get(state, weapon_class, first_set, weapon_type);
		control->animation_100 = animation_state_overlay_or_animation_get(state, weapon_class, second_set, weapon_type);
		control->animation_114 = animation_state_overlay_or_animation_get(state, weapon_class, 0x400004b, weapon_type);
		if (weapon_class == 0x400054b)
		{
			control->animation_104 = animation_state_overlay_or_animation_get(state, weapon_class, 0xb00054a, weapon_type);
			control->animation_108 = animation_state_overlay_or_animation_get(state, weapon_class, 0xb00054a,
				other_weapon_type);
			control->animation_10c = animation_state_overlay_or_animation_get(state, weapon_class, 0x400060b, weapon_type);
			control->animation_110 = animation_state_overlay_or_animation_get(state, weapon_class, 0x400060b,
				other_weapon_type);
		}
		else
		{
			control->animation_104 = c_type_709360();
			control->animation_108 = c_type_709360();
			control->animation_10c = animation_state_overlay_or_animation_get(state, weapon_class, 0x400060b, weapon_type);
			control->animation_110 = animation_state_overlay_or_animation_get(state, weapon_class, 0x400060b, weapon_type);
		}
		function_114330(unit_index);

		bool any_overlay = false;
		for (long i = 0; i < 3; i++)
		{
			control->overlays[i] = state->overlay_kind_get(i);
			if (control->overlays[i].index != NONE)
				any_overlay = true;
		}
		if (any_overlay)
			control->overlay = true;
		else
			control->overlay = false;
	}
}

// @retail 0x113740
void function_113740(long unit_index)
{
	s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
	s_unit_animation_control *control = UNIT_ANIMATION_CONTROL(unit);
	s_animation_state *state = UNIT_ANIMATION_STATE(unit);

	state->channel_refresh(&control->channel_9c, 0x7000101, 0x7000101);
	state->channel_refresh(&control->channel_dc, 0x7000101, 0x7000101);
	state->channel_refresh(&control->channel_bc, 0x400054b, control->weapon_type);
}

// @retail 0x1137b0
real function_1137b0(long set, bool other, real minimum, bool changed, long current_set)
{
	real result = 0.267f;

	if (set == 0x9000020 || set == 0x900001f || set == 0x90006b2)
		result = 0.1335f;
	if (changed || other)
		result = 0.267f;
	if (current_set == 0x400000c)
	{
		if (name_is_mirrored(set))
			result = 0.267f;
	}
	else if (name_is_mirrored(current_set) && (name_is_mirrored(set) || set == 0x400000c))
	{
		result = 0.267f;
	}
	return result > minimum ? result : minimum;
}

// @retail 0x10f260
void __stdcall function_10f260(long unit_index)
{
	s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
	s_unit_animation_control *control = UNIT_ANIMATION_CONTROL(unit);

	function_10f040(unit_index);
	control->unknown28 = NONE;
	control->flags = 0;
	memset(control->unknown04, 0, sizeof(control->unknown04));
	memset(control->unknown14, 0, sizeof(control->unknown14));
	control->unknown02 = 0;
	control->unknown03 = 0;

	s_unit_animation_definition *definition = (s_unit_animation_definition *)TAG_BYTES(unit->definition_index);
	control->marker_7c = (short)render_model_find_named_entry(definition->model_tag_index, 0x80001a2);
	control->marker_7e = (short)render_model_find_named_entry(definition->model_tag_index, 0x90001a3);

	s_animation_state *state = UNIT_ANIMATION_STATE(unit);
	if (state)
	{
		state->unknown74 = NONE;
		state->unknown78 = NONE;
	}
	function_10e920(unit_index);
	function_10dbc0(unit_index);
	function_113870(unit_index);
	control->weapon_class = NONE;
	control->weapon_type = NONE;
}

// @retail 0x10f1e0
void function_10f1e0(long unit_index)
{
	s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
	s_animation_state *state = UNIT_ANIMATION_STATE(unit);
	s_unit_animation_definition *definition = (s_unit_animation_definition *)TAG_BYTES(unit->definition_index);
	long graph_tag_index = ((s_unit_animation_model *)TAG_BYTES(definition->model_tag_index))->graph_tag_index;

	UNIT_ANIMATION_CONTROL(unit)->unknown03 = 0;
	if (state->graph_tag_index != graph_tag_index)
	{
		state->initialize(graph_tag_index, definition->model_tag_index, true);
		state->unknown74 = NONE;
		state->unknown78 = NONE;
		function_10f260(unit_index);
	}
}

// @retail 0x10f430
bool __stdcall function_10f430(long unit_index, long mode, long weapon_class, long weapon_type, long set, real blend,
	bool force, long flags)
{
	s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
	s_animation_state *state = UNIT_ANIMATION_STATE(unit);
	s_unit_animation_control *control = UNIT_ANIMATION_CONTROL(unit);
	long current_set = state->unknown7c;
	long old_mode = state->unknown70;
	bool result;

	if (current_set != NONE)
	{
		if (current_set == set || set == 0x7000101)
			flags |= 0x20;
		else if (current_set == mirrored_name_get(set))
			flags |= 0x40;
	}
	if (unit->field_13c != NONE && !g_5107f8)
		flags |= 0x200;
	if (mode == 0x7000101 && set == 0x7000101)
		result = true;
	else
		result = state->animation_set(mode, weapon_class, weapon_type, set, flags, 0x3f);
	if (result)
	{
		bool mode_changed = old_mode != mode && mode != 0x7000101 || force;
		bool set_changed = current_set != set && set != 0x7000101 || force;

		if (mode_changed)
		{
			function_113740(unit_index);
			if (state->overlay_exists())
				control->overlay = true;
			else
				control->overlay = false;
		}
		if (mode_changed || set_changed)
		{
			real seconds = function_1137b0(set, false, blend, mode_changed, current_set);

			if (seconds > 0.0f && !(bool)(((dword)(short)state->flags >> 4) & 1))
				function_ba350(unit_index, seconds);
			function_113410(unit_index);
		}
	}
	return result;
}

// @retail 0x1140b0
bool function_1140b0(long unit_index, long name)
{
	s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
	s_unit_animation_control *control = UNIT_ANIMATION_CONTROL(unit);
	bool result = false;
	if (control->channel_9c.unknown08 == name)
	{
		channel_reset_114240(&control->channel_9c);
		function_ba350(unit_index, 0.267f);
		return true;
	}
	if (control->channel_bc.unknown08 == name)
	{
		channel_reset_114240(&control->channel_bc);
		function_ba350(unit_index, 0.267f);
		return true;
	}
	long first, second;
	if (function_10f630(unit_index, &first, &second) && second == name)
	{
		function_10f430(unit_index, 0x7000101, 0x7000101, 0x7000101, 0x7000001, 0.0f, false, 0);
		return true;
	}
	return result;
}

// @retail 0x113e90
bool __stdcall function_113e90(long unit_index, long name, real blend, c_animation_channel **output, long mode)
{
	s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
	s_unit_animation_control *control = UNIT_ANIMATION_CONTROL(unit);
	s_animation_state *state = UNIT_ANIMATION_STATE(unit);
	c_animation_channel *channel = mode == 3 ? &control->channel_bc : &control->channel_9c;
	long weapon_class = 0x7000101;
	long weapon_type = 0x7000101;
	if (mode == 1 || mode == 3)
	{
		weapon_class = 0x400054b;
		weapon_type = control->weapon_type;
	}
	else if (mode == 2 && state->unknown74 == 0x400054b)
		weapon_class = control->weapon_class;
	bool can_blend = true;
	if (channel->graph_tag_index != NONE && channel->animation_id.index != NONE)
	{
		s_animation *animation = function_1daea0(GRAPH_GET(channel->graph_tag_index), channel->animation_id);
		can_blend = !TEST_FIELD_BIT(animation->flag1);
	}
	if (state->overlay_play(channel, 0x803f, name, weapon_class, weapon_type))
	{
		if (blend > 0.0f && can_blend && channel->is_unflagged0())
			function_ba350(unit_index, blend);
		if (output)
			*output = channel;
		return true;
	}
	long first, second;
	if ((function_10f630(unit_index, &first, &second) && second == name) ||
		function_10f430(unit_index, 0x7000101, 0x7000101, 0x7000101, name, blend, false, 0))
	{
		if (output)
			*output = &UNIT_ANIMATION_STATE(unit)->channels[0];
		return true;
	}
	return false;
}

// @retail 0x10f5c0
bool function_10f5c0(long unit_index, real blend, long flags, long mode, long set)
{
	return function_10f430(unit_index, mode, 0x7000101, 0x7000101, set, blend, false, flags);
}

// @retail 0x10fd40
bool function_10fd40(long unit_index, long weapon_type, long weapon_class, bool flag)
{
	s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
	s_unit_animation_control *control = UNIT_ANIMATION_CONTROL(unit);
	s_animation_state *state = UNIT_ANIMATION_STATE(unit);
	long old_weapon_class = state->unknown74;
	long old_weapon_type = state->unknown78;
	bool take_other = !flag && weapon_type != 0x7000001;
	bool drop_other = !flag && weapon_type == 0x7000001;

	if (TEST_FIELD_BIT(unit->animation_frozen))
		return true;

	long type = weapon_type;
	if (take_other)
	{
		weapon_class = 0x400054b;
		type = 0x7000101;
	}
	else if (drop_other)
	{
		weapon_class = control->weapon_class;
		type = 0x7000101;
	}

	bool result = state->animation_set(0x7000101, weapon_class, type, 0x7000101, 0x26, 0x3f);
	if (result)
	{
		bool class_changed = old_weapon_class != weapon_class && weapon_class != 0x7000101;
		bool type_changed = old_weapon_type != type && type != 0x7000101;

		if (class_changed || type_changed)
		{
			real seconds = function_1137b0(state->unknown7c, true, 0.0f, false, state->unknown7c);

			channel_refresh_if_valid(state, &control->channel_9c, 0x7000101, 0x7000101);
			channel_refresh_if_valid(state, &control->channel_dc, 0x7000101, 0x7000101);
			if (seconds > 0.0f && !(bool)(((dword)(short)state->flags >> 4) & 1))
				function_ba350(unit_index, seconds);
			if (take_other)
			{
				control->weapon_class = old_weapon_class;
				control->weapon_type = weapon_type;
				control->channel_bc.clear();
				result = channel_refresh_if_valid(state, &control->channel_bc, 0x400054b, control->weapon_type);
			}
			else if (drop_other)
			{
				control->channel_bc.clear();
				control->weapon_type = NONE;
				control->weapon_class = NONE;
			}
			function_113410(unit_index);
		}
	}
	return result;
}

// @retail 0x1101e0
bool function_1101e0(long animation_graph_index, long unit_index, long animation_name, bool flag, bool global_flag)
{
	bool result = false;

	if (UNIT_ANIMATION_OBJECT(unit_index)->animation_state_offset != NONE)
	{
		s_graph_tag *graph = GRAPH_GET(animation_graph_index);

		if (graph)
		{
			if (function_1dd0b0(graph, animation_name).index == NONE)
				return false;

			s_unit_animation_object *unit = UNIT_ANIMATION_OBJECT(unit_index);
			s_unit_animation_control *control = UNIT_ANIMATION_CONTROL(unit);
			s_animation_state *state = UNIT_ANIMATION_STATE(unit);
			s_unit_animation_definition *definition = (s_unit_animation_definition *)TAG_BYTES(unit->definition_index);

			function_11b710(unit_index, 0x7000101);
			if (state->graph_tag_index != animation_graph_index &&
				!state->initialize(animation_graph_index, definition->model_tag_index, true))
			{
				return false;
			}

			c_type_709360 animation_id = function_1dd0b0(GRAPH_GET(state->graph_tag_index), animation_name);
			function_10f260(unit_index);

			word channel_flags = flag ? 0x4003 : 0x4001;
			if (function_1daea0(GRAPH_GET(state->graph_tag_index), animation_id)->type)
			{
				control->channel_9c.clear();
				result = state->channel_play(&control->channel_9c, animation_id, channel_flags);
			}
			else
			{
				result = state->play(animation_id, channel_flags);
				if (result)
				{
					if (!global_flag)
						control->flag8 = true;
					else
						control->flag8 = false;
				}
			}
			function_114330(unit_index);
		}
	}
	return result;
}
