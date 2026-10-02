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
	union
	{
		dword flags;
		struct
		{
			dword : 17;
			dword flag17 : 1;
			dword : 14;
		};
	};
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

inline s_object *object_get(long object_index)
{
	return ((s_object_header *)g_4e0300->data)[object_index & 0xffff].object;
}

s_object *function_badc0(long object_index, dword type_mask);

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
};

/* the random seeds (146240) */
struct s_random_globals;
extern s_random_globals *g_4e7408;

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

/* the game options (globals.h) beyond s_game_options_view */
struct s_game_options_hs_view
{
	byte unknown00[8];
	long state;
	byte unknown0c[0x130 - 0xc];
	bool flag130;
	byte unknown131;
	short difficulty;
	bool flag134;
	byte unknown135[0x11fa - 0x135];
	short value11fa;
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

/* g_4ed288 (03d380) */
struct s_4ed288;
extern s_4ed288 *g_4ed288;
extern byte g_4ea936;

struct s_4ed288_view
{
	byte unknown000[0x241];
	bool flag241;
};

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
long g_4701f0;
long g_4701f4;
short g_4701f8;
byte g_547f6f;
byte g_547f70;
byte g_547f74;
byte g_547f75;
long *g_502248;
byte g_509415;
bool *g_4f55d0;
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

inline void object_set_flags_c0_bit10(long object_index, bool value)
{
	if (object_index != NONE)
	{
		s_object *object = object_get(object_index);
		SET_FLAG(object->flags_c0, 10, value);
	}
}

/* 64: void (object, boolean) */
// @retail 0x2a1850
void __stdcall function_2a1850(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		object_set_flags_c0_bit10(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b538 = { _hs_type_void, 0, function_2a1850, NULL, 2, { _hs_type_object, _hs_type_boolean } };

inline void object_set_flags19_bit1(long object_index, bool value)
{
	if (object_index != NONE)
	{
		s_object *object = object_get(object_index);
		SET_FLAG(object->flags19, 1, value);
	}
}

/* 66: void (object, boolean) */
// @retail 0x2a1920
void __stdcall function_2a1920(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		object_set_flags19_bit1(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b560 = { _hs_type_void, 0, function_2a1920, NULL, 2, { _hs_type_object, _hs_type_boolean } };

inline void object_set_flags19_bit0(long object_index, bool value)
{
	if (object_index != NONE)
	{
		s_object *object = object_get(object_index);
		SET_FLAG(object->flags19, 0, value);
	}
}

/* 67: void (object, boolean) */
// @retail 0x2a19a0
void __stdcall function_2a19a0(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		object_set_flags19_bit0(arguments[0], *(bool *)&arguments[1]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b574 = { _hs_type_void, 0, function_2a19a0, NULL, 2, { _hs_type_object, _hs_type_boolean } };

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

inline void object_set_flags_c0_bit7(long object_index, bool value)
{
	if (object_index != NONE)
	{
		s_object *object = object_get(object_index);
		SET_FLAG(object->flags_c0, 7, !value);
	}
}

/* 72: void (object, boolean) */
// @retail 0x2a1b50
void __stdcall function_2a1b50(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		object_set_flags_c0_bit7(arguments[0], *(bool *)&arguments[1]);
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

inline void object_set_flag17(long object_index)
{
	if (object_index != NONE)
		object_get(object_index)->flag17 = true;
}

/* 80: void (object) */
// @retail 0x2a1e20
void __stdcall function_2a1e20(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		object_set_flag17(arguments[0]);
		function_209ae0(thread_index, 0);
	}
}

hs_function_definition const g_44b684 = { _hs_type_void, 0, function_2a1e20, NULL, 1, { _hs_type_object } };

inline void object_set_flags_10a_bit14(long object_index, bool value)
{
	if (object_index != NONE)
	{
		s_object *object = object_get(object_index);
		SET_FLAG(object->flags_10a, 14, value);
	}
}

/* 85: void (object, boolean) */
// @retail 0x2a1f90
void __stdcall function_2a1f90(short function_index, long thread_index, bool initialize)
{
	hs_function_definition *definition = hs_function_get(function_index);
	long *arguments = hs_macro_function_evaluate(thread_index, definition->parameter_count, definition->parameter_types, initialize);
	if (arguments)
	{
		object_set_flags_10a_bit14(arguments[0], *(bool *)&arguments[1]);
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

inline void object_set_shield_fraction(long object_index, real fraction)
{
	if (object_index != NONE)
	{
		s_object *object = object_get(object_index);
		object->shield_vitality = object->maximum_vitality * PIN(fraction, 0.0f, 1.0f);
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

inline short random_range(short lower, short upper)
{
	dword *seed = (dword *)g_4e7408;
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
		*(short *)&result = random_range(*(short *)&arguments[0], *(short *)&arguments[1]);
		function_209ae0(thread_index, result);
	}
}

hs_function_definition const g_44ba04 = { _hs_type_short_integer, 0, function_2a29d0, NULL, 2, { _hs_type_short_integer, _hs_type_short_integer } };

inline real real_random_range(real lower, real upper)
{
	dword *seed = (dword *)g_4e7408;
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

/* 279: boolean () */
// @retail 0x2a4d40
void __stdcall function_2a4d40(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = *g_4f55d0;
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44c660 = { _hs_type_boolean, 0, function_2a4d40, NULL, 0 };

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

inline short game_difficulty_get_real(void)
{
	s_game_options_hs_view *options = (s_game_options_hs_view *)g_4e6948;
	if (options->state == 1)
	{
		short difficulty = options->difficulty;
		if (difficulty > 1)
			return difficulty;
	}
	return 1;
}

/* 468: game_difficulty () */
// @retail 0x2a8d30
void __stdcall function_2a8d30(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(short *)&result = game_difficulty_get_real();
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44d550 = { _hs_type_game_difficulty, 0, function_2a8d30, NULL, 0 };

/* 469: game_difficulty () */
// @retail 0x2a8d70
void __stdcall function_2a8d70(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	s_game_options_hs_view *options = (s_game_options_hs_view *)g_4e6948;
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
	((s_game_options_hs_view *)g_4e6948)->value11fa = 0;
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44d598 = { _hs_type_void, 0, function_2a8e70, NULL, 0 };

/* 473: void () */
// @retail 0x2a8e90
void __stdcall function_2a8e90(short function_index, long thread_index, bool initialize)
{
	s_4ed284_view *globals = (s_4ed284_view *)g_4ed284;
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
	globals->bitsc.bit12 = true;
	globals->bitsc.bit13 = true;
	globals->flags4 |= FLAG(21);
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44d774 = { _hs_type_void, 0, function_2a9470, NULL, 0 };

/* 502: void () */
// @retail 0x2a94b0
void __stdcall function_2a94b0(short function_index, long thread_index, bool initialize)
{
	s_4ed284_view *globals = (s_4ed284_view *)g_4ed284;
	globals->bitsc.bit12 = true;
	globals->bitsc.bit13 = true;
	globals->flags4 |= FLAG(22);
	function_209ae0(thread_index, 0);
}

hs_function_definition const g_44d784 = { _hs_type_void, 0, function_2a94b0, NULL, 0 };

/* 503: void () */
// @retail 0x2a94f0
void __stdcall function_2a94f0(short function_index, long thread_index, bool initialize)
{
	s_4ed284_view *globals = (s_4ed284_view *)g_4ed284;
	globals->bitsc.bit12 = false;
	globals->bitsc.bit13 = false;
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
	s_game_options_hs_view *options = (s_game_options_hs_view *)g_4e6948;
	*(bool *)&result = options->state == 1 && options->flag134;
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44dc60 = { _hs_type_boolean, 0, function_2a9a30, NULL, 0 };

/* 570: boolean () */
// @retail 0x2a9a70
void __stdcall function_2a9a70(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = ((s_game_options_hs_view *)g_4e6948)->flag130;
	function_209ae0(thread_index, result);
}

hs_function_definition const g_44dc70 = { _hs_type_boolean, 0, function_2a9a70, NULL, 0 };

/* starts stage of the sequence g_4701ec, unless it is already past it */
inline void sequence_start_stage(long stage)
{
	if ((long)g_4701ec < stage)
	{
		g_4701ec = stage;
		g_4701f0 = 0;
		g_4701f4 = g_510c54->game_time;
		g_4701f8 = 0;
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
	g_4701ec = 0;
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

inline bool function_2a9d10_active(void)
{
	return g_547f70 || g_4701ec != 0;
}

/* 592: boolean () */
// @retail 0x2a9d10
void __stdcall function_2a9d10(short function_index, long thread_index, bool initialize)
{
	long result = 0;
	*(bool *)&result = function_2a9d10_active();
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
	((s_4ed288_view *)g_4ed288)->flag241 = true;
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
