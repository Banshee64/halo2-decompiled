// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1CAFC0.CPP: the object that drives three animation channels of
   one graph (0x1cafc0..0x1ce1e0) */

#include "cseries.h"
#include "globals.h"
#include "unknown_1c62f0.h"

/* a 4 byte state at +0x60 and +0x64 of the animation state */
struct s_animation_bits
{
	byte unknown0;
	byte unknown1;
	byte unknown2;
	byte unknown3;

	s_animation_bits() : unknown0(0), unknown1(0), unknown2(0), unknown3(0) {}
};

/* the graph tag as the animation state reads it */
struct s_graph_mode
{
	long name;
	byte unknown04[0x1c];
};

struct s_graph_entry
{
	byte unknown00[0xc];
};

/* a tag block: a count and the elements */
struct s_graph_block
{
	long count;
	void *elements;
};

struct s_graph_definition
{
	byte unknown00[0xc];
	long mode_count;
	s_graph_mode *modes;
	long entry_count;
	s_graph_entry *entries;
	byte unknown1c[0x54 - 0x1c];
	s_graph_block unknown54;
};

/* the names an animation is looked up by */
struct s_animation_names
{
	long mode;
	long weapon_class;
	long weapon_type;
	long set;
};

/* an entry of the block at +0x54 of the graph (0x1dd560 finds one by name) */
struct s_graph_name_entry
{
	long name;
	long weapon_class;
};

/* a binary search of a sorted block (unknown_1dd560.cpp) */
struct s_sorted_array;
void *function_1dd560(s_sorted_array *array, long key, long element_size);

#define PIN(value, lower, upper) ((value) < (lower) ? (lower) : (value) > (upper) ? (upper) : (value))

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

	s_animation_state();
	void reset();
	void names_resolve(s_animation_names *names, long mode, long weapon_class, long weapon_type, long set);
	void channels_clear_partial();
	short mode_count_get();
	long mode_find(long name);
	s_graph_entry *entry_get(long index);
	long mode_count();
	s_graph_definition *graph_get();
	void update_blend_flags();
	void channels_clear();
	void secondary_channels_clear();
	c_animation_id *current_animation_get(c_animation_id *result);
	real blend_fraction_get();
};

bool g_46fbf4 = true;
bool g_46fbf5 = true;

inline s_graph_definition *graph_definition_get(long tag_index)
{
	return (s_graph_definition *)g_4e3b44[tag_index & 0xffff].bytes;
}

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
short s_animation_state::mode_count_get()
{
	return (short)graph_get()->mode_count;
}

// @retail 0x1cbde0
long s_animation_state::mode_find(long name)
{
	s_graph_definition *graph = graph_get();
	long i;

	for (i = 0; i < graph->mode_count; i++)
	{
		if (graph->modes[i].name == name)
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
long s_animation_state::mode_count()
{
	s_graph_definition *graph = graph_get();
	long count = 0;

	if (graph)
	{
		count = (short)graph->mode_count;
	}
	return count;
}

// @retail 0x1cc2d0
s_graph_definition *s_animation_state::graph_get()
{
	return graph_definition_get(graph_tag_index);
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
	entry = (s_graph_name_entry *)function_1dd560((s_sorted_array *)&graph_get()->unknown54, names->weapon_type, sizeof(s_graph_name_entry));
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
