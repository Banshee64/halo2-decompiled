// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_11C050.CPP: queries on the cluster of a location (s_location).
   Decompiled by lane R for the effects (0x178020, 0x179730). */

#include "cseries.h"
#include "globals.h"
#include "object_queries.h"

struct s_11c120_cluster
{
	byte unknown00[0x70];
	byte zone;
	byte unknown71[0xb0 - 0x71];
};

struct s_11c120_zone
{
	byte unknown00[2];
	short index;
	plane3f plane;
	byte unknown14[4];
};

struct s_11c120_bsp
{
	byte unknown00[0x68];
	s_11c120_zone *zones;
	byte unknown6c[0xa0 - 0x6c];
	s_11c120_cluster *clusters;
};

#define CLUSTER_BITS(offset) ((dword *)((byte *)g_4e6948 + (offset)))

// @retail 0x11c050
bool function_11c050(s_location const *location)
{
	long cluster_index = location->cluster_index;

	return (CLUSTER_BITS(0x1178)[cluster_index >> 5] & (1 << (cluster_index & 31))) != 0;
}

// @retail 0x11c080
bool function_11c080(s_location const *location)
{
	long cluster_index = location->cluster_index;

	return (CLUSTER_BITS(0x1138)[cluster_index >> 5] & (1 << (cluster_index & 31))) != 0;
}

// @retail 0x11c120
bool function_11c120(s_location const *location, point3f const *point, short *zone_index)
{
	bool result = false;
	short cluster_index = location->cluster_index;

	if (cluster_index != NONE)
	{
		s_11c120_bsp *bsp = (s_11c120_bsp *)g_4e0348;
		byte zone = bsp->clusters[cluster_index].zone;

		if (zone != 0xff)
		{
			s_11c120_zone *entry = &bsp->zones[zone & 0x7f];
			short index = entry->index;

			if (index != NONE)
			{
				if (!(zone & 0x80) || 0.0f > entry->plane.k * point->z + entry->plane.j * point->y + entry->plane.i * point->x - entry->plane.d)
				{
					result = true;
					if (zone_index)
						*zone_index = index;
				}
			}
		}
	}
	return result;
}
