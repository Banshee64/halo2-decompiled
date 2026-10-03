// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_11C120.CPP: the material of the structure bsp's cluster at a
   location (the cluster's weather or water volume) */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "object_queries.h"

/* a cluster's volume: its material and the plane bounding it */
struct s_structure_cluster_volume
{
	short unknown00;
	short material;
	real_plane3d plane;
	byte unknown14[0x18 - 0x14];
};

struct s_structure_cluster_view
{
	byte unknown00[0x70];
	byte volume_index;
	byte unknown71[0xb0 - 0x71];
};

struct s_structure_bsp_clusters_view
{
	byte unknown00[0x68];
	s_structure_cluster_volume *volumes;
	byte unknown6c[0xa0 - 0x6c];
	s_structure_cluster_view *clusters;
};

// @retail 0x11c120
bool function_11c120(
	s_location const *location,
	real_point3d const *point,
	short *material)
{
	bool result = false;

	if (location->cluster_index != NONE)
	{
		s_structure_bsp_clusters_view *bsp = (s_structure_bsp_clusters_view *)g_4e0348;
		byte volume_index = bsp->clusters[location->cluster_index].volume_index;

		if (volume_index != 0xff)
		{
			s_structure_cluster_volume *volume = &bsp->volumes[volume_index & 0x7f];
			short volume_material = volume->material;

			if (volume_material != NONE &&
				(!(volume_index & 0x80) || plane_distance_to_point(&volume->plane, point) < 0.0f))
			{
				result = true;
				if (material)
				{
					*material = volume_material;
				}
			}
		}
	}
	return result;
}
