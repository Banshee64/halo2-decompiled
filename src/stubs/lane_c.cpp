// stubs for lane C (0x1c0000..0x1cffff): callees outside the region that are
// not decompiled yet, and the library (Havok) functions the region calls
#include "cseries.h"
#include "unknown_1cec30.h"
#include "slot_owner.h"
#include "unknown_1c62f0.h"
#include "lane_c_callees.h"

// @stub 0x3123a0
hkPropertyValue hkEntity::removeProperty(dword key) { return hkPropertyValue(0); }

// @stub 0x312530
void hkEntity::addProperty(dword key, hkPropertyValue value) { }

/* slot handler callbacks of the region not decompiled yet (their handler
   structs hold their addresses) */

/* game functions outside the region called by the physics lifecycle callbacks */

// @stub 0x2263c0
void function_2263c0(void) { }

// @stub 0x146b30
void function_146b30(void) { }

// @stub 0x146b80
void function_146b80(void) { }

// @stub 0x146de0
void function_146de0(void) { }

// @stub 0x226440
void function_226440(void) { }
/* game functions outside the region called by the ai lifecycle callbacks */

// @stub 0x1dfae0
void function_1dfae0(void) { }

// @stub 0x28d930
void function_28d930(void) { }

// @stub 0x25c170
void function_25c170(void) { }

// @stub 0x200930
void function_200930(void) { }

// @stub 0x257d00
void function_257d00(void) { }

// @stub 0x20b930
void function_20b930(void) { }

// @stub 0x292130
void function_292130(void) { }

// @stub 0x1a6d80
void function_1a6d80(void) { }

// @stub 0x28d9d0
void function_28d9d0(void) { }

// @stub 0x292e00
void function_292e00(void) { }

// @stub 0x292f60
void function_292f60(void) { }
/* the animation graph lookups (0x1d9000..0x1de000) */

// @stub 0x1ddb40
void *__stdcall function_1ddb40(void *graph, c_animation_id animation_id) { return 0; }

/* the physics callees of the havok components */
struct s_havok_component;

// @stub 0x1d1260
void function_1d1260(s_havok_component *component) { }

// @stub 0x1d01c0
void __stdcall function_1d01c0(s_havok_component *component) { }
// @stub 0x3126f0
void hkRigidBody::setTransform(hkTransform const &transform) { }
/* callees of the slot handler callbacks (lane_c_callees.h) */

// @stub 0xb8d30
short function_b8d30(bool flag, long object_index, long marker_name, short count, s_object_marker *markers) { return 0; }

// @stub 0x259a0
real function_259a0(dword *seed) { return 0.0f; }

// @stub 0x1e4f90
s_actor_tag_entry_1e4f90 *function_1e4f90(long actor_index) { return 0; }

// @stub 0x1469f0
long __stdcall function_1469f0(long value) { return 0; }

// @stub 0x1697c0
bool __stdcall function_1697c0(long flags, real_point3d const *point, real_vector3d const *vector,
	long ignore_object_index, long ignore_unit_index, s_collision_result_1697c0 *result) { return false; }

// @stub 0x210a30
real function_210a30(s_actor_point_target const *a, s_actor_point_target const *b) { return 0.0f; }

// @stub 0x1f8a70
bool __stdcall function_1f8a70(long actor_index, bool flag) { return false; }

// @stub 0x1f90f0
void function_1f90f0(long actor_index, s_path_source *source) { }

// @stub 0x1f9240
void function_1f9240(long actor_index, s_path_query *query) { }

// @stub 0x271300
void function_271300(void *unknown, byte *buffer, s_path_query *query, s_path_source *source, long flags) { }

// @stub 0x2715a0
bool function_2715a0(byte *buffer) { return false; }

// @stub 0x270750
void function_270750(byte *buffer, long unknown, s_actor_point_target const *target, real *distance, long a, long b) { }

// @stub 0x2104b0
bool function_2104b0(short type, real_point3d *position, s_actor_point_target const *target) { return false; }

// @stub 0x261280
s_reference function_261280(s_prop_search *search, long actor_index, long *a, long *b, byte *buffer, long *c) { s_reference r = {0, 0}; return r; }

// @stub 0x2626b0
void __stdcall function_2626b0(long actor_index, s_reference reference, long a, byte *buffer, long b, bool flag) { }

// @stub 0x2605d0
void function_2605d0(long actor_index, s_prop_search *search, long a, long *b, byte *buffer, bool *flag) { }

// @stub 0x265cb0
void function_265cb0(long actor_index) { }

// @stub 0x26c590
void function_26c590(long node_index, real_point3d const *origin, s_path_trace_result *result,
	s_pathfinding_data *pathfinding, real_point3d const *position, long a, real_vector3d const *direction,
	real distance, long b) { }

// @stub 0x26d100
long function_26d100(real_vector3d const *up, s_collision_result_1697c0 *collision, long *unknown, real_point3d const *point) { return 0; }

// @stub 0x272700
short path_node_from_hash_table(path_state *state, long node_index) { return 0; }

// @stub 0x1fa6b0
bool function_1fa6b0(s_pathfinding_node *node, s_pathfinding_data *pathfinding, s_actor_view *actor) { return false; }

// @stub 0x1f34b0
bool function_1f34b0(long actor_index, real_vector3d const *direction, real_point3d const *position, long ticks, real distance) { return false; }

/* callees of the physics code (unknown_1c25a0.cpp, unknown_1cec30.cpp) */

// @stub 0x30bd50
void hkEntityApi::removeEntityListener(hkEntityListener *listener) { }

// @stub 0x30f800
hkBool hkWorld::removeEntity(hkEntity *entity) { return hkBool(); }

// @stub 0x278f00
void function_278f00(void) { }

// @stub 0x146bf0
void function_146bf0(void) { }

// @stub 0x1d1540
void function_1d1540(s_havok_component *component) { }

// @stub 0x1d56a0
void function_1d56a0(s_havok_component *component) { }

// @stub 0x1d56f0
void function_1d56f0(s_havok_component *component) { }

// @stub 0x1d5940
bool __stdcall function_1d5940(s_havok_component *component, long a, long b, long c) { return false; }

// @stub 0x1d6b80
void function_1d6b80(s_havok_component *component) { }

// @stub 0x1d6ca0
void function_1d6ca0(s_havok_component *component) { }

/* in the region, not decompiled yet */

// @stub 0x1c4b00
void function_1c4b00(long object_index, void *a, void *b, long c) { }

// @stub 0x30f2d0
void hkWorld::addEntity(hkEntity *entity) { }

// @stub 0x30cc60
void hkWorld::removeSimulationIsland(hkSimulationIsland *island) { }

// @stub 0x30bc90
void hkEntityApi::activate(void) { }

// @stub 0xa7670
bool function_a7670(long object_index) { return false; }

// @stub 0x1765e0
void function_1765e0(real_point3d *position, real_vector3d *velocity, real_vector3d const *up, long effect_index, long a, bool b) { }

/* the animation channels (unknown_1c62f0.cpp, unknown_1c62f0.h) */

// @stub 0x1daea0
s_animation *function_1daea0(s_graph_tag *graph, c_animation_id animation_id) { return 0; }

// @stub 0x1c69b0
void function_1c69b0(c_animation_channel *channel) { }

// @stub 0x1c66a0
void __stdcall function_1c66a0(c_animation_channel *channel, real frame, long a, long b, long c) { }

/* the sort of ai.cpp's importance list */

// @stub 0x13da70
void function_13da70(void *elements, long count, long element_size, bool (__stdcall *compare)(void const *a, void const *b, void const *context), void const *context) { }
