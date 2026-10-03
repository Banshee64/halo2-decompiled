// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1CAFC0.CPP: the object that drives three animation channels of
   one graph (0x1cafc0..0x1ce1e0) */

#include "cseries.h"
#include "globals.h"
#include "unknown_1c62f0.h"
#include "unknown_123680.h"

/* a 4 byte state at +0x60 and +0x64 of the animation state */
struct s_animation_bits
{
	byte unknown0;
	byte unknown1;
	byte unknown2;
	byte unknown3;

	s_animation_bits() : unknown0(0), unknown1(0), unknown2(0), unknown3(0) {}
};

/* the names an animation is looked up by */
struct s_animation_names
{
	long mode;
	long weapon_class;
	long weapon_type;
	long set;
};

/* an entry of the graph's weapon block at +0x54 (0x1dd560 finds one by name) */
struct s_graph_name_entry
{
	long name;
	long weapon_class;
};

/* the graph's animation search by names (animation_graph.cpp, not decompiled yet) */
c_animation_id *function_1db170(s_graph_tag *graph, c_animation_id *result, long mode, long weapon_class,
	long weapon_type, long set, long *found_mode, long *found_weapon_class, long *found_weapon_type);

#define PIN(value, lower, upper) ((value) < (lower) ? (lower) : (value) > (upper) ? (upper) : (value))

/* an orientation: a quaternion, a translation and a scale */
struct real_orientation_1ce110
{
	real quaternion[4];
	real_point3d translation;
	real scale;
};

struct s_animation_state
{
	c_animation_channel channels[3];
	s_animation_bits unknown60;
	s_animation_bits unknown64;
	long graph_tag_index;
	word flags;
	word unknown6e;
	long unknown70;
	long unknown74;
	long unknown78;
	long unknown7c;
	real unknown80;
	real_vector3d unknown84;

	s_animation_state();
	void reset();
	void names_resolve(s_animation_names *names, long mode, long weapon_class, long weapon_type, long set);
	bool animation_lookup(s_animation_names *found, s_animation_names *names, long mode, long weapon_class,
		long weapon_type, long set, long lookup_flags, c_animation_id *result);
	void channels_clear_partial();
	short node_count_get();
	long node_find(long name);
	s_graph_entry *entry_get(long index);
	long node_count();
	s_graph_tag *graph_get();
	void update_blend_flags();
	void channels_clear();
	void secondary_channels_clear();
	c_animation_id *current_animation_get(c_animation_id *result);
	real blend_fraction_get();
	void animation_touch(c_animation_id animation_id);
	bool node_map_build(long render_model_tag_index, long *node_count, long *node_map);
	bool channel_update(c_animation_channel *channel, long a, long b);
	s_graph_inheritance *inheritance_get(c_animation_id animation_id);
	c_animation_id *variant_get(c_animation_id *result, c_animation_id animation_id);
	bool channel_start(c_animation_channel *channel, c_animation_id animation_id, long unknown08, byte unknown0c,
		byte unknown0d, char unknown0e, word channel_flags);
	void translation_apply(real_orientation_1ce110 *orientation, real scale);
	void resources_request(long mode, long weapon_class, long weapon_type, bool urgent, bool other);
	void channels_finish();
};

bool g_46fbf4 = true;
bool g_46fbf5 = true;

inline bool channel_valid(c_animation_channel const *channel)
{
	return channel->graph_tag_index != NONE && channel->animation_id.index != NONE;
}

// @retail 0x1cafc0
void s_animation_state::reset()
{
	long i;

	for (i = 0; i < 3; i++)
	{
		channels[i].reset();
	}
	flags = 0;
	unknown6e = 0;
	graph_tag_index = NONE;
	unknown64.unknown1 = 0;
	unknown64.unknown0 = 0;
	unknown64.unknown3 = 0;
	unknown60.unknown1 = 0;
	unknown60.unknown0 = 0;
	unknown60.unknown3 = 0;
	unknown70 = NONE;
	unknown7c = NONE;
	unknown74 = NONE;
	unknown78 = NONE;
	unknown80 = 0.0f;
}

// @retail 0x1caed0
s_animation_state::s_animation_state()
{
	reset();
}

// @retail 0x1cb0a0
void s_animation_state::channels_clear_partial()
{
	c_animation_channel *channel = &channels[3];
	long i = 3;

	do
	{
		channel--;
		i--;
		channel->graph_tag_index = NONE;
		channel->animation_id.graph_index = NONE;
		channel->animation_id.index = NONE;
		channel->unknown08 = NONE;
		channel->unknown10 = 0;
	}
	while (i);
}

// @retail 0x1cbd30
short s_animation_state::node_count_get()
{
	return (short)graph_get()->node_count;
}

// @retail 0x1cbde0
long s_animation_state::node_find(long name)
{
	s_graph_tag *graph = graph_get();
	long i;

	for (i = 0; i < graph->node_count; i++)
	{
		if (graph->nodes[i].name == name)
		{
			return i;
		}
	}
	return NONE;
}

// @retail 0x1cbe20
s_graph_entry *s_animation_state::entry_get(long index)
{
	if (index == NONE)
	{
		return NULL;
	}
	return &graph_get()->entries[index];
}

// @retail 0x1cbe90
long s_animation_state::node_count()
{
	s_graph_tag *graph = graph_get();
	long count = 0;

	if (graph)
	{
		count = (short)graph->node_count;
	}
	return count;
}

// @retail 0x1cc2d0
s_graph_tag *s_animation_state::graph_get()
{
	return graph_tag_get(graph_tag_index);
}

// @retail 0x1cb680
void s_animation_state::update_blend_flags()
{
	dword flags0 = channels[0].flags;
	dword flags1 = channels[1].flags;

	if (!g_46fbf4 || !channel_valid(&channels[1]) || 0.45f > unknown80)
	{
		flags &= ~0x20;
		flags0 &= ~0x380;
		flags1 |= 0x380;
	}
	else if (unknown80 > 0.55f)
	{
		flags |= 0x20;
		flags0 |= 0x380;
		flags1 &= ~0x380;
	}
	else
	{
		return;
	}
	if (channel_valid(&channels[0]))
	{
		channels[0].flags = (word)flags0;
	}
	if (channel_valid(&channels[1]))
	{
		channels[1].flags = (word)flags1;
	}
}

// @retail 0x1ccba0
void s_animation_state::channels_clear()
{
	long i;

	for (i = 0; i < 3; i++)
	{
		channels[i].clear();
	}
}

// @retail 0x1ccc50
void s_animation_state::secondary_channels_clear()
{
	long i;

	for (i = 1; i < 3; i++)
	{
		channels[i].clear();
	}
}

// @retail 0x1cd510
c_animation_id *s_animation_state::current_animation_get(c_animation_id *result)
{
	c_animation_id none;

	if (g_46fbf5 && channel_valid(&channels[2]))
	{
		*result = channels[2].animation_id;
	}
	else if (g_46fbf4 && channel_valid(&channels[1]) && unknown80 >= 0.9999f)
	{
		*result = channels[1].animation_id;
	}
	else if (channel_valid(&channels[0]))
	{
		*result = channels[0].animation_id;
	}
	else
	{
		*result = none;
	}
	return result;
}

// @retail 0x1cdf00
real s_animation_state::blend_fraction_get()
{
	real fraction = PIN(unknown80 + 0.0001f, 0.0f, 0.9999f) * 1.0002f;

	return PIN(fraction, 0.0f, 1.0f);
}

// @retail 0x1cc2f0
void s_animation_state::names_resolve(s_animation_names *names, long mode, long weapon_class, long weapon_type, long set)
{
	s_graph_name_entry *entry;
	long resolved_weapon_class;

	names->mode = mode;
	names->weapon_class = weapon_class;
	names->weapon_type = weapon_type;
	names->set = set;
	if (mode == 0x7000101)
	{
		names->mode = unknown70;
		if (names->mode == NONE)
		{
			names->mode = 0x6000086;
		}
	}
	if (names->mode == 0x7000001)
	{
		names->mode = 0x6000086;
	}
	if (weapon_class == 0x7000101)
	{
		names->weapon_class = unknown74;
	}
	if (names->weapon_class == 0x7000001 || names->weapon_class == NONE)
	{
		names->weapon_class = 0x7000083;
	}
	if (weapon_type == 0x7000101)
	{
		names->weapon_type = unknown78;
	}
	if (names->weapon_type == 0x7000001 || names->weapon_type == NONE)
	{
		names->weapon_type = 0x30000d9;
	}
	resolved_weapon_class = names->weapon_class;
	entry = (s_graph_name_entry *)function_1dd560((s_sorted_array *)&graph_get()->weapons, names->weapon_type, sizeof(s_graph_name_entry));
	if (entry && entry->weapon_class != NONE && entry->weapon_class != 0x30000d9 && resolved_weapon_class != 0x400054b)
	{
		resolved_weapon_class = entry->weapon_class;
	}
	names->weapon_class = resolved_weapon_class;
	if (set == 0x7000101)
	{
		names->set = unknown7c;
		if (names->set == NONE)
		{
			names->set = 0x400000c;
		}
	}
	if (names->set == 0x7000001)
	{
		names->set = 0x400000c;
	}
}

// @retail 0x1ce1e0
void s_animation_state::animation_touch(c_animation_id animation_id)
{
	if (graph_tag_index != NONE)
	{
		function_1dd9d0(graph_tag_get(graph_tag_index), animation_id);
	}
}

// @retail 0x1ccb80
void function_1ccb80(c_animation_channel *channel, real seconds)
{
	if (channel_valid(channel))
	{
		channel->set_frame_position(seconds * 30.0f);
	}
}

// @retail 0x1cbdb0
bool s_animation_state::node_map_build(long render_model_tag_index, long *node_count, long *node_map)
{
	return function_1dd4c0(render_model_tag_index, graph_tag_get(graph_tag_index), node_count, node_map);
}

// @retail 0x1cb5b0
bool s_animation_state::channel_update(c_animation_channel *channel, long a, long b)
{
	bool result = false;

	if (graph_tag_index != NONE && channel_valid(channel))
	{
		if (!(flags & 1))
		{
			channel->update((long)this, a, b);
		}
		result = true;
	}
	return result;
}

// @retail 0x1cbe50
s_graph_inheritance *s_animation_state::inheritance_get(c_animation_id animation_id)
{
	s_graph_inheritance *result = NULL;

	if (animation_id.index != NONE && animation_id.graph_index != NONE)
	{
		result = function_1daff0(graph_tag_get(graph_tag_index), animation_id);
	}
	return result;
}

// @retail 0x1ccb40
real function_1ccb40(c_animation_channel const *channel)
{
	real result = 0.0f;

	if (channel_valid(channel))
	{
		result = (real)channel->get_animation()->frame_count * (1.0f / 30.0f);
	}
	return result;
}

// @retail 0x1cb3b0
c_animation_id *s_animation_state::variant_get(c_animation_id *result, c_animation_id animation_id)
{
	if (animation_id.index != NONE)
	{
		s_graph_tag *graph = graph_tag_get(graph_tag_index);

		if (graph)
		{
			*result = *function_1dd630(graph, &animation_id, animation_id, (flags >> 1) & 1);
			return result;
		}
	}
	*result = animation_id;
	return result;
}

// @retail 0x1cb410
bool s_animation_state::channel_start(c_animation_channel *channel, c_animation_id animation_id, long unknown08,
	byte unknown0c, byte unknown0d, char unknown0e, word channel_flags)
{
	bool result = false;

	if (graph_tag_index != NONE)
	{
		c_animation_id id = animation_id;

		if (channel_flags & 0x10)
		{
			id = *variant_get(&animation_id, animation_id);
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

// @retail 0x1ce110
void s_animation_state::translation_apply(real_orientation_1ce110 *orientation, real scale)
{
	if (unknown6e & 2)
	{
		c_animation_channel *channel = &channels[2];

		if (channel_valid(channel))
		{
			real fraction = channel->get_frame_ratio() * scale;

			real_point3d *translation = &orientation->translation;
			real x = translation->x + unknown84.i * fraction;
			real y = translation->y + unknown84.j * fraction;
			real z = translation->z + unknown84.k * fraction;

			translation->x = x;
			translation->y = y;
			translation->z = z;
		}
	}
}

// @retail 0x1ce180
void s_animation_state::resources_request(long mode, long weapon_class, long weapon_type, bool urgent, bool other)
{
	if (graph_tag_index != NONE)
	{
		s_animation_names names;

		names_resolve(&names, mode, weapon_class, weapon_type, 0x7000101);
		function_1ddd00(graph_tag_get(graph_tag_index), names.mode, names.weapon_class, names.weapon_type, urgent, other);
	}
}

// @retail 0x1cdf50
void s_animation_state::channels_finish()
{
	channels[2].clear();
	if (channel_valid(&channels[0]))
	{
		channels[0].set_frame_last();
	}
	if (channel_valid(&channels[1]))
	{
		channels[1].set_frame_last();
	}
	unknown64.unknown1 = 0;
	unknown64.unknown0 = 0;
	unknown64.unknown3 = 0;
	if (channel_valid(&channels[2]) && (channels[2].flags & 1))
	{
		channels[2].unknown11 |= 1;
	}
	if (channel_valid(&channels[0]) && (channels[0].flags & 1))
	{
		channels[0].unknown11 |= 1;
	}
	if (channel_valid(&channels[1]) && (channels[1].flags & 1))
	{
		channels[1].unknown11 |= 1;
	}
}

/* whether the graph has the mode; if so, requests its resources (lane B's
   units.cpp declares it with a void pointer) */
// @retail 0x1cb920
bool function_1cb920(void *data, long mode)
{
	s_animation_state *state = (s_animation_state *)data;
	bool result = false;

	if (state->graph_tag_index != NONE)
	{
		bool found = function_1dd560((s_sorted_array *)&state->graph_get()->mode_count, mode, 0x14) != NULL;

		if (found)
		{
			state->resources_request(mode, 0x7000101, 0x7000101, true, false);
		}
		result = found;
	}
	return result;
}

#define DEFAULT_WEAPON_NAME 0x30000d9

/* requests the urgent resources of a mode, weapon class and weapon type */
PRIVATE inline void graph_weapon_type_request(s_graph_tag *graph, long mode, long weapon_class, long weapon_type)
{
	s_graph_weapon_type *animations = (s_graph_weapon_type *)graph_weapon_type_get(graph, mode, weapon_class, weapon_type);

	if (animations)
	{
		long i;

		for (i = 0; i < animations->urgent_resource_count; i++)
		{
			s_cache_resource *resource = &graph->resources[animations->urgent_resources[i]];

			if (resource->streamed)
			{
				function_1236f0(resource, true);
			}
		}
	}
}

PRIVATE inline void graph_weapon_types_request(s_graph_tag *graph, long mode, long weapon_class, long weapon_type)
{
	graph_weapon_type_request(graph, mode, weapon_class, weapon_type);
	graph_weapon_type_request(graph, mode, weapon_class, DEFAULT_WEAPON_NAME);
	graph_weapon_type_request(graph, mode, DEFAULT_WEAPON_NAME, weapon_type);
	graph_weapon_type_request(graph, mode, DEFAULT_WEAPON_NAME, DEFAULT_WEAPON_NAME);
}

// @retail 0x1cc3f0
bool s_animation_state::animation_lookup(s_animation_names *found, s_animation_names *names, long mode, long weapon_class,
	long weapon_type, long set, long lookup_flags, c_animation_id *result)
{
	s_graph_tag *graph = graph_get();
	bool success = false;
	c_animation_id animation_id;

	names_resolve(names, mode, weapon_class, weapon_type, set);
	*found = *names;
	*result = *function_1db170(graph, &animation_id, names->mode, names->weapon_class, names->weapon_type, names->set,
		&found->mode, &found->weapon_class, &found->weapon_type);
	if (result->index != NONE)
	{
		success = true;
	}
	else
	{
		if (lookup_flags & 2)
		{
			*result = *function_1db170(graph, &animation_id, names->mode, names->weapon_class, names->weapon_type, 0x400000c,
				&found->mode, &found->weapon_class, &found->weapon_type);
			if (result->index != NONE)
			{
				found->set = 0x400000c;
				success = true;
			}
		}
		if (result->index == NONE && (lookup_flags & 4))
		{
			long names_mode = names->mode;
			long names_weapon_class = names->weapon_class;
			long names_weapon_type = names->weapon_type;

			if (function_1db120(graph, names_mode, names_weapon_class, names_weapon_type))
			{
				success = true;
			}
			else if (function_1db120(graph, names_mode, names_weapon_class, DEFAULT_WEAPON_NAME))
			{
				success = true;
				found->weapon_type = DEFAULT_WEAPON_NAME;
			}
			else if (function_1db120(graph, names_mode, DEFAULT_WEAPON_NAME, names_weapon_type))
			{
				success = true;
				found->weapon_class = DEFAULT_WEAPON_NAME;
			}
			else if (function_1db120(graph, names_mode, DEFAULT_WEAPON_NAME, DEFAULT_WEAPON_NAME))
			{
				success = true;
				found->weapon_class = DEFAULT_WEAPON_NAME;
				found->weapon_type = DEFAULT_WEAPON_NAME;
			}
			else if (function_1db120(graph, DEFAULT_WEAPON_NAME, DEFAULT_WEAPON_NAME, DEFAULT_WEAPON_NAME))
			{
				success = true;
				found->mode = DEFAULT_WEAPON_NAME;
				found->weapon_class = DEFAULT_WEAPON_NAME;
				found->weapon_type = DEFAULT_WEAPON_NAME;
			}
		}
		if (!success)
		{
			return success;
		}
	}
	{
		long found_mode = found->mode;
		long found_weapon_class = found->weapon_class;
		long found_weapon_type = found->weapon_type;
		long i;

		graph_weapon_types_request(graph, found_mode, found_weapon_class, found_weapon_type);
		for (i = 0; i < graph->inheritance_count; i++)
		{
			graph_weapon_types_request(graph_inherited_get(graph, i), found_mode, found_weapon_class, found_weapon_type);
		}
	}
	return success;
}