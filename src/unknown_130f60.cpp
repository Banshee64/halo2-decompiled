// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_130F60.CPP */

#include "unknown_11c920.h"
#include "globals.h"

/* the scenario's palette entries (8 bytes each): a tag reference */
struct s_scenario_palette_entry
{
	byte unknown00[4];
	long tag_index;
};

/* the palette sources at +0x214 (0x44 bytes each) */
struct s_palette_source_view
{
	byte unknown00[0x3c];
	byte flags;
	byte unknown3d[3];
	short palette_index;
	byte unknown42[2];
};

struct s_scenario_palette_view
{
	byte unknown000[8];
	long palette_count;
	s_scenario_palette_entry *palette;
	byte unknown010[0x214 - 0x10];
	s_palette_source_view *sources;
};

struct s_palette_owner
{
	byte unknown00[0x6c];
	char palette_index;
	byte unknown6d;
	char default_palette_index;
};

struct s_palette_tag_view
{
	byte unknown00[0x10];
	byte flags;
};

// @retail 0x130f60
long function_130f60(s_palette_owner const *owner, long *palette_index)
{
	s_scenario_palette_view *scenario = (s_scenario_palette_view *)g_4e0350;
	long result = NONE;
	*palette_index = NONE;

	if (owner->palette_index != NONE)
	{
		if (owner->palette_index >= 0 && owner->palette_index < scenario->palette_count)
		{
			s_scenario_palette_entry *entry = &scenario->palette[owner->palette_index];
			*palette_index = owner->palette_index;
			return entry->tag_index;
		}
	}
	else
	{
		if (owner->default_palette_index != NONE && owner->default_palette_index >= 0 && owner->default_palette_index < scenario->palette_count)
		{
			s_scenario_palette_entry *entry = &scenario->palette[owner->default_palette_index];
			if (entry->tag_index != NONE && !(((s_palette_tag_view *)g_4e3b44[entry->tag_index & 0xffff].bytes)->flags & 0x10))
			{
				*palette_index = owner->default_palette_index;
				return entry->tag_index;
			}
		}
	}

	s_palette_source_view *source = &scenario->sources[g_4686c4];
	if (source->flags & 1)
	{
		short index = source->palette_index;
		if (index != NONE && index >= 0 && index < scenario->palette_count)
		{
			s_scenario_palette_entry *entry = &scenario->palette[index];
			if (entry->tag_index != NONE)
			{
				*palette_index = index;
				result = entry->tag_index;
			}
		}
	}

	return result;
}
