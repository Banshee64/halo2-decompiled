/* LANE_R.CPP: stubs for the functions lane R's code (0x170000 to 0x17ffff)
   calls that are not decompiled yet. Those marked "in region" are lane R's
   own, still to be written. */

#include "cseries.h"
#include "real_math.h"

struct s_particle_system_datum;
struct s_particle_location_datum;
struct s_effect_datum;
struct s_effect_object_marker;
struct s_effect_color_query;

// @stub 0x2486e0
void __stdcall function_2486e0(long particle_location_index) { }

// @stub 0x248620
long function_248620(s_particle_system_datum *particle_system) { return NONE; }

// @stub 0x248d90
void function_248d90(s_particle_location_datum *particle_location, long *first_index, long *last_index) { }

// @stub 0x248970
void function_248970(s_particle_location_datum *particle_location, bool first_person, real unknown, s_particle_system_datum *particle_system, real *values, real_matrix4x3 const *matrix) { }

// @stub 0x248df0
real function_248df0(long index, void *a, void *b, void const *c) { return 0.0f; }

// @stub 0x3ebd0
bool __stdcall function_3ebd0(real_vector3d const *offset, real_matrix4x3 const *matrices, real_matrix4x3 *out, long count) { return false; }

// @stub 0x3ddd0
long __stdcall function_3ddd0(long object_index) { return NONE; }

// @stub 0xbab40
bool __stdcall function_bab40(long object_index, long name, real *value) { return false; }

// @stub 0xd2bb0
long function_d2bb0(void *source, s_effect_color_query *query) { return 0; }

// @stub 0xd2a50
long function_d2a50(long a, long b, long c, s_effect_color_query *query, long d, real_point3d const *point) { return 0; }

// @stub 0x1662c1
short __stdcall function_1662c1(long group_index, long name, s_effect_object_marker *markers, short count) { return 0; }

/* in region */
// @stub 0x179fb0
void function_179fb0(s_effect_datum *effect) { }

/* in region */
// @stub 0x17a380
void __stdcall function_17a380(s_effect_datum *effect) { }

/* in region */
// @stub 0x17c0e0
void function_17c0e0(long contrail_index, long count, bool flag) { }

/* in region */
// @stub 0x17c540
void __stdcall function_17c540(long contrail_index, real dt) { }

// @stub 0x43890
void function_43890(void) { }

// @stub 0x23aad0
void __stdcall function_23aad0(long a, long b, long c) { }
