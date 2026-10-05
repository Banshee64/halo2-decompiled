#include "unknown_11c920.h"
#include "unknown_0259d0.h"

// @flags /O2 /arch:SSE /Gr

struct s_prism_245270
{
	long field_00;
	long field_04;
	long field_08;
	byte field_0c;
	byte field_0d;
	short field_0e;
	plane3f field_10;
	real field_20;
	short field_24;
	byte field_26;
	byte field_27;
	long field_28;
	point2f field_2c[8];
};

struct s_collection_245270
{
	short field_00;
	short field_02;
	short field_04;
	byte field_06[0x4c08 - 6];
	s_prism_245270 field_4c08[256];
};

short function_120850(vector3f const *v);

PRIVATE inline void function_245270_project(point3f const *point, short axis,
	byte side, point2f *out)
{
	real x = point->n[g_440b94[axis * 2 + side][0]];
	real y = point->n[g_440b94[axis * 2 + side][1]];
	out->x = x;
	out->y = y;
}

// @retail 0x245270
void function_245270(real thickness, short count, point3f const *points, plane3f const *plane,
	real height, long field_00, long field_04, long field_08, byte field_0c,
	byte field_0d, short field_0e, s_collection_245270 *collection)
{
	point3f const *const *local_points = &points;
	if (collection->field_04 < 256)
	{
		s_prism_245270 *prism = &collection->field_4c08[collection->field_04++];
		prism->field_00 = field_00;
		prism->field_04 = field_04;
		prism->field_08 = field_08;
		prism->field_0c = field_0c;
		prism->field_0d = field_0d;
		prism->field_0e = field_0e;
		prism->field_10 = *plane;
		prism->field_20 = thickness;
		prism->field_24 = function_120850(&prism->field_10.n);
		prism->field_26 = prism->field_10.n.n[prism->field_24] > 0.f ? 1 : 0;
		prism->field_28 = count;
		for (short i = 0; i < prism->field_28; ++i)
		{
			function_245270_project(&(*local_points)[i], prism->field_24, prism->field_26, &prism->field_2c[i]);
		}
		if (height > 0.f && plane->k < 0.f)
		{
			prism->field_10.d -= prism->field_10.k * height;
			if (prism->field_24 != 2)
			{
				short axis = g_440b94[prism->field_24 * 2 + prism->field_26][1] == 2;
				for (short i = 0; i < prism->field_28; ++i)
					((real *)&prism->field_2c[i])[axis] -= height;
			}
		}
	}
}

struct s_material_245400
{
	long field_00;
	byte field_04;
	byte field_05;
	short field_06;
};

struct s_surface_245400
{
	byte field_00[8];
	short field_08;
	byte field_0a[2];
};

struct s_vertex_245400
{
	point3f field_00;
	word field_0c;
	word field_0e;
};

struct s_source_245400
{
	byte field_00[0x2c];
	s_material_245400 *field_2c;
	long field_30;
	s_surface_245400 *field_34;
	long field_38;
	s_vertex_245400 *field_3c;
};

struct s_line_list;
void function_244ca0(s_line_list *list, long a, long b, long c,
	byte d, byte e, short f, point3f const *position, real height, real radius);

PRIVATE inline point3f *function_245400_transform(transform4x3f const *matrix,
	point3f const *point, point3f *out)
{
	real x = point->x;
	real y = point->y;
	real z = point->z;
	if (matrix->scale != 1.f)
	{
		x = matrix->scale * x;
		y = matrix->scale * y;
		z = matrix->scale * z;
	}
	out->x = matrix->up.i * z + matrix->left.i * y + matrix->forward.i * x + matrix->position.x;
	out->y = matrix->up.j * z + matrix->left.j * y + matrix->forward.j * x + matrix->position.y;
	out->z = matrix->up.k * z + matrix->left.k * y + matrix->forward.k * x + matrix->position.z;
	return out;
}

// @retail 0x245400
void function_245400(long index, s_source_245400 const *source,
	transform4x3f const *matrix, real height, real radius, long field_00,
	long field_04, s_line_list *list)
{
	s_vertex_245400 const *vertex = &source->field_3c[index];
	long material_index = source->field_34[vertex->field_0c].field_08;
	s_material_245400 const *material = &source->field_2c[material_index];
	point3f transformed;
	point3f const *point = &vertex->field_00;
	if (matrix)
		point = function_245400_transform(matrix, point, &transformed);
	function_244ca0(list, field_00, field_04, material_index,
		material->field_04, material->field_05, material->field_06, point, height, radius);
}
