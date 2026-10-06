// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_17D9A0.CPP: decal placement and rendering (0x17d9a0 onwards) */

#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include <float.h>
#include "globals.h"
#include <string.h>

extern long g_4e7414;
extern bool g_4e7411;
short __stdcall function_14a5b0(short cluster_index, point3f const *point, real radius, long maximum_count, short *clusters);

struct s_decal_cluster_17e490
{
	byte unknown00[0x98];
	long count;
	short *indices;
	byte unknowna0[0x10];
};

struct s_decal_bound_17e490
{
	byte unknown00[0x3c];
	point3f center;
	real radius;
	byte unknown4c[0xc];
};

struct s_decal_bsp_17e490
{
	byte unknown00[0xa0];
	s_decal_cluster_17e490 *clusters;
	byte unknowna4[0xa0];
	s_decal_bound_17e490 *bounds;
};

// @retail 0x17e490
long function_17e490(long excluded, short cluster_index, point3f const *point, real radius, long maximum_count, s_decal_bound_17e490 **output)
{
	short clusters[512];
	short count = 0;
	if (cluster_index != NONE)
	{
		if (radius > 0.0f)
		{
			g_4e7414++;
			g_4e7411 = true;
			count = function_14a5b0(cluster_index, point, radius, 512, clusters);
			g_4e7411 = false;
			if (count > 512)
				count = 512;
		}
		else
		{
			clusters[0] = cluster_index;
			count = 1;
		}
	}
	dword visited[32];
	memset(visited, 0, sizeof(visited));
	long result = 0;
	if (excluded != NONE)
		visited[excluded >> 5] |= 1 << (excluded & 31);
	for (long i = 0; i < count; ++i)
	{
		s_decal_cluster_17e490 *cluster = &((s_decal_bsp_17e490 *)g_4e0348)->clusters[clusters[i]];
		for (long j = 0; j < cluster->count; ++j)
		{
			long index = cluster->indices[j];
			dword bit = 1 << (index & 31);
			if (!(visited[index >> 5] & bit) && result < maximum_count)
			{
				s_decal_bound_17e490 *bound = &((s_decal_bsp_17e490 *)g_4e0348)->bounds[index];
				real x = point->x - bound->center.x;
				real z = point->z - bound->center.z;
				real y = point->y - bound->center.y;
				real distance = x * x + z * z + y * y;
				real total_radius = bound->radius + radius;
				if (total_radius * total_radius >= distance)
				{
					output[result++] = bound;
					visited[index >> 5] |= bit;
				}
			}
		}
	}
	return result;
}

struct s_decal_mesh_face
{
	short plane_index;
	word first_edge;
	byte unknown04[4];
};

struct s_decal_quad_17d9a0
{
	transform4x3f matrix;
	box2f bounds;
	plane3f plane;
	short axis;
	bool positive;
	byte unknown57;
	point2f corners[4];
	struct s_edge { real i, j; } edges[2];
	real inverse_determinant;
};

short function_120850(vector3f const *v);

PRIVATE inline void decal_project_corner_17d9a0(transform4x3f const *matrix, real const &x, real const &y, s_decal_quad_17d9a0 *quad, point2f *out)
{
	point3f point;
	point.x = matrix->forward.i * x + matrix->left.i * y + matrix->position.x;
	point.y = matrix->left.j * y + matrix->forward.j * x + matrix->position.y;
	point.z = matrix->forward.k * x + matrix->left.k * y + matrix->position.z;
	short const *axes = g_440b94[quad->axis * 2 + quad->positive];
	out->x = ((real const *)&point)[axes[0]];
	out->y = ((real const *)&point)[axes[1]];
}

// @retail 0x17d9a0
void function_17d9a0(transform4x3f const *matrix, real const *bounds, s_decal_quad_17d9a0 *quad)
{
	s_decal_quad_17d9a0 *const *quad_reference = &quad;
	quad = *quad_reference;
	quad->matrix = *matrix;
	quad->bounds = *(box2f const *)bounds;
	plane3f *plane = &quad->plane;
	plane->n = matrix->up;
	plane->d = dot3f(&plane->n, (vector3f const *)&matrix->position);
	quad->axis = function_120850(&plane->n);
	quad->positive = ((real const *)&quad->plane.n)[quad->axis] > 0.0f;
	decal_project_corner_17d9a0(matrix, bounds[0], bounds[2], quad, &quad->corners[0]);
	decal_project_corner_17d9a0(matrix, bounds[1], bounds[2], quad, &quad->corners[1]);
	decal_project_corner_17d9a0(matrix, bounds[1], bounds[3], quad, &quad->corners[2]);
	decal_project_corner_17d9a0(matrix, bounds[0], bounds[3], quad, &quad->corners[3]);
	quad->edges[0].i = quad->corners[1].x - quad->corners[0].x;
	quad->edges[0].j = quad->corners[1].y - quad->corners[0].y;
	quad->edges[1].i = quad->corners[3].x - quad->corners[0].x;
	quad->edges[1].j = quad->corners[3].y - quad->corners[0].y;
	if (quad->edges[0].i * quad->edges[0].i + quad->edges[0].j * quad->edges[0].j > 0.0001f &&
		quad->edges[1].i * quad->edges[1].i + quad->edges[1].j * quad->edges[1].j > 0.0001f)
		quad->inverse_determinant = 1.0f / (quad->edges[1].j * quad->edges[0].i - quad->edges[0].j * quad->edges[1].i);
	else
		quad->inverse_determinant = 0.0f;
}

struct s_decal_mesh_edge
{
	word vertices[2];
	word next_edges[2];
	short faces[2];
};

struct s_decal_mesh_vertex
{
	point3f position;
	byte unknown0c[4];
};

struct s_decal_mesh_view
{
	byte unknown00[0x28];
	long face_count;
	s_decal_mesh_face const *faces;
	long edge_count;
	s_decal_mesh_edge const *edges;
	long vertex_count;
	s_decal_mesh_vertex const *vertices;
};

// @retail 0x17d900
real function_17d900(s_decal_mesh_view const *mesh, long face_index, plane3f const *plane)
{
	long edge_index = mesh->faces[face_index].first_edge;
	real minimum = FLT_MAX;
	long first_edge = edge_index;

	do
	{
		s_decal_mesh_edge const *edge = &mesh->edges[edge_index];
		bool reverse = edge->faces[1] == face_index;
		real distance = (real)fabs(plane_distance_to_point(plane, &mesh->vertices[edge->vertices[!reverse]].position));

		if (!(distance > minimum))
			minimum = distance;
		edge_index = edge->next_edges[reverse];
	} while (edge_index != first_edge);
	return minimum;
}

/* the state a decal is placed with (0x5c bytes) */
struct s_decal_placement
{
	long unknown00;
	long unknown04;
	point3f position;
	long unknown14;
	long unknown18;
	long unknown1c;
	long unknown20;
	short unknown24;
	byte unknown26[2];
	plane3f plane;
	long unknown38;
	long unknown3c;
	long unknown40;
	short unknown44;
	short unknown46;
	long unknown48;
	long unknown4c;
	long unknown50;
	long unknown54;
	byte unknown58;
	byte unknown59;
	short unknown5a;
};

// @retail 0x17ed70
void decal_placement_copy(s_decal_placement const *in, s_decal_placement *out)
{
	out->unknown00 = in->unknown00;
	out->unknown04 = in->unknown04;
	out->position = in->position;
	out->unknown14 = in->unknown14;
	out->unknown18 = in->unknown18;
	out->unknown1c = in->unknown1c;
	out->unknown20 = in->unknown20;
	out->unknown24 = in->unknown24;
	out->plane = in->plane;
	out->unknown38 = in->unknown38;
	out->unknown3c = in->unknown3c;
	out->unknown40 = in->unknown40;
	out->unknown44 = in->unknown44;
	out->unknown46 = in->unknown46;
	out->unknown48 = in->unknown48;
	out->unknown4c = in->unknown4c;
	out->unknown50 = in->unknown50;
	out->unknown54 = in->unknown54;
	out->unknown58 = in->unknown58;
	out->unknown59 = in->unknown59;
	out->unknown5a = in->unknown5a;
}
