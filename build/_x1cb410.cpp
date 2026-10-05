// @flags /O2 /Ob1 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "unknown_1cafc0.h"

// @retail 0x1cb410
bool s_animation_state::channel_start(c_animation_channel *channel, c_type_709360 animation_id, long unknown08,
	char unknown0c, char unknown0d, char unknown0e, word channel_flags)
{
	bool result = false;

	if (graph_tag_index != NONE)
	{
		c_type_709360 id = animation_id;

		if (channel_flags & 0x10)
		{
			id = variant_get(animation_id);
		}
		if (channel->set(graph_tag_index, channel_flags, id, unknown08, unknown0c, unknown0d, unknown0e))
		{
			if (channel_flags & 4)
			{
				channel->set_frame_position(0.0f);
			}
			if (channel_flags & 8)
			{
				channel->rate = 1.0f;
			}
			result = true;
		}
	}
	return result;
}
// @retail 0x1cbe50
s_graph_inheritance *s_animation_state::inheritance_get(c_type_709360 animation_id)
{
	s_graph_inheritance *result = NULL;

	if (animation_id.index != NONE && animation_id.graph_index != NONE)
	{
		result = function_1daff0(graph_tag_get(graph_tag_index), animation_id);
	}
	return result;
}
