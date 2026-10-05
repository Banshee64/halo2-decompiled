#include "unknown_11c920.h"
#include "unknown_0259d0.h"

/* stubs for the unit object type (unit_object_type.cpp): functions it
   calls that are not written yet */

// @stub 0xc49b0
bool __stdcall function_c49b0(long unit_index) { return false; }

// @stub 0xc58f0
bool __stdcall function_c58f0(long unit_index) { return false; }

// @stub 0xc60c0
bool __stdcall function_c60c0(long unit_index) { return false; }

/* outside the unit range */
// @stub 0x114c60
void function_114c60(long unit_index) { }

/* in the biped range (PR #28 writes it) */
// @stub 0xe4bd0
void function_e4bd0(long biped_index) { }

/* outside the unit range */
// @stub 0x114240
void function_114240(long unit_index) { }

// @stub 0xa94b0
void function_a94b0(long unit_index) { }

// @stub 0xa9440
void function_a9440(long unit_index, long player_index) { }

// @stub 0xa7bc0
void function_a7bc0(long unit_index) { }

// @stub 0xa9500
void function_a9500(long unit_index, long index) { }

/* outside the unit range */
// @stub 0x10cdf0
void function_10cdf0(long object_index) { }

// @stub 0x10ca80
void function_10ca80(long object_index, long a) { }

/* outside the unit range */
// @stub 0x1e3370
bool function_1e3370(long actor_index, void *unknown) { return false; }

// @stub 0x10cec0
void function_10cec0(long unit_index, long weapon_index, long parent_marker_name, long marker_name) { }

// @stub 0x1c9c80
void function_1c9c80(long object_index, long unknown2d0, word unknown2c8, real unknown2cc, long a, bool b) { }

// @stub 0xc98a0
void function_c98a0(long unit_index, long a, long b, long c) { }

// @stub 0xcaa60
void __stdcall function_caa60(long unit_index, long a, long b, long c, long d) { }

/* outside the unit range */
// @stub 0x1e9070
void function_1e9070(long player_index) { }

// @stub 0xa8950
void function_a8950(long unit_index, long definition_index) { }

/* outside the unit range */
// @stub 0x11bf90
void function_11bf90(long object_index, point3f *point) { }

/* in the biped range (PR #28 writes it) */
// @stub 0xdef60
void __stdcall function_def60(point3f *point, long biped_index, short mode, point3f const *origin,
	vector3f const *forward, real const *offsets) { }

/* outside the unit range */
// @stub 0xff5f0
bool __stdcall function_ff5f0(long weapon_index, long name, real *value, bool *active) { return false; }

// @stub 0x10b360
void function_10b360(long object_index) { }

/* in the biped range (PR #28 writes it) */
// @stub 0xe3f00
void function_e3f00(long biped_index) { }

/* outside the unit range */
// @stub 0x1bbdf0
void function_1bbdf0(long vehicle_index) { }

struct s_havok_component;
struct s_unit_move_result;
// @stub 0x1d48f0
bool function_1d48f0(s_havok_component *component, short rigid_body_index, long type, point3f const *target,
	vector3f const *offset, s_unit_move_result *result, long a5, real radius, long a7, point3f const *root_point,
	long root_index) { return false; }

/* outside the unit range */
// @stub 0x1c9500
bool function_1c9500(long unit_index, long actor_index, long a) { return false; }

/* outside the unit range */
// @stub 0xbc380
bool function_bc380(long object_index, long block_offset, long size, long a) { return false; }

// @stub 0x10f260
void __stdcall function_10f260(long unit_index) { }

// @stub 0x114ec0
void function_114ec0(long unit_index, long a) { }
