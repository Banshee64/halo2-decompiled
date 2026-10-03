/* LANE_C_CALLEES.H: game functions outside 0x1c0000..0x1cffff that lane C's
   sources call and nobody has decompiled yet. src/stubs/lane_c.cpp defines
   them (the ones lane B calls too are in slot_handler.h); whoever decompiles one moves its prototype to the callee's own header
   and deletes the stub. Functions retail calls with stack arguments only
   (ret N, nothing in registers) are declared __stdcall so the call sites
   match; the rest take LTCG register conventions retail chose from their
   bodies, which a stub can't reproduce. */

#ifndef LANE_C_CALLEES_H
#define LANE_C_CALLEES_H

#include "cseries.h"
#include "real_math.h"
#include "slot_handler.h"
#include "unknown_2605d0.h"
#include "unknown_2626b0.h"

/* the 0x70 byte object marker (objects) */
struct s_object_marker
{
	byte unknown00[0x70];
};

/* the actor's tag entry function_1e4f90 returns (0x40 bytes) */
struct s_actor_tag_entry_1e4f90
{
	long unknown00;
	real unknown04;
	real unknown08;
	real unknown0c;
	real unknown10;
	long unknown14;
	real unknown18;
	byte unknown1c[0x40 - 0x1c];
};

/* a collision result (0x4c bytes) */
struct s_collision_result_1697c0
{
	byte unknown00[8];
	real_point3d point;
	byte unknown14[0x24 - 0x14];
	short unknown24;
	byte unknown26[0x4c - 0x26];
};

/* the scenario's firing positions: g_4e0350 holds a block (+0x1d8) whose
   first element holds the zones; a position reference is the zone index in
   the high word and the position index in the low word */
struct s_firing_position
{
	byte unknown00[0x20];
	s_actor_point_target target;
	long unknown30;
	real facing;
	byte unknown38[4];
};

struct s_firing_zone
{
	byte unknown00[0x20];
	long position_count;
	s_firing_position *positions;
	short bsp_index;
	byte unknown2a[2];
	byte flags;
	byte unknown2d[3];
};

struct s_firing_zone_set
{
	long zone_count;
	s_firing_zone *zones;
};

struct s_scenario_firing_view
{
	byte unknown000[0x1d8];
	long zone_set_count;
	s_firing_zone_set *zone_sets;
};

inline s_firing_position *firing_position_get(long reference)
{
	s_scenario_firing_view *scenario = (s_scenario_firing_view *)g_4e0350;

	return &scenario->zone_sets->zones[(reference >> 16) & 0xffff].positions[reference & 0xffff];
}

/* a path query (0x68 bytes): function_1f9240 fills it and function_1f90f0
   its source at +0x1c */
struct s_path_source
{
	byte unknown00[0x44];
};

struct s_path_query
{
	byte unknown00[0x1c];
	s_path_source source;
	byte unknown60;
	bool unknown61;
	byte unknown62[2];
	real unknown64;
};

/* the prop search (0x758 bytes) of function_261280 and function_2605d0 */
struct s_prop_search_point
{
	real weight;
	real_point3d position;
};

struct s_prop_search
{
	short type;
	byte unknown02[0x19 - 0x2];
	bool unknown19;
	byte unknown1a[0x70 - 0x1a];
	long point_count;
	s_prop_search_point points[32];
	byte unknown274[0x758 - 0x274];
};

short function_b8d30(bool flag, long object_index, long marker_name, short count, s_object_marker *markers);
real function_259a0(dword *seed);
void *function_1e4f90(long actor_index);
long __stdcall function_1469f0(long value);
bool __stdcall function_1697c0(long flags, real_point3d const *point, real_vector3d const *vector,
	long ignore_object_index, long ignore_unit_index, s_collision_result_1697c0 *result);
bool __stdcall function_1f8a70(long actor_index, long unknown);
void function_1f90f0(long actor_index, s_path_source *source);
void function_1f9240(long actor_index, s_path_query *query);
void function_271300(void *unknown, byte *buffer, s_path_query *query, s_path_source *source, long flags);
bool function_2715a0(byte *buffer);
void function_270750(byte *buffer, long unknown, s_actor_point_target const *target, real *distance, long a, long b);
s_reference function_261280(s_prop_search *search, long actor_index, long *a, long *b, byte *buffer, long *c);
void function_265cb0(long actor_index);

/* the pathfinding of the structure bsp (g_4e0348 +0xc4): 8 byte nodes with
   flags in the first word */
struct s_pathfinding_node
{
	word flags;
	byte unknown2[6];
};

struct s_pathfinding_data
{
	byte unknown0[4];
	s_pathfinding_node *nodes;
};

/* where function_26c590 stops a trace */
struct s_path_trace_result
{
	long unknown00;
	real_point3d point;
	byte unknown10[8];
	real distance;
	byte unknown1c[8];
};

struct path_state;

void function_26c590(long node_index, real_point3d const *origin, s_path_trace_result *result,
	s_pathfinding_data *pathfinding, real_point3d const *position, long a, real_vector3d const *direction,
	real distance, long b);
long function_26d100(real_vector3d const *up, s_collision_result_1697c0 *collision, long *unknown, real_point3d const *point);
short path_node_from_hash_table(path_state *state, long node_index);
bool function_1fa6b0(s_pathfinding_node *node, s_pathfinding_data *pathfinding, s_actor_view *actor);
bool function_1f34b0(long actor_index, real_vector3d const *direction, real_point3d const *position, long ticks, real distance);

#endif
