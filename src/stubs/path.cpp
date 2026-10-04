#include "cseries.h"
#include "real_math.h"

struct s_node_point;
struct s_path_trace_result;
struct s_pathfinding_data;

// @stub 0x26c4e0
bool function_26c4e0(s_node_point const *start, s_node_point const *end,
	s_path_trace_result *result, s_pathfinding_data *pathfinding,
	long start_node_index, long end_node_index, long flags)
{
	return false;
}


// @stub 0x26f150
bool function_26f150(short type, real_point3d const *start, real_point3d const *end,
	real_point3d const *alternate_start, real_point3d const *alternate_end)
{
	return false;
}

struct path_state;
struct s_path_step_view;

// @stub 0x26f3f0
bool function_26f3f0(s_pathfinding_data *pathfinding, long surface_index,
	s_node_point const *entry, long actor_index, path_state *state,
	s_node_point const *parent_point, long parent_node_index,
	long *parent_node_index_out, s_node_point *out, long *out_node_index)
{
	return false;
}

// @stub 0x2c2060
void __stdcall function_2c2060(path_state *state, short count, s_path_step_view const *steps,
	short *out_count, s_path_step_view *out, bool *complete)
{
}

// @stub 0x2c41b0
bool __stdcall function_2c41b0(long actor_index, path_state *state, short count,
	s_path_step_view const *steps, bool avoid, short *out_count,
	s_path_step_view *out, bool *complete, long *object_index, long *type, bool *flag)
{
	return false;
}
