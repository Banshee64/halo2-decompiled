// stubs for game functions not decompiled yet, called by damage.cpp
#include "unknown_11c920.h"

struct s_object_child_iterator;
struct s_damage_owner;
struct s_damage_info;
struct s_damage_region_accumulator;
struct s_damage_object;

// @stub 0xb8b70
void function_b8b70(long object_index) { }
// @stub 0xb9c60
void function_b9c60(long object_index, bool flag) { }
// @stub 0xbef30
void __stdcall function_bef30(long object_index, long a, long b, long c, long d) { }
/* sets a region's permutation */
// @stub 0xa8360
void function_a8360(long object_index, long region_index, long permutation_index, bool a) { }
// @stub 0xe6460
void __stdcall function_e6460(long object_index) { }
// @stub 0xba7f0
void __stdcall function_ba7f0(long object_index, long a, long b, long c) { }
struct s_type_1e6529;
#include "unknown_0259d0.h"
/* the objects in a sphere */
/* damage.cpp's own, not written yet (temporary) */
/* the closest point of an object to an origin, and the surface normal there */
// @stub 0x183910
bool function_183910(long component_index, point3f const *origin, point3f *point, vector3f *normal) { return false; }
// @stub 0x153d10
void __stdcall function_153d10(short team, long definition_index, void *a, void *b, long c, float d, float e, long f) { }
// @stub 0x184250
void __stdcall function_184250(s_type_1e6529 const *data) { }
struct s_damage_report;
// @stub 0x119280
void function_119280(long object_index, unsigned long flags) { }
// @stub 0x1e9fa0
void __stdcall function_1e9fa0(void *engine_globals, long object_index, long player_index, unsigned short team, unsigned char kind) { }
// @stub 0xa80f0
void function_a80f0(long object_index, s_damage_report const *report) { }
/* called by function_d7b80 (0xd7b80) */
// @stub 0x155b60
void function_155b60(long unit_index) { }
/* called by function_d5de0 (0xd5de0) */
/* the physics model constraint iterator and the model node search (for
   0xdb810, 0xdbb40) */
struct s_physics_constraint_iterator;
struct s_physics_constraint_block;
