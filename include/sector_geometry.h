/* SECTOR_GEOMETRY.H: traces across the sectors of the pathfinding data
   (src/sector_geometry.cpp) */

#ifndef SECTOR_GEOMETRY_H
#define SECTOR_GEOMETRY_H

#include "cseries.h"
#include "real_math.h"
#include "unknown_1fa590.h"
#include "path.h"

/* where a sector trace stops (0x24 bytes) */
struct s_path_trace_result
{
	long unknown00;
	point3f point;
	byte unknown10[8];
	real distance;
	byte unknown1c[8];
};

/* the same, with its fields named */
struct s_sector_trace_result
{
	bool blocked;
	byte unknown01[3];
	point3f point;
	long sector_index;
	long edge_index;
	real distance;
	bool unknown1c;
	byte unknown1d[3];
	real unknown20;
};

/* traces along direction from origin, from sector_index across the sectors'
   edges up to distance, or until it reaches target_sector_index */
bool function_26c590(s_pathfinding_data const *pathfinding, point3f const *origin, long sector_index,
	long target_sector_index, vector3f const *direction, real distance, s_path_location const *location,
	s_path_trace_result *trace);

/* the order the sources written before 0x26c590 call it in (its node index
   first, an unused point, the result, then the rest); they can call the
   function above directly instead and drop this */
inline bool function_26c590(long node_index, point3f const *unused, s_path_trace_result *result,
	s_pathfinding_data *pathfinding, point3f const *position, long target_node_index,
	vector3f const *direction, real distance, long location)
{
	return function_26c590(pathfinding, position, node_index, target_node_index, direction, distance,
		(s_path_location const *)location, result);
}

#endif
