// @flags /O2 /arch:SSE /Gr
/* SCENARIO_FOG_CLUSTERS.CPP: part of scenario_fog.cpp (src/scenario_fog.cpp
   holds the rest): the clusters of the structure bsp a point can see through
   portals, nearest first, each with its distance through the portals. */

#include "cseries.h"
#include "real_math.h"
#include "globals.h"
#include <string.h>

#define MAXIMUM_CLUSTERS 512
#define k_fog_cluster_maximum_distance 10.0f

/* a portal between two clusters (0x24 bytes) */
struct s_fog_cluster_portal
{
	short front_cluster;
	short back_cluster;
	long plane_index;
	point3f center;
	real radius;
	byte unknown18[0xc];
};

/* a cluster (0xb0 bytes): the portals it has */
struct s_fog_cluster
{
	byte unknown00[0x8c];
	long portal_count;
	short *portal_indices;
	byte unknown94[0xb0 - 0x94];
};

/* the structure bsp (g_4e0348), as the clusters are walked */
struct s_fog_structure_bsp
{
	byte unknown00[0x60];
	s_fog_cluster_portal *portals;
	byte unknown64[0xa0 - 0x64];
	s_fog_cluster *clusters;
};

/* a cluster reached: its index, how far away it is, and the cluster it was
   reached from (0x18 bytes) */
struct s_fog_cluster_distance
{
	long cluster_index;
	real distance;
	byte unknown08[0xc];
	long previous_index;
};

struct s_14b240_owner;
struct s_bsp3d_disk;
real function_14b240(s_14b240_owner const *owner, s_bsp3d_disk const *disk, point3f const *point);

/* the clusters within 10 world units of a point through the portals,
   starting from the point's cluster, with the shortest distance found to
   each; returns nothing (the count ends the list with NONE) */
// @retail 0x12e960
void scenario_fog_clusters_find(long cluster_index, point3f const *point, s_fog_cluster_distance *clusters)
{
	s_fog_structure_bsp *local_9e9b6e = (s_fog_structure_bsp *)g_4e0348;
	dword visited[MAXIMUM_CLUSTERS / 32];
	short indices[MAXIMUM_CLUSTERS];
	long count;
	long current = 0;

	memset(visited, 0, sizeof(visited));
	indices[cluster_index] = (short)current;
	visited[cluster_index >> 5] |= 1 << (cluster_index & 31);
	clusters[0].cluster_index = cluster_index;
	clusters[0].distance = 0.0f;
	clusters[0].previous_index = NONE;
	count = 1;
	do
	{
		s_fog_cluster_distance *entry = &clusters[current++];
		s_fog_cluster *cluster = &local_9e9b6e->clusters[entry->cluster_index];

		for (long portal_index = 0; portal_index < cluster->portal_count; portal_index++)
		{
			s_fog_cluster_portal *portal = &local_9e9b6e->portals[cluster->portal_indices[portal_index]];
			long other = portal->front_cluster;

			if (other == entry->cluster_index)
				other = portal->back_cluster;
			if (other != NONE)
			{
				real distance = function_14b240((s_14b240_owner const *)local_9e9b6e, (s_bsp3d_disk const *)portal, point);

				if (visited[other >> 5] & (1 << (other & 31)))
				{
					s_fog_cluster_distance *existing = &clusters[indices[other]];

					if (distance > existing->distance)
						distance = existing->distance;
					existing->distance = distance;
				}
				else if (k_fog_cluster_maximum_distance > distance)
				{
					s_fog_cluster_distance *added = &clusters[count];

					indices[other] = (short)count++;
					added->cluster_index = other;
					added->distance = distance;
					added->previous_index = NONE;
					visited[other >> 5] |= 1 << (other & 31);
				}
			}
		}
	}
	while (current < count);
}
