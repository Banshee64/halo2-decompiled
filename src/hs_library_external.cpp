// @flags /O2 /Ob1 /arch:SSE /Gr
/* HS_LIBRARY_EXTERNAL.CPP: the evaluators of the script functions that call
   into the game, each followed by its definition (an entry of the function
   table g_4744e0, named by its .rdata address). The comment above each
   evaluator gives its index in the function table and its signature. */

#include "cseries.h"
#include "globals.h"
#include "game_state.h"
#include "hs_library_external.h"
#include "unknown_1eb550.h"
#include <string.h>

#define FLAG(bit) (1 << (bit))
#define SET_FLAG(flags, bit, value) ((value) ? ((flags) |= FLAG(bit)) : ((flags) &= ~FLAG(bit)))
#define PIN(x, lo, hi) ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))
#define NUMBEROF(array) (sizeof(array) / sizeof((array)[0]))

/* the objects (a local view; 0bad50 and 0bb760 keep their own) */
struct s_object
{
	byte unknown00[4];
	dword flags;
	byte unknown08[0xc];
	long parent_index;
	byte unknown18;
	byte flags19;
	byte unknown1a[0xc0 - 0x1a];
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
	byte unknown000[0x134];
	union
	{
		dword unit_flags;
		struct
		{
			dword : 12;
			dword unit_flag12 : 1;
			dword : 19;
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

/* the flags of g_4ed284 (185ab0 keeps its own view) */
struct s_unknown_185ab0;
extern s_unknown_185ab0 *g_4ed284;

struct s_4ed284_flags
{
	dword bit0 : 1;
	dword bit1 : 1;
	dword bit2 : 1;
	dword bit3 : 1;
	dword bit4 : 1;
	dword bit5 : 1;
	dword bit6 : 1;
	dword bit7 : 1;
	dword bit8 : 1;
	dword bit9 : 1;
	dword bit10 : 1;
	dword bit11 : 1;
	dword bit12 : 1;
	dword bit13 : 1;
	dword bit14 : 1;
	dword bit15 : 1;
	dword bit16 : 1;
	dword bit17 : 1;
	dword bit18 : 1;
	dword bit19 : 1;
	dword bit20 : 1;
	dword bit21 : 1;
	dword bit22 : 1;
	dword bit23 : 1;
	dword bit24 : 1;
	dword : 7;
};

struct s_4ed284_entry
{
	byte unknown00[0x32];
	short index;
	byte unknown34[0x8d - 0x34];
	bool flag;
	byte unknown8e[0x94 - 0x8e];
};

struct s_4ed284_view
{
	byte unknown00[4];
	union
	{
		dword flags4;
		s_4ed284_flags bits4;
	};
	union
	{
		dword flags8;
		s_4ed284_flags bits8;
	};
	union
	{
		dword flagsc;
		s_4ed284_flags bitsc;
	};
	s_4ed284_entry entries[4];
};

/* a state at g_510c6c (1552e0) */
struct s_unknown_78;
extern s_unknown_78 *g_510c6c;

struct s_510c6c_view
{
	byte unknown00[0x14];
	long value14;
};

/* a state at g_510c50 (13bf00) */
struct s_unknown_13bf00;
extern s_unknown_13bf00 *g_510c50;

struct s_510c50_view
{
	byte unknown00[6];
	bool flag6;
	byte unknown07[0x22 - 7];
	bool flag22;
};

/* the timed effect globals (01fbb0) */
struct s_timed_effect_globals;
extern s_timed_effect_globals *g_5093e0;
extern dword g_4b5690;
extern byte g_4b569d;

struct s_timed_effect_view
{
	byte unknown000[0x1ac];
	bool flag1ac;
	byte unknown1ad[0x1b4 - 0x1ad];
	bool flag1b4;
	byte unknown1b5[0x3fc - 0x1b5];
};

/* the hud state (24c7c1) */
struct s_hud_state;
extern s_hud_state *g_5023f4;

struct s_hud_state_view
{
	byte unknown0000[0x1380];
	long time1380;
	bool flag1384;
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
	byte unknown00[8];
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

/* the command scripts (0xd4 bytes each) and the one being run */
struct s_command_script
{
	byte unknown00[4];
	short type;
	short value_short;
	real value8;
	real valuec;
	byte unknown10[0x28 - 0x10];
	long index_a;
	long index_b;
	byte unknown30[0x45 - 0x30];
	bool flag45;
	bool flag46;
	byte unknown47;
	short type48;
	byte unknown4a[2];
	long index4c;
	byte unknown50[0x5c - 0x50];
	bool flag5c;
	byte unknown5d[3];
	real value60;
	bool flag64;
	byte unknown65[3];
	real value68;
	bool flag6c;
	byte unknown6d[3];
	real value70;
	bool flag74;
	bool flag75;
	bool flag76;
	byte unknown77[2];
	bool flag79;
	bool flag7a;
	byte unknown7b;
	short value7c;
	byte unknown7e;
	bool flag7f;
	bool flag80;
	bool flag81;
	bool flag82;
	bool flag83;
	short value84;
	bool flag86;
	byte unknown87;
	long style88;
	byte unknown8c[0xd4 - 0x8c];
};

long g_502410;

/* the command script setters below stand in for the script commands' own
   setters, which retail inlines into each evaluator. Which retail function
   each call mirrors is not identified, except that
   command_script_set_point(1, index, 0.0f) is 0x276990. */
inline s_command_script *command_script_get(long index)
{
	return &((s_command_script *)g_502408->data)[index & 0xffff];
}

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

/* 468: game_difficulty () */
// @retail 0x2a8d30
void __stdcall function_2a8d30(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	s_game_options_view *options = g_4e6948;
	if (options->state == 1)
		*(short *)&result = options->difficulty > 1 ? options->difficulty : 1;
	else
		*(short *)&result = 1;
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
	s_4ed284_view *globals = (s_4ed284_view *)g_4ed284;
	globals->entries[0].index = NONE;
	globals->entries[0].flag = false;
	globals->entries[1].index = NONE;
	globals->entries[1].flag = false;
	globals->entries[2].index = NONE;
	globals->entries[2].flag = false;
	globals->entries[3].index = NONE;
	globals->entries[3].flag = false;
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44d5a8 = { _hs_type_void, 0, function_2a8e90, NULL, 0 };

/* 479: void () */
// @retail 0x2a9020
void __stdcall function_2a9020(short function_index, long thread_index, bool initialize)
{
	s_4ed284_view *globals = (s_4ed284_view *)g_4ed284;
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
	*(bool *)&result = TEST_FIELD_BIT(((s_4ed284_view *)g_4ed284)->bits4.bit1);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d624 = { _hs_type_boolean, 0, function_2a9040, NULL, 0 };

/* 481: boolean () */
// @retail 0x2a9070
void __stdcall function_2a9070(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(((s_4ed284_view *)g_4ed284)->bits4.bit4);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d634 = { _hs_type_boolean, 0, function_2a9070, NULL, 0 };

/* 482: boolean () */
// @retail 0x2a90a0
void __stdcall function_2a90a0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(((s_4ed284_view *)g_4ed284)->bits4.bit5);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d644 = { _hs_type_boolean, 0, function_2a90a0, NULL, 0 };

/* 483: boolean () */
// @retail 0x2a90d0
void __stdcall function_2a90d0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(((s_4ed284_view *)g_4ed284)->bits4.bit20);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d654 = { _hs_type_boolean, 0, function_2a90d0, NULL, 0 };

/* 484: boolean () */
// @retail 0x2a9100
void __stdcall function_2a9100(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(((s_4ed284_view *)g_4ed284)->bits4.bit9);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d664 = { _hs_type_boolean, 0, function_2a9100, NULL, 0 };

/* 485: boolean () */
// @retail 0x2a9130
void __stdcall function_2a9130(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(((s_4ed284_view *)g_4ed284)->bits4.bit7);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d674 = { _hs_type_boolean, 0, function_2a9130, NULL, 0 };

/* 486: boolean () */
// @retail 0x2a9160
void __stdcall function_2a9160(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(((s_4ed284_view *)g_4ed284)->bits4.bit8);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d684 = { _hs_type_boolean, 0, function_2a9160, NULL, 0 };

/* 487: boolean () */
// @retail 0x2a9190
void __stdcall function_2a9190(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(((s_4ed284_view *)g_4ed284)->bits4.bit6);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d694 = { _hs_type_boolean, 0, function_2a9190, NULL, 0 };

/* 488: boolean () */
// @retail 0x2a91c0
void __stdcall function_2a91c0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	s_4ed284_view *globals = (s_4ed284_view *)g_4ed284;
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
	s_4ed284_view *globals = (s_4ed284_view *)g_4ed284;
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
	s_4ed284_view *globals = (s_4ed284_view *)g_4ed284;
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
	*(bool *)&result = TEST_FIELD_BIT(((s_4ed284_view *)g_4ed284)->bits4.bit10);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d6d4 = { _hs_type_boolean, 0, function_2a9280, NULL, 0 };

/* 492: boolean () */
// @retail 0x2a92b0
void __stdcall function_2a92b0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(((s_4ed284_view *)g_4ed284)->bits4.bit11);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d6e4 = { _hs_type_boolean, 0, function_2a92b0, NULL, 0 };

/* 493: boolean () */
// @retail 0x2a92e0
void __stdcall function_2a92e0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(((s_4ed284_view *)g_4ed284)->bits4.bit12);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d6f4 = { _hs_type_boolean, 0, function_2a92e0, NULL, 0 };

/* 494: boolean () */
// @retail 0x2a9310
void __stdcall function_2a9310(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(((s_4ed284_view *)g_4ed284)->bits4.bit13);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d704 = { _hs_type_boolean, 0, function_2a9310, NULL, 0 };

/* 495: boolean () */
// @retail 0x2a9340
void __stdcall function_2a9340(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = (((s_4ed284_view *)g_4ed284)->flags4 & 0x3c00) != 0;
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d714 = { _hs_type_boolean, 0, function_2a9340, NULL, 0 };

/* 496: boolean () */
// @retail 0x2a9370
void __stdcall function_2a9370(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = (((s_4ed284_view *)g_4ed284)->flags4 & 0x3c000) != 0;
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d724 = { _hs_type_boolean, 0, function_2a9370, NULL, 0 };

/* 497: boolean () */
// @retail 0x2a93a0
void __stdcall function_2a93a0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(((s_4ed284_view *)g_4ed284)->bits4.bit18);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d734 = { _hs_type_boolean, 0, function_2a93a0, NULL, 0 };

/* 498: boolean () */
// @retail 0x2a93d0
void __stdcall function_2a93d0(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(((s_4ed284_view *)g_4ed284)->bits4.bit19);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d744 = { _hs_type_boolean, 0, function_2a93d0, NULL, 0 };

/* 501: void () */
// @retail 0x2a9470
void __stdcall function_2a9470(short function_index, long thread_index, bool initialize)
{
	s_4ed284_view *globals = (s_4ed284_view *)g_4ed284;
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
	s_4ed284_view *globals = (s_4ed284_view *)g_4ed284;
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
	s_4ed284_view *globals = (s_4ed284_view *)g_4ed284;
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
	*(bool *)&result = TEST_FIELD_BIT(((s_4ed284_view *)g_4ed284)->bits4.bit23);
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d7a4 = { _hs_type_boolean, 0, function_2a9520, NULL, 0 };

/* 505: boolean () */
// @retail 0x2a9550
void __stdcall function_2a9550(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = TEST_FIELD_BIT(((s_4ed284_view *)g_4ed284)->bits4.bit24);
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

/* the evaluator of the script functions that do nothing in this build (68
   and some 300 others share it) */
// @retail 0x2ab130
void __stdcall function_2ab130(short function_index, long thread_index, bool initialize)
{
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44b588 = { _hs_type_void, 0, function_2ab130, NULL, 0 };

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

/* 750: void () */
// @retail 0x2ab7a0
void __stdcall function_2ab7a0(short function_index, long thread_index, bool initialize)
{
	memset(g_502248, 0, 5 * sizeof(long));
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44ea94 = { _hs_type_void, 0, function_2ab7a0, NULL, 0 };

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

/* 888: long () */
// @retail 0x2ac600
void __stdcall function_2ac600(short function_index, long thread_index, bool initialize)
{
	function_209ae0(thread_index, 0x37);
}

hs_function_definition const g_44f504 = { _hs_type_long_integer, 0, function_2ac600, NULL, 0 };

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
