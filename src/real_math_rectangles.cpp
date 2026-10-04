// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "unknown_11cb00.h"

long rectangle3d_build_vertices(real_rectangle3d const *rectangle,
	long maximum_vertex_count, real_point3d vertices[]);

typedef real_point3d rectangle3d_edge[2];

// @retail 0x11f770
real_rectangle3d *real_rectangle3d_enclose_points(real_rectangle3d *rectangle,
	long point_count, real_point3d const points[])
{
	for (long i = 0; i < point_count; i++)
	{
		if (rectangle->x0 > points[i].x)
			rectangle->x0 = points[i].x;
		if (points[i].x > rectangle->x1)
			rectangle->x1 = points[i].x;
		if (rectangle->y0 > points[i].y)
			rectangle->y0 = points[i].y;
		if (points[i].y > rectangle->y1)
			rectangle->y1 = points[i].y;
		if (rectangle->z0 > points[i].z)
			rectangle->z0 = points[i].z;
		if (points[i].z > rectangle->z1)
			rectangle->z1 = points[i].z;
	}
	return rectangle;
}

// @retail 0x11fa40
long rectangle3d_build_edges(real_rectangle3d const *rectangle,
	long maximum_edge_count, rectangle3d_edge edges[])
{
	long edge_vertices[12][2] =
	{
		{ 0, 2 }, { 2, 3 }, { 3, 1 }, { 1, 0 },
		{ 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 },
		{ 4, 5 }, { 5, 7 }, { 7, 6 }, { 6, 4 }
	};
	real_point3d vertices[8];
	rectangle3d_build_vertices(rectangle, 8, vertices);
	/* Retail always emits twelve edges; the capacity argument is unused. */
	for (long i = 0; i < 12; i++)
	{
		edges[i][0] = vertices[edge_vertices[i][0]];
		edges[i][1] = vertices[edge_vertices[i][1]];
	}
	return 12;
}
