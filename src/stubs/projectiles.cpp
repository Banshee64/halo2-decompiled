// stubs for game functions not decompiled yet, called by projectiles.cpp
#include "cseries.h"
#include "real_math.h"

// @stub 0xb9a90
void function_b9a90(long object_index) { }
/* the delta a projectile's guidance adds to its velocity */
// @stub 0x109a00
bool function_109a00(long projectile_index, union vector3f *delta, bool unknown, union vector3f *velocity) { return false; }
// @stub 0xb75a0
struct s_location;
void function_b75a0(long object_index, point3f const *point, union vector3f const *forward, union vector3f const *up, s_location const *location, bool unknown) { }
// @stub 0xb77d0
void __stdcall function_b77d0(long object_index, union vector3f const *linear_velocity, union vector3f const *angular_velocity) { }
/* attaches an object to a parent's node */
// @stub 0xb93b0
void __stdcall function_b93b0(long parent_index, long object_index, long node_index) { }
// @stub 0x1e2930
void __stdcall function_1e2930(long object_index, long actor_index) { }
// @stub 0xa83e0
void function_a83e0(long object_index, long parent_index, point3f const *point, long node_index, union vector3f const *forward) { }
struct s_damage_owner;
/* an object's damage owner */
// @stub 0xbc190
void function_bc190(long object_index, s_damage_owner *owner) { }
// @stub 0xa84e0
void __stdcall function_a84e0(long projectile_index, short *material_index, union vector3f const *vector, unsigned long flags) { }
// @stub 0x1ca690
void __stdcall function_1ca690(long object_index, void const *data, long a, long b, long c) { }
// @stub 0x1ca9f0
void __stdcall function_1ca9f0(long object_index, long unknown) { }
// @stub 0xb7740
void function_b7740(long object_index, union vector3f const *linear_velocity, union vector3f const *angular_velocity, bool unknown) { }
struct s_collision_result_1697c0;
struct s_type_1e6529;
// @stub 0x184060
void function_184060(long unknown3c, unsigned char unknown59, s_type_1e6529 *data, long unknown50) { }
// @stub 0xa85c0
void function_a85c0(long projectile_index, s_collision_result_1697c0 const *collision, union vector3f const *direction, bool unknown, float scale_a, float scale_b) { }
