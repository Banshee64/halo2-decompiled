// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_0D0690.CPP: object child iteration, object state helpers and the
   render model triangle interpolation (positions, texture coordinates,
   normals and colors at barycentric coordinates) */

#include "cseries.h"
#include "real_math.h"
#include "globals.h"
#include "unknown_0d0690.h"

color3f *unpack_color3f(dword pixel, color3f *color);

struct s_object_tag
{
	byte unknown00[0x1d4];
	real value1d4;
	byte unknown1d8[4];
	real value1dc;
	byte unknown1e0[0x2c0 - 0x1e0];
	long count;
	struct s_object_range *ranges;
};

struct s_object_range
{
	byte unknown00[0xa];
	short maximum_a;
	short maximum_b;
};

struct s_object
{
	long tag_index;
	dword unknown04 : 26;
	dword flag26 : 1;
	dword unknown04b : 5;
	byte unknown08[4];
	long next_sibling;
	long first_child;
	long value14;
	byte unknown18[0xaa - 0x18];
	byte type;
	byte unknownab[0x15c - 0xab];
	vector3f vector15c;
	vector3f vector168;
	byte unknown174[0x180 - 0x174];
	union
	{
		vector3f vector180;
		struct
		{
			real unknown180;
			real value184;
			real unknown188;
		};
	};
	byte unknown18c[0x1fa - 0x18c];
	byte counter1fa;
	byte counter1fb;
	short value1fc;
	byte unknown1fe[0x212 - 0x1fe];
	char slot212;
	byte unknown213[5];
	long children[1];
	byte unknown21c[0x22a - 0x21c];
	short value22a;
	short value22c;
	byte unknown22e[0x2a0 - 0x22e];
	long value2a0;
	short value2a4;
	byte unknown2a6[2];
	long value2a8;
	long value2ac;
	byte unknown2b0[0x334 - 0x2b0];
	real value334;
	real value338;
	byte unknown33c[2];
	short offset33e;
};

struct s_object_entry
{
	byte unknown00[8];
	long value;
};

struct s_object_header
{
	short identifier;
	byte flags;
	byte type;
	byte unknown04[4];
	s_object *object;
};

#define OBJECT_FROM_INDEX(index) (((s_object_header *)g_4e0300->data)[(index) & 0xffff].object)
#define TAG_FROM_OBJECT(object) ((s_object_tag *)g_4e3b44[(object)->tag_index & 0xffff].bytes)


// @retail 0xd0690
bool function_d0690(s_object_child_iterator *iterator)
{
	bool result = false;

	if (iterator->root == iterator->current)
	{
		while (iterator->next != NONE)
		{
			long index = iterator->next;
			s_object *child = OBJECT_FROM_INDEX(index);
			iterator->next = child->next_sibling;
			if ((1 << child->type) & 3)
			{
				iterator->child_index = index;
				iterator->child_short = child->value1fc;
				iterator->child_value = child->value14;
				result = true;
				break;
			}
		}
		if (!result)
		{
			long index = OBJECT_FROM_INDEX(iterator->root)->first_child;
			while (index != NONE)
			{
				s_object *child = OBJECT_FROM_INDEX(index);
				if ((1 << child->type) & 3)
				{
					if (TEST_FIELD_BIT(child->flag26))
						break;
				}
				index = child->next_sibling;
			}
			iterator->current = index;
			if (index != NONE)
				iterator->next = OBJECT_FROM_INDEX(index)->first_child;
		}
	}

	if (iterator->root != iterator->current && iterator->current != NONE)
	{
		do
		{
			while (iterator->next != NONE)
			{
				long index = iterator->next;
				s_object *child = OBJECT_FROM_INDEX(index);
				iterator->next = child->next_sibling;
				if ((1 << child->type) & 3)
				{
					iterator->child_index = index;
					iterator->child_short = child->value1fc;
					iterator->child_value = child->value14;
					return true;
				}
			}
			if (result)
				return result;
			long index = OBJECT_FROM_INDEX(iterator->current)->next_sibling;
			while (index != NONE)
			{
				s_object *child = OBJECT_FROM_INDEX(index);
				if ((1 << child->type) & 3)
				{
					if (TEST_FIELD_BIT(child->flag26))
						break;
				}
				index = child->next_sibling;
			}
			iterator->current = index;
			if (index != NONE)
				iterator->next = OBJECT_FROM_INDEX(index)->first_child;
		} while (iterator->current != NONE);
	}

	return result;
}

// @retail 0xd0930
void function_d0930(long object_index, vector3f *vector)
{
	s_object *object = OBJECT_FROM_INDEX(object_index);

	object->vector15c = *vector;
	object->vector180 = *vector;
}

// @retail 0xd0980
void __stdcall function_d0980(long object_index)
{
	s_object *object = OBJECT_FROM_INDEX(object_index);

	object->value2a0 = NONE;
	object->value2a4 = NONE;
	object->value2a8 = NONE;
	object->value2ac = NONE;
}

// @retail 0xd0b70
void function_d0b70(long object_index, long a, long b, real value, long slot)
{
	s_object *object = OBJECT_FROM_INDEX(object_index);
	long child_index = object->children[slot];

	if (child_index != NONE)
	{
		s_object *child = OBJECT_FROM_INDEX(child_index);
		s_object_tag *tag = TAG_FROM_OBJECT(child);

		child->value184 = value;
		if (tag->count > 0)
		{
			s_object_range *range = tag->ranges;

			if (a < 0)
				a = 0;
			else if (a > range->maximum_a)
				a = range->maximum_a;
			child->value22c = (short)a;
			if (b < 0)
				b = 0;
			else if (b > range->maximum_b)
				b = range->maximum_b;
			child->value22a = (short)b;
		}
	}
}

// @retail 0xd0dc0
void function_d0dc0(long object_index, long value)
{
	s_object *object = OBJECT_FROM_INDEX(object_index);
	s_object_entry *entry = (s_object_entry *)((byte *)object + object->offset33e + 0x30);

	if (entry->value == value)
		entry->value = NONE;
}

// @retail 0xd1210
real function_d1210(long object_index)
{
	s_object *object = OBJECT_FROM_INDEX(object_index);

	if (object->value334 > 0.0f)
	{
		s_object_tag *tag = TAG_FROM_OBJECT(object);

		if (object->value338 == 0.0f)
		{
			if (tag->value1d4 > object->value334)
				return object->value334 / tag->value1d4;
			return 1.0f;
		}
		if (object->value334 > object->value338)
			return (object->value334 - object->value338) / tag->value1dc;
	}
	return 0.0f;
}

// @retail 0xd1490
real function_d1490(long object_index, long child_index)
{
	real result = 1.0f;

	if (object_index != NONE && child_index != NONE)
	{
		s_object *object = OBJECT_FROM_INDEX(object_index);
		short slot = object->slot212;
		long current = NONE;
		byte counter;
		real value;

		if (slot != NONE)
			current = object->children[slot];
		if (current == child_index)
		{
			counter = object->counter1fb;
			if (counter == 0)
				return 0.0f;
			value = (real)counter * g_510c54->rate;
		}
		else
		{
			counter = object->counter1fa;
			if (counter == 0)
				return 0.0f;
			value = (real)counter * g_510c54->rate;
		}
		value *= 10.0f / 3.0f;
		if (0.0f > value)
			result = 0.0f;
		else if (value > 1.0f)
			result = 1.0f;
		else
			result = value;
	}
	return result;
}

/* the callbacks of the unit object type's table at 0x4679c0 that are
   decompiled (entry 17 is function_d0980) */
void *g_4679c0[29] =
{
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, (void *)function_d0980, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0
};

struct s_vertex_block
{
	byte type;
	byte unknown01[3];
	long offset4;
	long offset;
	byte *data;
	byte unknown10[0x10];
};

struct s_triangle
{
	word a, b, c;
};

struct s_mesh
{
	byte unknown00[0x24];
	s_triangle *triangles;
	byte unknown28[0x38 - 0x28];
	long block_count;
	s_vertex_block *blocks;
};

struct s_rgb24
{
	byte r, g, b;
};

#define MESH_BLOCK_UV_FLOAT 0x18
#define MESH_BLOCK_UV_A 0x1e
#define MESH_BLOCK_UV_B 0x1f
#define MESH_BLOCK_COLOR_A 0x2e
#define MESH_BLOCK_COLOR_B 0x2f
#define MESH_BLOCK_NORMAL 0x30

static inline real dequantize16(short value)
{
	return ((real)value * 2.0f + 1.0f) * (1.0f / 65535.0f);
}

static inline void unpack_point_at(short *p, point2f *point)
{
	point->x = dequantize16(p[0]);
	point->y = dequantize16(p[1]);
}

static inline void unpack_point(byte *data, long index, point2f *point)
{
	short *p = (short *)data + index * 2;

	point->x = dequantize16(p[0]);
	point->y = dequantize16(p[1]);
}

// @retail 0xd2bf0
void function_d2bf0(s_mesh *mesh, long triangle_index, real u, real v, real *out_a, real *out_b)
{
	long i = 3;
	bool compressed = false;

	if (mesh->block_count > i)
	{
		s_vertex_block *block = mesh->blocks + i;
		s_vertex_block *found;

		do
		{
			found = block;
			if (block->type == MESH_BLOCK_UV_A)
				break;
			if (block->type == MESH_BLOCK_UV_B)
			{
				compressed = true;
				break;
			}
			i++;
			block++;
		}
		while (i < mesh->block_count);

		s_triangle *triangle = mesh->triangles + triangle_index;
		byte *data = found->data;

		if (!compressed)
		{
			point2f *pa = (point2f *)data + triangle->a;
			point2f *pb = (point2f *)data + triangle->b;
			point2f *pc = (point2f *)data + triangle->c;

			real ax = pa->x, bx = pb->x, cx = pc->x;
			*out_a = (cx - ax) * v + (bx - ax) * u + ax;
			real ay = pa->y, by = pb->y, cy = pc->y;
			*out_b = (by - ay) * u + (cy - ay) * v + ay;
		}
		else
		{
			point2f a, b, c;

			unpack_point(data, triangle->a, &a);
			unpack_point(data, triangle->b, &b);
			unpack_point(data, triangle->c, &c);

			*out_a = (c.x - a.x) * v + (b.x - a.x) * u + a.x;
			*out_b = (c.y - a.y) * v + (b.y - a.y) * u + a.y;
		}
	}
}

static inline real pin_real(real value, real minimum, real maximum)
{
	if (minimum > value)
		return minimum;
	if (value > maximum)
		return maximum;
	return value;
}

// @retail 0xd2dc0
void function_d2dc0(s_mesh *mesh, long triangle_index, real u, real v, real *out_a, real *out_b)
{
	s_vertex_block *block = mesh->blocks + 1;
	s_triangle *triangle = mesh->triangles + triangle_index;
	byte *data = block->data + block->offset;

	if (block->type == MESH_BLOCK_UV_FLOAT)
	{
		point2f *pa = (point2f *)data + triangle->a;
		point2f *pb = (point2f *)data + triangle->b;
		point2f *pc = (point2f *)data + triangle->c;

		real ax = pa->x, bx = pb->x, cx = pc->x;
		*out_a = (cx - ax) * v + (bx - ax) * u + ax;
		real ay = pa->y, by = pb->y, cy = pc->y;
		*out_b = (by - ay) * u + (cy - ay) * v + ay;
	}
	else
	{
		short *pa = (short *)data + triangle->a * 2;
		short *pb = (short *)data + triangle->b * 2;
		short *pc = (short *)data + triangle->c * 2;
		point2f a, b, c;

		unpack_point_at(pa, &a);
		unpack_point_at(pb, &b);
		unpack_point_at(pc, &c);

		*out_a = (c.x - a.x) * v + (b.x - a.x) * u + a.x;
		*out_b = (c.y - a.y) * v + (b.y - a.y) * u + a.y;
	}
	*out_a = pin_real(*out_a, -1000.0f, 1000.0f);
	*out_b = pin_real(*out_b, -1000.0f, 1000.0f);
}

static inline void unpack_normal(dword value, vector3f *vector)
{
	vector->i = ((real)(long)(value << 21) * (1.0f / 1048576.0f) + 1.0f) * (1.0f / 2047.0f);
	value >>= 11;
	vector->j = ((real)(long)(value << 21) * (1.0f / 1048576.0f) + 1.0f) * (1.0f / 2047.0f);
	value >>= 11;
	vector->k = ((real)(long)(value << 22) * (1.0f / 2097152.0f) + 1.0f) * (1.0f / 1023.0f);
}

// @retail 0xd31c0
bool function_d31c0(s_mesh *mesh, real u, real v, long triangle_index, vector3f *out)
{
	long i = 3;
	bool result = false;

	if (mesh->block_count > i)
	{
		s_vertex_block *block = mesh->blocks + i;
		s_vertex_block *found;

		do
		{
			found = block;
			if (block->type == MESH_BLOCK_NORMAL)
				break;
			i++;
			block++;
		}
		while (i < mesh->block_count);

		s_triangle *triangle = mesh->triangles + triangle_index;
		dword *data = (dword *)found->data;
		vector3f a, b, c;

		unpack_normal(data[triangle->a], &a);
		unpack_normal(data[triangle->b], &b);
		unpack_normal(data[triangle->c], &c);

		out->i = (b.i - a.i) * u + (c.i - a.i) * v + a.i;
		out->j = (b.j - a.j) * u + (c.j - a.j) * v + a.j;
		out->k = (b.k - a.k) * u + (c.k - a.k) * v + a.k;
		result = true;
	}
	return result;
}

// @retail 0xd33a0
bool function_d33a0(s_mesh *mesh, long triangle_index, real u, real v, color3f *out)
{
	long i = 3;
	bool result = false;
	bool compressed = false;

	if (mesh->block_count > i)
	{
		s_vertex_block *block = mesh->blocks + i;
		s_vertex_block *found;
		color3f a, b, c;

		do
		{
			found = block;
			if (block->type == MESH_BLOCK_COLOR_A)
				break;
			if (block->type == MESH_BLOCK_COLOR_B)
			{
				compressed = true;
				break;
			}
			i++;
			block++;
		}
		while (i < mesh->block_count);

		s_triangle *triangle = mesh->triangles + triangle_index;

		if (!compressed)
		{
			dword *data = (dword *)found->data;

			unpack_color3f(data[triangle->a], &a);
			unpack_color3f(data[triangle->b], &b);
			unpack_color3f(data[triangle->c], &c);
		}
		else
		{
			s_rgb24 *data = (s_rgb24 *)found->data;
			s_rgb24 pa = data[triangle->a];
			s_rgb24 pb = data[triangle->b];
			s_rgb24 pc = data[triangle->c];

			a.red = (real)pa.r * (1.0f / 255.0f);
			a.green = (real)pa.g * (1.0f / 255.0f);
			a.blue = (real)pa.b * (1.0f / 255.0f);
			b.red = (real)pb.r * (1.0f / 255.0f);
			b.green = (real)pb.g * (1.0f / 255.0f);
			b.blue = (real)pb.b * (1.0f / 255.0f);
			c.red = (real)pc.r * (1.0f / 255.0f);
			c.green = (real)pc.g * (1.0f / 255.0f);
			c.blue = (real)pc.b * (1.0f / 255.0f);
		}

		out->red = (c.red - a.red) * v + (b.red - a.red) * u + a.red;
		out->green = (c.green - a.green) * v + (b.green - a.green) * u + a.green;
		out->blue = (c.blue - a.blue) * v + (b.blue - a.blue) * u + a.blue;
		result = true;
	}
	return result;
}

// @retail 0xd3630
bool function_d3630(s_vertex_block *block, long triangle_index, s_mesh *mesh, real u, real v, color3f *out)
{
	bool result = false;

	if (block)
	{
		s_triangle *triangle = mesh->triangles + triangle_index;
		byte *data = block->data + block->offset4;
		color3f a, b, c;

		if (block->type == MESH_BLOCK_COLOR_A)
		{
			unpack_color3f(((dword *)data)[triangle->a], &a);
			unpack_color3f(((dword *)data)[triangle->b], &b);
			unpack_color3f(((dword *)data)[triangle->c], &c);
		}
		else
		{
			s_rgb24 *colors = (s_rgb24 *)data;
			s_rgb24 pa = colors[triangle->a];
			s_rgb24 pb = colors[triangle->b];
			s_rgb24 pc = colors[triangle->c];

			a.red = (real)pa.r * (1.0f / 255.0f);
			a.green = (real)pa.g * (1.0f / 255.0f);
			a.blue = (real)pa.b * (1.0f / 255.0f);
			b.red = (real)pb.r * (1.0f / 255.0f);
			b.green = (real)pb.g * (1.0f / 255.0f);
			b.blue = (real)pb.b * (1.0f / 255.0f);
			c.red = (real)pc.r * (1.0f / 255.0f);
			c.green = (real)pc.g * (1.0f / 255.0f);
			c.blue = (real)pc.b * (1.0f / 255.0f);
		}

		out->red = (c.red - a.red) * v + (b.red - a.red) * u + a.red;
		out->green = (c.green - a.green) * v + (b.green - a.green) * u + a.green;
		out->blue = (c.blue - a.blue) * v + (b.blue - a.blue) * u + a.blue;
		result = true;
	}
	return result;
}

// @retail 0xd0620
void function_d0620(long object_index, s_object_child_iterator *iterator)
{
	s_object *object;

	for (;;)
	{
		object = OBJECT_FROM_INDEX(object_index);
		long parent_index = object->value14;
		if (parent_index == NONE || !TEST_FIELD_BIT(object->flag26) || !((1 << OBJECT_FROM_INDEX(parent_index)->type) & 3))
			break;
		object_index = parent_index;
	}

	iterator->root = object_index;
	iterator->current = object_index;
	iterator->next = object->first_child;
}
