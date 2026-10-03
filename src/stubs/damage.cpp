// stubs for game functions not decompiled yet, called by damage.cpp
#include "cseries.h"

/* the difficulty multiplier of a team (kind 1 body, 2 shield); retail passes
   both arguments in registers and returns in xmm0 */
// @stub 0x1e9720
real function_1e9720(long kind, short team) { return 1.0f; }
struct s_object_child_iterator;
struct s_damage_owner;
struct s_damage_info;
struct s_damage_region_accumulator;
struct s_damage_object;

// @stub 0xb8b70
void function_b8b70(long object_index) { }
/* damage.cpp's own, not written yet (temporary) */
// @stub 0xda110
void function_da110(long permutation_index, s_damage_info *info, long object_index, s_damage_owner const *owner, long region_index, s_damage_region_accumulator *accumulator) { }
// @stub 0xd9d60
void function_d9d60(bool flag, long a, long object_index, long effect_index, s_damage_owner const *owner) { }
/* sets a region's permutation */
// @stub 0xa8360
void function_a8360(long object_index, long region_index, long permutation_index, bool a) { }
// @stub 0xdbc80
void __stdcall function_dbc80(long object_index, short a, short b) { }
// @stub 0xe6460
void __stdcall function_e6460(long object_index) { }
// @stub 0xba7f0
void __stdcall function_ba7f0(long object_index, long a, long b, long c) { }
struct damage_data;
#include "real_math.h"
/* the objects in a sphere */
// @stub 0xbb050
short __stdcall function_bb050(long a, unsigned long type_mask, void const *location, real_point3d const *position, float radius, long *objects, short maximum_count) { return 0; }
/* damage.cpp's own, not written yet (temporary) */
// @stub 0xd6f90
bool __stdcall function_d6f90(long object_index, real_point3d const *point, damage_data *data) { return false; }
// @stub 0xd7b80
void __stdcall function_d7b80(damage_data *data, long object_index, long a, long b, long c, long d) { }
/* the closest point of an object to an origin, and the surface normal there */
// @stub 0xbaff0
void function_baff0(long object_index, real_point3d const *origin, real_point3d *closest_point, union real_vector3d *normal) { }
// @stub 0x153d10
void __stdcall function_153d10(short team, long definition_index, void *a, void *b, long c, float d, float e, long f) { }
// @stub 0x184250
void __stdcall function_184250(damage_data const *data) { }
/* an object's model states */
// @stub 0xba690
void function_ba690(long object_index, unsigned char **states, long *state_count, long *a, long *b) { }
