#include "cseries.h"

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

struct path_state;
struct s_path_node_key_view;
struct s_path_link_view;

// @stub 0x272020
short __stdcall build_path_links_for_sector(s_pathfinding_data const *pathfinding,
	s_path_node_key_view const *node, s_path_link_view *links, path_state const *state)
{
	return 0;
}
