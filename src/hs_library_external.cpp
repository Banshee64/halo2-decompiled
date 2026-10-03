// @flags /O2 /Ob1 /arch:SSE /Gr
/* HS_LIBRARY_EXTERNAL.CPP: the evaluators of the script functions that call
   into the game, each followed by its definition (an entry of the function
   table g_4744e0, named by its .rdata address). The comment above each
   evaluator gives its index in the function table and its signature. */

#include "cseries.h"
#include "globals.h"
#include "game_state.h"
#include "hs_library_external.h"
#include "kill_volumes.h"
#include "recorded_animations.h"
#include "timed_effect.h"
#include "unknown_1eb550.h"
#include "ai_script.h"
#include "unit_requests.h"
#include "unknown_11a4d0.h"
#include "unknown_134d20.h"
#include "unknown_0bbf40.h"
#include "unknown_10aca0.h"
#include "command_scripts.h"
#include "unknown_276f80.h"
#include "unknown_1dee50.h"
#include "unknown_107590.h"
#include "unknown_16d180.h"
#include <string.h>
#include <math.h>

#define FLAG(bit) (1 << (bit))
#define SET_FLAG(flags, bit, value) ((value) ? ((flags) |= FLAG(bit)) : ((flags) &= ~FLAG(bit)))
#define PIN(x, lo, hi) ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))
#define NUMBEROF(array) (sizeof(array) / sizeof((array)[0]))
#define DEGREES_TO_RADIANS (3.14159265358979323846f / 180.0f)

/* the objects (a local view; 0bad50 and 0bb760 keep their own) */
struct s_object
{
	byte unknown00[4];
	dword flags;
	byte unknown08[0xc];
	long parent_index;
	byte unknown18;
	byte flags19;
	byte unknown1a[0x30 - 0x1a];
	real_point3d center;
	byte unknown3c[0xc0 - 0x3c];
	word flags_c0;
	byte unknownc2[0xe8 - 0xc2];
	real maximum_vitality;
	real body_vitality;
	real shield_vitality;
	byte unknownf4[0x104 - 0xf4];
	short value_104;
	byte unknown106[4];
	union
	{
		word flags_10a;
		struct
		{
			word : 2;
			word flag_10a_2 : 1;
			word : 13;
		};
	};
};

struct s_object_header
{
	byte unknown00[8];
	s_object *object;
};

/* the object accessor each file writes for its own view (no retail function) */
inline s_object *object_get(long object_index)
{
	return ((s_object_header *)g_4e0300->data)[object_index & 0xffff].object;
}

s_object *function_badc0(long object_index, dword type_mask);

/* the units and devices (views of the object data beyond s_object) */
struct s_unit
{
	byte unknown000[0x12c];
	long index_12c;
	byte unknown130[4];
	union
	{
		dword unit_flags;
		struct
		{
			dword : 12;
			dword unit_flag12 : 1;
			dword : 14;
			dword unit_flag27 : 1;
			dword : 4;
		};
	};
	short value_138;
	byte unknown13a[0x1fc - 0x13a];
	short index_1fc;
	byte unknown1fe[0x23e - 0x1fe];
	char counts_23e[2];
	byte unknown240[0x248 - 0x240];
	long index_248;
	long index_24c;
	byte unknown250[0x33e - 0x250];
	short offset_33e;
};

struct s_device
{
	byte unknown000[0x134];
	real position;
	byte unknown138[0x140 - 0x138];
	real power;
	byte unknown144[0x1cc - 0x144];
	dword device_flags;
};

/* the device groups (g_4e0328, globals.h), 12 bytes each */
struct s_device_group
{
	byte unknown00[4];
	real value;
	byte unknown08[4];
};

/* the object lists (12 bytes each) */
struct s_object_list_datum
{
	byte unknown00[6];
	short count;
	long first_reference_index;
};

s_data_array *g_4f55d8;

/* the object globals (0bb760 keeps its own view) */
struct s_object_list;
extern s_object_list *g_4de2f4;

struct s_object_globals_view
{
	byte unknown00;
	bool garbage_collect_now;
	bool garbage_collect_unsafe;
	byte unknown03[0x1c - 3];
	real_point3d point_1c;
	byte unknown28[0x70 - 0x28];
	real values_70[4];
	bool flag80;
	bool flag81;
	bool flag82;
};

/* a byte array of 0x4201 bytes (183c60) */
extern byte *g_4ed280;

/* a state at g_510c6c (1552e0) */
struct s_unknown_78;
extern s_unknown_78 *g_510c6c;

struct s_510c6c_view
{
	byte unknown00;
	bool flag1;
	short value2;
	byte unknown04[0x14 - 4];
	long value14;
	byte unknown18[0x3c - 0x18];
	long unit_index;
};

/* a state at g_510c50 (13bf00) */
struct s_unknown_13bf00;
extern s_unknown_13bf00 *g_510c50;

struct s_510c50_view
{
	real value0;
	bool flag4;
	byte unknown05;
	bool flag6;
	bool flag7;
	byte unknown08[0x22 - 8];
	bool flag22;
};

/* the timed effect globals g_5093e0 (timed_effect.h) */
extern dword g_4b5690;
extern byte g_4b569d;

struct s_timed_effect_view
{
	byte unknown000[0xac];
	bool flagac;
	bool flagad;
	byte unknownae[0x170 - 0xae];
	bool flag170;
	byte unknown171[3];
	real value174;
	real start178;
	real end17c;
	real values180[4];
	real value190;
	real value194;
	bool flag198;
	byte unknown199[3];
	real value19c;
	real value1a0;
	real start1a4;
	real end1a8;
	bool flag1ac;
	byte unknown1ad[0x1b4 - 0x1ad];
	bool flag1b4;
	byte unknown1b5[3];
	real_argb_color color1b8;
	real_argb_color color1c8;
	byte unknown1d8[0x3f8 - 0x1d8];
	real value3f8;
};

void function_01fc30(real a, real b, real c, real d, real e, real f, real g);
void function_0204b0(long tag_index);

byte g_4b72b0;
byte g_4b72b1;

/* the input globals (g_4e61b8, input_xbox.cpp): the flags at +0..+2 */
struct s_input_globals;
extern s_input_globals g_4e61b8;

struct s_input_globals_view
{
	bool initialized;
	bool suppressed;
	bool feedback_suppressed;
};

/* the globals of 0x5093e4 */
struct s_5093e4
{
	bool flag0;
	bool flag1;
	byte unknown02[2];
	real value4;
	real value8;
	real valuec;
	real value10;
	real value14;
	bool flag18;
	bool flag19;
	bool flag1a;
	byte unknown1b;
	real value1c;
	real value20;
};

s_5093e4 *g_5093e4;

/* an ai data array (ai.cpp) of 0x28 byte elements */
struct s_51ecb4_datum
{
	byte unknown00[0xe];
	bool flag_e;
	byte unknown0f[0x28 - 0xf];
};

extern s_data_array *g_51ecb4;

/* the script threads (0x418 bytes each, g_4f9384 of unknown_209ae0.cpp) */
struct s_hs_thread_view
{
	byte unknown000[8];
	long sleep_until;
	byte unknown00c[0x418 - 0xc];
};

extern s_data_array *g_4f9384;

enum
{
	k_hs_sleep_command_script = -3
};

/* retail function not identified */
inline void hs_thread_set_sleep(long thread_index, long sleep_until)
{
	((s_hs_thread_view *)g_4f9384->data)[thread_index & 0xffff].sleep_until = sleep_until;
}

/* returns a real from a script function; retail function not identified */
inline void hs_return_real(long thread_index, real value)
{
	function_209ae0(thread_index, *(long *)&value);
}

inline long game_seconds_to_ticks_round(real seconds)
{
	real ticks_real = (real)g_510c54->ticks_per_second * seconds;
	long ticks;
	__asm
	{
		fld ticks_real
		fistp ticks
	}
	return ticks;
}
byte g_5107ee;
bool g_4f55dc[16];

/* the hud state (24c7c1) */
struct s_hud_state;
extern s_hud_state *g_5023f4;

struct s_hud_state_view
{
	byte unknown0000[0x1380];
	long time1380;
	bool flag1384;
	byte unknown1385[0x1398 - 0x1385];
	long time1398;
	short ticks139c;
	short ticks139e;
	short value13a0;
	short value13a2;
	short corner13a4;
	bool flag13a6;
	bool flag13a7;
};

extern byte g_4ea936;

/* the clumps (26b230), 0x888 bytes each */
struct s_clump_view
{
	byte unknown000[0x504];
	short state;
	byte unknown506[6];
	bool active;
	byte unknown50d[0x888 - 0x50d];
};

/* the globals of 0x5107e8 */
struct s_5107e8
{
	bool flag0;
	byte unknown01[3];
	long time4;
	bool flag8;
};

s_5107e8 *g_5107e8;

long g_4b9970[12];
byte g_5093fc;
byte g_547f6f;
byte g_547f70;
byte g_547f74;
byte g_547f75;
long *g_502248;
byte g_509415;
long g_50240c;

long function_29f480(void);

/* the core save name (the last byte stays the terminator) and the flag
   that asks for it */
byte g_547f2d;
char g_547f2e[0x40];

/* g_4e8c20 (globals.h): two flags before its entries */
struct s_4e8c20_view
{
	byte unknown00[5];
	bool flag5;
	bool flag6;
	bool flag7;
};

real_point3d g_4e8c28;

/* the elements (0x18 bytes) of g_4ed28c (globals.h), found by
   function_18d1c0 */
struct s_4ed28c_element
{
	byte unknown00[4];
	word flags4;
	byte unknown06[2];
	real scale8;
	byte unknown0c[0x18 - 0xc];
};

long __stdcall function_18d1c0(long value);

/* a state at g_51ebf4 (225a40) */
struct s_unknown_225a40;
extern s_unknown_225a40 *g_51ebf4;

struct s_51ebf4_view
{
	byte unknown00[0x14];
	bool flag14;
	byte unknown15[0x20 - 0x15];
	long id20;
	real value24;
};

struct s_510c4c
{
	byte unknown000[0x1d2];
	bool flag1d2;
};

s_510c4c *g_510c4c;

/* the view globals (24c7c1) */
struct s_view_globals;
extern s_view_globals *g_510c98;

struct s_510c98_view
{
	byte unknown00;
	bool flag1;
	bool flag2;
};

/* a state at g_51e9c0 (1e6a40) */
struct s_unknown_1e6a40;
extern s_unknown_1e6a40 *g_51e9c0;

struct s_51e9c0_view
{
	byte unknown000[0x6c0];
	long value6c0;
	bool flag6c4;
	bool flag6c5;
};

/* the vehicles (a view of the object data beyond s_object) */
struct s_vehicle
{
	byte unknown000[0x3ac];
	long value_3ac;
};

void __stdcall function_221980(char const *name, long value_bits, real time);
void function_221a20(char const *name, bool set);
void function_b7360(long object_index);
void function_24c831(short index);
void function_24c878(short index);
void function_24c8e2(short a, short b);
void function_24c93f(bool flag);

/* the object of an object name (0bb760) */
long function_bb760(short name_index);

/* the scenario's object names (g_4e0350, globals.h), 0x24 bytes each */
struct s_scenario_object_name
{
	char name[0x20];
	short type;
	short index;
};

struct s_scenario_object_names_view
{
	byte unknown00[0x48];
	long object_name_count;
	s_scenario_object_name *object_names;
};

typedef void (__stdcall *hs_object_name_callback)(short name_index);

// @retail 0x29fd40
void __stdcall function_29fd40(short name_index)
{
	if (name_index != NONE)
		function_bb670(name_index, false);
}

// @retail 0x29fd60
void __stdcall function_29fd60(short name_index)
{
	if (name_index != NONE)
		function_bb670(name_index, true);
}

// @retail 0x29fd80
void __stdcall function_29fd80(short name_index)
{
	if (name_index != NONE)
	{
		long object_index = function_bb760(name_index);
		if (object_index != NONE && !function_beb30(object_index))
			function_b8540(object_index);
		function_bb670(name_index, false);
	}
}

// @retail 0x29fdd0
void __stdcall function_29fdd0(short name_index)
{
	if (name_index != NONE)
	{
		long object_index = function_bb760(name_index);
		if (object_index != NONE && !function_beb30(object_index))
			function_b8540(object_index);
	}
}

real_point3d *function_b9dd0(long object_index, real_point3d *result);

/* the distance between two points (real_math.cpp keeps an out of line copy) */
inline real distance3d_inline(real_point3d const *a, real_point3d const *b)
{
	real_vector3d v;
	v.i = b->x - a->x;
	v.j = b->y - a->y;
	v.k = b->z - a->z;
	return (real)sqrt(v.j * v.j + (v.i * v.i + v.k * v.k));
}

bool function_11c470(long trigger_volume_index, real_point3d const *point);

/* whether every (or any) object of an object list is inside a trigger
   volume */
// @retail 0x29f6c0
bool function_29f6c0(long list_index, short trigger_volume_index, bool all)
{
	long reference_index;
	long object_index = object_list_get_first(list_index, &reference_index);
	while (object_index != NONE)
	{
		if (function_11c470(trigger_volume_index, &object_get(object_index)->center))
		{
			if (!all)
				return true;
		}
		else if (all)
		{
			return false;
		}
		object_index = object_list_get_next(&reference_index);
	}
	return all;
}

/* the players (g_4e8c24), 0x21c bytes each */
struct s_player_29f5b0
{
	byte unknown000[0x2c];
	long unit_index;
	byte unknown030[0x21c - 0x30];
};

/* moves the unit of every player outside a trigger volume to a cutscene
   flag */
// @retail 0x29f5b0
void function_29f5b0(short trigger_volume_index, short cutscene_flag_index)
{
	s_data_array *players = g_4e8c24;
	long player_index = data_datum_index(players, data_next_absolute_index(players, 0));
	while (player_index != NONE)
	{
		s_player_29f5b0 *player = &((s_player_29f5b0 *)g_4e8c24->data)[player_index & 0xffff];
		if (player->unit_index != NONE &&
			!function_11c470(trigger_volume_index, &object_get(player->unit_index)->center))
		{
			function_29ffb0(player->unit_index, cutscene_flag_index, true, true);
		}
		players = g_4e8c24;
		player_index = data_datum_index(players, data_find_index(players, player_index == NONE ? 0 : (player_index & 0xffff) + 1));
	}
}

/* the distance from an object to the nearest object of an object list, -1
   when there is none */
// @retail 0x29fad0
real objects_distance_to_object(long list_index, long object_index)
{
	real minimum_distance = 3.4028234663852886e+38f;
	if (object_index != NONE)
	{
		real_point3d origin;
		function_b9dd0(object_index, &origin);
		long reference_index;
		long list_object_index = object_list_get_first(list_index, &reference_index);
		while (list_object_index != NONE)
		{
			real_point3d point;
			function_b9dd0(list_object_index, &point);
			real distance = distance3d_inline(&origin, &point);
			if (minimum_distance > distance)
				minimum_distance = distance;
			list_object_index = object_list_get_next(&reference_index);
		}
	}
	if (minimum_distance == 3.4028234663852886e+38f)
		return -1.0f;
	return minimum_distance;
}
/* the scenario's cutscene flags (g_4e0350, globals.h), 0x38 bytes each */
struct s_scenario_cutscene_flag_view
{
	byte unknown00[0x24];
	real_point3d position;
	real yaw;
	real pitch;
};

struct s_scenario_cutscene_flags_view
{
	byte unknown000[0x1e0];
	long cutscene_flag_count;
	s_scenario_cutscene_flag_view *cutscene_flags;
};

/* the distance from a cutscene flag to the nearest object of an object
   list, -1 when there is none */
// @retail 0x29fc00
real objects_distance_to_flag(long list_index, short cutscene_flag_index)
{
	real minimum_distance = 3.4028234663852886e+38f;
	if (cutscene_flag_index >= 0 && cutscene_flag_index < ((s_scenario_cutscene_flags_view *)g_4e0350)->cutscene_flag_count)
	{
		real_point3d *position = &((s_scenario_cutscene_flags_view *)g_4e0350)->cutscene_flags[cutscene_flag_index].position;
		long reference_index;
		long list_object_index = object_list_get_first(list_index, &reference_index);
		while (list_object_index != NONE)
		{
			real_point3d point;
			function_b9dd0(list_object_index, &point);
			real distance = distance3d_inline(position, &point);
			if (minimum_distance > distance)
				minimum_distance = distance;
			list_object_index = object_list_get_next(&reference_index);
		}
	}
	if (minimum_distance == 3.4028234663852886e+38f)
		return -1.0f;
	return minimum_distance;
}
/* calls callback with each object name that contains string */
// @retail 0x29ff60
void hs_object_iterate_names_containing(char const *string, hs_object_name_callback callback)
{
	s_scenario_object_names_view *scenario = (s_scenario_object_names_view *)g_4e0350;
	for (short name_index = 0; name_index < scenario->object_name_count; name_index++)
	{
		s_scenario_object_name *object_name = &scenario->object_names[name_index];
		if (strstr(object_name->name, string))
			callback(name_index);
	}
}

/* the references of the object lists (1dee80) */
extern s_data_array *g_4f55d4;


/* the object at index in an object list, NONE past its end */
// @retail 0x2a0280
long object_list_get_element(long list_index, short index)
{
	long reference_index;
	long object_index = object_list_get_first(list_index, &reference_index);
	while (index > 0 && object_index != NONE)
	{
		object_index = object_list_get_next(&reference_index);
		index--;
	}
	return object_index;
}

/* sets the timer at +0x104 of an object, in seconds */
// @retail 0x2a0310
void function_2a0310(long object_index, real seconds)
{
	if (object_index != NONE)
	{
		long ticks = game_seconds_to_ticks_round(seconds);
		s_object *object = object_get(object_index);
		object->value_104 = (short)PIN(ticks, 0, 0x7ffe);
	}
}

/* the model of an object's definition (a local view) */
struct s_object_definition_model_view
{
	byte unknown00[0x38];
	long model_index;
};

struct s_object_definition_header_view
{
	long definition_index;
};


/* sets the state of a region (by name) of an object's model */
// @retail 0x2a0380
void function_2a0380(long object_index, long region_name, short state)
{
	if (object_index != NONE)
	{
		s_object_definition_header_view *object = (s_object_definition_header_view *)object_get(object_index);
		long model_index = ((s_object_definition_model_view *)g_4e3b44[object->definition_index & 0xffff].bytes)->model_index;
		if (model_index != NONE)
			function_ba6f0(object_index, model_find_region_by_name(model_index, (string_id)region_name), state, true);
	}
}

/* calls function_bbec0 on each object of an object list */
// @retail 0x2a03d0
void function_2a03d0(long list_index, bool flag)
{
	long reference_index;
	long object_index = object_list_get_first(list_index, &reference_index);
	while (object_index != NONE)
	{
		function_bbec0(object_index, flag ? true : false);
		object_index = object_list_get_next(&reference_index);
	}
}

long function_1765e0(real_point3d const *point, real_vector3d const *direction, real_vector3d const *normal, long tag_index, long mode, long deterministic);

/* creates an effect at a cutscene flag, facing the flag's direction */
// @retail 0x2a05f0
void function_2a05f0(long effect_index, short cutscene_flag_index)
{
	s_scenario_cutscene_flag_view *flag = &((s_scenario_cutscene_flags_view *)g_4e0350)->cutscene_flags[cutscene_flag_index];
	real_point3d *position = &flag->position;
	real_vector3d forward;
	forward.i = cosf(flag->yaw) * cosf(flag->pitch);
	forward.j = sinf(flag->yaw) * cosf(flag->pitch);
	forward.k = sinf(flag->pitch);
	function_1765e0(position, &forward, g_4687b0, effect_index, 1, 1);
}
/* a view of a player slot (g_54e8e0, globals.h) */
struct s_player_profile_view
{
	__int64 unknown000;
	byte unknown008[0x5c - 8];
	long value5c;
	byte unknown060[0x1e0 - 0x60];
};

struct s_player_slot_view
{
	byte flags;
	byte unknown001[0x18 - 1];
	s_player_profile_view profile;
	long value1f8;
};

static inline s_player_slot_view *player_slot_get(long index)
{
	return index != NONE ? (s_player_slot_view *)&g_54e8e0[index] : NULL;
}

// @retail 0x2a0960
long function_2a0960(short index)
{
	long result = 1;
	s_player_slot_view *slot = player_slot_get(index);
	if (slot && (slot->flags & FLAG(4)))
	{
		s_player_profile_view profile = slot->profile;
		if (slot->value1f8 != NONE)
			result = profile.value5c;
	}
	return result;
}

/* the script syntax nodes (209ae0), 20 bytes each */
extern s_data_array *g_4f9394;

struct s_hs_syntax_node_view
{
	byte unknown00[4];
	short type;
	byte unknown06[0xc - 6];
	long source_offset;
	long value;
};

/* the scenario's script string data (g_4e0350, globals.h) */
struct s_4e0350_view
{
	byte unknown000[0x1b4];
	long string_data;
};

/* points the string constants of the syntax nodes into the string data */
// @retail 0x2a09c0
void function_2a09c0(void)
{
	s_data_array *array = g_4f9394;
	long string_data = ((s_4e0350_view *)g_4e0350)->string_data;
	long datum = data_datum_index(array, data_next_absolute_index(array, 0));
	while (datum != NONE)
	{
		s_hs_syntax_node_view *node = (s_hs_syntax_node_view *)array->data + (datum & 0xffff);
		if (node->type == _hs_type_string)
			node->value = node->source_offset + string_data;
		datum = data_datum_index(array, data_find_index(array, datum == NONE ? 0 : (datum & 0xffff) + 1));
	}
}

/* a fade: its state, the current and target values, the state it goes to
   and its rate; each change is recorded at g_50942c */
struct s_fade_record
{
	long state;
	real current;
	real target;
	long next_state;
	real rate;
	dword marker;
};

long g_509420;
long g_509424;
real g_509428;
s_fade_record *g_50942c;
real g_4670fc = 1.0f;
real g_467100 = 1.0f;

inline void fade_record(void)
{
	s_fade_record *record = g_50942c;
	if (record)
	{
		record->state = g_509420;
		record->current = g_4670fc;
		record->target = g_467100;
		record->next_state = g_509424;
		record->rate = g_509428;
		record->marker = 0xdeadbeef;
	}
}

// @retail 0x2a0aa0
void function_2a0aa0(real seconds)
{
	if (seconds < 0.0001f)
		seconds = 0.0001f;
	if (g_509420 == 1)
	{
		g_509424 = 1;
		g_509428 = g_467100 / seconds;
	}
	fade_record();
}

// @retail 0x2a0b20
void function_2a0b20(real seconds)
{
	if (seconds < 0.0001f)
		seconds = 0.0001f;
	if (g_509420 == 0)
	{
		g_509424 = 2;
		g_509428 = (0.0f - g_4670fc) / seconds;
	}
	fade_record();
}

// @retail 0x2a0ba0
void function_2a0ba0(real seconds, real target)
{
	if (seconds < 0.0001f)
		seconds = 0.0001f;
	if (target > 1.0f)
		target = 1.0f;
	else if (target < 0.0f)
		target = 0.0f;
	g_467100 = target;
	if (g_509420 != 1)
	{
		g_509428 = (target - g_4670fc) / seconds;
		g_509424 = 3;
	}
	fade_record();
}

/* 25: boolean (boolean) */
// @retail 0x2a0c40
void __stdcall function_2a0c40(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = !*(bool *)&arguments[0];
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44b228 = { _hs_type_boolean, 0, function_2a0c40, NULL, 1, { _hs_type_boolean } };

/* real_pin of the math library; retail function not identified */
inline real real_pin(real value, real lower, real upper)
{
	return value < lower ? lower : (value > upper ? upper : value);
}

/* 26: real (real, real, real) */
// @retail 0x2a0c90
void __stdcall function_2a0c90(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		real result = real_pin(*(real *)&arguments[0], *(real *)&arguments[1], *(real *)&arguments[2]);
		function_209ae0(thread_index, *(long *)&result);
	}
}

hs_function_definition const g_44b23c = { _hs_type_real, 0, function_2a0c90, NULL, 3, { _hs_type_real, _hs_type_real, _hs_type_real } };

/* 28: object_list () */
// @retail 0x2a0d10
void __stdcall function_2a0d10(short function_index, long thread_index, bool initialize)
{
	function_209ae0(thread_index, function_29f480());
}

hs_function_definition const g_44b268 = { _hs_type_object_list, 0, function_2a0d10, NULL, 0 };

void function_1ec4c0(long index);

/* 29: void (trigger_volume) */
// @retail 0x2a0d30
void __stdcall function_2a0d30(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_1ec4c0(*(short *)&arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b278 = { _hs_type_void, 0, function_2a0d30, NULL, 1, { _hs_type_trigger_volume } };

/* the scenario's trigger volumes (g_4e0350, globals.h), 0x44 bytes each */
struct s_scenario_trigger_volume
{
	byte unknown00[0x40];
	short kill_volume_index;
	byte unknown42[2];
};

struct s_scenario_trigger_volumes_view
{
	byte unknown000[0x108];
	long trigger_volume_count;
	s_scenario_trigger_volume *trigger_volumes;
};


/* 30: void (trigger_volume) */
// @retail 0x2a0d70
void __stdcall function_2a0d70(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long  trigger_volume_index = *(short *)&arguments[0];
		if (trigger_volume_index != NONE)
		{
			s_scenario_trigger_volume *trigger_volume = &((s_scenario_trigger_volumes_view *)g_4e0350)->trigger_volumes[trigger_volume_index];
			if (trigger_volume->kill_volume_index != NONE)
			{
				long index = trigger_volume->kill_volume_index;
				g_51e9c8->bits[index >> 5] |= 1 << (index & 31);
			}
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b28c = { _hs_type_void, 0, function_2a0d70, NULL, 1, { _hs_type_trigger_volume } };

/* 31: void (trigger_volume, cutscene_flag) */
// @retail 0x2a0df0
void __stdcall function_2a0df0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_29f5b0(*(short *)&arguments[0], *(short *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b2a0 = { _hs_type_void, 0, function_2a0df0, NULL, 2, { _hs_type_trigger_volume, _hs_type_cutscene_flag } };

/* 32: boolean (trigger_volume, object) */
// @retail 0x2a0e40
void __stdcall function_2a0e40(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long object_index = arguments[1];
		long trigger_volume_index = *(short *)&arguments[0];
		bool inside = false;
		if (object_index != NONE)
		{
			real_point3d *center = &object_get(object_index)->center;
			if (function_11c470(trigger_volume_index, center))
				inside = true;
		}
		*(bool *)&result = inside;
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44b2b4 = { _hs_type_boolean, 0, function_2a0e40, NULL, 2, { _hs_type_trigger_volume, _hs_type_object } };

/* 33: boolean (trigger_volume, object_list) */
// @retail 0x2a0ec0
void __stdcall function_2a0ec0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long list_index = arguments[1];
		long trigger_volume_index = *(short *)&arguments[0];
		*(bool *)&result = function_29f6c0(list_index, trigger_volume_index, false);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44b2c8 = { _hs_type_boolean, 0, function_2a0ec0, NULL, 2, { _hs_type_trigger_volume, _hs_type_object_list } };

/* 34: boolean (trigger_volume, object_list) */
// @retail 0x2a0f20
void __stdcall function_2a0f20(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long list_index = arguments[1];
		long trigger_volume_index = *(short *)&arguments[0];
		*(bool *)&result = function_29f6c0(list_index, trigger_volume_index, true);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44b2dc = { _hs_type_boolean, 0, function_2a0f20, NULL, 2, { _hs_type_trigger_volume, _hs_type_object_list } };

/* 35: object_list (trigger_volume) */
// @retail 0x2a0f80
void __stdcall function_2a0f80(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_209ae0(thread_index, function_11c5f0(*(short *)&arguments[0], NONE));
	}
}

hs_function_definition const g_44b2f0 = { _hs_type_object_list, 0, function_2a0f80, NULL, 1, { _hs_type_trigger_volume } };

/* 36: object_list (trigger_volume, long_integer) */
// @retail 0x2a0fc0
void __stdcall function_2a0fc0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_209ae0(thread_index, function_11c5f0(*(short *)&arguments[0], arguments[1]));
	}
}

hs_function_definition const g_44b304 = { _hs_type_object_list, 0, function_2a0fc0, NULL, 2, { _hs_type_trigger_volume, _hs_type_long_integer } };

/* 37: object (object_list, short) */
// @retail 0x2a1010
void __stdcall function_2a1010(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
		function_209ae0(thread_index, object_list_get_element(arguments[0], *(short *)&arguments[1]));
}

hs_function_definition const g_44b318 = { _hs_type_object, 0, function_2a1010, NULL, 2, { _hs_type_object_list, _hs_type_short_integer } };

/* 38: short (object_list) */
// @retail 0x2a1060
void __stdcall function_2a1060(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long list_index = arguments[0];
		short count = 0;
		if (list_index != NONE)
			count = ((s_object_list_datum *)g_4f55d8->data)[list_index & 0xffff].count;
		*(short *)&result = count;
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44b32c = { _hs_type_short_integer, 0, function_2a1060, NULL, 1, { _hs_type_object_list } };

/* retail function not identified */
inline void object_globals_set_value_70(long index, real value)
{
	if (index >= 0 && index < NUMBEROF(((s_object_globals_view *)g_4de2f4)->values_70))
		((s_object_globals_view *)g_4de2f4)->values_70[index] = PIN(value, 0.0f, 1.0f);
}

inline void object_name_function_bb670(short name_index, bool flag)
{
	if (name_index != NONE)
		function_bb670(name_index, flag);
}

/* 40: void (effect, cutscene_flag) */
// @retail 0x2a1120
void __stdcall function_2a1120(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_2a05f0(arguments[0], *(short *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b354 = { _hs_type_void, 0, function_2a1120, NULL, 2, { _hs_type_effect, _hs_type_cutscene_flag } };

/* 46: void (object_name) */
// @retail 0x2a12f0
void __stdcall function_2a12f0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		object_name_function_bb670(*(short *)&arguments[0], false);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b3d0 = { _hs_type_void, 0, function_2a12f0, NULL, 1, { _hs_type_object_name } };

/* 47: void (object_name) */
// @retail 0x2a1340
void __stdcall function_2a1340(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		object_name_function_bb670(*(short *)&arguments[0], true);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b3e4 = { _hs_type_void, 0, function_2a1340, NULL, 1, { _hs_type_object_name } };

/* 48: void (object_name) */
// @retail 0x2a1390
void __stdcall function_2a1390(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_29fd80(*(short *)&arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b3f8 = { _hs_type_void, 0, function_2a1390, NULL, 1, { _hs_type_object_name } };

/* 49: void (string) */
// @retail 0x2a13e0
void __stdcall function_2a13e0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		hs_object_iterate_names_containing((char const *)arguments[0], function_29fd40);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b40c = { _hs_type_void, 0, function_2a13e0, NULL, 1, { _hs_type_string } };
/* 50: void (string) */
// @retail 0x2a1430
void __stdcall function_2a1430(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		hs_object_iterate_names_containing((char const *)arguments[0], function_29fd60);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b420 = { _hs_type_void, 0, function_2a1430, NULL, 1, { _hs_type_string } };
/* 51: void (string) */
// @retail 0x2a1480
void __stdcall function_2a1480(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		hs_object_iterate_names_containing((char const *)arguments[0], function_29fd80);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b434 = { _hs_type_void, 0, function_2a1480, NULL, 1, { _hs_type_string } };
/* 52: void (object) */
// @retail 0x2a14d0
void __stdcall function_2a14d0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long object_index = arguments[0];
		if (object_index != NONE && !function_beb30(object_index))
			function_b8540(object_index);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b448 = { _hs_type_void, 0, function_2a14d0, NULL, 1, { _hs_type_object } };

/* 53: void (string) */
// @retail 0x2a1520
void __stdcall function_2a1520(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		hs_object_iterate_names_containing((char const *)arguments[0], function_29fdd0);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b45c = { _hs_type_void, 0, function_2a1520, NULL, 1, { _hs_type_string } };
/* 54: void () */
// @retail 0x2a1570
void __stdcall function_2a1570(short function_index, long thread_index, bool initialize)
{
	function_29fe10(NONE);
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44b470 = { _hs_type_void, 0, function_2a1570, NULL, 0 };

/* retail function not identified */
inline void object_function_b9d70(long object_index, bool flag)
{
	if (object_index != NONE)
		function_b9d70(object_index, flag);
}

/* 55: void (long_integer) */
// @retail 0x2a1590
void __stdcall function_2a1590(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_29fe10(arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b480 = { _hs_type_void, 0, function_2a1590, NULL, 1, { _hs_type_long_integer } };

/* 58: void (object, boolean) */
// @retail 0x2a1660
void __stdcall function_2a1660(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		object_function_b9d70(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b4bc = { _hs_type_void, 0, function_2a1660, NULL, 2, { _hs_type_object, _hs_type_boolean } };

/* 59: void (long, real) */
// @retail 0x2a16b0
void __stdcall function_2a16b0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		object_globals_set_value_70(arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b4d0 = { _hs_type_void, 0, function_2a16b0, NULL, 2, { _hs_type_long_integer, _hs_type_real } };

/* retail function not identified */
inline void unit_function_10e9a0(long unit_index, long name, real value, real time)
{
	if (unit_index != NONE)
		function_10e9a0(unit_index, name, value, time);
}

/* 61: void (object, string_id) */
// @retail 0x2a1770
void __stdcall function_2a1770(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_10aca0(arguments[0], arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b4fc = { _hs_type_void, 0, function_2a1770, NULL, 2, { _hs_type_object, _hs_type_string_id } };

/* 62: void (object) */
// @retail 0x2a17c0
void __stdcall function_2a17c0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_10ace0(arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b510 = { _hs_type_void, 0, function_2a17c0, NULL, 1, { _hs_type_object } };

void function_10ad40(long object_index, bool flag);
/* 63: void (object, boolean) */
// @retail 0x2a1800
void __stdcall function_2a1800(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_10ad40(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b524 = { _hs_type_void, 0, function_2a1800, NULL, 2, { _hs_type_object, _hs_type_boolean } };

/* 64: void (object, boolean) */
// @retail 0x2a1850
void __stdcall function_2a1850(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long object_index = arguments[0];
		if (object_index != NONE)
		{
			s_object *object = object_get(object_index);
			word *flags = &object->flags_c0;
			SET_FLAG(*flags, 10, *(bool *)&arguments[1]);
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b538 = { _hs_type_void, 0, function_2a1850, NULL, 2, { _hs_type_object, _hs_type_boolean } };

/* 66: void (object, boolean) */
// @retail 0x2a1920
void __stdcall function_2a1920(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long object_index = arguments[0];
		if (object_index != NONE)
		{
			s_object *object = object_get(object_index);
			byte *flags = &object->flags19;
			SET_FLAG(*flags, 1, *(bool *)&arguments[1]);
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b560 = { _hs_type_void, 0, function_2a1920, NULL, 2, { _hs_type_object, _hs_type_boolean } };

/* 67: void (object, boolean) */
// @retail 0x2a19a0
void __stdcall function_2a19a0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long object_index = arguments[0];
		if (object_index != NONE)
		{
			s_object *object = object_get(object_index);
			byte *flags = &object->flags19;
			SET_FLAG(*flags, 0, *(bool *)&arguments[1]);
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b574 = { _hs_type_void, 0, function_2a19a0, NULL, 2, { _hs_type_object, _hs_type_boolean } };

/* retail function not identified */
inline real object_get_body_vitality(long object_index)
{
	s_object *object = function_badc0(object_index, NONE);
	real result = -1.0f;
	if (object)
	{
		result = 0.0f;
		if (!TEST_FIELD_BIT(object->flag_10a_2))
			result = object->body_vitality;
	}
	return result;
}

/* 69: real (object); 239: real (unit) */
// @retail 0x2a1a20
void __stdcall function_2a1a20(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		real result = object_get_body_vitality(arguments[0]);
		function_209ae0(thread_index, *(long *)&result);
	}
}

hs_function_definition const g_44b598 = { _hs_type_real, 0, function_2a1a20, NULL, 1, { _hs_type_object } };
hs_function_definition const g_44c334 = { _hs_type_real, 0, function_2a1a20, NULL, 1, { _hs_type_unit } };

/* retail function not identified */
inline real object_get_shield_vitality(long object_index)
{
	s_object *object = function_badc0(object_index, NONE);
	real result = -1.0f;
	if (object)
	{
		result = 0.0f;
		if (!TEST_FIELD_BIT(object->flag_10a_2))
			result = object->shield_vitality;
	}
	return result;
}

/* 70: real (object); 240: real (unit) */
// @retail 0x2a1a90
void __stdcall function_2a1a90(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		real result = object_get_shield_vitality(arguments[0]);
		function_209ae0(thread_index, *(long *)&result);
	}
}

hs_function_definition const g_44b5ac = { _hs_type_real, 0, function_2a1a90, NULL, 1, { _hs_type_object } };
hs_function_definition const g_44c348 = { _hs_type_real, 0, function_2a1a90, NULL, 1, { _hs_type_unit } };

/* 71: void (object, real, real) */
// @retail 0x2a1b00
void __stdcall function_2a1b00(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_10ad90(arguments[0], *(real *)&arguments[1], *(real *)&arguments[2]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b5c0 = { _hs_type_void, 0, function_2a1b00, NULL, 3, { _hs_type_object, _hs_type_real, _hs_type_real } };

/* 72: void (object, boolean) */
// @retail 0x2a1b50
void __stdcall function_2a1b50(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long object_index = arguments[0];
		if (object_index != NONE)
		{
			s_object *object = object_get(object_index);
			word *flags = &object->flags_c0;
			SET_FLAG(*flags, 7, !*(bool *)&arguments[1]);
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b5d8 = { _hs_type_void, 0, function_2a1b50, NULL, 2, { _hs_type_object, _hs_type_boolean } };

/* 73: object (object) */
// @retail 0x2a1bd0
void __stdcall function_2a1bd0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long object_index = arguments[0];
		long parent_index = NONE;
		if (object_index != NONE)
			parent_index = object_get(object_index)->parent_index;
		function_209ae0(thread_index, parent_index);
	}
}

hs_function_definition const g_44b5ec = { _hs_type_object, 0, function_2a1bd0, NULL, 1, { _hs_type_object } };

long function_10ae60(long object_index, long marker_name);
/* 75: object (object, string_id) */
// @retail 0x2a1c80
void __stdcall function_2a1c80(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_209ae0(thread_index, function_10ae60(arguments[0], arguments[1]));
	}
}

hs_function_definition const g_44b618 = { _hs_type_object, 0, function_2a1c80, NULL, 2, { _hs_type_object, _hs_type_string_id } };

/* 77: void (object, real, short_integer) */
// @retail 0x2a1d30
void __stdcall function_2a1d30(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_10af80(arguments[0], *(real *)&arguments[1], *(short *)&arguments[2]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b640 = { _hs_type_void, 0, function_2a1d30, NULL, 3, { _hs_type_object, _hs_type_real, _hs_type_short_integer } };

/* 80: void (object) */
// @retail 0x2a1e20
void __stdcall function_2a1e20(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long object_index = arguments[0];
		if (object_index != NONE)
		{
			s_object *object = object_get(object_index);
			dword *flags = &object->flags;
			SET_FLAG(*flags, 17, true);
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b684 = { _hs_type_void, 0, function_2a1e20, NULL, 1, { _hs_type_object } };

/* 82: boolean (object, string_id) */
// @retail 0x2a1e80
void __stdcall function_2a1e80(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = function_d88f0(arguments[0], arguments[1]) > 0;
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44b6ac = { _hs_type_boolean, 0, function_2a1e80, NULL, 2, { _hs_type_object, _hs_type_string_id } };

/* 83: short_integer (object, string_id) */
// @retail 0x2a1ee0
void __stdcall function_2a1ee0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(short *)&result = function_d88f0(arguments[0], arguments[1]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44b6c0 = { _hs_type_short_integer, 0, function_2a1ee0, NULL, 2, { _hs_type_object, _hs_type_string_id } };

/* 85: void (object, boolean) */
// @retail 0x2a1f90
void __stdcall function_2a1f90(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long object_index = arguments[0];
		if (object_index != NONE)
		{
			s_object *object = object_get(object_index);
			word *flags = &object->flags_10a;
			SET_FLAG(*flags, 14, *(bool *)&arguments[1]);
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b6ec = { _hs_type_void, 0, function_2a1f90, NULL, 2, { _hs_type_object, _hs_type_boolean } };

bool function_dc310(long object_index);

/* 86: boolean (object) */
// @retail 0x2a2010
void __stdcall function_2a2010(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = function_dc310(arguments[0]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44b700 = { _hs_type_boolean, 0, function_2a2010, NULL, 1, { _hs_type_object } };

/* 87: void () */
// @retail 0x2a2060
void __stdcall function_2a2060(short function_index, long thread_index, bool initialize)
{
	s_object_globals_view *globals = (s_object_globals_view *)g_4de2f4;
	globals->garbage_collect_now = true;
	globals->garbage_collect_unsafe = false;
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44b714 = { _hs_type_void, 0, function_2a2060, NULL, 0 };

/* 88: void () */
// @retail 0x2a2080
void __stdcall function_2a2080(short function_index, long thread_index, bool initialize)
{
	s_object_globals_view *globals = (s_object_globals_view *)g_4de2f4;
	globals->garbage_collect_now = true;
	globals->garbage_collect_unsafe = true;
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44b724 = { _hs_type_void, 0, function_2a2080, NULL, 0 };

/* retail function not identified */
inline void object_globals_set_point_1c(real x, real y, real z)
{
	s_object_globals_view *globals = (s_object_globals_view *)g_4de2f4;
	globals->point_1c.x = x;
	globals->point_1c.y = y;
	globals->point_1c.z = z;
}

/* 89: void () */
// @retail 0x2a20a0
void __stdcall function_2a20a0(short function_index, long thread_index, bool initialize)
{
	function_159ac0();
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44b734 = { _hs_type_void, 0, function_2a20a0, NULL, 0 };

/* 92: void (object, boolean) */
// @retail 0x2a2140
void __stdcall function_2a2140(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_bbf40(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b76c = { _hs_type_void, 0, function_2a2140, NULL, 2, { _hs_type_object, _hs_type_boolean } };

/* 94: void (object, boolean) */
// @retail 0x2a21e0
void __stdcall function_2a21e0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_bc150(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b794 = { _hs_type_void, 0, function_2a21e0, NULL, 2, { _hs_type_object, _hs_type_boolean } };

/* 95: void (object, boolean) */
// @retail 0x2a2230
void __stdcall function_2a2230(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_bbf80(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b7a8 = { _hs_type_void, 0, function_2a2230, NULL, 2, { _hs_type_object, _hs_type_boolean } };

/* 96: void (real, real, real, real, real) */
// @retail 0x2a2280
void __stdcall function_2a2280(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_bbfc0(*(real *)&arguments[0], *(real *)&arguments[1], *(real *)&arguments[2], *(real *)&arguments[3], *(real *)&arguments[4]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b7bc = { _hs_type_void, 0, function_2a2280, NULL, 5, { _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real } };

/* 97: void (real, real, real, real, real) */
// @retail 0x2a22d0
void __stdcall function_2a22d0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_bc070(*(real *)&arguments[0], *(real *)&arguments[1], *(real *)&arguments[2], *(real *)&arguments[3], *(real *)&arguments[4]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b7d8 = { _hs_type_void, 0, function_2a22d0, NULL, 5, { _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real } };

/* 98: void (real, real, real) */
// @retail 0x2a2320
void __stdcall function_2a2320(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		object_globals_set_point_1c(*(real *)&arguments[0], *(real *)&arguments[1], *(real *)&arguments[2]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b7f4 = { _hs_type_void, 0, function_2a2320, NULL, 3, { _hs_type_real, _hs_type_real, _hs_type_real } };

/* retail function not identified */
inline void object_set_shield_fraction(long object_index, real fraction)
{
	if (object_index != NONE)
	{
		s_object *object = object_get(object_index);
		fraction = PIN(fraction, 0.0f, 1.0f);
		object->shield_vitality = object->maximum_vitality * fraction;
	}
}

/* 99: void (object_list); 100: void (object_list) */
// @retail 0x2a2380
void __stdcall function_2a2380(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_2a03d0(arguments[0], false);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b80c = { _hs_type_void, 0, function_2a2380, NULL, 1, { _hs_type_object_list } };
hs_function_definition const g_44b820 = { _hs_type_void, 0, function_2a2380, NULL, 1, { _hs_type_object_list } };

/* 101: void (object_list) */
// @retail 0x2a23c0
void __stdcall function_2a23c0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_2a03d0(arguments[0], true);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b834 = { _hs_type_void, 0, function_2a23c0, NULL, 1, { _hs_type_object_list } };

/* 105: void (object, cutscene_flag) */
// @retail 0x2a24e0
void __stdcall function_2a24e0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_29ffb0(arguments[0], *(short *)&arguments[1], true, true);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b884 = { _hs_type_void, 0, function_2a24e0, NULL, 2, { _hs_type_object, _hs_type_cutscene_flag } };

/* 106: void (object, cutscene_flag) */
// @retail 0x2a2530
void __stdcall function_2a2530(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_29ffb0(arguments[0], *(short *)&arguments[1], false, true);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b898 = { _hs_type_void, 0, function_2a2530, NULL, 2, { _hs_type_object, _hs_type_cutscene_flag } };

/* 107: void (object, real) */
// @retail 0x2a2580
void __stdcall function_2a2580(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		object_set_shield_fraction(arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b8ac = { _hs_type_void, 0, function_2a2580, NULL, 2, { _hs_type_object, _hs_type_real } };

/* 108: void (object, real) */
// @retail 0x2a2610
void __stdcall function_2a2610(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_2a0310(arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b8c0 = { _hs_type_void, 0, function_2a2610, NULL, 2, { _hs_type_object, _hs_type_real } };

/* 109: void (object) */
// @retail 0x2a2660
void __stdcall function_2a2660(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long object_index = arguments[0];
		if (object_index != NONE)
			object_get(object_index)->value_104 = 0x7fff;
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b8d4 = { _hs_type_void, 0, function_2a2660, NULL, 1, { _hs_type_object } };

/* the body of 0x20b100, which draws from the second seed of g_4e7408 */
inline short _random_range(dword *seed, short lower, short upper)
{
	*seed = *seed * 0x19660d + 0x3c6ef35f;
	return lower + (short)(((upper - lower) * (*seed >> 16)) >> 16);
}

/* 110: void (object, string_id, string_id) */
// @retail 0x2a26c0
void __stdcall function_2a26c0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long object_index = arguments[0];
		if (object_index != NONE)
			function_ba410(object_index, arguments[1], arguments[2]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b8e8 = { _hs_type_void, 0, function_2a26c0, NULL, 3, { _hs_type_object, _hs_type_string_id, _hs_type_string_id } };

/* 111: void (object, string_id, model_state) */
// @retail 0x2a2710
void __stdcall function_2a2710(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_2a0380(arguments[0], arguments[1], *(short *)&arguments[2]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b900 = { _hs_type_void, 0, function_2a2710, NULL, 3, { _hs_type_object, _hs_type_string_id, _hs_type_model_state } };

/* 114: real (object_list, object) */
// @retail 0x2a2820
void __stdcall function_2a2820(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
		hs_return_real(thread_index, objects_distance_to_object(arguments[0], arguments[1]));
}

hs_function_definition const g_44b948 = { _hs_type_real, 0, function_2a2820, NULL, 2, { _hs_type_object_list, _hs_type_object } };

/* 115: real (object_list, cutscene_flag) */
// @retail 0x2a2870
void __stdcall function_2a2870(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
		hs_return_real(thread_index, objects_distance_to_flag(arguments[0], *(short *)&arguments[1]));
}

hs_function_definition const g_44b95c = { _hs_type_real, 0, function_2a2870, NULL, 2, { _hs_type_object_list, _hs_type_cutscene_flag } };

/* 123: object_list () */
// @retail 0x2a29b0
void __stdcall function_2a29b0(short function_index, long thread_index, bool initialize)
{
	function_209ae0(thread_index, function_15e730());
}

hs_function_definition const g_44b9f4 = { _hs_type_object_list, 0, function_2a29b0, NULL, 0 };

/* 124: short (short, short) */
// @retail 0x2a29d0
void __stdcall function_2a29d0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(short *)&result = _random_range(&g_4e7408->unknown0, *(short *)&arguments[0], *(short *)&arguments[1]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44ba04 = { _hs_type_short_integer, 0, function_2a29d0, NULL, 2, { _hs_type_short_integer, _hs_type_short_integer } };

/* _real_random_range (0x259d0, real_math.cpp) on the first seed of g_4e7408 */
inline real real_random_range(real lower, real upper)
{
	dword *seed = &g_4e7408->unknown0;
	*seed = *seed * 0x19660d + 0x3c6ef35f;
	return lower + (upper - lower) * ((real)(*seed >> 16) * (1.0f / 65535.0f));
}

/* 125: real (real, real) */
// @retail 0x2a2a50
void __stdcall function_2a2a50(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		real result = real_random_range(*(real *)&arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, *(long *)&result);
	}
}

hs_function_definition const g_44ba18 = { _hs_type_real, 0, function_2a2a50, NULL, 2, { _hs_type_real, _hs_type_real } };

/* 126: void () */
// @retail 0x2a2ad0
void __stdcall function_2a2ad0(short function_index, long thread_index, bool initialize)
{
	s_unknown_1eb550 *globals = g_51e9c4;
	globals->unknown0 = 4.1712594f;
	globals->unknown4 = 1.0f;
	globals->unknown8 = 0.0011f;
	globals->unknown18 = 0;
	globals->vector = *g_4687a4;
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44ba2c = { _hs_type_void, 0, function_2a2ad0, NULL, 0 };

/* 127: void (real) */
// @retail 0x2a2b30
void __stdcall function_2a2b30(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		g_51e9c4->unknown0 = *(real *)&arguments[0] * 4.1712594f;
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44ba3c = { _hs_type_void, 0, function_2a2b30, NULL, 1, { _hs_type_real } };

/* retail function not identified */
inline void function_2a2b80_set_vector(real i, real j, real k)
{
	s_unknown_1eb550 *globals = g_51e9c4;
	globals->vector.i = i;
	globals->vector.j = j;
	globals->vector.k = k;
}

/* 128: void (real, real, real) */
// @retail 0x2a2b80
void __stdcall function_2a2b80(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_2a2b80_set_vector(*(real *)&arguments[0], *(real *)&arguments[1], *(real *)&arguments[2]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44ba50 = { _hs_type_void, 0, function_2a2b80, NULL, 3, { _hs_type_real, _hs_type_real, _hs_type_real } };

/* 129: void (real) */
// @retail 0x2a2be0
void __stdcall function_2a2be0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		real seconds = *(real *)&arguments[0];
		seconds = PIN(seconds, 0.0f, 5.0f);
		s_game_time_globals *game_time = g_510c54;
		long game_time_now = game_time->game_time;
		real ticks_real = game_time->ticks_per_second * seconds;
		long ticks;
		__asm
		{
			fld ticks_real
			fistp ticks
		}
		g_51e9c4->unknown18 = ticks + game_time_now;
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44ba68 = { _hs_type_void, 0, function_2a2be0, NULL, 1, { _hs_type_real } };

/* 136: void (boolean) */
// @retail 0x2a2c70
void __stdcall function_2a2c70(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*g_4ed280 = *(byte *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44bae0 = { _hs_type_void, 0, function_2a2c70, NULL, 1, { _hs_type_boolean } };

/* 137: void () */
// @retail 0x2a2cb0
void __stdcall function_2a2cb0(short function_index, long thread_index, bool initialize)
{
	memset(g_4ed280, 0xff, 0x4201);
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44baf4 = { _hs_type_void, 0, function_2a2cb0, NULL, 0 };

/* 141: void (unit) */
// @retail 0x2a2e00
void __stdcall function_2a2e00(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		s_recorded_animation *animation = recorded_animation_find(arguments[0], NULL);
		if (animation)
			animation->flags |= 3;
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44bb40 = { _hs_type_void, 0, function_2a2e00, NULL, 1, { _hs_type_unit } };

/* 142: short_integer (unit) */
// @retail 0x2a2e50
void __stdcall function_2a2e50(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(short *)&result = (short)recorded_animation_get_frames(arguments[0]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44bb54 = { _hs_type_short_integer, 0, function_2a2e50, NULL, 1, { _hs_type_unit } };

/* 143: boolean (boolean) */
// @retail 0x2a2ea0
void __stdcall function_2a2ea0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		bool value = *(bool *)&arguments[0];
		g_5107e8->flag8 = value;
		*(bool *)&result = value;
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44bb68 = { _hs_type_boolean, 0, function_2a2ea0, NULL, 1, { _hs_type_boolean } };

void function_25600(long object_index, long string_id, real seconds);

/* 144: void (object, string_id, real) */
// @retail 0x2a2f00
void __stdcall function_2a2f00(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_25600(arguments[0], arguments[1], *(real *)&arguments[2]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44bb7c = { _hs_type_void, 0, function_2a2f00, NULL, 3, { _hs_type_object, _hs_type_string_id, _hs_type_real } };

/* 145: void () */
// @retail 0x2a2f50
void __stdcall function_2a2f50(short function_index, long thread_index, bool initialize)
{
	memset(g_4b9970, 0, sizeof(g_4b9970));
	g_4b9970[0] = NONE;
	g_5093fc = false;
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44bb94 = { _hs_type_void, 0, function_2a2f50, NULL, 0 };

void function_30e30(long key, long value, bool add, real x);

/* 146: void (boolean, object, string_id, real) */
// @retail 0x2a2f80
void __stdcall function_2a2f80(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_30e30(arguments[1], arguments[2], *(bool *)&arguments[0], *(real *)&arguments[3]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44bba4 = { _hs_type_void, 0, function_2a2f80, NULL, 4, { _hs_type_boolean, _hs_type_object, _hs_type_string_id, _hs_type_real } };

void function_b7360(long object_index);
void function_24c831(short index);
void function_24c878(short index);
void function_24c8e2(short a, short b);
void function_24c93f(bool flag);

/* 192: void (unit, boolean) */
// @retail 0x2a3200
void __stdcall function_2a3200(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_11a7f0(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44bf40 = { _hs_type_void, 0, function_2a3200, NULL, 2, { _hs_type_unit, _hs_type_boolean } };

/* 193: void (unit, boolean, real) */
// @retail 0x2a3250
void __stdcall function_2a3250(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_11a830(arguments[0], *(bool *)&arguments[1], *(real *)&arguments[2]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44bf54 = { _hs_type_void, 0, function_2a3250, NULL, 3, { _hs_type_unit, _hs_type_boolean, _hs_type_real } };

/* 194: void (unit) */
// @retail 0x2a32a0
void __stdcall function_2a32a0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_11a8c0(arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44bf6c = { _hs_type_void, 0, function_2a32a0, NULL, 1, { _hs_type_unit } };

/* 195: void (unit) */
// @retail 0x2a32e0
void __stdcall function_2a32e0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_11a910(arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44bf80 = { _hs_type_void, 0, function_2a32e0, NULL, 1, { _hs_type_unit } };

/* 196: void (unit) */
// @retail 0x2a3320
void __stdcall function_2a3320(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long unit_index = arguments[0];
		if (unit_index != NONE)
		{
			s_object *unit = object_get(unit_index);
			word *flags = &unit->flags_10a;
			SET_FLAG(*flags, 5, true);
			function_b7360(unit_index);
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44bf94 = { _hs_type_void, 0, function_2a3320, NULL, 1, { _hs_type_unit } };

/* 197: void (unit) */
// @retail 0x2a3390
void __stdcall function_2a3390(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long unit_index = arguments[0];
		if (unit_index != NONE)
		{
			s_object *unit = object_get(unit_index);
			word *flags = &unit->flags_10a;
			SET_FLAG(*flags, 6, true);
			function_b7360(unit_index);
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44bfa8 = { _hs_type_void, 0, function_2a3390, NULL, 1, { _hs_type_unit } };

/* 198: boolean (unit) */
// @retail 0x2a3400
void __stdcall function_2a3400(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = function_11a960(arguments[0]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44bfbc = { _hs_type_boolean, 0, function_2a3400, NULL, 1, { _hs_type_unit } };

/* 199: short_integer (unit) */
// @retail 0x2a3450
void __stdcall function_2a3450(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		real ticks_real = function_11b6b0(arguments[0]) * 30.0f;
		long ticks;
		__asm
		{
			fld ticks_real
			fistp ticks
		}
		*(short *)&result = (short)(ticks - 2 > 0 ? ticks - 2 : 0);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44bfd0 = { _hs_type_short_integer, 0, function_2a3450, NULL, 1, { _hs_type_unit } };

/* 207: boolean (unit) */
// @retail 0x2a37a0
void __stdcall function_2a37a0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = function_11b930(arguments[0]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44c094 = { _hs_type_boolean, 0, function_2a37a0, NULL, 1, { _hs_type_unit } };

/* 208: void (boolean) */
// @retail 0x2a37f0
void __stdcall function_2a37f0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		((s_object_globals_view *)g_4de2f4)->flag80 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c0a8 = { _hs_type_void, 0, function_2a37f0, NULL, 1, { _hs_type_boolean } };

/* 209: void (boolean) */
// @retail 0x2a3840
void __stdcall function_2a3840(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		((s_object_globals_view *)g_4de2f4)->flag81 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c0bc = { _hs_type_void, 0, function_2a3840, NULL, 1, { _hs_type_boolean } };

/* 210: void (unit, boolean) */
// @retail 0x2a3890
void __stdcall function_2a3890(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long unit_index = arguments[0];
		if (unit_index != NONE)
		{
			s_unit *unit = (s_unit *)object_get(unit_index);
			dword *flags = &unit->unit_flags;
			SET_FLAG(*flags, 1, *(bool *)&arguments[1]);
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c0d0 = { _hs_type_void, 0, function_2a3890, NULL, 2, { _hs_type_unit, _hs_type_boolean } };

/* retail function not identified */
inline short unit_get_value_138(long unit_index)
{
	s_unit *unit = (s_unit *)function_badc0(unit_index, 3);
	short result = NONE;
	if (unit)
		result = unit->value_138;
	return result;
}

/* 211: short (unit) */
// @retail 0x2a3910
void __stdcall function_2a3910(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(short *)&result = unit_get_value_138(arguments[0]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44c0e4 = { _hs_type_short_integer, 0, function_2a3910, NULL, 1, { _hs_type_unit } };

/* retail function not identified */
inline bool unit_test_flag12(long unit_index)
{
	bool result = true;
	if (unit_index != NONE)
		result = TEST_FIELD_BIT(((s_unit *)object_get(unit_index))->unit_flag12);
	return result;
}

/* 212: void (unit, boolean) */
// @retail 0x2a3970
void __stdcall function_2a3970(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_11b3e0(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c0f8 = { _hs_type_void, 0, function_2a3970, NULL, 2, { _hs_type_unit, _hs_type_boolean } };

/* 213: void (unit, boolean) */
// @retail 0x2a39c0
void __stdcall function_2a39c0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_11b420(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c10c = { _hs_type_void, 0, function_2a39c0, NULL, 2, { _hs_type_unit, _hs_type_boolean } };

/* 214: boolean (unit) */
// @retail 0x2a3a10
void __stdcall function_2a3a10(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = unit_test_flag12(arguments[0]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44c120 = { _hs_type_boolean, 0, function_2a3a10, NULL, 1, { _hs_type_unit } };

/* retail function not identified */
inline bool unit_has_index_1fc(long unit_index)
{
	bool result = false;
	if (unit_index != NONE && ((s_unit *)object_get(unit_index))->index_1fc != NONE)
		result = true;
	return result;
}

/* 215: void (unit, boolean) */
// @retail 0x2a3a80
void __stdcall function_2a3a80(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_11b460(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c134 = { _hs_type_void, 0, function_2a3a80, NULL, 2, { _hs_type_unit, _hs_type_boolean } };

void function_10e9f0(long object_index, short channel, real value, real time);

/* 216: void (unit, vehicle, string_id) */
// @retail 0x2a3ad0
void __stdcall function_2a3ad0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_11ade0(arguments[0], arguments[1], arguments[2]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c148 = { _hs_type_void, 0, function_2a3ad0, NULL, 3, { _hs_type_unit, _hs_type_vehicle, _hs_type_string_id } };

/* 217: void (unit, string_id) */
// @retail 0x2a3b20
void __stdcall function_2a3b20(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_11af10(arguments[0], arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c160 = { _hs_type_void, 0, function_2a3b20, NULL, 2, { _hs_type_unit, _hs_type_string_id } };

/* 218: void (unit, short_integer) */
// @retail 0x2a3b70
void __stdcall function_2a3b70(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		if (arguments[0] != NONE)
			function_10e9f0(arguments[0], *(short *)&arguments[1], 1.0f, 1.0f);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c174 = { _hs_type_void, 0, function_2a3b70, NULL, 2, { _hs_type_unit, _hs_type_short_integer } };

/* 219: void (unit, string_id) */
// @retail 0x2a3bd0
void __stdcall function_2a3bd0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		unit_function_10e9a0(arguments[0], arguments[1], 1.0f, 1.0f);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c188 = { _hs_type_void, 0, function_2a3bd0, NULL, 2, { _hs_type_unit, _hs_type_string_id } };

/* 220: void (unit, string_id, real, short_integer) */
// @retail 0x2a3c30
void __stdcall function_2a3c30(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		unit_function_10e9a0(arguments[0], arguments[1], *(real *)&arguments[2], (real)*(short *)&arguments[3] * (1.0f / 30.0f));
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c19c = { _hs_type_void, 0, function_2a3c30, NULL, 4, { _hs_type_unit, _hs_type_string_id, _hs_type_real, _hs_type_short_integer } };

/* 221: void (unit, boolean) */
// @retail 0x2a3ca0
void __stdcall function_2a3ca0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_1130a0(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c1b4 = { _hs_type_void, 0, function_2a3ca0, NULL, 2, { _hs_type_unit, _hs_type_boolean } };

/* 222: boolean (unit) */
// @retail 0x2a3ce0
void __stdcall function_2a3ce0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = unit_has_index_1fc(arguments[0]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44c1c8 = { _hs_type_boolean, 0, function_2a3ce0, NULL, 1, { _hs_type_unit } };

/* 223: boolean (vehicle, string_id, object_list) */
// @retail 0x2a3d50
void __stdcall function_2a3d50(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = function_11ac30(arguments[0], arguments[1], arguments[2]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44c1dc = { _hs_type_boolean, 0, function_2a3d50, NULL, 3, { _hs_type_vehicle, _hs_type_string_id, _hs_type_object_list } };

/* 224: boolean (vehicle, string_id, unit) */
// @retail 0x2a3db0
void __stdcall function_2a3db0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = function_11ab60(arguments[0], arguments[1], arguments[2]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44c1f4 = { _hs_type_boolean, 0, function_2a3db0, NULL, 3, { _hs_type_vehicle, _hs_type_string_id, _hs_type_unit } };

/* 225: void (unit, boolean) */
// @retail 0x2a3e10
void __stdcall function_2a3e10(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		s_unit *unit = (s_unit *)function_badc0(arguments[0], 3);
		if (unit)
			SET_FLAG(unit->unit_flags, 17, *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c20c = { _hs_type_void, 0, function_2a3e10, NULL, 2, { _hs_type_unit, _hs_type_boolean } };

/* 226: void (unit) */
// @retail 0x2a3e90
void __stdcall function_2a3e90(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long unit_index = arguments[0];

		if (unit_index != NONE)
		{
			s_object *unit = object_get(unit_index);

			if (unit->parent_index != NONE && ((s_unit *)unit)->index_1fc != NONE)
				function_e68c0(0x1d, unit_index);
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c220 = { _hs_type_void, 0, function_2a3e90, NULL, 1, { _hs_type_unit } };

/* 227: void (unit, short_integer) */
// @retail 0x2a3f10
void __stdcall function_2a3f10(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_11af90(arguments[0], *(short *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c234 = { _hs_type_void, 0, function_2a3f10, NULL, 2, { _hs_type_unit, _hs_type_short_integer } };

void object_initialize_vitality(long object_index, real *body_vitality, real *shield_vitality);

/* 228: void (unit, real, real) */
// @retail 0x2a3f60
void __stdcall function_2a3f60(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		real shield_vitality = *(real *)&arguments[2];
		real body_vitality = *(real *)&arguments[1];
		long unit_index = arguments[0];
		if (unit_index != NONE && !TEST_FIELD_BIT(object_get(unit_index)->flag_10a_2))
			object_initialize_vitality(unit_index, &body_vitality, &shield_vitality);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c248 = { _hs_type_void, 0, function_2a3f60, NULL, 3, { _hs_type_unit, _hs_type_real, _hs_type_real } };

/* 229: void (object_list, real, real) */
// @retail 0x2a3ff0
void __stdcall function_2a3ff0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_11a220(arguments[0], *(real *)&arguments[1], *(real *)&arguments[2]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c260 = { _hs_type_void, 0, function_2a3ff0, NULL, 3, { _hs_type_object_list, _hs_type_real, _hs_type_real } };

/* 232: short_integer (object, unit_seat_mapping, object_list) */
// @retail 0x2a40e0
void __stdcall function_2a40e0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(short *)&result = function_11b0c0(arguments[0], arguments[1], arguments[2]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44c2a8 = { _hs_type_short_integer, 0, function_2a40e0, NULL, 3, { _hs_type_object, _hs_type_unit_seat_mapping, _hs_type_object_list } };

/* 233: short_integer (object, unit_seat_mapping) */
// @retail 0x2a4140
void __stdcall function_2a4140(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(short *)&result = function_11b2b0(arguments[0], arguments[1]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44c2c0 = { _hs_type_short_integer, 0, function_2a4140, NULL, 2, { _hs_type_object, _hs_type_unit_seat_mapping } };

/* 234: void (unit, string_id) */
// @retail 0x2a41a0
void __stdcall function_2a41a0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long unit_index = arguments[0];
		if (unit_index != NONE)
		{
			s_unit *unit = (s_unit *)object_get(unit_index);
			*(long *)((byte *)unit + unit->offset_33e + 0x2c) = arguments[1];
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c2d4 = { _hs_type_void, 0, function_2a41a0, NULL, 2, { _hs_type_unit, _hs_type_string_id } };

/* retail function not identified */
inline long unit_get_index_248(long unit_index)
{
	s_unit *unit = (s_unit *)function_badc0(unit_index, 3);
	long result = NONE;
	if (unit)
		result = unit->index_248;
	return result;
}

/* 235: void () */
// @retail 0x2a4210
void __stdcall function_2a4210(short function_index, long thread_index, bool initialize)
{
	function_11b350();
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44c2e8 = { _hs_type_void, 0, function_2a4210, NULL, 0 };

/* 237: unit (unit) */
// @retail 0x2a4270
void __stdcall function_2a4270(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
		function_209ae0(thread_index, unit_get_index_248(arguments[0]));
}

hs_function_definition const g_44c30c = { _hs_type_unit, 0, function_2a4270, NULL, 1, { _hs_type_unit } };

/* retail function not identified */
inline long unit_get_index_24c(long unit_index)
{
	s_unit *unit = (s_unit *)function_badc0(unit_index, 3);
	long result = NONE;
	if (unit)
		result = unit->index_24c;
	return result;
}

/* 238: unit (unit) */
// @retail 0x2a42c0
void __stdcall function_2a42c0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
		function_209ae0(thread_index, unit_get_index_24c(arguments[0]));
}

hs_function_definition const g_44c320 = { _hs_type_unit, 0, function_2a42c0, NULL, 1, { _hs_type_unit } };

/* retail function not identified */
inline short unit_get_count_23e(long unit_index)
{
	s_unit *unit = (s_unit *)function_badc0(unit_index, 3);
	short result = 0;
	if (unit)
		result = unit->counts_23e[0] + unit->counts_23e[1];
	return result;
}

/* 241: short (unit) */
// @retail 0x2a4310
void __stdcall function_2a4310(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(short *)&result = unit_get_count_23e(arguments[0]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44c35c = { _hs_type_short_integer, 0, function_2a4310, NULL, 1, { _hs_type_unit } };

/* 243: boolean (unit, object_definition) */
// @retail 0x2a43e0
void __stdcall function_2a43e0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = function_11a4d0(arguments[0], arguments[1]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44c384 = { _hs_type_boolean, 0, function_2a43e0, NULL, 2, { _hs_type_unit, _hs_type_object_definition } };

/* 244: void (unit, short_integer) */
// @retail 0x2a4440
void __stdcall function_2a4440(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_11ab10(arguments[0], *(short *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c398 = { _hs_type_void, 0, function_2a4440, NULL, 2, { _hs_type_unit, _hs_type_short_integer } };

/* 245: void (unit, short_integer) */
// @retail 0x2a4490
void __stdcall function_2a4490(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_11aac0(arguments[0], *(short *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c3ac = { _hs_type_void, 0, function_2a4490, NULL, 2, { _hs_type_unit, _hs_type_short_integer } };

/* 247: void (object_list) */
// @retail 0x2a44e0
void __stdcall function_2a44e0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_11a680(arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c3d4 = { _hs_type_void, 0, function_2a44e0, NULL, 1, { _hs_type_object_list } };

/* 248: void (object_list, boolean) */
// @retail 0x2a4520
void __stdcall function_2a4520(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_11a570(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c3e8 = { _hs_type_void, 0, function_2a4520, NULL, 2, { _hs_type_object_list, _hs_type_boolean } };

/* 249: void (unit, boolean) */
// @retail 0x2a4570
void __stdcall function_2a4570(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_11a770(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c3fc = { _hs_type_void, 0, function_2a4570, NULL, 2, { _hs_type_unit, _hs_type_boolean } };

void function_102a00(long weapon_index, long trigger_index, bool flag);
/* 251: void (weapon, long_integer, boolean) */
// @retail 0x2a4610
void __stdcall function_2a4610(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_102a00(arguments[0], arguments[1], *(bool *)&arguments[2]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c428 = { _hs_type_void, 0, function_2a4610, NULL, 3, { _hs_type_weapon, _hs_type_long_integer, _hs_type_boolean } };

/* 252: void (boolean) */
// @retail 0x2a4660
void __stdcall function_2a4660(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		((s_object_globals_view *)g_4de2f4)->flag82 = !*(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c440 = { _hs_type_void, 0, function_2a4660, NULL, 1, { _hs_type_boolean } };

/* 253: void (device, boolean) */
// @retail 0x2a46b0
void __stdcall function_2a46b0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long device_index = arguments[0];
		if (device_index != NONE)
		{
			s_device *device = (s_device *)function_badc0(device_index, 0x80);
			if (device)
				SET_FLAG(device->device_flags, 4, *(bool *)&arguments[1]);
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c454 = { _hs_type_void, 0, function_2a46b0, NULL, 2, { _hs_type_device, _hs_type_boolean } };

/* retail function not identified */
inline real device_get_position(long device_index)
{
	real result = 0.0f;
	if (device_index != NONE)
		result = ((s_device *)object_get(device_index))->position;
	return result;
}

void function_107190(long device_index, real value);
/* 254: void (device, real) */
// @retail 0x2a4730
void __stdcall function_2a4730(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_107190(arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c468 = { _hs_type_void, 0, function_2a4730, NULL, 2, { _hs_type_device, _hs_type_real } };

/* 255: real (device) */
// @retail 0x2a4780
void __stdcall function_2a4780(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		real result = device_get_position(arguments[0]);
		function_209ae0(thread_index, *(long *)&result);
	}
}

hs_function_definition const g_44c47c = { _hs_type_real, 0, function_2a4780, NULL, 1, { _hs_type_device } };

/* retail function not identified */
inline real device_get_power(long device_index)
{
	real result = 0.0f;
	if (device_index != NONE)
		result = ((s_device *)object_get(device_index))->power;
	return result;
}

bool function_107140(long device_index, real value);
/* 256: boolean (device, real) */
// @retail 0x2a47f0
void __stdcall function_2a47f0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = function_107140(arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44c490 = { _hs_type_boolean, 0, function_2a47f0, NULL, 2, { _hs_type_device, _hs_type_real } };

/* 257: real (device) */
// @retail 0x2a4850
void __stdcall function_2a4850(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		real result = device_get_power(arguments[0]);
		function_209ae0(thread_index, *(long *)&result);
	}
}

hs_function_definition const g_44c4a4 = { _hs_type_real, 0, function_2a4850, NULL, 1, { _hs_type_device } };

/* retail function not identified */
inline real device_group_get(long device_group_index)
{
	return ((s_device_group *)g_4e0328.groups->data)[device_group_index & 0xffff].value;
}

void function_107370(long device_index, real value);
/* 258: void (device, real) */
// @retail 0x2a48c0
void __stdcall function_2a48c0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_107370(arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c4b8 = { _hs_type_void, 0, function_2a48c0, NULL, 2, { _hs_type_device, _hs_type_real } };

/* 259: real (device_group) */
// @retail 0x2a4910
void __stdcall function_2a4910(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		real result = device_group_get(arguments[0]);
		function_209ae0(thread_index, *(long *)&result);
	}
}

hs_function_definition const g_44c4cc = { _hs_type_real, 0, function_2a4910, NULL, 1, { _hs_type_device_group } };

/* 260: boolean (device, device_group, real) */
// @retail 0x2a4970
void __stdcall function_2a4970(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = function_1071e0(arguments[1], *(real *)&arguments[2]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44c4e0 = { _hs_type_boolean, 0, function_2a4970, NULL, 3, { _hs_type_device, _hs_type_device_group, _hs_type_real } };

void function_107430(long group_index, real value);
/* 261: void (device_group, real) */
// @retail 0x2a49d0
void __stdcall function_2a49d0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_107430(arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c4f8 = { _hs_type_void, 0, function_2a49d0, NULL, 2, { _hs_type_device_group, _hs_type_real } };

/* 262: void (device, boolean) */
// @retail 0x2a4a20
void __stdcall function_2a4a20(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		s_device *device = (s_device *)function_badc0(arguments[0], 0x80);
		if (device)
			SET_FLAG(device->device_flags, 2, *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c50c = { _hs_type_void, 0, function_2a4a20, NULL, 2, { _hs_type_device, _hs_type_boolean } };

/* 263: void (device, boolean) */
// @retail 0x2a4aa0
void __stdcall function_2a4aa0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_107590(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c520 = { _hs_type_void, 0, function_2a4aa0, NULL, 2, { _hs_type_device, _hs_type_boolean } };

/* 264: void (device, boolean) */
// @retail 0x2a4af0
void __stdcall function_2a4af0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_1075e0(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c534 = { _hs_type_void, 0, function_2a4af0, NULL, 2, { _hs_type_device, _hs_type_boolean } };

void function_107630(long group_index, bool flag);
/* 265: void (device_group, boolean) */
// @retail 0x2a4b40
void __stdcall function_2a4b40(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_107630(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c548 = { _hs_type_void, 0, function_2a4b40, NULL, 2, { _hs_type_device_group, _hs_type_boolean } };

/* 266: boolean (device, string_id, real) */
// @retail 0x2a4b90
void __stdcall function_2a4b90(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = function_107ed0(arguments[0], arguments[1], *(real *)&arguments[2]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44c55c = { _hs_type_boolean, 0, function_2a4b90, NULL, 3, { _hs_type_device, _hs_type_string_id, _hs_type_real } };

/* 278: void (boolean) */
// @retail 0x2a4d00
void __stdcall function_2a4d00(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		g_4f55d0->enabled = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c64c = { _hs_type_void, 0, function_2a4d00, NULL, 1, { _hs_type_boolean } };

/* 279: boolean () */
// @retail 0x2a4d40
void __stdcall function_2a4d40(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = g_4f55d0->enabled;
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44c660 = { _hs_type_boolean, 0, function_2a4d40, NULL, 0 };

/* 280: void (boolean) */
// @retail 0x2a4d70
void __stdcall function_2a4d70(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		g_4f55d0->unknown340 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c670 = { _hs_type_void, 0, function_2a4d70, NULL, 1, { _hs_type_boolean } };

/* 281: void (boolean) */
// @retail 0x2a4dc0
void __stdcall function_2a4dc0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		g_4f55d0->unknown20 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c684 = { _hs_type_void, 0, function_2a4dc0, NULL, 1, { _hs_type_boolean } };

bool game_allegiance_remove(short team_a, short team_b);

/* the checks around game_allegiance_remove (0x1df770); retail function not identified */
inline void allegiance_remove(short team_a, short team_b)
{
	if (team_a != NONE && team_b != NONE)
		game_allegiance_remove(team_a, team_b);
}

/* the unit of the first actor an ai index names; retail function not identified */
inline long ai_get_unit(long ai_index)
{
	long unit_index = NONE;
	if (ai_index != NONE)
	{
		s_ai_actor_iterator iterator;
		ai_actor_iterator_new(ai_index, &iterator);
		s_actor_datum *actor = ai_actor_iterator_next(&iterator);
		if (actor)
			unit_index = actor->unit_index;
	}
	return unit_index;
}

/* 284: object (ai); 285: unit (ai) */
// @retail 0x2a4e10
void __stdcall function_2a4e10(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
		function_209ae0(thread_index, ai_get_unit(arguments[0]));
}

hs_function_definition const g_44c6bc = { _hs_type_object, 0, function_2a4e10, NULL, 1, { _hs_type_ai } };
hs_function_definition const g_44c6d0 = { _hs_type_unit, 0, function_2a4e10, NULL, 1, { _hs_type_ai } };
/* 288: void (unit) */
// @retail 0x2a4f20
void __stdcall function_2a4f20(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long unit_index = arguments[0];
		if (unit_index != NONE)
		{
			long index = ((s_unit *)object_get(unit_index))->index_12c;
			if (index != NONE)
				function_1e1a00(index, 0);
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c70c = { _hs_type_void, 0, function_2a4f20, NULL, 1, { _hs_type_unit } };

/* 289: void (object_list) */
// @retail 0x2a4f90
void __stdcall function_2a4f90(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_273150(arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c720 = { _hs_type_void, 0, function_2a4f90, NULL, 1, { _hs_type_object_list } };

/* 293: void (ai, boolean) */
// @retail 0x2a50d0
void __stdcall function_2a50d0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_273200(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c770 = { _hs_type_void, 0, function_2a50d0, NULL, 2, { _hs_type_ai, _hs_type_boolean } };

/* 294: boolean (ai) */
// @retail 0x2a5120
void __stdcall function_2a5120(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = function_2732e0(arguments[0]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44c784 = { _hs_type_boolean, 0, function_2a5120, NULL, 1, { _hs_type_ai } };

/* 297: void (ai) */
// @retail 0x2a5210
void __stdcall function_2a5210(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_2736c0(arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c7c0 = { _hs_type_void, 0, function_2a5210, NULL, 1, { _hs_type_ai } };

/* 299: void (ai, boolean) */
// @retail 0x2a5270
void __stdcall function_2a5270(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_2738a0(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c7e4 = { _hs_type_void, 0, function_2a5270, NULL, 2, { _hs_type_ai, _hs_type_boolean } };

/* 302: void (ai, boolean) */
// @retail 0x2a52c0
void __stdcall function_2a52c0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_273900(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c81c = { _hs_type_void, 0, function_2a52c0, NULL, 2, { _hs_type_ai, _hs_type_boolean } };

/* 303: void (ai, boolean) */
// @retail 0x2a5310
void __stdcall function_2a5310(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_2739d0(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c830 = { _hs_type_void, 0, function_2a5310, NULL, 2, { _hs_type_ai, _hs_type_boolean } };

/* 304: void (ai, ai) */
// @retail 0x2a5360
void __stdcall function_2a5360(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_273ac0(arguments[0], arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c844 = { _hs_type_void, 0, function_2a5360, NULL, 2, { _hs_type_ai, _hs_type_ai } };

/* 305: void (ai, object) */
// @retail 0x2a53b0
void __stdcall function_2a53b0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_273d30(arguments[0], arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c858 = { _hs_type_void, 0, function_2a53b0, NULL, 2, { _hs_type_ai, _hs_type_object } };

/* 307: void (ai, boolean) */
// @retail 0x2a5450
void __stdcall function_2a5450(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_273ef0(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c880 = { _hs_type_void, 0, function_2a5450, NULL, 2, { _hs_type_ai, _hs_type_boolean } };

/* 309: void (team, team) */
// @retail 0x2a54f0
void __stdcall function_2a54f0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_2742f0(*(short *)&arguments[0], *(short *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c8a8 = { _hs_type_void, 0, function_2a54f0, NULL, 2, { _hs_type_team, _hs_type_team } };

/* 310: void (team, team) */
// @retail 0x2a5540
void __stdcall function_2a5540(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		allegiance_remove(*(short *)&arguments[0], *(short *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c8bc = { _hs_type_void, 0, function_2a5540, NULL, 2, { _hs_type_team, _hs_type_team } };

/* 313: void (object_list, boolean) */
// @retail 0x2a5640
void __stdcall function_2a5640(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_275160(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c8f8 = { _hs_type_void, 0, function_2a5640, NULL, 2, { _hs_type_object_list, _hs_type_boolean } };

/* 314: void (object_list, boolean) */
// @retail 0x2a5690
void __stdcall function_2a5690(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_275270(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c90c = { _hs_type_void, 0, function_2a5690, NULL, 2, { _hs_type_object_list, _hs_type_boolean } };

/* 317: void (ai, boolean) */
// @retail 0x2a5760
void __stdcall function_2a5760(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_275a50(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c948 = { _hs_type_void, 0, function_2a5760, NULL, 2, { _hs_type_ai, _hs_type_boolean } };

/* 318: void (unit, boolean) */
// @retail 0x2a57b0
void __stdcall function_2a57b0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_275ad0(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c95c = { _hs_type_void, 0, function_2a57b0, NULL, 2, { _hs_type_unit, _hs_type_boolean } };

/* 321: void (ai, boolean) */
// @retail 0x2a5800
void __stdcall function_2a5800(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_275b20(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44c994 = { _hs_type_void, 0, function_2a5800, NULL, 2, { _hs_type_ai, _hs_type_boolean } };

/* 324: boolean (ai) */
// @retail 0x2a58a0
void __stdcall function_2a58a0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = false;
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44c9d0 = { _hs_type_boolean, 0, function_2a58a0, NULL, 1, { _hs_type_ai } };

/* 326: short_integer (ai) */
// @retail 0x2a5940
void __stdcall function_2a5940(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(short *)&result = (short)function_273f30(arguments[0], 0, NULL, NULL);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44c9f8 = { _hs_type_short_integer, 0, function_2a5940, NULL, 1, { _hs_type_ai } };

/* 327: real (ai) */
// @retail 0x2a59a0
void __stdcall function_2a59a0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long actor_count;
		real result = 0.0f;
		long count = function_273f30(arguments[0], 0, &actor_count, NULL);
		if (actor_count > 0)
			result = (real)count / (real)actor_count;
		hs_return_real(thread_index, result);
	}
}

hs_function_definition const g_44ca0c = { _hs_type_real, 0, function_2a59a0, NULL, 1, { _hs_type_ai } };

/* 328: real (ai) */
// @retail 0x2a5a10
void __stdcall function_2a5a10(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		real vitality = 0.0f;
		function_273f30(arguments[0], 0, NULL, &vitality);
		hs_return_real(thread_index, vitality);
	}
}

hs_function_definition const g_44ca20 = { _hs_type_real, 0, function_2a5a10, NULL, 1, { _hs_type_ai } };

/* 329: short_integer (ai) */
// @retail 0x2a5a70
void __stdcall function_2a5a70(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(short *)&result = (short)function_273f30(arguments[0], 1, NULL, NULL);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44ca34 = { _hs_type_short_integer, 0, function_2a5a70, NULL, 1, { _hs_type_ai } };

/* 330: short_integer (ai) */
// @retail 0x2a5ad0
void __stdcall function_2a5ad0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(short *)&result = (short)function_273f30(arguments[0], 2, NULL, NULL);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44ca48 = { _hs_type_short_integer, 0, function_2a5ad0, NULL, 1, { _hs_type_ai } };
bool game_team_is_enemy(short team_a, short team_b);
bool game_team_is_ally(short team_a, short team_b);

/* game_team_is_ally (0x1df5d0) and game_team_is_enemy (0x1df560) together; retail function not identified */
inline bool allegiance_broken(short team_a, short team_b)
{
	bool result = false;
	if (team_a != NONE && team_b != NONE)
		result = game_team_is_ally(team_a, team_b) && game_team_is_enemy(team_a, team_b);
	return result;
}

/* 331: object_list (ai) */
// @retail 0x2a5b30
void __stdcall function_2a5b30(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_209ae0(thread_index, function_272ea0(arguments[0]));
	}
}

hs_function_definition const g_44ca5c = { _hs_type_object_list, 0, function_2a5b30, NULL, 1, { _hs_type_ai } };

/* 332: boolean (team, team) */
// @retail 0x2a5b70
void __stdcall function_2a5b70(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = allegiance_broken(*(short *)&arguments[0], *(short *)&arguments[1]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44ca70 = { _hs_type_boolean, 0, function_2a5b70, NULL, 2, { _hs_type_team, _hs_type_team } };

/* 334: short_integer (ai) */
// @retail 0x2a5c60
void __stdcall function_2a5c60(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(short *)&result = function_275d70(arguments[0]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44ca98 = { _hs_type_short_integer, 0, function_2a5c60, NULL, 1, { _hs_type_ai } };

/* 335: ai (object) */
// @retail 0x2a5cb0
void __stdcall function_2a5cb0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_209ae0(thread_index, function_272ff0(arguments[0]));
	}
}

hs_function_definition const g_44caac = { _hs_type_ai, 0, function_2a5cb0, NULL, 1, { _hs_type_object } };

/* 337: boolean (); 722: boolean () */
// @retail 0x2a5d50
void __stdcall function_2a5d50(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = false;
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44cad4 = { _hs_type_boolean, 0, function_2a5d50, NULL, 0 };
hs_function_definition const g_44e874 = { _hs_type_boolean, 0, function_2a5d50, NULL, 0 };

long g_502410;

/* the command script setters below stand in for the script commands' own
   setters, which retail inlines into each evaluator. Which retail function
   each call mirrors is not identified, except that
   command_script_set_point(1, index, 0.0f) is 0x276990. */

inline void command_script_set_point(short type, long index, real value)
{
	long script_index = g_502410;
	if (script_index != NONE)
	{
		s_command_script *script = command_script_get(script_index);
		script->type = type;
		script->index_a = index;
		script->value8 = value;
	}
}

inline void command_script_set_points(short type, long index_a, long index_b, real value)
{
	long script_index = g_502410;
	if (script_index != NONE)
	{
		s_command_script *script = command_script_get(script_index);
		script->type = type;
		script->index_a = index_a;
		script->index_b = index_b;
		script->value8 = value;
	}
}

inline void command_script_set_index_pair(short type, long index_a, long index_b)
{
	long script_index = g_502410;
	if (script_index != NONE)
	{
		s_command_script *script = command_script_get(script_index);
		script->type = type;
		script->index_a = index_a;
		script->index_b = index_b;
	}
}

inline void command_script_set_index(short type, long index)
{
	long script_index = g_502410;
	if (script_index != NONE)
	{
		s_command_script *script = command_script_get(script_index);
		script->type = type;
		script->index_a = index;
	}
}

inline void command_script_set_target(short type, bool enable, long index)
{
	long script_index = g_502410;
	if (script_index != NONE)
	{
		s_command_script *script = command_script_get(script_index);
		script->flag46 = enable;
		if (enable)
		{
			script->type48 = type;
			script->index4c = index;
		}
	}
}

inline void command_script_set_value_68(real value)
{
	long script_index = g_502410;
	if (script_index != NONE)
	{
		s_command_script *script = command_script_get(script_index);
		if (value > 0.0f)
		{
			script->flag64 = true;
			script->value68 = value;
		}
		else
		{
			script->flag64 = false;
		}
	}
}

inline void command_script_set_index_short(short type, long index, short value)
{
	long script_index = g_502410;
	if (script_index != NONE)
	{
		s_command_script *script = command_script_get(script_index);
		script->type = type;
		script->index_a = index;
		script->value_short = value;
	}
}

inline void command_script_set_reals(short type, real value8, real valuec)
{
	long script_index = g_502410;
	if (script_index != NONE)
	{
		s_command_script *script = command_script_get(script_index);
		script->type = type;
		script->value8 = value8;
		script->valuec = valuec;
	}
}

inline void command_script_set_short(short type, short value)
{
	long script_index = g_502410;
	if (script_index != NONE)
	{
		s_command_script *script = command_script_get(script_index);
		script->type = type;
		script->value_short = value;
	}
}

inline void command_script_set_value_60(bool enable, real value)
{
	long script_index = g_502410;
	if (script_index != NONE)
	{
		s_command_script *script = command_script_get(script_index);
		script->flag5c = enable;
		if (value > 0.0001f)
			script->value60 = 1.0f / value;
	}
}

inline void command_script_set_value_70(bool enable, real value)
{
	long script_index = g_502410;
	if (script_index != NONE)
	{
		s_command_script *script = command_script_get(script_index);
		script->flag6c = enable;
		if (enable)
			script->value70 = value;
	}
}

inline void command_script_set_flag_7f(bool value)
{
	long script_index = g_502410;
	if (script_index != NONE)
	{
		s_command_script *script = command_script_get(script_index);
		script->flag7f = value;
		if (value)
			script->flag80 = value;
	}
}

inline void command_script_set_style(long style)
{
	long script_index = g_502410;
	if (script_index != NONE)
	{
		s_command_script *script = command_script_get(script_index);
		script->style88 = style;
		script->flag86 = true;
	}
}

inline void command_script_set_value_84(short value)
{
	long script_index = g_502410;
	if (script_index != NONE && value >= 0 && value < 5)
		command_script_get(script_index)->value84 = value;
}

inline long ai_index_get_starting_location_vehicle(long ai_index)
{
	long vehicle_index = NONE;
	if ((ai_index & 0xc0000000) == 0xc0000000)
	{
		vehicle_index = function_272c90(ai_index);
	}
	return vehicle_index;
}

/* 339: vehicle (ai) */
// @retail 0x2a5d70
void __stdcall function_2a5d70(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
		function_209ae0(thread_index, function_275e20(arguments[0]));
}

hs_function_definition const g_44caf4 = { _hs_type_vehicle, 0, function_2a5d70, NULL, 1, { _hs_type_ai } };

/* 340: vehicle (ai) */
// @retail 0x2a5db0
void __stdcall function_2a5db0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_209ae0(thread_index, ai_index_get_starting_location_vehicle(arguments[0]));
	}
}

hs_function_definition const g_44cb08 = { _hs_type_vehicle, 0, function_2a5db0, NULL, 1, { _hs_type_ai } };

/* 341: boolean (vehicle, string_id, boolean) */
// @retail 0x2a5e00
void __stdcall function_2a5e00(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = function_275eb0(arguments[0], arguments[1], *(bool *)&arguments[2]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44cb1c = { _hs_type_boolean, 0, function_2a5e00, NULL, 3, { _hs_type_vehicle, _hs_type_string_id, _hs_type_boolean } };

/* 342: boolean (vehicle, boolean) */
// @retail 0x2a5e60
void __stdcall function_2a5e60(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = function_275fc0(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44cb34 = { _hs_type_boolean, 0, function_2a5e60, NULL, 2, { _hs_type_vehicle, _hs_type_boolean } };

/* 343: void (ai, unit, unit_seat_mapping) */
// @retail 0x2a5ec0
void __stdcall function_2a5ec0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_274a50(arguments[0], arguments[1], arguments[2], false);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44cb48 = { _hs_type_void, 0, function_2a5ec0, NULL, 3, { _hs_type_ai, _hs_type_unit, _hs_type_unit_seat_mapping } };

/* 344: void (ai, unit) */
// @retail 0x2a5f10
void __stdcall function_2a5f10(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_274a50(arguments[0], arguments[1], NONE, false);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44cb60 = { _hs_type_void, 0, function_2a5f10, NULL, 2, { _hs_type_ai, _hs_type_unit } };

/* 345: void (ai, unit, unit_seat_mapping) */
// @retail 0x2a5f60
void __stdcall function_2a5f60(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_274a50(arguments[0], arguments[1], arguments[2], true);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44cb74 = { _hs_type_void, 0, function_2a5f60, NULL, 3, { _hs_type_ai, _hs_type_unit, _hs_type_unit_seat_mapping } };

/* 346: void (ai, unit) */
// @retail 0x2a5fb0
void __stdcall function_2a5fb0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_274a50(arguments[0], arguments[1], NONE, true);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44cb8c = { _hs_type_void, 0, function_2a5fb0, NULL, 2, { _hs_type_ai, _hs_type_unit } };

/* 347: short_integer (ai) */
// @retail 0x2a6000
void __stdcall function_2a6000(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(short *)&result = function_274470(arguments[0]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44cba0 = { _hs_type_short_integer, 0, function_2a6000, NULL, 1, { _hs_type_ai } };

/* 348: void (ai, unit_seat_mapping) */
// @retail 0x2a6050
void __stdcall function_2a6050(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_274da0(arguments[0], arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44cbb4 = { _hs_type_void, 0, function_2a6050, NULL, 2, { _hs_type_ai, _hs_type_unit_seat_mapping } };

/* 349: void (ai) */
// @retail 0x2a60a0
void __stdcall function_2a60a0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_274da0(arguments[0], NONE);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44cbc8 = { _hs_type_void, 0, function_2a60a0, NULL, 1, { _hs_type_ai } };

bool function_f5dc0(long object_index);

/* 350: boolean (vehicle) */
// @retail 0x2a60f0
void __stdcall function_2a60f0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long vehicle_index = arguments[0];
		bool value = false;

		if (vehicle_index != NONE)
			value = function_f5dc0(vehicle_index);
		*(bool *)&result = value;
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44cbdc = { _hs_type_boolean, 0, function_2a60f0, NULL, 1, { _hs_type_vehicle } };

/* 351: void (vehicle) */
// @retail 0x2a6150
void __stdcall function_2a6150(short function_index, long thread_index, bool initialize)
{
	s_unit_request request;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long unit_index = arguments[0];

		request.type = 0x21;
		request.type1c.object_index = NONE;
		function_e6900(unit_index, &request);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44cbf0 = { _hs_type_void, 0, function_2a6150, NULL, 1, { _hs_type_vehicle } };

/* 352: short_integer (ai) */
// @retail 0x2a61b0
void __stdcall function_2a61b0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(short *)&result = function_276050(arguments[0]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44cc04 = { _hs_type_short_integer, 0, function_2a61b0, NULL, 1, { _hs_type_ai } };

/* 353: boolean (string_id) */
// @retail 0x2a6200
void __stdcall function_2a6200(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		bool found = false;
		long index = function_2958a0(arguments[0]);
		if (index != NONE)
		{
			found = true;
			((s_51ecb4_datum *)g_51ecb4->data)[index & 0xffff].flag_e = true;
		}
		*(bool *)&result = found;
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44cc18 = { _hs_type_boolean, 0, function_2a6200, NULL, 1, { _hs_type_string_id } };

/* 354: boolean (string_id) */
// @retail 0x2a6270
void __stdcall function_2a6270(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		bool found = false;
		long index = function_2958a0(arguments[0]);
		if (index != NONE)
		{
			((s_51ecb4_datum *)g_51ecb4->data)[index & 0xffff].flag_e = false;
			found = true;
		}
		*(bool *)&result = found;
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44cc2c = { _hs_type_boolean, 0, function_2a6270, NULL, 1, { _hs_type_string_id } };

/* 358: boolean (ai) */
// @retail 0x2a6390
void __stdcall function_2a6390(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = function_276380(arguments[0]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44cc78 = { _hs_type_boolean, 0, function_2a6390, NULL, 1, { _hs_type_ai } };

bool function_292080(long object_index, long vocalization_name, real *duration);

short function_276160(long ai_index, long vocalization_name);

/* 359: real (ai, string_id) */
// @retail 0x2a63e0
void __stdcall function_2a63e0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		real value = (real)function_276160(arguments[0], arguments[1]);
		function_209ae0(thread_index, *(long *)&value);
	}
}

hs_function_definition const g_44cc8c = { _hs_type_real, 0, function_2a63e0, NULL, 2, { _hs_type_ai, _hs_type_string_id } };

short function_2761d0(long ai_index, long vocalization_name);

/* 360: real (ai, string_id) */
// @retail 0x2a6440
void __stdcall function_2a6440(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		real value = (real)function_2761d0(arguments[0], arguments[1]);
		function_209ae0(thread_index, *(long *)&value);
	}
}

hs_function_definition const g_44cca0 = { _hs_type_real, 0, function_2a6440, NULL, 2, { _hs_type_ai, _hs_type_string_id } };

/* 361: real (object, string_id) */
// @retail 0x2a64a0
void __stdcall function_2a64a0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		real duration = 0.0f;
		long name = arguments[1];
		if (name != NONE)
			function_292080(arguments[0], name, &duration);
		duration *= 30.0f;
		long ticks;
		__asm
		{
			fld duration
			fistp ticks
		}
		duration = (real)(short)ticks;
		function_209ae0(thread_index, *(long *)&duration);
	}
}

hs_function_definition const g_44ccb4 = { _hs_type_real, 0, function_2a64a0, NULL, 2, { _hs_type_object, _hs_type_string_id } };

/* 365: void (ai, ai_command_script) */
// @retail 0x2a6680
void __stdcall function_2a6680(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_2764c0(arguments[0], *(short *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44cd14 = { _hs_type_void, 0, function_2a6680, NULL, 2, { _hs_type_ai, _hs_type_ai_command_script } };

/* 366: void (ai, ai_command_script) */
// @retail 0x2a66d0
void __stdcall function_2a66d0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_276440(arguments[0], *(short *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44cd28 = { _hs_type_void, 0, function_2a66d0, NULL, 2, { _hs_type_ai, _hs_type_ai_command_script } };

/* 367: void (ai, ai_command_script) */
// @retail 0x2a6720
void __stdcall function_2a6720(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_276480(arguments[0], *(short *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44cd3c = { _hs_type_void, 0, function_2a6720, NULL, 2, { _hs_type_ai, _hs_type_ai_command_script } };

/* 368: void (ai_command_script, ai, ai) */
// @retail 0x2a6770
void __stdcall function_2a6770(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_276500(*(short *)&arguments[0], arguments[1], arguments[2]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44cd50 = { _hs_type_void, 0, function_2a6770, NULL, 3, { _hs_type_ai_command_script, _hs_type_ai, _hs_type_ai } };

/* 369: void (ai_command_script, ai, ai, ai) */
// @retail 0x2a67c0
void __stdcall function_2a67c0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_276560(*(short *)&arguments[0], arguments[1], arguments[2], arguments[3]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44cd68 = { _hs_type_void, 0, function_2a67c0, NULL, 4, { _hs_type_ai_command_script, _hs_type_ai, _hs_type_ai, _hs_type_ai } };

/* 370: boolean (ai, ai_command_script) */
// @retail 0x2a6810
void __stdcall function_2a6810(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = function_2766f0(arguments[0], *(short *)&arguments[1]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44cd80 = { _hs_type_boolean, 0, function_2a6810, NULL, 2, { _hs_type_ai, _hs_type_ai_command_script } };

/* 371: boolean (ai, ai_command_script) */
// @retail 0x2a6870
void __stdcall function_2a6870(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = function_276770(arguments[0], *(short *)&arguments[1]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44cd94 = { _hs_type_boolean, 0, function_2a6870, NULL, 2, { _hs_type_ai, _hs_type_ai_command_script } };

/* 372: short_integer (ai) */
// @retail 0x2a68d0
void __stdcall function_2a68d0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(short *)&result = function_2767f0(arguments[0]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44cda8 = { _hs_type_short_integer, 0, function_2a68d0, NULL, 1, { _hs_type_ai } };

bool function_258340(short participant_index, long joint_index);

/* 373: void (string_id) */
// @retail 0x2a6920
void __stdcall function_2a6920(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_2768d0(arguments[0]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44cdbc = { _hs_type_void, 1, function_2a6920, NULL, 1, { _hs_type_string_id } };

/* 374: void (short_integer) */
// @retail 0x2a6980
void __stdcall function_2a6980(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		short participant_index = *(short *)&arguments[0];

		if (g_502410 != NONE)
		{
			s_command_script *script = command_script_get(g_502410);

			script->type = 0x16;
			if (script->joint_index != NONE)
				function_258340(participant_index, script->joint_index);
		}
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44cdd0 = { _hs_type_void, 1, function_2a6980, NULL, 1, { _hs_type_short_integer } };

/* 375: void (ai) */
// @retail 0x2a6a10
void __stdcall function_2a6a10(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_276860(arguments[0], 0);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44cde4 = { _hs_type_void, 1, function_2a6a10, NULL, 1, { _hs_type_ai } };

/* 376: void (ai) */
// @retail 0x2a6a70
void __stdcall function_2a6a70(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_276860(arguments[0], 1);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44cdf8 = { _hs_type_void, 1, function_2a6a70, NULL, 1, { _hs_type_ai } };

/* 377: void (ai) */
// @retail 0x2a6ad0
void __stdcall function_2a6ad0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_276860(arguments[0], 2);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44ce0c = { _hs_type_void, 1, function_2a6ad0, NULL, 1, { _hs_type_ai } };

/* 378: void (point_reference) */
// @retail 0x2a6b30
void __stdcall function_2a6b30(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_point(0xf, arguments[0], 0.0f);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44ce20 = { _hs_type_void, 0, function_2a6b30, NULL, 1, { _hs_type_point_reference } };

/* 379: void (point_reference, real) */
// @retail 0x2a6bc0
void __stdcall function_2a6bc0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_point(0xf, arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44ce34 = { _hs_type_void, 0, function_2a6bc0, NULL, 2, { _hs_type_point_reference, _hs_type_real } };

/* 380: void (point_reference, point_reference) */
// @retail 0x2a6c50
void __stdcall function_2a6c50(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_points(0x11, arguments[0], arguments[1], 0.0f);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44ce48 = { _hs_type_void, 0, function_2a6c50, NULL, 2, { _hs_type_point_reference, _hs_type_point_reference } };

/* 381: void (point_reference, point_reference, real) */
// @retail 0x2a6ce0
void __stdcall function_2a6ce0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_points(0x11, arguments[0], arguments[1], *(real *)&arguments[2]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44ce5c = { _hs_type_void, 0, function_2a6ce0, NULL, 3, { _hs_type_point_reference, _hs_type_point_reference, _hs_type_real } };

/* 382: void (point_reference) */
// @retail 0x2a6d70
void __stdcall function_2a6d70(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_point(0x10, arguments[0], 0.0f);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44ce74 = { _hs_type_void, 0, function_2a6d70, NULL, 1, { _hs_type_point_reference } };

/* 383: void (point_reference, real) */
// @retail 0x2a6e00
void __stdcall function_2a6e00(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_point(0x10, arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44ce88 = { _hs_type_void, 0, function_2a6e00, NULL, 2, { _hs_type_point_reference, _hs_type_real } };

/* 384: void (point_reference) */
// @retail 0x2a6e90
void __stdcall function_2a6e90(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_point(1, arguments[0], 0.0f);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44ce9c = { _hs_type_void, 0, function_2a6e90, NULL, 1, { _hs_type_point_reference } };

/* 385: void (point_reference, real) */
// @retail 0x2a6f20
void __stdcall function_2a6f20(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_point(1, arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44ceb0 = { _hs_type_void, 0, function_2a6f20, NULL, 2, { _hs_type_point_reference, _hs_type_real } };

/* 386: void (point_reference, point_reference) */
// @retail 0x2a6fb0
void __stdcall function_2a6fb0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_points(2, arguments[0], arguments[1], 0.0f);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44cec4 = { _hs_type_void, 0, function_2a6fb0, NULL, 2, { _hs_type_point_reference, _hs_type_point_reference } };

/* 387: void (point_reference, point_reference, real) */
// @retail 0x2a7040
void __stdcall function_2a7040(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_points(2, arguments[0], arguments[1], *(real *)&arguments[2]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44ced8 = { _hs_type_void, 0, function_2a7040, NULL, 3, { _hs_type_point_reference, _hs_type_point_reference, _hs_type_real } };

/* 388: void (point_reference, point_reference) */
// @retail 0x2a70d0
void __stdcall function_2a70d0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_index_pair(3, arguments[0], arguments[1]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44cef0 = { _hs_type_void, 0, function_2a70d0, NULL, 2, { _hs_type_point_reference, _hs_type_point_reference } };

/* 389: void (point_reference) */
// @retail 0x2a7160
void __stdcall function_2a7160(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_index(0x13, arguments[0]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44cf04 = { _hs_type_void, 0, function_2a7160, NULL, 1, { _hs_type_point_reference } };

void function_2769d0(long point_reference);

/* 390: void (point_reference) */
// @retail 0x2a71e0
void __stdcall function_2a71e0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_2769d0(arguments[0]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44cf18 = { _hs_type_void, 1, function_2a71e0, NULL, 1, { _hs_type_point_reference } };

/* 391: boolean () */
// @retail 0x2a7240
void __stdcall function_2a7240(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	long clump_index = g_50240c;
	bool active = false;
	if (clump_index != NONE)
	{
		s_clump_view *clump = &((s_clump_view *)g_4f55f0->data)[clump_index & 0xffff];
		if (clump->active && clump->state == 1)
			active = true;
	}
	*(bool *)&result = active;
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44cf2c = { _hs_type_boolean, 0, function_2a7240, NULL, 0 };

/* 392: void (boolean, point_reference) */
// @retail 0x2a72a0
void __stdcall function_2a72a0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_target(2, *(bool *)&arguments[0], arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44cf3c = { _hs_type_void, 0, function_2a72a0, NULL, 2, { _hs_type_boolean, _hs_type_point_reference } };

/* 393: void (boolean) */
// @retail 0x2a7310
void __stdcall function_2a7310(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_2770c0(*(bool *)&arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44cf50 = { _hs_type_void, 0, function_2a7310, NULL, 1, { _hs_type_boolean } };

/* 394: void (boolean, object) */
// @retail 0x2a7350
void __stdcall function_2a7350(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_target(1, *(bool *)&arguments[0], arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44cf64 = { _hs_type_void, 0, function_2a7350, NULL, 2, { _hs_type_boolean, _hs_type_object } };

/* retail function not identified */
inline void command_script_look_at_point(bool enable, long point_index)
{
	long script_index = g_502410;
	if (script_index != NONE)
	{
		s_command_script *script = command_script_get(script_index);
		function_276f80(enable, point_index);
		if (enable)
		{
			script->flag50 = true;
			script->flag51 = true;
		}
	}
}

/* retail function not identified */
inline void command_script_look_at_object(bool enable, long object_index)
{
	long script_index = g_502410;
	if (script_index != NONE)
	{
		s_command_script *script = command_script_get(script_index);
		function_277060(enable, object_index);
		if (enable)
		{
			script->flag50 = true;
			script->flag51 = true;
		}
	}
}

/* 395: void (boolean, point_reference) */
// @retail 0x2a73c0
void __stdcall function_2a73c0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_276f80(*(bool *)&arguments[0], arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44cf78 = { _hs_type_void, 0, function_2a73c0, NULL, 2, { _hs_type_boolean, _hs_type_point_reference } };

/* 396: void (boolean) */
// @retail 0x2a7410
void __stdcall function_2a7410(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_276fd0(*(bool *)&arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44cf8c = { _hs_type_void, 0, function_2a7410, NULL, 1, { _hs_type_boolean } };

/* 397: void (boolean, object) */
// @retail 0x2a7450
void __stdcall function_2a7450(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_277060(*(bool *)&arguments[0], arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44cfa0 = { _hs_type_void, 0, function_2a7450, NULL, 2, { _hs_type_boolean, _hs_type_object } };

/* 398: void (boolean, point_reference) */
// @retail 0x2a74a0
void __stdcall function_2a74a0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_look_at_point(*(bool *)&arguments[0], arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44cfb4 = { _hs_type_void, 0, function_2a74a0, NULL, 2, { _hs_type_boolean, _hs_type_point_reference } };

/* 399: void (boolean) */
// @retail 0x2a7520
void __stdcall function_2a7520(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		bool enable = *(bool *)&arguments[0];
		long script_index = g_502410;
		if (script_index != NONE)
		{
			s_command_script *script = command_script_get(script_index);
			function_276fd0(enable);
			if (enable)
			{
				script->flag50 = true;
				script->flag51 = true;
			}
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44cfc8 = { _hs_type_void, 0, function_2a7520, NULL, 1, { _hs_type_boolean } };

/* 400: void (boolean, object) */
// @retail 0x2a75a0
void __stdcall function_2a75a0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_look_at_object(*(bool *)&arguments[0], arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44cfdc = { _hs_type_void, 0, function_2a75a0, NULL, 2, { _hs_type_boolean, _hs_type_object } };

/* 401: void (real, real, real) */
// @retail 0x2a7620
void __stdcall function_2a7620(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_277120(*(real *)&arguments[0], *(real *)&arguments[1], *(real *)&arguments[2]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44cff0 = { _hs_type_void, 1, function_2a7620, NULL, 3, { _hs_type_real, _hs_type_real, _hs_type_real } };

/* 402: void (real) */
// @retail 0x2a7690
void __stdcall function_2a7690(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_277250(*(real *)&arguments[0]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44d008 = { _hs_type_void, 1, function_2a7690, NULL, 1, { _hs_type_real } };

/* 403: void (boolean) */
// @retail 0x2a76f0
void __stdcall function_2a76f0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long index = g_502410;
		if (index != NONE)
			command_script_get(index)->flag45 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d01c = { _hs_type_void, 0, function_2a76f0, NULL, 1, { _hs_type_boolean } };

/* 404: void (boolean, object) */
// @retail 0x2a7750
void __stdcall function_2a7750(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_2771d0(*(bool *)&arguments[0], arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d030 = { _hs_type_void, 0, function_2a7750, NULL, 2, { _hs_type_boolean, _hs_type_object } };

/* 405: void (boolean, point_reference) */
// @retail 0x2a7790
void __stdcall function_2a7790(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_277210(*(bool *)&arguments[0], arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d044 = { _hs_type_void, 0, function_2a7790, NULL, 2, { _hs_type_boolean, _hs_type_point_reference } };

/* 406: void (real) */
// @retail 0x2a77d0
void __stdcall function_2a77d0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_value_68(*(real *)&arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d058 = { _hs_type_void, 0, function_2a77d0, NULL, 1, { _hs_type_real } };

/* 407: void (point_reference, short) */
// @retail 0x2a7850
void __stdcall function_2a7850(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_index_short(5, arguments[0], *(short *)&arguments[1]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44d06c = { _hs_type_void, 0, function_2a7850, NULL, 2, { _hs_type_point_reference, _hs_type_short_integer } };

/* 408: void (real, real) */
// @retail 0x2a78e0
void __stdcall function_2a78e0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_reals(7, *(real *)&arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44d080 = { _hs_type_void, 0, function_2a78e0, NULL, 2, { _hs_type_real, _hs_type_real } };

/* 409: void (real, real) */
// @retail 0x2a7970
void __stdcall function_2a7970(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_reals(8, *(real *)&arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44d094 = { _hs_type_void, 0, function_2a7970, NULL, 2, { _hs_type_real, _hs_type_real } };

/* 410: void (short) */
// @retail 0x2a7a00
void __stdcall function_2a7a00(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_short(0xb, *(short *)&arguments[0]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44d0a8 = { _hs_type_void, 0, function_2a7a00, NULL, 1, { _hs_type_short_integer } };

/* 411: void (sound) */
// @retail 0x2a7a80
void __stdcall function_2a7a80(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long script_index = g_502410;
		if (script_index != NONE)
		{
			function_2760a0(g_50240c, script_index, 0, arguments[0], 1.0f, 1.0f);
		}
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44d0bc = { _hs_type_void, 1, function_2a7a80, NULL, 1, { _hs_type_sound } };

/* 412: void (sound, real) */
// @retail 0x2a7b00
void __stdcall function_2a7b00(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long script_index = g_502410;
		if (script_index != NONE)
		{
			function_2760a0(g_50240c, script_index, 0, arguments[0], *(real *)&arguments[1], 1.0f);
		}
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44d0d0 = { _hs_type_void, 1, function_2a7b00, NULL, 2, { _hs_type_sound, _hs_type_real } };

/* 413: void (sound, real, real) */
// @retail 0x2a7b80
void __stdcall function_2a7b80(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long script_index = g_502410;
		if (script_index != NONE)
		{
			function_2760a0(g_50240c, script_index, 0, arguments[0], *(real *)&arguments[1], *(real *)&arguments[2]);
		}
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44d0e4 = { _hs_type_void, 1, function_2a7b80, NULL, 3, { _hs_type_sound, _hs_type_real, _hs_type_real } };

/* 415: void (animation_graph, string_id, real, boolean) */
// @retail 0x2a7c60
void __stdcall function_2a7c60(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_2772b0(arguments[0], arguments[1], *(real *)&arguments[2], *(bool *)&arguments[3]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44d110 = { _hs_type_void, 1, function_2a7c60, NULL, 4, { _hs_type_animation_graph, _hs_type_string_id, _hs_type_real, _hs_type_boolean } };

/* 416: void () */
// @retail 0x2a7cd0
void __stdcall function_2a7cd0(short function_index, long thread_index, bool initialize)
{
	function_277380();
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44d128 = { _hs_type_void, 0, function_2a7cd0, NULL, 0 };

bool function_291ea0(long actor_index, long vocalization_name, long script_index, real *duration);

/* 417: void (string_id) */
// @retail 0x2a7cf0
void __stdcall function_2a7cf0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long script_index = g_502410;
		if (script_index != NONE && g_50240c != NONE)
		{
			function_291ea0(g_50240c, arguments[0], script_index, NULL);
		}
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44d138 = { _hs_type_void, 1, function_2a7cf0, NULL, 1, { _hs_type_string_id } };

/* 418: void (short) */
// @retail 0x2a7d70
void __stdcall function_2a7d70(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_short(0xd, *(short *)&arguments[0]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44d14c = { _hs_type_void, 0, function_2a7d70, NULL, 1, { _hs_type_short_integer } };

/* 421: void (short_integer) */
// @retail 0x2a7e40
void __stdcall function_2a7e40(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_2775d0(*(short *)&arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d188 = { _hs_type_void, 0, function_2a7e40, NULL, 1, { _hs_type_short_integer } };

/* 422: void (boolean) */
// @retail 0x2a7e80
void __stdcall function_2a7e80(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long index = g_502410;
		if (index != NONE)
			command_script_get(index)->flag5c = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d19c = { _hs_type_void, 0, function_2a7e80, NULL, 1, { _hs_type_boolean } };

/* 423: void (boolean, real) */
// @retail 0x2a7ee0
void __stdcall function_2a7ee0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_value_60(*(bool *)&arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d1b0 = { _hs_type_void, 0, function_2a7ee0, NULL, 2, { _hs_type_boolean, _hs_type_real } };

/* 420: void (long, short); 424: void (real) */
// @retail 0x2a7f60
void __stdcall function_2a7f60(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44d174 = { _hs_type_void, 0, function_2a7f60, NULL, 2, { _hs_type_long_integer, _hs_type_short_integer } };
hs_function_definition const g_44d1c4 = { _hs_type_void, 0, function_2a7f60, NULL, 1, { _hs_type_real } };

/* 425: void (vehicle) */
// @retail 0x2a7fc0
void __stdcall function_2a7fc0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_index(6, arguments[0]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44d1d8 = { _hs_type_void, 0, function_2a7fc0, NULL, 1, { _hs_type_vehicle } };

/* 426: void (ai_behavior) */
// @retail 0x2a8040
void __stdcall function_2a8040(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_short(0xe, *(short *)&arguments[0]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44d1ec = { _hs_type_void, 0, function_2a8040, NULL, 1, { _hs_type_ai_behavior } };

/* 427: void (short_integer, ai, point_reference, point_reference) */
// @retail 0x2a80c0
void __stdcall function_2a80c0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_277620(*(short *)&arguments[0], arguments[1], arguments[2], arguments[3]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44d200 = { _hs_type_void, 1, function_2a80c0, NULL, 4, { _hs_type_short_integer, _hs_type_ai, _hs_type_point_reference, _hs_type_point_reference } };

/* 428: void (point_reference) */
// @retail 0x2a8130
void __stdcall function_2a8130(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_index(0x15, arguments[0]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44d218 = { _hs_type_void, 0, function_2a8130, NULL, 1, { _hs_type_point_reference } };

/* 429: void (object, real, real, real) */
// @retail 0x2a81b0
void __stdcall function_2a81b0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_276b40(arguments[0], *(real *)&arguments[1], *(real *)&arguments[2], *(real *)&arguments[3]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44d22c = { _hs_type_void, 1, function_2a81b0, NULL, 4, { _hs_type_object, _hs_type_real, _hs_type_real, _hs_type_real } };

/* 430: void (object, real, real, real) */
// @retail 0x2a8220
void __stdcall function_2a8220(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_276c00(arguments[0], *(real *)&arguments[1], *(real *)&arguments[2], *(real *)&arguments[3]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44d244 = { _hs_type_void, 1, function_2a8220, NULL, 4, { _hs_type_object, _hs_type_real, _hs_type_real, _hs_type_real } };

/* 431: void (real, real, real) */
// @retail 0x2a8290
void __stdcall function_2a8290(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_276cc0(*(real *)&arguments[0], *(real *)&arguments[1], *(real *)&arguments[2]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44d25c = { _hs_type_void, 1, function_2a8290, NULL, 3, { _hs_type_real, _hs_type_real, _hs_type_real } };

/* 432: void (real, real, real) */
// @retail 0x2a8300
void __stdcall function_2a8300(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_276d50(*(real *)&arguments[0], *(real *)&arguments[1], *(real *)&arguments[2]);
		function_209ae0(thread_index, 0);
		hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
	}
}

hs_function_definition const g_44d274 = { _hs_type_void, 1, function_2a8300, NULL, 3, { _hs_type_real, _hs_type_real, _hs_type_real } };

void function_259e70(long cs_index);

/* 433: void () */
// @retail 0x2a8370
void __stdcall function_2a8370(short function_index, long thread_index, bool initialize)
{
	if (g_502410 != NONE)
		function_259e70(g_502410);
	function_209ae0(thread_index, 0);
	hs_thread_set_sleep(thread_index, k_hs_sleep_command_script);
}

hs_function_definition const g_44d28c = { _hs_type_void, 1, function_2a8370, NULL, 0 };

/* 434: void (boolean) */
// @retail 0x2a83b0
void __stdcall function_2a83b0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long index = g_502410;
		if (index != NONE)
			command_script_get(index)->flag75 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d29c = { _hs_type_void, 0, function_2a83b0, NULL, 1, { _hs_type_boolean } };

/* 435: void (boolean, real) */
// @retail 0x2a8410
void __stdcall function_2a8410(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_value_70(*(bool *)&arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d2b0 = { _hs_type_void, 0, function_2a8410, NULL, 2, { _hs_type_boolean, _hs_type_real } };

/* 436: void (real) */
// @retail 0x2a8480
void __stdcall function_2a8480(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_277680(*(real *)&arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d2c4 = { _hs_type_void, 0, function_2a8480, NULL, 1, { _hs_type_real } };

/* 437: void (boolean) */
// @retail 0x2a84c0
void __stdcall function_2a84c0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long index = g_502410;
		if (index != NONE)
			command_script_get(index)->flag74 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d2d8 = { _hs_type_void, 0, function_2a84c0, NULL, 1, { _hs_type_boolean } };

/* 438: void (boolean) */
// @retail 0x2a8520
void __stdcall function_2a8520(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long index = g_502410;
		if (index != NONE)
			command_script_get(index)->flag79 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d2ec = { _hs_type_void, 0, function_2a8520, NULL, 1, { _hs_type_boolean } };

/* 439: void (boolean) */
// @retail 0x2a8580
void __stdcall function_2a8580(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long index = g_502410;
		if (index != NONE)
			command_script_get(index)->flag7a = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d300 = { _hs_type_void, 0, function_2a8580, NULL, 1, { _hs_type_boolean } };

/* 440: void (short) */
// @retail 0x2a85e0
void __stdcall function_2a85e0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long index = g_502410;
		if (index != NONE)
			command_script_get(index)->value7c = *(short *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d314 = { _hs_type_void, 0, function_2a85e0, NULL, 1, { _hs_type_short_integer } };

/* 441: void (boolean) */
// @retail 0x2a8640
void __stdcall function_2a8640(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_flag_7f(*(bool *)&arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d328 = { _hs_type_void, 0, function_2a8640, NULL, 1, { _hs_type_boolean } };

/* 442: void (boolean) */
// @retail 0x2a86b0
void __stdcall function_2a86b0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long index = g_502410;
		if (index != NONE)
			command_script_get(index)->flag80 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d33c = { _hs_type_void, 0, function_2a86b0, NULL, 1, { _hs_type_boolean } };

/* 443: void (boolean) */
// @retail 0x2a8710
void __stdcall function_2a8710(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long index = g_502410;
		if (index != NONE)
			command_script_get(index)->flag81 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d350 = { _hs_type_void, 0, function_2a8710, NULL, 1, { _hs_type_boolean } };

/* 444: void (boolean) */
// @retail 0x2a8770
void __stdcall function_2a8770(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long index = g_502410;
		if (index != NONE)
			command_script_get(index)->flag82 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d364 = { _hs_type_void, 0, function_2a8770, NULL, 1, { _hs_type_boolean } };

/* 445: void (boolean) */
// @retail 0x2a87d0
void __stdcall function_2a87d0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long index = g_502410;
		if (index != NONE)
			command_script_get(index)->flag83 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d378 = { _hs_type_void, 0, function_2a87d0, NULL, 1, { _hs_type_boolean } };

/* 446: void (style) */
// @retail 0x2a8830
void __stdcall function_2a8830(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_style(arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d38c = { _hs_type_void, 0, function_2a8830, NULL, 1, { _hs_type_style } };

/* 447: void (short) */
// @retail 0x2a88a0
void __stdcall function_2a88a0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		command_script_set_value_84(*(short *)&arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d3a0 = { _hs_type_void, 0, function_2a88a0, NULL, 1, { _hs_type_short_integer } };

/* 448: void (boolean) */
// @retail 0x2a8910
void __stdcall function_2a8910(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long index = g_502410;
		if (index != NONE)
			command_script_get(index)->flag76 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d3b4 = { _hs_type_void, 0, function_2a8910, NULL, 1, { _hs_type_boolean } };

void __stdcall function_155a30(byte value);

/* 449: void (boolean) */
// @retail 0x2a8970
void __stdcall function_2a8970(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_155a30(*(bool *)&arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d3c8 = { _hs_type_void, 0, function_2a8970, NULL, 1, { _hs_type_boolean } };

/* 456: void (unit) */
// @retail 0x2a8b80
void __stdcall function_2a8b80(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long unit_index = arguments[0];
		if (unit_index != NONE)
		{
			s_510c6c_view *state = (s_510c6c_view *)g_510c6c;
			state->value2 = 4;
			state->flag1 = true;
			state->unit_index = unit_index;
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d464 = { _hs_type_void, 0, function_2a8b80, NULL, 1, { _hs_type_unit } };

/* 459: short () */
// @retail 0x2a8bd0
void __stdcall function_2a8bd0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	real ticks_real = (real)((s_510c6c_view *)g_510c6c)->value14 * g_510c54->rate * 30.0f;
	long ticks;
	__asm
	{
		fld ticks_real
		fistp ticks
	}
	*(short *)&result = (short)ticks;
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d49c = { _hs_type_short_integer, 0, function_2a8bd0, NULL, 0 };

/* 460: void (real, short_integer) */
// @retail 0x2a8c20
void __stdcall function_2a8c20(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_16c740(*(real *)&arguments[0], *(short *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d4ac = { _hs_type_void, 0, function_2a8c20, NULL, 2, { _hs_type_real, _hs_type_short_integer } };

/* 468: game_difficulty () */
// @retail 0x2a8d30
void __stdcall function_2a8d30(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	s_game_options_view *options = g_4e6948;
	if (options->state == 1)
	{
		long difficulty = options->difficulty;
		*(short *)&result = (short)difficulty;
		if ((short)difficulty > 1)
			goto done;
	}
	*(short *)&result = 1;
done:
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d550 = { _hs_type_game_difficulty, 0, function_2a8d30, NULL, 0 };

/* 469: game_difficulty () */
// @retail 0x2a8d70
void __stdcall function_2a8d70(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	s_game_options_view *options = g_4e6948;
	if (options->state == 1)
		*(short *)&result = options->difficulty;
	else
		*(short *)&result = 1;
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d560 = { _hs_type_game_difficulty, 0, function_2a8d70, NULL, 0 };

/* 470: void (object) */
// @retail 0x2a8dc0
void __stdcall function_2a8dc0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long object_index = arguments[0];
		s_game_options_view *options = g_4e6948;
		if (object_index == NONE)
		{
			options->value11fa = 0;
		}
		else
		{
			options->value11fc = object_index;
			options->value11fa = 1;
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d570 = { _hs_type_void, 0, function_2a8dc0, NULL, 1, { _hs_type_object } };

void function_138a40(short point_index);

/* 471: void (cutscene_camera_point) */
// @retail 0x2a8e30
void __stdcall function_2a8e30(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_138a40(*(short *)&arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d584 = { _hs_type_void, 0, function_2a8e30, NULL, 1, { _hs_type_cutscene_camera_point } };

/* 472: void () */
// @retail 0x2a8e70
void __stdcall function_2a8e70(short function_index, long thread_index, bool initialize)
{
	g_4e6948->value11fa = 0;
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44d598 = { _hs_type_void, 0, function_2a8e70, NULL, 0 };

/* 473: void () */
// @retail 0x2a8e90
void __stdcall function_2a8e90(short function_index, long thread_index, bool initialize)
{
	s_unknown_185ab0 *globals = g_4ed284;
	globals->entries[0].flag = false;
	globals->entries[0].index = NONE;
	globals->entries[1].flag = false;
	globals->entries[1].index = NONE;
	globals->entries[2].flag = false;
	globals->entries[2].index = NONE;
	globals->entries[3].flag = false;
	globals->entries[3].index = NONE;
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44d5a8 = { _hs_type_void, 0, function_2a8e90, NULL, 0 };

/* 474: void (boolean) */
// @retail 0x2a8ee0
void __stdcall function_2a8ee0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		((s_4e8c20_view *)g_4e8c20)->flag6 = !*(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d5b8 = { _hs_type_void, 0, function_2a8ee0, NULL, 1, { _hs_type_boolean } };

/* 475: void (boolean) */
// @retail 0x2a8f30
void __stdcall function_2a8f30(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		((s_4e8c20_view *)g_4e8c20)->flag7 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d5cc = { _hs_type_void, 0, function_2a8f30, NULL, 1, { _hs_type_boolean } };

/* 476: boolean () */
// @retail 0x2a8f80
void __stdcall function_2a8f80(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = function_14ed80();
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d5e0 = { _hs_type_boolean, 0, function_2a8f80, NULL, 0 };

/* 477: boolean () */
// @retail 0x2a8fa0
void __stdcall function_2a8fa0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = function_14ece0();
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d5f0 = { _hs_type_boolean, 0, function_2a8fa0, NULL, 0 };

/* 478: boolean (boolean) */
// @retail 0x2a8fc0
void __stdcall function_2a8fc0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		bool value = *(bool *)&arguments[0];
		dword *flags = &g_4ed284->flags10;
		SET_FLAG(*flags, 0, !value);
		*(bool *)&result = value;
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44d600 = { _hs_type_boolean, 0, function_2a8fc0, NULL, 1, { _hs_type_boolean } };

/* 479: void () */
// @retail 0x2a9020
void __stdcall function_2a9020(short function_index, long thread_index, bool initialize)
{
	s_unknown_185ab0 *globals = g_4ed284;
	globals->flags4 = 0;
	globals->flags8 = 0;
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44d614 = { _hs_type_void, 0, function_2a9020, NULL, 0 };

/* 480: boolean () */
// @retail 0x2a9040
void __stdcall function_2a9040(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(g_4ed284->bits4.bit1);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d624 = { _hs_type_boolean, 0, function_2a9040, NULL, 0 };

/* 481: boolean () */
// @retail 0x2a9070
void __stdcall function_2a9070(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(g_4ed284->bits4.bit4);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d634 = { _hs_type_boolean, 0, function_2a9070, NULL, 0 };

/* 482: boolean () */
// @retail 0x2a90a0
void __stdcall function_2a90a0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(g_4ed284->bits4.bit5);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d644 = { _hs_type_boolean, 0, function_2a90a0, NULL, 0 };

/* 483: boolean () */
// @retail 0x2a90d0
void __stdcall function_2a90d0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(g_4ed284->bits4.bit20);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d654 = { _hs_type_boolean, 0, function_2a90d0, NULL, 0 };

/* 484: boolean () */
// @retail 0x2a9100
void __stdcall function_2a9100(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(g_4ed284->bits4.bit9);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d664 = { _hs_type_boolean, 0, function_2a9100, NULL, 0 };

/* 485: boolean () */
// @retail 0x2a9130
void __stdcall function_2a9130(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(g_4ed284->bits4.bit7);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d674 = { _hs_type_boolean, 0, function_2a9130, NULL, 0 };

/* 486: boolean () */
// @retail 0x2a9160
void __stdcall function_2a9160(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(g_4ed284->bits4.bit8);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d684 = { _hs_type_boolean, 0, function_2a9160, NULL, 0 };

/* 487: boolean () */
// @retail 0x2a9190
void __stdcall function_2a9190(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(g_4ed284->bits4.bit6);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d694 = { _hs_type_boolean, 0, function_2a9190, NULL, 0 };

/* 488: boolean () */
// @retail 0x2a91c0
void __stdcall function_2a91c0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	s_unknown_185ab0 *globals = g_4ed284;
	globals->bits8.bit0 = true;
	globals->bitsc.bit0 = true;
	*(bool *)&result = TEST_FIELD_BIT(globals->bits4.bit0);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d6a4 = { _hs_type_boolean, 0, function_2a91c0, NULL, 0 };

/* 489: boolean () */
// @retail 0x2a9200
void __stdcall function_2a9200(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	s_unknown_185ab0 *globals = g_4ed284;
	globals->bits8.bit2 = true;
	globals->bitsc.bit2 = true;
	*(bool *)&result = TEST_FIELD_BIT(globals->bits4.bit2);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d6b4 = { _hs_type_boolean, 0, function_2a9200, NULL, 0 };

/* 490: boolean () */
// @retail 0x2a9240
void __stdcall function_2a9240(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	s_unknown_185ab0 *globals = g_4ed284;
	globals->bits8.bit3 = true;
	globals->bitsc.bit3 = true;
	*(bool *)&result = TEST_FIELD_BIT(globals->bits4.bit3);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d6c4 = { _hs_type_boolean, 0, function_2a9240, NULL, 0 };

/* 491: boolean () */
// @retail 0x2a9280
void __stdcall function_2a9280(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(g_4ed284->bits4.bit10);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d6d4 = { _hs_type_boolean, 0, function_2a9280, NULL, 0 };

/* 492: boolean () */
// @retail 0x2a92b0
void __stdcall function_2a92b0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(g_4ed284->bits4.bit11);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d6e4 = { _hs_type_boolean, 0, function_2a92b0, NULL, 0 };

/* 493: boolean () */
// @retail 0x2a92e0
void __stdcall function_2a92e0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(g_4ed284->bits4.bit12);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d6f4 = { _hs_type_boolean, 0, function_2a92e0, NULL, 0 };

/* 494: boolean () */
// @retail 0x2a9310
void __stdcall function_2a9310(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(g_4ed284->bits4.bit13);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d704 = { _hs_type_boolean, 0, function_2a9310, NULL, 0 };

/* 495: boolean () */
// @retail 0x2a9340
void __stdcall function_2a9340(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = (g_4ed284->flags4 & 0x3c00) != 0;
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d714 = { _hs_type_boolean, 0, function_2a9340, NULL, 0 };

/* 496: boolean () */
// @retail 0x2a9370
void __stdcall function_2a9370(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = (g_4ed284->flags4 & 0x3c000) != 0;
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d724 = { _hs_type_boolean, 0, function_2a9370, NULL, 0 };

/* 497: boolean () */
// @retail 0x2a93a0
void __stdcall function_2a93a0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(g_4ed284->bits4.bit18);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d734 = { _hs_type_boolean, 0, function_2a93a0, NULL, 0 };

/* 498: boolean () */
// @retail 0x2a93d0
void __stdcall function_2a93d0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(g_4ed284->bits4.bit19);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d744 = { _hs_type_boolean, 0, function_2a93d0, NULL, 0 };

real function_187f30(void);

real g_54e880;

/* 499: boolean () */
// @retail 0x2a9400
void __stdcall function_2a9400(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = function_187f30() >= g_54e880;
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d754 = { _hs_type_boolean, 0, function_2a9400, NULL, 0 };

/* 500: boolean () */
// @retail 0x2a9430
void __stdcall function_2a9430(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = -function_187f30() >= g_54e880;
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d764 = { _hs_type_boolean, 0, function_2a9430, NULL, 0 };

/* 501: void () */
// @retail 0x2a9470
void __stdcall function_2a9470(short function_index, long thread_index, bool initialize)
{
	s_unknown_185ab0 *globals = g_4ed284;
	dword *flags = &globals->flagsc;
	SET_FLAG(*flags, 12, true);
	SET_FLAG(*flags, 13, true);
	globals->flags4 |= FLAG(21);
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44d774 = { _hs_type_void, 0, function_2a9470, NULL, 0 };

/* 502: void () */
// @retail 0x2a94b0
void __stdcall function_2a94b0(short function_index, long thread_index, bool initialize)
{
	s_unknown_185ab0 *globals = g_4ed284;
	dword *flags = &globals->flagsc;
	SET_FLAG(*flags, 12, true);
	SET_FLAG(*flags, 13, true);
	globals->flags4 |= FLAG(22);
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44d784 = { _hs_type_void, 0, function_2a94b0, NULL, 0 };

/* 503: void () */
// @retail 0x2a94f0
void __stdcall function_2a94f0(short function_index, long thread_index, bool initialize)
{
	s_unknown_185ab0 *globals = g_4ed284;
	dword *flags = &globals->flagsc;
	SET_FLAG(*flags, 12, false);
	SET_FLAG(*flags, 13, false);
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44d794 = { _hs_type_void, 0, function_2a94f0, NULL, 0 };

/* 504: boolean () */
// @retail 0x2a9520
void __stdcall function_2a9520(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(g_4ed284->bits4.bit23);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d7a4 = { _hs_type_boolean, 0, function_2a9520, NULL, 0 };

/* 505: boolean () */
// @retail 0x2a9550
void __stdcall function_2a9550(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(g_4ed284->bits4.bit24);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d7b4 = { _hs_type_boolean, 0, function_2a9550, NULL, 0 };

/* 510: short () */
// @retail 0x2a95c0
void __stdcall function_2a95c0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(short *)&result = g_4686c4;
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d810 = { _hs_type_short_integer, 0, function_2a95c0, NULL, 0 };

/* sets the point g_4e8c28 and starts the timer of g_510c5c */
inline void point_timer_start(real x, real y, real z, short script_ticks)
{
	g_4e8c28.x = x;
	g_4e8c28.y = y;
	g_4e8c28.z = z;
	g_510c5c->duration = (short)game_seconds_to_ticks_round((real)script_ticks * (1.0f / 30.0f));
	g_510c5c->reverse = true;
	g_510c5c->start_time = g_510c54->game_time;
}

void function_154220(short seconds, real x, real y, real z);

void game_time_set_speed(real initial_speed, real speed, real duration);

/* 530: void (real, real, real) */
// @retail 0x2a95f0
void __stdcall function_2a95f0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		game_time_set_speed(*(real *)&arguments[0], *(real *)&arguments[1], *(real *)&arguments[2]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44d998 = { _hs_type_void, 0, function_2a95f0, NULL, 3, { _hs_type_real, _hs_type_real, _hs_type_real } };

/* 552: void (real, real, real, short_integer) */
// @retail 0x2a9640
void __stdcall function_2a9640(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_154220(*(short *)&arguments[3], *(real *)&arguments[0], *(real *)&arguments[1], *(real *)&arguments[2]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44db20 = { _hs_type_void, 0, function_2a9640, NULL, 4, { _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_short_integer } };

/* 553: void (real, real, real, short) */
// @retail 0x2a9690
void __stdcall function_2a9690(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		point_timer_start(*(real *)&arguments[0], *(real *)&arguments[1], *(real *)&arguments[2], *(short *)&arguments[3]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44db38 = { _hs_type_void, 0, function_2a9690, NULL, 4, { _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_short_integer } };

/* 554: void () */
// @retail 0x2a9740
void __stdcall function_2a9740(short function_index, long thread_index, bool initialize)
{
	function_13bff0();
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44db50 = { _hs_type_void, 0, function_2a9740, NULL, 0 };

/* 555: void () */
// @retail 0x2a9760
void __stdcall function_2a9760(short function_index, long thread_index, bool initialize)
{
	function_13ca80();
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44db60 = { _hs_type_void, 0, function_2a9760, NULL, 0 };

/* 556: void () */
// @retail 0x2a9780
void __stdcall function_2a9780(short function_index, long thread_index, bool initialize)
{
	((s_510c50_view *)g_510c50)->flag6 = true;
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44db70 = { _hs_type_void, 0, function_2a9780, NULL, 0 };

/* 557: void () */
// @retail 0x2a97a0
void __stdcall function_2a97a0(short function_index, long thread_index, bool initialize)
{
	((s_510c50_view *)g_510c50)->flag6 = false;
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44db80 = { _hs_type_void, 0, function_2a97a0, NULL, 0 };

/* 558: void (boolean) */
// @retail 0x2a97c0
void __stdcall function_2a97c0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		((s_510c50_view *)g_510c50)->flag4 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44db90 = { _hs_type_void, 0, function_2a97c0, NULL, 1, { _hs_type_boolean } };

/* 559: void (boolean) */
// @retail 0x2a9810
void __stdcall function_2a9810(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		bool value = *(bool *)&arguments[0];
		s_510c50_view *state = (s_510c50_view *)g_510c50;
		state->flag4 = value;
		state->value0 = value ? 1.0f : 0.0f;
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44dba4 = { _hs_type_void, 0, function_2a9810, NULL, 1, { _hs_type_boolean } };

/* 560: void (cutscene_title) */
// @retail 0x2a9870
void __stdcall function_2a9870(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_13c1e0(*(short *)&arguments[0], 0.0f);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44dbb8 = { _hs_type_void, 0, function_2a9870, NULL, 1, { _hs_type_cutscene_title } };

/* 561: void (cutscene_title, real) */
// @retail 0x2a98c0
void __stdcall function_2a98c0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_13c1e0(*(short *)&arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44dbcc = { _hs_type_void, 0, function_2a98c0, NULL, 2, { _hs_type_cutscene_title, _hs_type_real } };

/* 562: void (boolean) */
// @retail 0x2a9910
void __stdcall function_2a9910(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		((s_510c50_view *)g_510c50)->flag7 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44dbe0 = { _hs_type_void, 0, function_2a9910, NULL, 1, { _hs_type_boolean } };

void function_13cb50(long string_id, real seconds);

/* 563: void (string_id, real) */
// @retail 0x2a9960
void __stdcall function_2a9960(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_13cb50(arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44dbf4 = { _hs_type_void, 0, function_2a9960, NULL, 2, { _hs_type_string_id, _hs_type_real } };

/* 566: void () */
// @retail 0x2a99b0
void __stdcall function_2a99b0(short function_index, long thread_index, bool initialize)
{
	function_1388e0();
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44dc2c = { _hs_type_void, 0, function_2a99b0, NULL, 0 };

void function_138960(bool start);

/* 567: void (boolean) */
// @retail 0x2a99d0
void __stdcall function_2a99d0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_138960(*(bool *)&arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44dc3c = { _hs_type_void, 0, function_2a99d0, NULL, 1, { _hs_type_boolean } };

/* 568: void () */
// @retail 0x2a9a10
void __stdcall function_2a9a10(short function_index, long thread_index, bool initialize)
{
	g_547f6f = true;
	g_547f75 = true;
	g_547f74 = true;
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44dc50 = { _hs_type_void, 0, function_2a9a10, NULL, 0 };

/* 569: boolean () */
// @retail 0x2a9a30
void __stdcall function_2a9a30(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	s_game_options_view *options = g_4e6948;
	*(bool *)&result = options->state == 1 && options->flag134;
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44dc60 = { _hs_type_boolean, 0, function_2a9a30, NULL, 0 };

/* 570: boolean () */
// @retail 0x2a9a70
void __stdcall function_2a9a70(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = g_4e6948->flag130;
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44dc70 = { _hs_type_boolean, 0, function_2a9a70, NULL, 0 };

/* starts stage of the sequence g_4701ec, unless it is already past it */
inline void sequence_start_stage(long stage)
{
	if (g_4701ec.stage < stage)
	{
		g_4701ec.stage = stage;
		g_4701ec.unknown4 = 0;
		g_4701ec.start_time = g_510c54->game_time;
		g_4701ec.unknownc = 0;
	}
}

/* 571: void (boolean) */
// @retail 0x2a9aa0
void __stdcall function_2a9aa0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		s_5107e8 *globals = g_5107e8;
		bool value = *(bool *)&arguments[0];
		if (globals->flag0 != value)
		{
			globals->flag0 = value;
			globals->time4 = g_4e6948 && g_4e6948->flag1120 ? g_510c54->game_time : 0;
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44dc80 = { _hs_type_void, 0, function_2a9aa0, NULL, 1, { _hs_type_boolean } };

/* asks for a core save to be loaded */
inline void core_load_name(char const *name)
{
	g_547f2d = true;
	strncpy(g_547f2e, name, sizeof(g_547f2e));
	g_547f2e[sizeof(g_547f2e) - 1] = 0;
}

/* 576: void () */
// @retail 0x2a9b20
void __stdcall function_2a9b20(short function_index, long thread_index, bool initialize)
{
	core_load_name("core");
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44dcdc = { _hs_type_void, 0, function_2a9b20, NULL, 0 };

/* 577: void (string) */
// @retail 0x2a9b50
void __stdcall function_2a9b50(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		core_load_name((char const *)arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44dcec = { _hs_type_void, 0, function_2a9b50, NULL, 1, { _hs_type_string } };

/* 585: boolean () */
// @retail 0x2a9bb0
void __stdcall function_2a9bb0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = function_226190();
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44dd80 = { _hs_type_boolean, 0, function_2a9bb0, NULL, 0 };

/* 586: boolean () */
// @retail 0x2a9bd0
void __stdcall function_2a9bd0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	long value;
	*(bool *)&result = !function_fa9a0(&value) && !((s_4e8c20_view *)g_4e8c20)->flag5;
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44dd90 = { _hs_type_boolean, 0, function_2a9bd0, NULL, 0 };

/* 587: boolean () */
// @retail 0x2a9c10
void __stdcall function_2a9c10(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = function_225fe0();
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44dda0 = { _hs_type_boolean, 0, function_2a9c10, NULL, 0 };

/* 588: void () */
// @retail 0x2a9c30
void __stdcall function_2a9c30(short function_index, long thread_index, bool initialize)
{
	sequence_start_stage(1);
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44ddb0 = { _hs_type_void, 0, function_2a9c30, NULL, 0 };

/* 589: void () */
// @retail 0x2a9c70
void __stdcall function_2a9c70(short function_index, long thread_index, bool initialize)
{
	g_4701ec.stage = 0;
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44ddc0 = { _hs_type_void, 0, function_2a9c70, NULL, 0 };

/* 590: void () */
// @retail 0x2a9c90
void __stdcall function_2a9c90(short function_index, long thread_index, bool initialize)
{
	sequence_start_stage(2);
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44ddd0 = { _hs_type_void, 0, function_2a9c90, NULL, 0 };

/* 591: void () */
// @retail 0x2a9cd0
void __stdcall function_2a9cd0(short function_index, long thread_index, bool initialize)
{
	sequence_start_stage(3);
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44dde0 = { _hs_type_void, 0, function_2a9cd0, NULL, 0 };

/* 592: boolean () */
// @retail 0x2a9d10
void __stdcall function_2a9d10(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	bool active = false;
	if (g_547f70 || g_4701ec.stage != 0)
		active = true;
	*(bool *)&result = active;
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44ddf0 = { _hs_type_boolean, 0, function_2a9d10, NULL, 0 };

/* 593: boolean () */
// @retail 0x2a9d40
void __stdcall function_2a9d40(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = game_state_globals.game_time == g_510c54->game_time;
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44de00 = { _hs_type_boolean, 0, function_2a9d40, NULL, 0 };

inline s_4ed28c_element *looping_sound_get(long sound_index)
{
	return (s_4ed28c_element *)g_4ed28c->data + (sound_index & 0xffff);
}

inline void looping_sound_set_scale(long tag_index, real scale)
{
	if (tag_index != NONE)
	{
		long sound_index = function_18d1c0(tag_index);
		if (sound_index != NONE)
		{
			s_4ed28c_element *element = looping_sound_get(sound_index);
			element->scale8 = real_pin(scale, 0.0f, 1.0f);
		}
	}
}

void sound_definition_request_first_chunk(long definition_index);
/* 595: void (sound) */
// @retail 0x2a9d70
void __stdcall function_2a9d70(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		sound_definition_request_first_chunk(arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44de30 = { _hs_type_void, 0, function_2a9d70, NULL, 1, { _hs_type_sound } };

/* 597: void (sound, object, real) */
// @retail 0x2a9e00
void __stdcall function_2a9e00(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_189cd0(arguments[0], arguments[1], *(real *)&arguments[2], g_444ae0, g_444ae0, NONE, 0);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44de5c = { _hs_type_void, 0, function_2a9e00, NULL, 3, { _hs_type_sound, _hs_type_object, _hs_type_real } };

/* a gain (0..1) as decibels, returned as the bits of a real (unknown_2197f0.cpp) */
long function_2197f0(real gain);

/* 598: void (sound, object, real, real, real) */
// @retail 0x2a9e60
void __stdcall function_2a9e60(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_189cd0(arguments[0], arguments[1], *(real *)&arguments[2], function_2197f0(*(real *)&arguments[3]), function_2197f0(*(real *)&arguments[4]), NONE, 0);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44de74 = { _hs_type_void, 0, function_2a9e60, NULL, 5, { _hs_type_sound, _hs_type_object, _hs_type_real, _hs_type_real, _hs_type_real } };

/* 599: void (sound, object, real, string_id) */
// @retail 0x2a9ed0
void __stdcall function_2a9ed0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_189cd0(arguments[0], arguments[1], *(real *)&arguments[2], g_444ae0, g_444ae0, arguments[3], 0);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44de90 = { _hs_type_void, 0, function_2a9ed0, NULL, 4, { _hs_type_sound, _hs_type_object, _hs_type_real, _hs_type_string_id } };

long function_18a1c0(long datum_index);
long function_18a2f0(long datum_index, long seconds);
void function_18a4d0(long tag_index);
void function_18a530(long tag_index);

/* 600: long_integer (sound) */
// @retail 0x2a9f30
void __stdcall function_2a9f30(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
		function_209ae0(thread_index, function_18a1c0(arguments[0]));
}

hs_function_definition const g_44dea8 = { _hs_type_long_integer, 0, function_2a9f30, NULL, 1, { _hs_type_sound } };

/* 601: long_integer (sound, long_integer) */
// @retail 0x2a9f70
void __stdcall function_2a9f70(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long ticks = function_18a2f0(arguments[0], arguments[1]);
		real ticks_real = (real)ticks * g_510c54->rate * 30.0f;
		long rounded;
		__asm
		{
			fld ticks_real
			fistp rounded
		}
		function_209ae0(thread_index, rounded);
	}
}

hs_function_definition const g_44debc = { _hs_type_long_integer, 0, function_2a9f70, NULL, 2, { _hs_type_sound, _hs_type_long_integer } };

/* 606: void (looping_sound, object, real) */
// @retail 0x2aa0f0
void __stdcall function_2aa0f0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_18a430(arguments[0], arguments[1], *(real *)&arguments[2]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44df24 = { _hs_type_void, 0, function_2aa0f0, NULL, 3, { _hs_type_looping_sound, _hs_type_object, _hs_type_real } };

/* 607: void (looping_sound) */
// @retail 0x2aa140
void __stdcall function_2aa140(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_18a4d0(arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44df3c = { _hs_type_void, 0, function_2aa140, NULL, 1, { _hs_type_looping_sound } };

/* 608: void (looping_sound) */
// @retail 0x2aa180
void __stdcall function_2aa180(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_18a530(arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44df50 = { _hs_type_void, 0, function_2aa180, NULL, 1, { _hs_type_looping_sound } };

/* 609: void (looping_sound, real) */
// @retail 0x2aa1c0
void __stdcall function_2aa1c0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		looping_sound_set_scale(arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44df64 = { _hs_type_void, 0, function_2aa1c0, NULL, 2, { _hs_type_looping_sound, _hs_type_real } };

inline void looping_sound_set_flag4(long tag_index, bool value)
{
	if (tag_index != NONE)
	{
		long sound_index = function_18d1c0(tag_index);
		if (sound_index != NONE)
		{
			word *flags = &looping_sound_get(sound_index)->flags4;
			SET_FLAG(*flags, 4, value);
		}
	}
}

/* 610: void (looping_sound, boolean) */
// @retail 0x2aa240
void __stdcall function_2aa240(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		looping_sound_set_flag4(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44df78 = { _hs_type_void, 0, function_2aa240, NULL, 2, { _hs_type_looping_sound, _hs_type_boolean } };

inline void sound_class_set_gain(char const *name, real gain, short ticks)
{
	function_221980(name, *(long *)&gain, (real)ticks * (1.0f / 30.0f));
}

/* 615: void (string, real, short_integer) */
// @retail 0x2aa2c0
void __stdcall function_2aa2c0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_221980((char const *)arguments[0], function_2197f0(*(real *)&arguments[1]), (real)*(short *)&arguments[2] * (1.0f / 30.0f));
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44dfd8 = { _hs_type_void, 0, function_2aa2c0, NULL, 3, { _hs_type_string, _hs_type_real, _hs_type_short_integer } };

/* 616: void (string, real, short) */
// @retail 0x2aa330
void __stdcall function_2aa330(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		sound_class_set_gain((char const *)arguments[0], *(real *)&arguments[1], *(short *)&arguments[2]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44dff0 = { _hs_type_void, 0, function_2aa330, NULL, 3, { _hs_type_string, _hs_type_real, _hs_type_short_integer } };

/* 617: void (string, boolean) */
// @retail 0x2aa3a0
void __stdcall function_2aa3a0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_221a20((char const *)arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e008 = { _hs_type_void, 0, function_2aa3a0, NULL, 2, { _hs_type_string, _hs_type_boolean } };

/* 620: void (string_id, real) */
// @retail 0x2aa430
void __stdcall function_2aa430(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		s_51ebf4_view *state = (s_51ebf4_view *)g_51ebf4;
		if (state->flag14 && state->id20 == arguments[0])
			state->value24 = *(real *)&arguments[1];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e044 = { _hs_type_void, 0, function_2aa430, NULL, 2, { _hs_type_string_id, _hs_type_real } };

/* 622: long (vehicle) */
// @retail 0x2aa4c0
void __stdcall function_2aa4c0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long vehicle_index = arguments[0];
		long result = 0;
		if (vehicle_index != NONE)
			result = ((s_vehicle *)object_get(vehicle_index))->value_3ac;
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44e06c = { _hs_type_long_integer, 0, function_2aa4c0, NULL, 1, { _hs_type_vehicle } };

/* 623: void (unit) */
// @retail 0x2aa520
void __stdcall function_2aa520(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long unit_index = arguments[0];
		if (unit_index != NONE)
		{
			s_object *object = function_badc0(unit_index, 1);
			if (object)
			{
				object->flags_10a |= FLAG(6);
				((s_unit *)object)->unit_flag27 = true;
				function_b7360(unit_index);
			}
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e080 = { _hs_type_void, 0, function_2aa520, NULL, 1, { _hs_type_unit } };

/* 624: void (real, real) */
// @retail 0x2aa590
void __stdcall function_2aa590(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_13b306(*(real *)&arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e094 = { _hs_type_void, 0, function_2aa590, NULL, 2, { _hs_type_real, _hs_type_real } };

/* 625: boolean (boolean) */
// @retail 0x2aa5e0
void __stdcall function_2aa5e0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		bool value = *(bool *)&arguments[0];
		g_510c4c->flag1d2 = value;
		*(bool *)&result = value;
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44e0a8 = { _hs_type_boolean, 0, function_2aa5e0, NULL, 1, { _hs_type_boolean } };

/* 626: boolean (boolean) */
// @retail 0x2aa640
void __stdcall function_2aa640(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		bool value = *(bool *)&arguments[0];
		((s_510c98_view *)g_510c98)->flag1 = value;
		*(bool *)&result = value;
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44e0bc = { _hs_type_boolean, 0, function_2aa640, NULL, 1, { _hs_type_boolean } };

/* 627: boolean (boolean) */
// @retail 0x2aa6a0
void __stdcall function_2aa6a0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		bool value = *(bool *)&arguments[0];
		((s_510c98_view *)g_510c98)->flag2 = value;
		*(bool *)&result = value;
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44e0d0 = { _hs_type_boolean, 0, function_2aa6a0, NULL, 1, { _hs_type_boolean } };

/* 628: void (boolean) */
// @retail 0x2aa700
void __stdcall function_2aa700(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		bool value = *(bool *)&arguments[0];
		s_hud_state_view *hud = (s_hud_state_view *)g_5023f4;
		if (value && !hud->flag1384)
			hud->time1380 = g_510c54->game_time;
		hud->flag1384 = value;
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e0e4 = { _hs_type_void, 0, function_2aa700, NULL, 1, { _hs_type_boolean } };

/* 629: void () */
// @retail 0x2aa770
void __stdcall function_2aa770(short function_index, long thread_index, bool initialize)
{
	s_hud_state_view *hud = (s_hud_state_view *)g_5023f4;
	if (hud->flag1384)
		hud->time1380 = g_510c54->game_time;
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44e0f8 = { _hs_type_void, 0, function_2aa770, NULL, 0 };

/* 630: void (boolean) */
// @retail 0x2aa7a0
void __stdcall function_2aa7a0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		((s_51e9c0_view *)g_51e9c0)->flag6c4 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e108 = { _hs_type_void, 0, function_2aa7a0, NULL, 1, { _hs_type_boolean } };

/* 631: void (string_id) */
// @retail 0x2aa7f0
void __stdcall function_2aa7f0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		((s_51e9c0_view *)g_51e9c0)->value6c0 = arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e11c = { _hs_type_void, 0, function_2aa7f0, NULL, 1, { _hs_type_string_id } };

/* 632: void (boolean) */
// @retail 0x2aa840
void __stdcall function_2aa840(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		((s_51e9c0_view *)g_51e9c0)->flag6c5 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e130 = { _hs_type_void, 0, function_2aa840, NULL, 1, { _hs_type_boolean } };

inline void point_timer_set_point(real x, real y, real z)
{
	s_game_speed *state = g_510c5c;
	state->point.x = x;
	state->point.y = y;
	state->point.z = z;
}

/* 633: void () */
// @retail 0x2aa890
void __stdcall function_2aa890(short function_index, long thread_index, bool initialize)
{
	function_1e7800();
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44e144 = { _hs_type_void, 0, function_2aa890, NULL, 0 };

/* 634: void () */
// @retail 0x2aa8b0
void __stdcall function_2aa8b0(short function_index, long thread_index, bool initialize)
{
	function_1e78b0();
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44e154 = { _hs_type_void, 0, function_2aa8b0, NULL, 0 };

/* 635: void () */
// @retail 0x2aa8d0
void __stdcall function_2aa8d0(short function_index, long thread_index, bool initialize)
{
	function_1e7960();
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44e164 = { _hs_type_void, 0, function_2aa8d0, NULL, 0 };

void hud_activate_nav_point(long player_index, short type, short is_object, long object_index, real value);
void hud_deactivate_nav_point(long player_index, short is_object, long object_index);

/* the player controlling a unit, NONE if none (unknown_14b560.cpp) */
long unit_get_player_index(long unit_index);

/* a navigation point of the player controlling a unit */
inline void unit_activate_nav_point(short type, long unit_index, short is_object, long object_index, real value)
{
	long player_index = unit_get_player_index(unit_index);
	if (player_index != NONE)
		hud_activate_nav_point(player_index, type, is_object, object_index, value);
}

inline void unit_deactivate_nav_point(long unit_index, short is_object, long object_index)
{
	long player_index = unit_get_player_index(unit_index);
	if (player_index != NONE)
		hud_deactivate_nav_point(player_index, is_object, object_index);
}

/* 636: void (navpoint, unit, cutscene_flag, real) */
// @retail 0x2aa8f0
void __stdcall function_2aa8f0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		real value = *(real *)&arguments[3];
		short cutscene_flag_index = *(short *)&arguments[2];
		short type = *(short *)&arguments[0];
		long unit_index = arguments[1];
		long player_index = unit_get_player_index(unit_index);
		if (player_index != NONE)
			hud_activate_nav_point(player_index, type, false, cutscene_flag_index, value);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e174 = { _hs_type_void, 0, function_2aa8f0, NULL, 4, { _hs_type_navpoint, _hs_type_unit, _hs_type_cutscene_flag, _hs_type_real } };

/* 637: void (navpoint, unit, object, real) */
// @retail 0x2aa970
void __stdcall function_2aa970(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		unit_activate_nav_point(*(short *)&arguments[0], arguments[1], true, arguments[2], *(real *)&arguments[3]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e18c = { _hs_type_void, 0, function_2aa970, NULL, 4, { _hs_type_navpoint, _hs_type_unit, _hs_type_object, _hs_type_real } };

/* 638: void (navpoint, team, cutscene_flag, real) */
// @retail 0x2aa9f0
void __stdcall function_2aa9f0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_24d7ac(*(short *)&arguments[0], *(short *)&arguments[1], false, *(short *)&arguments[2], *(real *)&arguments[3]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e1a4 = { _hs_type_void, 0, function_2aa9f0, NULL, 4, { _hs_type_navpoint, _hs_type_team, _hs_type_cutscene_flag, _hs_type_real } };

/* 639: void (navpoint, team, object, real) */
// @retail 0x2aaa50
void __stdcall function_2aaa50(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_24d7ac(*(short *)&arguments[0], *(short *)&arguments[1], true, arguments[2], *(real *)&arguments[3]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e1bc = { _hs_type_void, 0, function_2aaa50, NULL, 4, { _hs_type_navpoint, _hs_type_team, _hs_type_object, _hs_type_real } };

/* 640: void (unit, cutscene_flag) */
// @retail 0x2aaab0
void __stdcall function_2aaab0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long unit_index = arguments[0];
		short cutscene_flag_index = *(short *)&arguments[1];
		long player_index = unit_get_player_index(unit_index);
		if (player_index != NONE)
			hud_deactivate_nav_point(player_index, false, cutscene_flag_index);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e1d4 = { _hs_type_void, 0, function_2aaab0, NULL, 2, { _hs_type_unit, _hs_type_cutscene_flag } };

/* 641: void (unit, object) */
// @retail 0x2aab10
void __stdcall function_2aab10(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		unit_deactivate_nav_point(arguments[0], true, arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e1e8 = { _hs_type_void, 0, function_2aab10, NULL, 2, { _hs_type_unit, _hs_type_object } };

/* 642: void (team, cutscene_flag) */
// @retail 0x2aab70
void __stdcall function_2aab70(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_24d877(*(short *)&arguments[0], false, *(short *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e1fc = { _hs_type_void, 0, function_2aab70, NULL, 2, { _hs_type_team, _hs_type_cutscene_flag } };

/* 643: void (team, object) */
// @retail 0x2aabc0
void __stdcall function_2aabc0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_24d877(*(short *)&arguments[0], true, arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e210 = { _hs_type_void, 0, function_2aabc0, NULL, 2, { _hs_type_team, _hs_type_object } };

/* 651: void (real, real, real) */
// @retail 0x2aac10
void __stdcall function_2aac10(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		point_timer_set_point(*(real *)&arguments[0], *(real *)&arguments[1], *(real *)&arguments[2]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e2a0 = { _hs_type_void, 0, function_2aac10, NULL, 3, { _hs_type_real, _hs_type_real, _hs_type_real } };

inline void point_timer_set_angles(real yaw, real pitch, real roll)
{
	s_game_speed *state = g_510c5c;
	state->angles[0] = yaw * DEGREES_TO_RADIANS;
	state->angles[1] = pitch * DEGREES_TO_RADIANS;
	state->angles[2] = roll * DEGREES_TO_RADIANS;
}

/* 652: void (real, real, real) */
// @retail 0x2aac70
void __stdcall function_2aac70(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		point_timer_set_angles(*(real *)&arguments[0], *(real *)&arguments[1], *(real *)&arguments[2]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e2b8 = { _hs_type_void, 0, function_2aac70, NULL, 3, { _hs_type_real, _hs_type_real, _hs_type_real } };

inline void state_502120_set_values(real a, real b)
{
	s_speed_table *state = g_502120;
	state->value220 = a;
	state->value224 = b;
}

/* 653: void (real, real) */
// @retail 0x2aace0
void __stdcall function_2aace0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		state_502120_set_values(*(real *)&arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e2d0 = { _hs_type_void, 0, function_2aace0, NULL, 2, { _hs_type_real, _hs_type_real } };

inline void point_timer_set_value(long value, real seconds)
{
	s_game_speed *state = g_510c5c;
	state->value20 = value;
	short ticks = (short)game_seconds_to_ticks_round(seconds);
	state->flag1 = false;
	state->flag0 = true;
	state->timer24 = ticks;
	state->timer26 = ticks;
}

/* 654: void (real, real) */
// @retail 0x2aad30
void __stdcall function_2aad30(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		point_timer_set_value(arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e2e4 = { _hs_type_void, 0, function_2aad30, NULL, 2, { _hs_type_real, _hs_type_real } };

/* 655: void (real) */
// @retail 0x2aadb0
void __stdcall function_2aadb0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		short ticks = (short)game_seconds_to_ticks_round(*(real *)&arguments[0]);
		s_game_speed *state = g_510c5c;
		state->timer24 = ticks;
		state->timer26 = ticks;
		state->flags |= FLAG(1);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e2f8 = { _hs_type_void, 0, function_2aadb0, NULL, 1, { _hs_type_real } };

/* 664: void () */
// @retail 0x2aae20
void __stdcall function_2aae20(short function_index, long thread_index, bool initialize)
{
	scripted_hud_messages_clear();
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44e3ac = { _hs_type_void, 0, function_2aae20, NULL, 0 };

/* 665: void (hud_message) */
// @retail 0x2aae40
void __stdcall function_2aae40(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_24c831(*(short *)&arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e3bc = { _hs_type_void, 0, function_2aae40, NULL, 1, { _hs_type_hud_message } };

/* 666: void (hud_message) */
// @retail 0x2aae90
void __stdcall function_2aae90(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_24c878(*(short *)&arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e3d0 = { _hs_type_void, 0, function_2aae90, NULL, 1, { _hs_type_hud_message } };

/* 667: void (short, short) */
// @retail 0x2aaee0
void __stdcall function_2aaee0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_24c8e2(*(short *)&arguments[0], *(short *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e3e4 = { _hs_type_void, 0, function_2aaee0, NULL, 2, { _hs_type_short_integer, _hs_type_short_integer } };

/* 668: void (short, short) */
// @retail 0x2aaf30
void __stdcall function_2aaf30(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		short minutes = *(short *)&arguments[0];
		short seconds = *(short *)&arguments[1];
		((s_hud_state_view *)g_5023f4)->ticks139e = (short)((minutes * 60 + seconds) * g_510c54->ticks_per_second);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e3f8 = { _hs_type_void, 0, function_2aaf30, NULL, 2, { _hs_type_short_integer, _hs_type_short_integer } };

inline void hud_set_position(short x, short y, short corner)
{
	s_hud_state_view *hud = (s_hud_state_view *)g_5023f4;
	hud->value13a0 = x;
	hud->value13a2 = y;
	hud->corner13a4 = PIN(corner, 0, 4);
}

/* 669: void (short, short, hud_corner) */
// @retail 0x2aaf90
void __stdcall function_2aaf90(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		hud_set_position(*(short *)&arguments[0], *(short *)&arguments[1], *(short *)&arguments[2]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e40c = { _hs_type_void, 0, function_2aaf90, NULL, 3, { _hs_type_short_integer, _hs_type_short_integer, _hs_type_hud_corner } };

/* 670: void (boolean) */
// @retail 0x2ab010
void __stdcall function_2ab010(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		((s_hud_state_view *)g_5023f4)->flag13a7 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e424 = { _hs_type_void, 0, function_2ab010, NULL, 1, { _hs_type_boolean } };

/* 671: void (boolean) */
// @retail 0x2ab060
void __stdcall function_2ab060(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_24c93f(*(bool *)&arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e438 = { _hs_type_void, 0, function_2ab060, NULL, 1, { _hs_type_boolean } };

/* 672: short () */
// @retail 0x2ab0a0
void __stdcall function_2ab0a0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	s_hud_state_view *hud = (s_hud_state_view *)g_5023f4;
	short ticks = 0;
	if (hud->flag13a7)
	{
		ticks = hud->ticks139c;
		if (ticks != NONE && !hud->flag13a6)
			ticks = ticks + (short)hud->time1398 - (short)g_510c54->game_time;
	}
	short script_ticks;
	if (ticks == NONE)
	{
		script_ticks = NONE;
	}
	else
	{
		real ticks_real = (real)ticks * g_510c54->rate * 30.0f;
		long rounded;
		__asm
		{
			fld ticks_real
			fistp rounded
		}
		script_ticks = (short)rounded;
	}
	*(short *)&result = script_ticks;
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44e44c = { _hs_type_short_integer, 0, function_2ab0a0, NULL, 0 };

/* the evaluator of the script functions that do nothing in this build (68
   and some 300 others share it) */
// @retail 0x2ab130
void __stdcall function_2ab130(short function_index, long thread_index, bool initialize)
{
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44b588 = { _hs_type_void, 0, function_2ab130, NULL, 0 };

/* 704: void (short, real) */
// @retail 0x2ab140
void __stdcall function_2ab140(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		short index = *(short *)&arguments[0];
		s_timed_effect_view *globals = (s_timed_effect_view *)g_5093e0;
		if (globals)
		{
			if (index >= 0 && index < 4)
				globals->values180[index] = *(real *)&arguments[1];
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e6ec = { _hs_type_void, 0, function_2ab140, NULL, 2, { _hs_type_short_integer, _hs_type_real } };

/* 705: void (boolean) */
// @retail 0x2ab1a0
void __stdcall function_2ab1a0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		s_timed_effect_view *globals = (s_timed_effect_view *)g_5093e0;
		if (globals)
		{
			if (*(bool *)&arguments[0] || !globals->flagad)
			{
				memset(globals->unknown000, 0, sizeof(globals->unknown000));
				globals->flagad = true;
			}
			globals->flagac = true;
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e700 = { _hs_type_void, 0, function_2ab1a0, NULL, 1, { _hs_type_boolean } };

/* 706: void (real, real, real, real, real, real, real) */
// @retail 0x2ab210
void __stdcall function_2ab210(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_01fc30(*(real *)&arguments[0], *(real *)&arguments[1], *(real *)&arguments[2], *(real *)&arguments[3], *(real *)&arguments[4], *(real *)&arguments[5], *(real *)&arguments[6]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e714 = { _hs_type_void, 0, function_2ab210, NULL, 7, { _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real } };

inline void timed_effect_start_fade(real seconds, real value)
{
	s_timed_effect_view *globals = (s_timed_effect_view *)g_5093e0;
	if (globals)
	{
		double time = g_4858a0;
		globals->flag170 = true;
		globals->value174 = value;
		globals->start178 = (real)time;
		globals->end17c = (real)(seconds + time);
	}
}

/* 707: void (real) */
// @retail 0x2ab270
void __stdcall function_2ab270(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		timed_effect_start_fade(*(real *)&arguments[0], 1.0f);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e734 = { _hs_type_void, 0, function_2ab270, NULL, 1, { _hs_type_real } };

/* 708: void (real, real) */
// @retail 0x2ab2f0
void __stdcall function_2ab2f0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		timed_effect_start_fade(*(real *)&arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e748 = { _hs_type_void, 0, function_2ab2f0, NULL, 2, { _hs_type_real, _hs_type_real } };

/* 709: void () */
// @retail 0x2ab370
void __stdcall function_2ab370(short function_index, long thread_index, bool initialize)
{
	s_timed_effect_view *globals = (s_timed_effect_view *)g_5093e0;
	if (globals)
	{
		memset(globals, 0, sizeof(*globals));
		if (g_4b5690 && !g_4b569d)
			g_4b569d = true;
	}
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44e75c = { _hs_type_void, 0, function_2ab370, NULL, 0 };

/* 710: void (real) */
// @retail 0x2ab3b0
void __stdcall function_2ab3b0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		s_timed_effect_view *globals = (s_timed_effect_view *)g_5093e0;
		if (globals)
			globals->value190 = *(real *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e76c = { _hs_type_void, 0, function_2ab3b0, NULL, 1, { _hs_type_real } };

/* 711: void (real) */
// @retail 0x2ab400
void __stdcall function_2ab400(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		s_timed_effect_view *globals = (s_timed_effect_view *)g_5093e0;
		if (globals)
			globals->value194 = *(real *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e780 = { _hs_type_void, 0, function_2ab400, NULL, 1, { _hs_type_real } };

inline void timed_effect_start_blend(real a, real b, real seconds)
{
	s_timed_effect_view *globals = (s_timed_effect_view *)g_5093e0;
	if (globals)
	{
		globals->flag198 = true;
		globals->start1a4 = (real)g_4858a0;
		globals->value19c = a;
		globals->value1a0 = b;
		globals->end1a8 = (real)(seconds + g_4858a0);
	}
}

/* 712: void (real, real, real) */
// @retail 0x2ab450
void __stdcall function_2ab450(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		timed_effect_start_blend(*(real *)&arguments[0], *(real *)&arguments[1], *(real *)&arguments[2]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e794 = { _hs_type_void, 0, function_2ab450, NULL, 3, { _hs_type_real, _hs_type_real, _hs_type_real } };

/* 713: void (bitmap) */
// @retail 0x2ab4e0
void __stdcall function_2ab4e0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_0204b0(arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e7ac = { _hs_type_void, 0, function_2ab4e0, NULL, 1, { _hs_type_bitmap } };

/* 714: void () */
// @retail 0x2ab520
void __stdcall function_2ab520(short function_index, long thread_index, bool initialize)
{
	s_timed_effect_view *globals = (s_timed_effect_view *)g_5093e0;
	if (globals)
		globals->flag1ac = true;
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44e7c0 = { _hs_type_void, 0, function_2ab520, NULL, 0 };

inline void timed_effect_set_colors(real red0, real green0, real blue0, real alpha0, real red1, real green1, real blue1, real alpha1)
{
	s_timed_effect_view *globals = (s_timed_effect_view *)g_5093e0;
	if (globals)
	{
		globals->flag1b4 = true;
		globals->color1b8.red = red0;
		globals->color1b8.green = green0;
		globals->color1b8.blue = blue0;
		globals->color1b8.alpha = alpha0;
		globals->color1c8.red = red1;
		globals->color1c8.green = green1;
		globals->color1c8.blue = blue1;
		globals->color1c8.alpha = alpha1;
	}
}

/* 715: void (real, real, real, real, real, real, real, real) */
// @retail 0x2ab540
void __stdcall function_2ab540(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		timed_effect_set_colors(*(real *)&arguments[0], *(real *)&arguments[1], *(real *)&arguments[2], *(real *)&arguments[3], *(real *)&arguments[4], *(real *)&arguments[5], *(real *)&arguments[6], *(real *)&arguments[7]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e7d0 = { _hs_type_void, 0, function_2ab540, NULL, 8, { _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real } };

/* 716: void () */
// @retail 0x2ab600
void __stdcall function_2ab600(short function_index, long thread_index, bool initialize)
{
	s_timed_effect_view *globals = (s_timed_effect_view *)g_5093e0;
	if (globals)
		globals->flag1b4 = false;
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44e7f0 = { _hs_type_void, 0, function_2ab600, NULL, 0 };

/* 718: void (boolean, boolean) */
// @retail 0x2ab670
void __stdcall function_2ab670(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		g_4b72b1 = *(bool *)&arguments[1];
		g_4b72b0 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e818 = { _hs_type_void, 0, function_2ab670, NULL, 2, { _hs_type_boolean, _hs_type_boolean } };

/* the evaluator of the script functions with arguments that do nothing in
   this build (27, 81, 300, 319, 323, 579, 719, 723 and 781 share it) */
// @retail 0x2ab6c0
void __stdcall function_2ab6c0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
		function_209ae0(thread_index, 0);
}

hs_function_definition const g_44b254 = { _hs_type_void, 0, function_2ab6c0, NULL, 1, { _hs_type_string } };

/* 726: void (boolean) */
// @retail 0x2ab700
void __stdcall function_2ab700(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_187df0(*(bool *)&arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44e8bc = { _hs_type_void, 0, function_2ab700, NULL, 1, { _hs_type_boolean } };

/* 727: boolean () */
// @retail 0x2ab740
void __stdcall function_2ab740(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = function_187ec0();
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44e8d0 = { _hs_type_boolean, 0, function_2ab740, NULL, 0 };

/* 729: long (short) */
// @retail 0x2ab760
void __stdcall function_2ab760(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
		function_209ae0(thread_index, function_2a0960(*(short *)&arguments[0]));
}

hs_function_definition const g_44e8f4 = { _hs_type_long_integer, 0, function_2ab760, NULL, 1, { _hs_type_short_integer } };

/* 750: void () */
// @retail 0x2ab7a0
void __stdcall function_2ab7a0(short function_index, long thread_index, bool initialize)
{
	memset(g_502248, 0, 5 * sizeof(long));
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44ea94 = { _hs_type_void, 0, function_2ab7a0, NULL, 0 };

void function_22c041(long index);

/* 751: void (long_integer) */
// @retail 0x2ab7d0
void __stdcall function_2ab7d0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_22c041(arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44eaa4 = { _hs_type_void, 0, function_2ab7d0, NULL, 1, { _hs_type_long_integer } };

/* 752: void (long) */
// @retail 0x2ab810
void __stdcall function_2ab810(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long last = arguments[0];
		if (PIN(last, 0, 4) == last)
		{
			for (long i = 0; i <= last; i++)
				g_502248[i] = 2;
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44eab8 = { _hs_type_void, 0, function_2ab810, NULL, 1, { _hs_type_long_integer } };

/* 753: void (boolean) */
// @retail 0x2ab880
void __stdcall function_2ab880(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		((s_input_globals_view *)&g_4e61b8)->feedback_suppressed = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44eacc = { _hs_type_void, 0, function_2ab880, NULL, 1, { _hs_type_boolean } };

/* 780: long_integer () */
// @retail 0x2ab910
void __stdcall function_2ab910(short function_index, long thread_index, bool initialize)
{
	function_209ae0(thread_index, bink_playback_ticks_remaining());
}

hs_function_definition const g_44ecb4 = { _hs_type_long_integer, 0, function_2ab910, NULL, 0 };

/* stops an interpolator; retail function not identified */
inline long interpolator_stop(long name)
{
	long index = NONE;
	s_interpolator_state *state = interpolator_get(name, &index);
	if (state)
		state->active = false;
	return index;
}

/* sets an interpolator's value; retail function not identified */
inline long interpolator_set(long name, real value)
{
	long index = NONE;
	s_interpolator_state *state = interpolator_get(name, &index);
	if (state)
	{
		state->value = value;
		state->active = true;
	}
	return index;
}

/* 847: long_integer (string_id, real, real) */
// @retail 0x2ab9c0
void __stdcall function_2ab9c0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_209ae0(thread_index, interpolator_start(arguments[0], *(real *)&arguments[1], *(real *)&arguments[2]));
	}
}

hs_function_definition const g_44f1c0 = { _hs_type_long_integer, 0, function_2ab9c0, NULL, 3, { _hs_type_string_id, _hs_type_real, _hs_type_real } };

/* 848: long_integer (string_id, real, real) */
// @retail 0x2aba10
void __stdcall function_2aba10(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_209ae0(thread_index, function_135180(arguments[0], *(real *)&arguments[1], *(real *)&arguments[2]));
	}
}

hs_function_definition const g_44f1d8 = { _hs_type_long_integer, 0, function_2aba10, NULL, 3, { _hs_type_string_id, _hs_type_real, _hs_type_real } };

/* 849: long_integer (string_id) */
// @retail 0x2aba60
void __stdcall function_2aba60(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_209ae0(thread_index, interpolator_stop(arguments[0]));
	}
}

hs_function_definition const g_44f1f0 = { _hs_type_long_integer, 0, function_2aba60, NULL, 1, { _hs_type_string_id } };

/* 850: long_integer (string_id) */
// @retail 0x2abac0
void __stdcall function_2abac0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_209ae0(thread_index, interpolator_resume(arguments[0]));
	}
}

hs_function_definition const g_44f204 = { _hs_type_long_integer, 0, function_2abac0, NULL, 1, { _hs_type_string_id } };

/* 851: boolean (string_id); 852: boolean (string_id) */
// @retail 0x2abb00
void __stdcall function_2abb00(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = interpolator_exists(arguments[0]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44f218 = { _hs_type_boolean, 0, function_2abb00, NULL, 1, { _hs_type_string_id } };
hs_function_definition const g_44f22c = { _hs_type_boolean, 0, function_2abb00, NULL, 1, { _hs_type_string_id } };

/* 853: long_integer (string_id, real) */
// @retail 0x2abb50
void __stdcall function_2abb50(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_209ae0(thread_index, interpolator_set(arguments[0], *(real *)&arguments[1]));
	}
}

hs_function_definition const g_44f240 = { _hs_type_long_integer, 0, function_2abb50, NULL, 2, { _hs_type_string_id, _hs_type_real } };

/* 857: real (string_id) */
// @retail 0x2abd30
void __stdcall function_2abd30(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
		hs_return_real(thread_index, interpolator_get_value18(arguments[0]));
}

hs_function_definition const g_44f290 = { _hs_type_real, 0, function_2abd30, NULL, 1, { _hs_type_string_id } };

/* 859: real (string_id) */
// @retail 0x2abdd0
void __stdcall function_2abdd0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
		hs_return_real(thread_index, interpolator_get_time10(arguments[0]));
}

hs_function_definition const g_44f2b8 = { _hs_type_real, 0, function_2abdd0, NULL, 1, { _hs_type_string_id } };

/* 860: real (string_id) */
// @retail 0x2abe20
void __stdcall function_2abe20(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
		hs_return_real(thread_index, interpolator_get_end_time(arguments[0]));
}

hs_function_definition const g_44f2cc = { _hs_type_real, 0, function_2abe20, NULL, 1, { _hs_type_string_id } };

/* 865: void () */
// @retail 0x2ac020
void __stdcall function_2ac020(short function_index, long thread_index, bool initialize)
{
	function_135750();
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44f340 = { _hs_type_void, 0, function_2ac020, NULL, 0 };

/* 866: void () */
// @retail 0x2ac040
void __stdcall function_2ac040(short function_index, long thread_index, bool initialize)
{
	function_135790();
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44f350 = { _hs_type_void, 0, function_2ac040, NULL, 0 };

/* 867: void () */
// @retail 0x2ac060
void __stdcall function_2ac060(short function_index, long thread_index, bool initialize)
{
	function_135820();
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44f360 = { _hs_type_void, 0, function_2ac060, NULL, 0 };

/* 869: void (real) */
// @retail 0x2ac080
void __stdcall function_2ac080(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_2a0aa0(*(real *)&arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f384 = { _hs_type_void, 0, function_2ac080, NULL, 1, { _hs_type_real } };

/* 870: void (real) */
// @retail 0x2ac0c0
void __stdcall function_2ac0c0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_2a0b20(*(real *)&arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f398 = { _hs_type_void, 0, function_2ac0c0, NULL, 1, { _hs_type_real } };

/* 871: void (real, real) */
// @retail 0x2ac100
void __stdcall function_2ac100(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_2a0ba0(*(real *)&arguments[0], *(real *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f3ac = { _hs_type_void, 0, function_2ac100, NULL, 2, { _hs_type_real, _hs_type_real } };

/* 873: void (object, string_id, string_id) */
// @retail 0x2ac150
void __stdcall function_2ac150(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_13c250(arguments[0], arguments[1], arguments[2]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f3d0 = { _hs_type_void, 0, function_2ac150, NULL, 3, { _hs_type_object, _hs_type_string_id, _hs_type_string_id } };

/* 875: void (boolean) */
// @retail 0x2ac1f0
void __stdcall function_2ac1f0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		g_5107ee = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f400 = { _hs_type_void, 0, function_2ac1f0, NULL, 1, { _hs_type_boolean } };

inline bool function_2ac270_get(long index)
{
	if (index >= 0 && index < 15 && g_4e6948->state == 1)
		return g_4f55dc[index];
	return false;
}

/* 877: boolean (long) */
// @retail 0x2ac270
void __stdcall function_2ac270(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		*(bool *)&result = function_2ac270_get(arguments[0]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44f428 = { _hs_type_boolean, 0, function_2ac270, NULL, 1, { _hs_type_long_integer } };

struct s_sound_driver_volumes;
void function_220fd0(s_sound_driver_volumes const *volumes);

/* retail function not identified */
inline void looping_sound_set_volumes(real a, real b, real c, real d, long e, real f)
{
	s_looping_sound_globals *globals = g_4ed288;

	globals->gains[0] = a;
	globals->gains[1] = b;
	globals->gains[2] = c;
	globals->gains[3] = d;
	globals->value238 = e;
	globals->value23c = f;
	function_220fd0((s_sound_driver_volumes const *)globals->gains);
}

/* 878: void (real, real, real, real, long_integer, real) */
// @retail 0x2ac2f0
void __stdcall function_2ac2f0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		looping_sound_set_volumes(*(real *)&arguments[0], *(real *)&arguments[1], *(real *)&arguments[2], *(real *)&arguments[3], arguments[4], *(real *)&arguments[5]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f43c = { _hs_type_void, 0, function_2ac2f0, NULL, 6, { _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_long_integer, _hs_type_real } };

/* 879: void () */
// @retail 0x2ac380
void __stdcall function_2ac380(short function_index, long thread_index, bool initialize)
{
	function_1915f0();
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44f458 = { _hs_type_void, 0, function_2ac380, NULL, 0 };

/* 880: void () */
// @retail 0x2ac3a0
void __stdcall function_2ac3a0(short function_index, long thread_index, bool initialize)
{
	sequence_start_stage(4);
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44f468 = { _hs_type_void, 0, function_2ac3a0, NULL, 0 };

/* 881: void () */
// @retail 0x2ac3e0
void __stdcall function_2ac3e0(short function_index, long thread_index, bool initialize)
{
	((s_510c50_view *)g_510c50)->flag22 = true;
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44f478 = { _hs_type_void, 0, function_2ac3e0, NULL, 0 };

/* 882: void (boolean) */
// @retail 0x2ac400
void __stdcall function_2ac400(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		g_4ed288->flag240 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f488 = { _hs_type_void, 0, function_2ac400, NULL, 1, { _hs_type_boolean } };

/* 883: void (real) */
// @retail 0x2ac450
void __stdcall function_2ac450(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		real value = *(real *)&arguments[0];
		value = PIN(value, 0.0f, 5.0f);
		((s_timed_effect_view *)g_5093e0)->value3f8 = value;
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f49c = { _hs_type_void, 0, function_2ac450, NULL, 1, { _hs_type_real } };

void function_16e510(long index, short bsp_index, bool sections);

/* 885: void (structure_bsp, long_integer, boolean) */
// @retail 0x2ac500
void __stdcall function_2ac500(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_16e510(arguments[1], *(short *)&arguments[0], *(bool *)&arguments[2]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f4c4 = { _hs_type_void, 0, function_2ac500, NULL, 3, { _hs_type_structure_bsp, _hs_type_long_integer, _hs_type_boolean } };

void function_16e5a0(long cluster_index, short bsp_index);

/* 886: void (structure_bsp, long_integer) */
// @retail 0x2ac550
void __stdcall function_2ac550(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		function_16e5a0(arguments[1], *(short *)&arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f4dc = { _hs_type_void, 0, function_2ac550, NULL, 2, { _hs_type_structure_bsp, _hs_type_long_integer } };

/* a bitmap tag's bitmaps (0x74 bytes each) */
struct s_bitmap_data;

struct s_bitmap_group_view
{
	byte unknown00[0x44];
	long bitmap_count;
	byte *bitmaps;
};

bool texture_cache_bitmap_request(s_bitmap_data *bitmap);

/* 887: void (bitmap, long_integer) */
// @retail 0x2ac5a0
void __stdcall function_2ac5a0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		long bitmap_index = arguments[0];
		if (bitmap_index != NONE)
		{
			s_bitmap_group_view *group = (s_bitmap_group_view *)g_4e3b44[bitmap_index & 0xffff].bytes;
			texture_cache_bitmap_request((s_bitmap_data *)(group->bitmaps + arguments[1] * 0x74));
		}
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f4f0 = { _hs_type_void, 0, function_2ac5a0, NULL, 2, { _hs_type_bitmap, _hs_type_long_integer } };

/* 888: long () */
// @retail 0x2ac600
void __stdcall function_2ac600(short function_index, long thread_index, bool initialize)
{
	function_209ae0(thread_index, 0x37);
}

hs_function_definition const g_44f504 = { _hs_type_long_integer, 0, function_2ac600, NULL, 0 };

/* 889: void (boolean) */
// @retail 0x2ac620
void __stdcall function_2ac620(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		g_5093e4->flag0 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f514 = { _hs_type_void, 0, function_2ac620, NULL, 1, { _hs_type_boolean } };

/* 890: void (boolean) */
// @retail 0x2ac660
void __stdcall function_2ac660(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		g_5093e4->flag1 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f528 = { _hs_type_void, 0, function_2ac660, NULL, 1, { _hs_type_boolean } };

/* 891: void (real) */
// @retail 0x2ac6b0
void __stdcall function_2ac6b0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		g_5093e4->value4 = *(real *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f53c = { _hs_type_void, 0, function_2ac6b0, NULL, 1, { _hs_type_real } };

/* 892: void (real) */
// @retail 0x2ac700
void __stdcall function_2ac700(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		g_5093e4->value8 = *(real *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f550 = { _hs_type_void, 0, function_2ac700, NULL, 1, { _hs_type_real } };

/* 893: void (real) */
// @retail 0x2ac750
void __stdcall function_2ac750(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		g_5093e4->valuec = *(real *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f564 = { _hs_type_void, 0, function_2ac750, NULL, 1, { _hs_type_real } };

/* 894: void (real) */
// @retail 0x2ac7a0
void __stdcall function_2ac7a0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		g_5093e4->value10 = *(real *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f578 = { _hs_type_void, 0, function_2ac7a0, NULL, 1, { _hs_type_real } };

/* 895: void (real) */
// @retail 0x2ac7f0
void __stdcall function_2ac7f0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		g_5093e4->value14 = *(real *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f58c = { _hs_type_void, 0, function_2ac7f0, NULL, 1, { _hs_type_real } };

/* 896: void (boolean) */
// @retail 0x2ac840
void __stdcall function_2ac840(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		g_5093e4->flag18 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f5a0 = { _hs_type_void, 0, function_2ac840, NULL, 1, { _hs_type_boolean } };

/* 897: void (boolean) */
// @retail 0x2ac890
void __stdcall function_2ac890(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		g_5093e4->flag19 = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f5b4 = { _hs_type_void, 0, function_2ac890, NULL, 1, { _hs_type_boolean } };

/* 898: void (boolean) */
// @retail 0x2ac8e0
void __stdcall function_2ac8e0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		g_5093e4->flag1a = *(bool *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f5c8 = { _hs_type_void, 0, function_2ac8e0, NULL, 1, { _hs_type_boolean } };

/* 899: void (real) */
// @retail 0x2ac930
void __stdcall function_2ac930(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		g_5093e4->value1c = *(real *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f5dc = { _hs_type_void, 0, function_2ac930, NULL, 1, { _hs_type_real } };

/* 900: void (real) */
// @retail 0x2ac980
void __stdcall function_2ac980(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		g_5093e4->value20 = *(real *)&arguments[0];
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44f5f0 = { _hs_type_void, 0, function_2ac980, NULL, 1, { _hs_type_real } };

/* 901: void () */
// @retail 0x2ac9d0
void __stdcall function_2ac9d0(short function_index, long thread_index, bool initialize)
{
	g_4ea936 = true;
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44f604 = { _hs_type_void, 0, function_2ac9d0, NULL, 0 };

/* 902: void () */
// @retail 0x2ac9f0
void __stdcall function_2ac9f0(short function_index, long thread_index, bool initialize)
{
	g_4ed288->flag241 = true;
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44f614 = { _hs_type_void, 0, function_2ac9f0, NULL, 0 };

/* 906: void () */
// @retail 0x2aca10
void __stdcall function_2aca10(short function_index, long thread_index, bool initialize)
{
	g_509415 = false;
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44f660 = { _hs_type_void, 0, function_2aca10, NULL, 0 };

/* 907: void () */
// @retail 0x2aca30
void __stdcall function_2aca30(short function_index, long thread_index, bool initialize)
{
	g_509415 = true;
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44f670 = { _hs_type_void, 0, function_2aca30, NULL, 0 };
