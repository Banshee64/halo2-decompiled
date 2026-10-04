// stubs for game functions not decompiled yet, called by bipeds.cpp
#include "cseries.h"
#include "real_math.h"

struct s_animation_state;
struct s_havok_component;
struct s_havok_component_element0c;
struct s_biped_physics_input;
struct s_biped_physics_output;
struct s_biped_physics_result;
struct s_biped_physics_move;
struct s_biped_ground_collision;

// @stub 0xcba50
void __stdcall function_cba50(long unit_index, real_vector3d *aim, long unknown) { }

// @stub 0x1e5af0
void function_1e5af0(s_biped_physics_output *output, void *physics, real_vector3d const *up, real_vector3d const *forward) { }

// @stub 0x1ec500
bool function_1ec500(long biped_index) { return 0; }

// @stub 0x1e5a60
void function_1e5a60(s_biped_physics_result *result, s_biped_physics_input const *input, void *physics) { }

// @stub 0x1e5bb0
void function_1e5bb0(s_biped_physics_output *output, void *physics, void *state, real speed_scale, long havok_component_index, long biped_index, void const *definition_physics, long a, bool b, bool turning, bool c, bool landing, bool d, bool grounded, bool e, bool f, real gravity, real boost, real_vector3d const *control, real_point3d const *position, real_vector3d const *forward, real_vector3d const *up, real_vector3d const *desired_facing, real_vector3d const *facing, real_vector3d const *ground_velocity, long material) { }

// @stub 0x1e6120
void function_1e6120(s_biped_physics_output *output, void *physics, real rate, bool airborne, bool b, bool c, real crouch) { }

// @stub 0x1e6360
void function_1e6360(s_biped_physics_output *output, real height, long biped_index, real crouch) { }

// @stub 0x183670
bool __stdcall function_183670(long component_a, long component_b, real_point3d *a, real_point3d *b, real *distance) { return 0; }

// @stub 0xe5d50
void __stdcall function_e5d50(long biped_index, long target_index, real_vector3d *offset, real_point3d *point) { }

// @stub 0xe59e0
void function_e59e0(long biped_index, real_vector3d *velocity, real_point3d *position) { }

// @stub 0xe57e0
void __stdcall function_e57e0(long biped_index) { }

// @stub 0x1ed340
void __stdcall function_1ed340(void *physics, long biped_index) { }

// @stub 0x1696d0
bool function_1696d0(long flags, s_biped_ground_collision *collision, long object_index, real_point3d const *point, real_vector3d const *vector, long a, long b) { return 0; }

// @stub 0xc5460
bool function_c5460(long unit_index, long ignore_index, real_point3d const *point, long a, long b, real radius, long c) { return 0; }

// @stub 0x1d5120
bool function_1d5120(s_havok_component *component, long rigid_body_index, long a, long b, real rate, char *result, char value) { return 0; }

// @stub 0x1ec5f0
real function_1ec5f0(void *ragdoll) { return 0; }

// @stub 0x1d35d0
void function_1d35d0(long rigid_body_index, s_havok_component *component, real scale) { }

// @stub 0x1ec640
real function_1ec640(void *ragdoll) { return 0; }

// @stub 0x1d1230
real function_1d1230(long rigid_body_index, s_havok_component *component) { return 0; }

// @stub 0xbfa40
void function_bfa40(long object_index, long a) { }

// @stub 0x1ecef0
bool function_1ecef0(void *ragdoll) { return 0; }

// @stub 0x1ed430
bool function_1ed430(void *ragdoll, real_vector3d *direction, long *value) { return 0; }

// @stub 0x1faf80
void function_1faf80(real_vector3d *facing, long biped_index, void const *definition_flight, real_vector3d const *control, real rate, real *turn) { }

// @stub 0xbf5d0
bool function_bf5d0(long object_index) { return 0; }

// @stub 0xe63b0
bool function_e63b0(real_vector3d const *aim, bool flag) { return 0; }

// @stub 0x1cff80
void function_1cff80(s_havok_component_element0c const *constraint, real_point3d *pivot_a, real_point3d *pivot_b) { }

// @stub 0x114b60
void function_114b60(long a, long b, long biped_index, long c, long d) { }

// @stub 0x1c9c00
void function_1c9c00(long object_index) { }

// @stub 0x1e20b0
real function_1e20b0(long actor_index) { return 0; }

// @stub 0xe5790
void function_e5790(long biped_index) { }

// @stub 0x1e55d0
void __stdcall function_1e55d0(s_biped_physics_move *move, void *physics, s_biped_physics_output *output) { }

// @stub 0x1cd8a0
void __stdcall function_1cd8a0(s_animation_state *state, long biped_index, real_vector3d const *velocity) { }

// @stub 0x1cdb00
void __stdcall function_1cdb00(long biped_index, real_vector3d const *control) { }

// @stub 0x1c4a80
void function_1c4a80(long object_index, long a, long b) { }
