/* LANE_R.CPP: stubs for the functions lane R's code (0x170000 to 0x17ffff)
   calls that are not decompiled yet. Those marked "in region" are lane R's
   own, still to be written. */

#include "unknown_11c920.h"
#include "unknown_0259d0.h"

struct s_particle_system_datum;
struct s_particle_location_datum;
struct s_effect_datum;
struct s_effect_object_marker;
struct s_effect_color_query;
struct s_location;
struct s_effect_beam;
struct s_effect_object_placement;
struct s_effect_owner;
struct s_effect_source;

// @stub 0x248970
void function_248970(s_particle_location_datum *particle_location, bool field_b4, real unknown, s_particle_system_datum *particle_system, real *values, transform4x3f const *matrix) { }

// @stub 0x248df0
real function_248df0(long index, void *a, void *b, void const *c) { return 0.0f; }

// @stub 0x3ebd0
bool __stdcall function_3ebd0(vector3f const *offset, transform4x3f const *matrices, transform4x3f *out, long count) { return false; }

// @stub 0x3ddd0
long __stdcall function_3ddd0(long object_index) { return NONE; }

// @stub 0xbab40
bool __stdcall function_bab40(long object_index, long name, real *value) { return false; }

// @stub 0xd2bb0
long function_d2bb0(void *source, s_effect_color_query *query) { return 0; }

// @stub 0xd2a50
long function_d2a50(long a, long b, long c, s_effect_color_query *query, long d, point3f const *point) { return 0; }

// @stub 0xa7640
bool function_a7640(s_effect_object_placement *data) { return false; }

// @stub 0xc0350
void function_c0350(long tag_index, long object_index, long node_index, vector3f const *up, vector3f const *forward, point3f const *position, real scale) { }

// @stub 0x16a8e0
void function_16a8e0(long name, point3f const *point, real radius, long object_index, long unknown, point3f const *origin, real *radius_reference) { }

// @stub 0x156b60
void function_156b60(s_effect_beam *beam, real progress, transform4x3f const *matrix) { }

// @stub 0x248c60
void function_248c60(s_particle_location_datum *particle_location, s_particle_system_datum *particle_system, transform4x3f const *matrix, bool field_b4) { }

/* in region */
// @stub 0x174a30
bool function_174a30(s_particle_system_datum *particle_system, real dt) { return false; }

/* in region */
// @stub 0x179880
void __stdcall function_179880(s_effect_datum *effect, long effect_index) { }

/* in region */
// @stub 0x17e670
void function_17e670(s_effect_source *source, point3f const *point, long tag_index, vector3f const *vector, real radius, long unknown0, long unknown1, long unknown2) { }

// @stub 0x211060
void function_211060(long unknown0, void *physics, s_location *location, long unknown3, point3f *position, long unknown5, long unknown6, long unknown7, real radius, real dt, vector3f *velocity) { }

// @stub 0x43890
void function_43890(void) { }

// @stub 0x23aad0
void __stdcall function_23aad0(long a, long b, long c) { }
