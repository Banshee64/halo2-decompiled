#include "cseries.h"
#include "real_math.h"
#include "globals.h"

// @flags /O2 /arch:SSE /Gr

/* a bsp3d: nodes of 8 bytes (a plane index and two 24-bit child indices,
   whose bit 23 marks a leaf) and planes of 16 bytes */
struct s_bsp3d_node
{
	short plane;
	byte children[6];
};

struct s_bsp3d_plane
{
	real_vector3d normal;
	real distance;
};

struct s_bsp3d
{
	byte unknown00[4];
	s_bsp3d_node *nodes;
	byte unknown08[4];
	s_bsp3d_plane *planes;
};

// @retail 0x14a280
long function_14a280(s_bsp3d *bsp, real_point3d *point, long index)
{
	do
	{
		s_bsp3d_node *node = &bsp->nodes[index & 0x7fffff];
		s_bsp3d_plane *plane = &bsp->planes[node->plane];
		real distance = plane->normal.k * point->z + plane->normal.j * point->y + plane->normal.i * point->x - plane->distance;
		bool side = distance >= 0.0f;

		index = *(long *)&node->children[side * 3 - 1] >> 8;
	}
	while (!(index & 0x800000));

	if (index != NONE)
		return index & 0x7fffff;
	return NONE;
}

// @retail 0x14a300
real_plane3d *bsp3d_get_plane(s_bsp3d const *bsp, short plane_index, real_plane3d *plane)
{
	s_bsp3d_plane *source = &bsp->planes[plane_index & 0x7fff];

	if (plane_index < 0)
	{
		plane->i = 0.0f - source->normal.i;
		plane->j = 0.0f - source->normal.j;
		plane->k = 0.0f - source->normal.k;
		plane->d = 0.0f - source->distance;
	}
	else
	{
		*plane = *(real_plane3d *)source;
	}
	return plane;
}

// @retail 0x14a550
void structure_clusters_from_bit_vector(dword const *bits, short *count, short maximum_count, short *clusters)
{
	s_match_globals *bsp = g_4e0348;
	short cluster_count = 0;
	long i;

	*count = cluster_count;
	for (i = 0; i < bsp->list_count; i++)
	{
		if (bits[i >> 5] & (1 << (i & 0x1f)))
		{
			if (cluster_count < maximum_count)
			{
				clusters[cluster_count] = (short)i;
				cluster_count++;
			}
			(*count)++;
		}
	}
}

/* the fields of a structure bsp's clusters (0xb0 bytes each) read here */
struct s_cluster_view
{
	byte unknown00[0x6e];
	char sky_index;
	byte unknown6f[0xb0 - 0x6f];
};

struct s_structure_bsp_clusters_view
{
	byte unknown00[0xa0];
	s_cluster_view *clusters;
};

struct s_scenario_sky_reference
{
	byte unknown00[4];
	long sky_index;
};

struct s_scenario_skies_view
{
	byte unknown00[8];
	long sky_count;
	s_scenario_sky_reference *skies;
	byte unknown10[2];
	byte flags;
};

struct s_sky_view
{
	byte unknown00[4];
	long unknown04;
	byte unknown08[8];
	byte flags;
	byte unknown11[0xa0 - 0x11];
	real_vector3d vector;
};

// @retail 0x14b360
void cluster_get_sky(long cluster_index, long *sky_index, bool *found, real_vector3d *vector)
{
	s_scenario_skies_view *scenario = (s_scenario_skies_view *)g_4e0350;

	*found = false;
	*sky_index = NONE;

	if (cluster_index != NONE)
	{
		*sky_index = ((s_structure_bsp_clusters_view *)g_4e0348)->clusters[cluster_index].sky_index;
		if (*sky_index == NONE && (scenario->flags & 2) && scenario->sky_count > 0)
		{
			*sky_index = 0;
		}

		short index = (short)*sky_index;
		long tag_index = NONE;

		if (index >= 0 && index < scenario->sky_count)
		{
			tag_index = scenario->skies[index].sky_index;
		}

		if (tag_index != NONE)
		{
			s_sky_view *sky = (s_sky_view *)g_4e3b44[tag_index & 0xffff].bytes;

			if (sky && sky->unknown04 != NONE && (sky->flags & 0x20))
			{
				*vector = sky->vector;
				*found = true;
			}
		}
	}
}
