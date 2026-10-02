#include "cseries.h"
#include "real_math.h"

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
