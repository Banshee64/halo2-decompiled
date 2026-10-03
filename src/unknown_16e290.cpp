// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_16E290.CPP: requests for the geometry blocks of the structure bsp
   and of render models (geometry_cache.h), and two small initializers */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "geometry_cache.h"
#include "unknown_218850.h"

#include <string.h>

bool function_12dcb0(s_geometry_block_info *block);

extern dword g_4e6494;
extern real_vector3d *g_4687b0;

/* a structure bsp, as read here */
struct s_16e290_cluster
{
	byte unknown00[0xc];
	s_geometry_block_info block;
	byte unknown_end[0x38 - 0xc - sizeof(s_geometry_block_info)];
};

struct s_16e290_index
{
	word unknown0;
	word cluster_index;
	byte unknown4[8];
};

/* a bitmap reference of the structure bsp (4 bytes) */
struct s_16e290_bitmap_reference
{
	short bitmap_index;
	byte unknown2[2];
};

struct s_16e290_bsp
{
	byte unknown00[4];
	long checksum;
	byte unknown08[0x1c - 0x8];
	long unknown1c;
	byte unknown20[0x2c - 0x20];
	s_16e290_bitmap_reference *bitmaps2c;
	byte unknown30[0x40 - 0x30];
	long cluster_count;
	s_16e290_cluster *clusters;
	byte unknown48[4];
	s_16e290_bitmap_reference *bitmaps4c;
	byte unknown50[4];
	s_16e290_index *indices54;
	byte unknown58[0x64 - 0x58];
	s_16e290_index *indices64;
};

struct s_16e290_bsp_globals
{
	byte unknown00[0x80];
	long bsp_count;
	s_16e290_bsp *bsp;
};

struct s_16e290_match_view
{
	byte unknown0[8];
	long checksum;
};

s_16e290_bsp_globals *g_4e0344;

/* the object with a geometry block at +0x28 and a result at +0x50 */
struct s_16e290_resource
{
	byte unknown00[0x28];
	s_geometry_block_info block;
	byte unknown_pad[0x50 - 0x28 - sizeof(s_geometry_block_info)];
	long value;
};

// @retail 0x16e290
long function_16e290(s_16e290_resource *resource)
{
	long result = 0;

	if (!g_4e6494 || function_12de70(&resource->block, 0))
	{
		result = resource->value;
	}
	return result;
}

#define PIN(x, lo, hi) ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

// @retail 0x16e5a0
void function_16e5a0(long cluster_index, short bsp_index)
{
	if (bsp_index == g_4686c4)
	{
		s_16e290_bsp *bsp = g_4e0344->bsp;

		if (cluster_index != NONE && PIN(cluster_index, 0, bsp->cluster_count - 1) == cluster_index)
		{
			function_12dcb0(&bsp->clusters[cluster_index].block);
		}
	}
}

/* the render model sections, as read here */
struct s_16e290_section
{
	byte unknown00[0x28];
	s_geometry_block_info block;
	byte unknown_pad[0xc8 - 0x28 - sizeof(s_geometry_block_info)];
};

struct s_16e290_permutation
{
	byte unknown00[0x34];
	short section_index;
	byte unknown36[0x58 - 0x36];
};

struct s_16e290_render_model
{
	byte unknown000[0x13c];
	s_16e290_section *sections;
	byte unknown140[4];
	s_16e290_permutation *permutations;
};

// @retail 0x16e770
bool function_16e770(long render_model_index, long permutation_index)
{
	s_16e290_render_model *render_model = (s_16e290_render_model *)g_4e3b44[render_model_index & 0xffff].bytes;
	s_16e290_section *section = &render_model->sections[render_model->permutations[permutation_index].section_index];

	return function_12dcb0(&section->block);
}

// @retail 0x16e890
bool function_16e890(long index)
{
	bool result = true;

	if (g_4e0344 && g_4e0344->bsp_count > 0 && g_4e0348)
	{
		s_16e290_bsp *bsp = g_4e0344->bsp;

		if (bsp->unknown1c != NONE && bsp->checksum == ((s_16e290_match_view *)g_4e0348)->checksum)
		{
			s_16e290_cluster *cluster = &bsp->clusters[bsp->indices64[index].cluster_index];

			result = function_12dcb0(&cluster->block);
		}
	}
	return result;
}

// @retail 0x16e8f0
bool function_16e8f0(long index)
{
	bool result = true;

	if (g_4e0344 && g_4e0344->bsp_count > 0 && g_4e0348)
	{
		s_16e290_bsp *bsp = g_4e0344->bsp;

		if (bsp->unknown1c != NONE && bsp->checksum == ((s_16e290_match_view *)g_4e0348)->checksum)
		{
			s_16e290_cluster *cluster = &bsp->clusters[bsp->indices54[index].cluster_index];

			result = function_12dcb0(&cluster->block);
		}
	}
	return result;
}

/* a 0x44 byte state cleared with the default vectors */
struct s_16f460
{
	byte unknown00[0x28];
	real scale;
	real_vector3d forward;
	real_vector3d up;
	byte unknown44[0xac - 0x44];
};

extern real g_54e854;

// @retail 0x16f460
void function_16f460(s_16f460 *state)
{
	memset(state, 0, sizeof(s_16f460));
	state->forward = *g_4687a8;
	state->up = *g_4687b0;
	state->scale = g_54e854;
}

bool g_4ea935;
extern byte g_4ea936;
extern long g_4e64a0;
extern long g_4e64a4;
extern long g_4e64ac; /* unknown_12de70.cpp */
extern long g_4e6470; /* unknown_12be90.cpp */
long g_4e6474;

// @retail 0x16f200
void function_16f200(void)
{
	if (g_4ea935)
	{
		if (g_4e6948->state != 2)
		{
			g_4e64a4 = 1;
			g_4e64a0 = 3;
			g_4e64ac = 0;
			g_4e6470 = 3;
			g_4e6474 = 1;
		}
		g_4ea935 = false;
	}
	if (g_4ea936)
	{
		if (g_4e6948->state != 2)
		{
			g_4e64a4 = 0;
			g_4e64a0 = 3;
			g_4e64ac = 0;
			g_4e6470 = 3;
			g_4e6474 = 0;
		}
		g_4ea936 = false;
	}
}

struct s_location;
void function_11bed0(real_point3d const *point, s_location *location);

/* the state at g_510c50 (13bf00), as read here */
struct s_16f120_state
{
	byte unknown0[5];
	bool active;
};

struct s_unknown_13bf00;
extern s_unknown_13bf00 *g_510c50;

static inline bool local_user_in_use(long user_index)
{
	return g_4e8c20->entries[user_index] != NONE;
}

// @retail 0x16f120
void function_16f120(void)
{
	long user_index;

	for (user_index = 0; user_index < 4; user_index++)
	{
		if (user_index != NONE && local_user_in_use(user_index))
		{
			s_player_state *state = &g_4e9bd4[user_index].state;

			function_11bed0(&state->position, (s_location *)state->unknown0c);
		}
	}
	if ((!g_510c50 || !((s_16f120_state *)g_510c50)->active) && g_510c54->game_time > 0)
	{
		g_4ea935 = true;
	}
}

/* a camera-like state: a point, a location and three vectors */
struct s_16f3c0
{
	real_point3d position;
	long unknown0c;
	short unknown10;
	short bsp_index;
	real_vector3d vector14;
	real_vector3d forward;
	real_vector3d up;
	real scale;
	byte unknown3c[4];
	real unknown40;
	real unknown44;
};

extern real_point3d *g_468788;
extern real_vector3d *g_4687a4;

// @retail 0x16f3c0
void function_16f3c0(s_16f3c0 *state)
{
	state->position = *g_468788;
	state->bsp_index = g_4686c4;
	state->unknown0c = NONE;
	state->unknown10 = NONE;
	state->vector14 = *g_4687a4;
	state->forward = *g_4687a8;
	state->up = *g_4687b0;
	state->scale = g_54e854;
	state->unknown40 = 0.0f;
	state->unknown44 = 0.0f;
}
/* a bit table of rows of count bits each (0x16e150) */
struct s_16e150_bits
{
	byte unknown00[0x54];
	dword size;
	dword *bits;
	byte unknown5c[0x9c - 0x5c];
	long count;
};

// @retail 0x16e150
dword *function_16e150(s_16e150_bits const *table, short index)
{
	long row_words = (table->count + 31) >> 5;
	long table_words = table->count * row_words;

	if ((dword)(table_words * 4) < table->size)
	{
		return &table->bits[index * row_words + table_words];
	}
	return &table->bits[((table->count + 31) >> 5) * index];
}

/* the cache flags (bit 4 forces every cluster test to pass) */
struct s_4e64c8_flags
{
	dword unknown0 : 4;
	dword all_clusters : 1;
	dword unknown5 : 27;
};

s_4e64c8_flags g_4e64c8;

struct s_16e210_cluster
{
	byte unknown00[0x70];
	char cluster_reference;
	byte unknown71[0xb0 - 0x71];
};

struct s_16e210_reference
{
	short index;
	byte unknown02[0x18 - 2];
};

struct s_16e210_match_view
{
	byte unknown00[0x68];
	s_16e210_reference *references;
	byte unknown6c[0xa0 - 0x6c];
	s_16e210_cluster *clusters;
};

struct s_16e210_source
{
	byte unknown00[8];
	long value;
	byte unknown0c[4];
};

struct s_16e210_globals_view
{
	byte unknown000[0x34c];
	s_16e210_source *sources;
};

// @retail 0x16e210
bool function_16e210(long cluster_index, long value)
{
	s_16e210_match_view *match = (s_16e210_match_view *)g_4e0348;
	s_16e210_globals_view *globals = (s_16e210_globals_view *)g_4e0350;
	bool result = false;

	if (globals && match && cluster_index != NONE)
	{
		if (TEST_FIELD_BIT(g_4e64c8.all_clusters))
		{
			result = true;
		}
		else
		{
			long reference = match->clusters[cluster_index].cluster_reference;

			if ((char)reference != NONE)
			{
				if (value == NONE)
				{
					result = true;
				}
				else
				{
					long index = (char)reference < 0 ? match->references[reference & 0x7f].index : reference & 0x7f;

					if (index != NONE && globals->sources[index].value == value)
					{
						result = true;
					}
				}
			}
		}
	}
	return result;
}

/* a local player's state (g_4e9bd4, 0x358 bytes), as initialized here */
struct s_16f4b0_player
{
	dword signature;
	byte unknown004[4];
	s_16f460 view;
	byte unknown0b4;
	bool unknown0b5;
	bool unknown0b6;
	byte unknown0b7;
	s_16f3c0 state;
	byte unknown100[0x130 - 0x100];
	real scale;
	real_vector3d forward;
	real_vector3d up;
	byte unknown14c[0x354 - 0x14c];
	dword signature354;
};

// @retail 0x16f4b0
void __stdcall function_16f4b0(void *player_)
{
	s_16f4b0_player *player = (s_16f4b0_player *)player_;

	player->forward = *g_4687a8;
	player->up = *g_4687b0;
	player->scale = g_54e854;
	function_16f3c0(&player->state);
	function_16f460(&player->view);
	player->signature354 = 'rad!';
	player->signature = 'rad!';
	player->unknown0b4 = 1;
	player->unknown0b5 = false;
	player->unknown0b6 = false;
}

/* the bitmaps of a bitmap tag (0x74 bytes each), as read here */
struct s_bitmap_data;

struct s_16e290_bitmap_group
{
	byte unknown00[0x48];
	byte *bitmaps;
};

bool texture_cache_bitmap_request(s_bitmap_data *bitmap);

// @retail 0x16e7b0
bool function_16e7b0(long index)
{
	bool result = true;

	if (g_4e0344 && g_4e0344->bsp_count > 0 && g_4e0348)
	{
		s_16e290_bsp *bsp = g_4e0344->bsp;

		if (bsp->unknown1c != NONE && bsp->checksum == ((s_16e290_match_view *)g_4e0348)->checksum)
		{
			short bitmap_index = bsp->bitmaps2c[index].bitmap_index;

			if (bitmap_index != NONE)
			{
				s_16e290_bitmap_group *group = (s_16e290_bitmap_group *)g_4e3b44[bsp->unknown1c & 0xffff].bytes;

				s_bitmap_data *bitmap = (s_bitmap_data *)(group->bitmaps + bitmap_index * 0x74);

				result = texture_cache_bitmap_request(bitmap);
			}
		}
	}
	return result;
}

// @retail 0x16e820
bool function_16e820(long index)
{
	bool result = true;

	if (g_4e0344 && g_4e0344->bsp_count > 0 && g_4e0348)
	{
		s_16e290_bsp *bsp = g_4e0344->bsp;

		if (bsp->unknown1c != NONE && bsp->checksum == ((s_16e290_match_view *)g_4e0348)->checksum)
		{
			long bitmap_index = bsp->bitmaps4c[index].bitmap_index;

			if (bitmap_index != NONE)
			{
				s_16e290_bitmap_group *group = (s_16e290_bitmap_group *)g_4e3b44[bsp->unknown1c & 0xffff].bytes;

				s_bitmap_data *bitmap = (s_bitmap_data *)(group->bitmaps + bitmap_index * 0x74);

				result = texture_cache_bitmap_request(bitmap);
			}
		}
	}
	return result;
}

/* the geometry of a model-like tag, as read here */
struct s_16e950_section
{
	byte unknown00[0x38];
	s_geometry_block_info block;
};

struct s_16e950_part
{
	byte unknown00[4];
	short section_index;
	byte unknown06[0x10 - 6];
};

struct s_16e950_group
{
	byte unknown00[8];
	long part_count;
	s_16e950_part *parts;
};

struct s_16e950_definition
{
	byte unknown00[0x1c];
	long group_count;
	s_16e950_group *groups;
	long section_count;
	s_16e950_section *sections;
};

// @retail 0x16e950
bool function_16e950(long tag_index, short mode)
{
	s_16e950_definition *definition = (s_16e950_definition *)g_4e3b44[tag_index & 0xffff].bytes;
	bool result = true;

	if (mode != 1)
	{
		long section_index;

		for (section_index = 0; section_index < definition->section_count; section_index++)
		{
			s_16e950_section *section = &definition->sections[section_index];
			bool loaded = function_12dcb0(&section->block);

			result = result && loaded;
		}
	}
	else
	{
		long group_index;

		for (group_index = 0; group_index < definition->group_count; group_index++)
		{
			s_16e950_group *group = &definition->groups[group_index];
			long part_index;

			for (part_index = 0; part_index < group->part_count; part_index++)
			{
				s_16e950_section *section = &definition->sections[group->parts[part_index].section_index];
				bool loaded = function_12dcb0(&section->block);

				result = result && loaded;
			}
		}
	}
	return result;
}

/* the sound tables of the sound globals, as read here */
struct s_16ea60_pitch_range
{
	byte unknown00[8];
	short first_permutation;
	short permutation_count;
};

struct s_16ea60_permutation
{
	byte unknown00[0xc];
	short first_chunk;
	byte unknown0e[2];
};

struct s_16ea60_tables
{
	byte unknown00[0x24];
	s_16ea60_pitch_range *pitch_ranges;
	byte unknown28[4];
	s_16ea60_permutation *permutations;
	byte unknown30[0x44 - 0x30];
	s_sound_chunk *chunks;
};

struct s_16ea60_sound
{
	byte unknown00[8];
	short first_pitch_range;
	char pitch_range_count;
};

struct s_sound_globals;
extern s_sound_globals *g_51ebd4;

// @retail 0x16ea60
bool function_16ea60(long sound_index)
{
	s_16ea60_sound *sound = (s_16ea60_sound *)g_4e3b44[sound_index & 0xffff].bytes;
	long pitch_range_count = sound->pitch_range_count;
	bool result = true;
	long pitch_range_index;

	for (pitch_range_index = 0; pitch_range_index < pitch_range_count; pitch_range_index++)
	{
		s_16ea60_tables *tables = (s_16ea60_tables *)g_51ebd4;
		s_16ea60_pitch_range *pitch_range = &tables->pitch_ranges[sound->first_pitch_range + pitch_range_index];
		long permutation_count = pitch_range->permutation_count;
		long permutation_index;

		for (permutation_index = 0; permutation_index < permutation_count; permutation_index++)
		{
			s_16ea60_permutation *permutation = &tables->permutations[pitch_range->first_permutation + permutation_index];
			dword flags = function_218850(sound_index, &tables->chunks[permutation->first_chunk], 8);

			result = result && (flags & 2);
			tables = (s_16ea60_tables *)g_51ebd4;
		}
	}
	return result;
}

extern byte g_4ea934;
void __stdcall function_16f4b0(void *player_);

// @retail 0x16f0e0
void function_16f0e0(void)
{
	long i;

	g_4ea934 = 1;
	for (i = 0; i < 4; i++)
	{
		function_16f4b0(&g_4e9bd4[i]);
	}
	for (i = 0; i < 5; i++)
	{
		g_510c70[i] = NONE;
	}
}

/* a list of ranges (0x48 bytes each) */
struct s_16e1b0_range
{
	byte unknown00[6];
	word first;
	word count;
	byte unknown0a[0x48 - 0xa];
};

struct s_16e1b0_list
{
	long count;
	s_16e1b0_range *ranges;
};

// @retail 0x16e1b0
void function_16e1b0(long value, s_16e1b0_list const *list, long *unknown, long *range_index, long *offset)
{
	long i;

	*unknown = 0;
	for (i = 0; i < list->count; i++)
	{
		s_16e1b0_range const *range = &list->ranges[i];

		if (range->first <= value && range->first + range->count > value)
		{
			*offset = value - range->first;
			break;
		}
	}
	if (i == list->count)
	{
		*range_index = NONE;
	}
	else
	{
		*range_index = i;
	}
}

/* a resource a tag predicts it will need (8 bytes) */
struct s_predicted_resource
{
	short type;
	short index;
	long tag_index;
};

struct s_predicted_resource_block
{
	long count;
	s_predicted_resource *resources;
};

/* the clusters of a structure bsp tag (0xb0 bytes each) */
struct s_16e5e0_cluster
{
	byte unknown00[0x28];
	s_geometry_block_info block;
	byte unknown4c[0xb0 - 0x28 - sizeof(s_geometry_block_info)];
};

struct s_16e5e0_bsp
{
	byte unknown00[0xa0];
	s_16e5e0_cluster *clusters;
};

/* not decompiled yet (src/stubs/lane_t.cpp) */
long function_3bcb0(s_bitmap_data *bitmap);

// @retail 0x16e5e0
bool function_16e5e0(s_predicted_resource_block const *block, short mode)
{
	bool result = true;
	long i;

	for (i = 0; i < block->count; i++)
	{
		s_predicted_resource const *resource = &block->resources[i];
		bool loaded = true;

		switch (resource->type)
		{
		case 0:
			if (mode != 1)
			{
				if (mode == 2)
				{
					s_16e290_bitmap_group *group = (s_16e290_bitmap_group *)g_4e3b44[resource->tag_index & 0xffff].bytes;

					function_3bcb0((s_bitmap_data *)(group->bitmaps + resource->index * 0x74));
				}
				else
				{
					s_16e290_bitmap_group *group = (s_16e290_bitmap_group *)g_4e3b44[resource->tag_index & 0xffff].bytes;

					texture_cache_bitmap_request((s_bitmap_data *)(group->bitmaps + resource->index * 0x74));
				}
			}
			break;
		case 1:
			loaded = function_16ea60(resource->tag_index);
			break;
		case 2:
			{
				s_16e5e0_bsp *bsp = (s_16e5e0_bsp *)g_4e3b44[resource->tag_index & 0xffff].bytes;
				s_16e5e0_cluster *cluster = &bsp->clusters[resource->index];

				loaded = function_12dcb0(&cluster->block);
			}
			break;
		case 3:
			if (mode != 3)
			{
				loaded = function_16e770(resource->tag_index, resource->index);
			}
			break;
		case 4:
			if (mode != 1)
			{
				loaded = function_16e7b0(resource->index);
			}
			break;
		case 5:
			if (mode != 1)
			{
				loaded = function_16e820(resource->index);
			}
			break;
		case 6:
			loaded = function_16e890(resource->index);
			break;
		case 7:
			loaded = function_16e8f0(resource->index);
			break;
		case 8:
			loaded = function_16e950(resource->tag_index, mode);
			break;
		}
		result = result && loaded;
	}
	return result;
}