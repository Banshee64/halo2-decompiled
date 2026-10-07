// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_23AA70.CPP: a pass over the structure's clusters */

#include "unknown_11c920.h"
#include "globals.h"
#include <math.h>

/* the structure's clusters (0xb0 bytes each), as this file reads them */
struct s_cluster_23aa
{
	byte unknown00[0x7c];
	short value7c;
	word count7e;
	byte unknown80[0xb0 - 0x80];
};

struct s_cluster_spawn_23aa;

struct s_structure_23aa
{
	byte unknown00[0x9c];
	long cluster_count;
	s_cluster_23aa *clusters;
	byte unknowna4[0xf4 - 0xa4];
	long valuef4;
	s_cluster_spawn_23aa *entries;
};

void function_17d710(short cluster_index);

/* calls 0x17d710 on every cluster that has a value7c and a count */
// @retail 0x23aa70
void function_23aa70(void)
{
	s_structure_23aa *structure = (s_structure_23aa *)g_4e0348;

	if (structure->valuef4)
	{
		for (short i = 0; i < structure->cluster_count; i++)
		{
			s_cluster_23aa *cluster = &structure->clusters[i];

			if (cluster->value7c != NONE && cluster->count7e > 0)
			{
				function_17d710(i);
			}
		}
	}
}

struct s_cluster_spawn_23aa
{
	point3f position;
	byte palette_index;
	byte unknown0d;
	char yaw;
	char pitch;
};

struct s_cluster_palette_23aa
{
	long unused;
	long tag_index;
};

struct s_cluster_palette_globals_23aa
{
	byte unknown00[0x144];
	s_cluster_palette_23aa *types;
};

struct s_effect_source;
void function_17e670(s_effect_source *source, point3f const *point, long tag_index,
	vector3f const *direction, real radius, long first, long second, long third);

/* Starts and stops cluster effects when the active cluster masks change. */
// @retail 0x23aad0
void __stdcall function_23aad0(long previous_address, long current_address, long count)
{
	(void)&previous_address;
	(void)&current_address;
	(void)&count;
	s_structure_23aa *structure = (s_structure_23aa *)g_4e0348;
	dword const *previous = (dword const *)previous_address;
	dword const *current = (dword const *)current_address;
	if (structure->valuef4)
	{
		for (long i = 0; (short)i < count; i++)
		{
			s_cluster_23aa *cluster = &structure->clusters[(short)i];
			if (cluster->value7c != NONE && cluster->count7e > 0)
			{
				long slot = (short)i >> 5;
				dword mask = 1UL << ((short)i & 0x1f);
				dword was_active = previous[slot] & mask;
				if (was_active && !(current[slot] & mask))
				{
					function_17d710((short)i);
				}
				else if (!was_active && (current[slot] & mask))
				{
					for (long j = 0; j < cluster->count7e; j++)
					{
						s_cluster_spawn_23aa *entry = &structure->entries[cluster->value7c + j];
						long tag_index = ((s_cluster_palette_globals_23aa *)g_4e0350)->types[entry->palette_index].tag_index;
						real yaw = entry->yaw * 0.02473695f;
						real pitch = entry->pitch * 0.012368475f;
						vector3f direction;
						direction.i = (real)(cos(pitch) * cos(yaw));
						direction.j = (real)(cos(pitch) * sin(yaw));
						direction.k = (real)sin(pitch);
						function_17e670(0, &entry->position, tag_index, &direction, 1.0f, 1, NONE, 0);
					}
				}
			}
		}
	}
}
