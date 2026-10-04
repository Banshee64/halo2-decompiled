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
struct s_location;
struct s_effect_beam;
struct s_effect_object_placement;
struct s_effect_owner;
struct s_effect_source;

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

// @stub 0xa7640
bool function_a7640(s_effect_object_placement *data) { return false; }

// @stub 0xc0350
void function_c0350(long tag_index, long object_index, long node_index, real_vector3d const *up, real_vector3d const *forward, real_point3d const *position, real scale) { }

// @stub 0x16a8e0
void function_16a8e0(long name, real_point3d const *point, real radius, long object_index, long unknown, real_point3d const *origin, real *radius_reference) { }

// @stub 0x156b60
void function_156b60(s_effect_beam *beam, real progress, real_matrix4x3 const *matrix) { }

// @stub 0x248c60
void function_248c60(s_particle_location_datum *particle_location, s_particle_system_datum *particle_system, real_matrix4x3 const *matrix, bool first_person) { }

/* in region */
// @stub 0x174a30
bool function_174a30(s_particle_system_datum *particle_system, real dt) { return false; }

/* in region */
// @stub 0x179880
void __stdcall function_179880(s_effect_datum *effect, long effect_index) { }

/* in region */
// @stub 0x17e670
void function_17e670(s_effect_source *source, real_point3d const *point, long tag_index, real_vector3d const *vector, real radius, long unknown0, long unknown1, long unknown2) { }

// @stub 0x211060
void function_211060(long unknown0, void *physics, s_location *location, long unknown3, real_point3d *position, long unknown5, long unknown6, long unknown7, real radius, real dt, real_vector3d *velocity) { }

// @stub 0x43890
void function_43890(void) { }

// @stub 0x23aad0
void __stdcall function_23aad0(long a, long b, long c) { }
