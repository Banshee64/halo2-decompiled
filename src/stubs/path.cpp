#include "unknown_11c920.h"
#include "unknown_0259d0.h"

struct s_type_c3b527;
struct s_path_trace_result;
struct s_pathfinding_data;

// @stub 0x26c4e0
bool function_26c4e0(s_type_c3b527 const *start, s_type_c3b527 const *end,
	s_path_trace_result *result, s_pathfinding_data *pathfinding,
	long start_node_index, long end_node_index, long flags)
{
	return false;
}


// @stub 0x26f150
bool function_26f150(short type, point3f const *start, point3f const *end,
	point3f const *alternate_start, point3f const *alternate_end)
{
	return false;
}

struct s_type_f17a25;
struct s_path_step_view;

// @stub 0x26f3f0
bool function_26f3f0(s_pathfinding_data *pathfinding, long surface_index,
	s_type_c3b527 const *entry, long actor_index, s_type_f17a25 *state,
	s_type_c3b527 const *parent_point, long parent_node_index,
	long *parent_node_index_out, s_type_c3b527 *out, long *out_node_index)
{
	return false;
}

// @stub 0x2c2060
void __stdcall function_2c2060(s_type_f17a25 *state, short count, s_path_step_view const *steps,
	short *out_count, s_path_step_view *out, bool *complete)
{
}

// @stub 0x2c41b0
bool __stdcall function_2c41b0(long actor_index, s_type_f17a25 *state, short count,
	s_path_step_view const *steps, bool avoid, short *out_count,
	s_path_step_view *out, bool *complete, long *object_index, long *type, bool *flag)
{
	return false;
}
