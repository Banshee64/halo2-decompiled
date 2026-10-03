// stubs for game functions not decompiled yet, called by projectiles.cpp
#include "cseries.h"
#include "real_math.h"

// @stub 0xb9a90
void function_b9a90(long object_index) { }
/* the delta a projectile's guidance adds to its velocity */
// @stub 0x109a00
bool function_109a00(long projectile_index, union real_vector3d *delta, bool unknown, union real_vector3d *velocity) { return false; }
/* projectiles.cpp's own, not written yet (temporary) */
// @stub 0xf8eb0
void function_f8eb0(long projectile_index, union real_vector3d const *displacement) { }
// @stub 0xb75a0
void function_b75a0(long object_index, real_point3d const *point, union real_vector3d const *forward, long a, long b) { }
// @stub 0xb77d0
void __stdcall function_b77d0(long object_index, union real_vector3d const *velocity) { }
/* attaches an object to a parent's node */
// @stub 0xb93b0
void __stdcall function_b93b0(long parent_index, long object_index, long node_index) { }
// @stub 0x1e2930
void __stdcall function_1e2930(long object_index, long actor_index) { }
// @stub 0xa83e0
void function_a83e0(long object_index, long parent_index, real_point3d const *point, long node_index, union real_vector3d const *forward) { }
