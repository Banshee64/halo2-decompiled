// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0AA8E0.CPP: the writing of a scenario object name into a simulation
   event as its index in the scenario's sorted block (outside lane J's region;
   the event encodings of src/unknown_09a5e0.cpp call it, and inline the
   reading back) */

#include "unknown_11c920.h"
#include "globals.h"
#include "bitstream.h"

typedef long (__stdcall *t_bsearch_compare_function)(const void *, const void *, const void *);
long function_13ddd0(const void *key, const void *base, long count, long element_size, t_bsearch_compare_function compare, const void *context);
/* 0x122cf0, the comparison of two longs (unknown_122870.cpp) */
long __stdcall cache_tag_group_compare(void const *a, void const *b, void const *context);

struct s_object_name_scenario_view
{
	byte unknown000[0x3d8];
	long object_name_count;
	long *object_names;
};

// @retail 0xaa8e0
void scenario_object_name_encode(long object_name, s_bitstream *stream)
{
	long index = NONE;
	if (object_name != NONE)
	{
		long key = object_name;
		s_object_name_scenario_view *scenario = (s_object_name_scenario_view *)g_4e0350;
		index = NONE;
		if (scenario && scenario->object_name_count > 0)
			index = function_13ddd0(&key, scenario->object_names, scenario->object_name_count, sizeof(long), cache_tag_group_compare, 0);
	}
	stream_write_checked(stream, index + 1, 9);
}

#include "unknown_1946f0.h"
struct s_z_transform_state
{
 long identifier;
 vector3f position;
 vector3f forward;
 vector3f up;
 vector3f linear_velocity;
 vector3f angular_velocity;
};

bool function_ab9f0(s_z_transform_state const *state);
void simulation_read_position(s_bitstream *stream, real *position, long bits);
void function_195070(s_bitstream *stream, vector3f *out, real lo, real hi, long bits);

// @retail 0xab960
bool function_ab960(s_bitstream *stream, s_z_transform_state *state)
{
 long index = function_1959c0(stream, 10);
 byte salt = (byte)function_1959c0(stream, 4);
 state->identifier = index | ((dword)salt << 28);
 simulation_read_position(stream, (real *)&state->position, 16);
 function_195240(stream, &state->forward, &state->up);
 function_195070(stream, &state->linear_velocity, 0.03f, 350.0f, 10);
 function_195070(stream, &state->angular_velocity, 0.03f, 30.0f, 8);
 return function_ab9f0(state) != false;
}

#include <string.h>
struct s_z_blend_state
{
 real first;
 real second;
 long identifier;
 long slot;
};

PRIVATE inline real z_read_blend_fraction(s_bitstream *stream)
{
 long value = function_1959c0(stream, 4);
 real result;
 if (value == 0) result = 0.0f;
 else if (value >= 15) result = 1.0f;
 else result = ((15 - value) * 0.0f + value * 1.0f) * (1.0f / 15.0f);
 return result;
}

// @retail 0xab700
bool function_ab700(s_bitstream *stream, s_z_blend_state *state)
{
 memset(state, 0, sizeof(*state));
 bool result = true;
 state->identifier = NONE;
 state->slot = NONE;
 if (function_1957d0(stream))
 {
  state->first = z_read_blend_fraction(stream);
  state->second = z_read_blend_fraction(stream);
  long index = function_1959c0(stream, 10);
  byte salt = (byte)function_1959c0(stream, 4);
  state->identifier = index | ((dword)salt << 28);
  result = state->identifier != NONE;
  if (function_1957d0(stream))
   state->slot = function_1959c0(stream, 5);
 }
 return result;
}

PRIVATE inline void z_write_blend_fraction(s_bitstream *stream, real value)
{
 real scaled = value * 15.0f;
 long quantized;
 __asm
 {
  fld scaled
  fistp quantized
 }
 function_195720(stream, quantized, 4);
}

void function_b5650(long identifier, s_bitstream *stream);
void function_194830(s_bitstream *stream, bool value);

// @retail 0xab5e0
void function_ab5e0(s_z_blend_state const *state, s_bitstream *stream)
{
 if ((state->first > 0.0f || state->second > 0.0f) && state->identifier != NONE)
 {
  stream_write_bit(stream, true);
  real first = state->first > 0.0f ? state->first : 0.0f;
  real second = state->second > 0.0f ? state->second : 0.0f;
  z_write_blend_fraction(stream, first);
  z_write_blend_fraction(stream, second);
  function_b5650(state->identifier, stream);
  function_194830(stream, state->slot != NONE);
  if (state->slot != NONE)
   stream_write_checked(stream, state->slot, 5);
 }
 else
  stream_write_bit(stream, false);
}

void simulation_write_position(real const *position, long bits, s_bitstream *stream, bool keep_inside);
void function_194d30(s_bitstream *stream, vector3f const *forward, vector3f const *up);
void function_194c10(s_bitstream *stream, vector3f const *vector, real lo, real hi, long bits);
bool __stdcall function_a75d0(vector3f *vector, real maximum);

// @retail 0xab7f0
void function_ab7f0(s_bitstream *stream, s_z_transform_state const *state)
{
 function_b5650(state->identifier, stream);
 simulation_write_position((real const *)&state->position, 16, stream, false);
 function_194d30(stream, &state->forward, &state->up);
 vector3f velocity = state->linear_velocity;
 function_a75d0(&velocity, 350.0f);
 function_194c10(stream, &velocity, 0.03f, 350.0f, 10);
 velocity = state->angular_velocity;
 function_a75d0(&velocity, 30.0f);
 function_194c10(stream, &velocity, 0.03f, 30.0f, 8);
}
