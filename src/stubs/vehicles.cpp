// stubs for game functions not decompiled yet, called by vehicles.cpp
#include "cseries.h"
#include "real_math.h"

struct s_havok_component;
struct s_vehicle_physics_state;
struct s_vehicle_ray;

// @stub 0xc42e0
void __stdcall function_c42e0(long unit_index, void const *placement) { }

// @stub 0x2053c0
void function_2053c0(real *value, real const *rates, real direction, real dt) { }

// @stub 0x2054b0
void function_2054b0(real *value, real const *rates, real direction, real dt, real target) { }

// @stub 0x1d2460
void function_1d2460(s_havok_component *component) { }

// @stub 0x205510
bool function_205510(void *buffer, void const *definition_physics, s_havok_component *component) { return 0; }

// @stub 0x1cfb90
void function_1cfb90(real_matrix4x3 const *matrix, void const *buffer) { }

// @stub 0x205be0
bool __stdcall function_205be0(s_vehicle_physics_state *state, long vehicle_index) { return 0; }

// @stub 0x2056e0
void __stdcall function_2056e0(long vehicle_index, s_vehicle_physics_state *state, real braking, real_vector3d const *force, real_vector3d const *torque) { }

// @stub 0xcc380
bool function_cc380(long object_index) { return 0; }

// @stub 0x113e40
bool function_113e40(long unit_index) { return 0; }

// @stub 0xe6830
bool __stdcall function_e6830(long unit_index) { return 0; }

// @stub 0x111650
bool __stdcall function_111650(long unit_index, long *names) { return 0; }

// @stub 0x1fa3a0
long function_1fa3a0(long a, long b, long c, real_point3d const *point) { return 0; }

// @stub 0xa75d0
void __stdcall function_a75d0(real_vector3d *vector, real maximum) { }

// @stub 0x182800
bool function_182800(s_vehicle_ray *ray, long ignore_index, void *world) { return 0; }

// @stub 0x168f40
bool __stdcall function_168f40(long flags, s_vehicle_ray const *ray, long ignore_object_index, long ignore_unit_index) { return 0; }
