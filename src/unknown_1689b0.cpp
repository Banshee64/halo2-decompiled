// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1689B0.CPP: collision queries (0x1689b0..0x16b569): the flag
   conversions between the query flags and the collision tests' own, and the
   test of a surface against the query */

#include "cseries.h"
#include "globals.h"

#define FLAG(bit) (1 << (bit))
#define TEST_FLAG(flags, bit) (((flags) & FLAG(bit)) != 0)
#define SET_FLAG(flags, bit, value) ((value) ? ((flags) |= FLAG(bit)) : ((flags) &= ~FLAG(bit)))

/* a collision result, as read here */
struct s_collision_surface_entry
{
	byte unknown0[4];
	dword flags;
};

struct s_collision_result_view
{
	byte unknown00[0x98];
	long surface_count;
	s_collision_surface_entry *surfaces;
};

struct s_collision_index_block
{
	byte unknown00[0x40];
	long count;
	short *indices;
};

struct s_match_globals_collision_view
{
	byte unknown00[0xc4];
	long block_count;
	s_collision_index_block *blocks;
};

// @retail 0x1689b0
bool collision_surface_test(s_collision_result_view const *result, long surface_index, dword flags)
{
	bool valid = true;

	if (result->surface_count <= 0 || (result->surfaces[result->surface_count - 1].flags & 0x10))
	{
		valid = false;
	}
	else if (flags & 0x40000)
	{
		s_match_globals_collision_view *globals = (s_match_globals_collision_view *)g_4e0348;

		if (globals->block_count > 0 && globals->blocks)
		{
			s_collision_index_block *block = globals->blocks;

			if (surface_index < 0 || surface_index >= block->count || block->indices[surface_index * 2] == NONE)
			{
				valid = false;
			}
		}
	}
	return valid;
}

// @retail 0x168ae0
dword collision_flags_to_test_flags(dword flags)
{
	dword result = 0;

	SET_FLAG(result, 0, TEST_FLAG(flags, 23));
	SET_FLAG(result, 1, TEST_FLAG(flags, 24));
	SET_FLAG(result, 2, TEST_FLAG(flags, 25));
	SET_FLAG(result, 3, TEST_FLAG(flags, 26));
	SET_FLAG(result, 4, TEST_FLAG(flags, 27));
	SET_FLAG(result, 5, TEST_FLAG(flags, 28));
	return result;
}

// @retail 0x16b500
word collision_flags_to_object_flags(dword flags)
{
	word result = 0;

	SET_FLAG(result, 0, true);
	SET_FLAG(result, 1, TEST_FLAG(flags, 19));
	SET_FLAG(result, 2, TEST_FLAG(flags, 18));
	SET_FLAG(result, 3, TEST_FLAG(flags, 30));
	SET_FLAG(result, 4, TEST_FLAG(flags, 20));
	SET_FLAG(result, 5, TEST_FLAG(flags, 22));
	SET_FLAG(result, 6, TEST_FLAG(flags, 21));
	return result;
}

/* the object header and object fields the collision filters read */
struct s_collision_object_header
{
	byte unknown00[2];
	byte flags;
	byte type;
	byte unknown04[4];
	struct s_collision_object *object;
};

struct s_collision_object
{
	byte unknown000[4];
	dword unknown_bit0 : 1;
	dword unknown_bits1 : 19;
	dword bit20 : 1;
	dword bit21 : 1;
	dword bit22 : 1;
	dword unknown_bits23 : 8;
	dword bit31 : 1;
	byte unknown008[0xaa - 0x8];
	char type;
	byte unknown0ab[0x10a - 0xab];
	word unknown10a_bits0 : 2;
	word bit10a_2 : 1;
	word unknown10a_bits3 : 13;
	byte unknown10c[0x12c - 0x10c];
	dword unknown12c_bit0 : 1;
	dword bit12c_1 : 1;
	dword unknown12c_bits2 : 30;
	byte unknown130[0x348 - 0x130];
	word unknown348_bits0 : 3;
	word bit348_3 : 1;
	word unknown348_bits4 : 12;
};

// @retail 0x168a10
bool collision_object_test(long object_index, s_collision_object_header const *header, s_collision_object const *object, dword flags,
	long ignore_object_index, long ignore_object_index2)
{
	bool result = false;

	if (object_index != ignore_object_index && object_index != ignore_object_index2 && !(header->flags & 0x10) &&
		!(*(dword const *)((byte const *)object + 4) & 1))
	{
		word type = header->type;

		if ((flags & (1 << (type + 4))) &&
			!((flags & 0x80000) && TEST_FIELD_BIT(object->bit20)) &&
			!((flags & 0x40000) && !TEST_FIELD_BIT(object->bit22)) &&
			!((flags & 0x40000000) && !TEST_FIELD_BIT(object->bit31)))
		{
			if (type == 0)
			{
				if (!((flags & 0x100000) && TEST_FIELD_BIT(object->bit10a_2)) && !((flags & 0x400000) && TEST_FIELD_BIT(object->bit348_3)))
				{
					result = true;
				}
			}
			else if (type != 0xb || !((flags & 0x200000) && TEST_FIELD_BIT(object->bit12c_1)))
			{
				result = true;
			}
		}
	}
	return result;
}

// @retail 0x16b430
word collision_object_flags(long object_index)
{
	s_collision_object_header *header = &((s_collision_object_header *)g_4e0300->data)[object_index & 0xffff];
	s_collision_object *object = header->object;
	long type = object->type;
	word result = 0;

	if ((header->flags & 0x10) || (*(dword *)((byte *)object + 4) & 1))
	{
		result = 1;
	}
	SET_FLAG(result, 1, TEST_FIELD_BIT(object->bit20));
	SET_FLAG(result, 2, !TEST_FIELD_BIT(object->bit22));
	if (!TEST_FIELD_BIT(object->bit31))
	{
		result |= FLAG(3);
	}
	if (type == 0)
	{
		SET_FLAG(result, 4, TEST_FIELD_BIT(object->bit10a_2));
		SET_FLAG(result, 5, TEST_FIELD_BIT(object->bit348_3));
	}
	else if (type == 0xb)
	{
		SET_FLAG(result, 6, TEST_FIELD_BIT(object->bit12c_1));
	}
	return result;
}

/* the lookup of 0x1efb40 (unknown_1efac0.cpp) */
struct s_lookup
{
	byte unknown00[0x14];

	bool initialize(long object_handle);
};

struct s_table_holder;
void *function_1efd80(s_table_holder *holder, dword position);

/* defined in unknown_183ee0.cpp */
struct s_slot_entry_list;
extern s_slot_entry_list *g_4e0340;

/* a collision result's surface reference, as read here */
struct s_collision_reference
{
	long type;
	byte unknown04[0x3c - 0x4];
	long permutation_index;
	long object_handle;
	byte unknown44[4];
	dword position;
};

struct s_168ce0_section
{
	byte unknown00[0x70];
	byte data[0xc8 - 0x70];
};

struct s_168ce0_permutation
{
	byte unknown00[0x34];
	short section_index;
	byte unknown36[0x58 - 0x36];
};

struct s_168ce0_bsp_view
{
	byte unknown000[0x13c];
	s_168ce0_section *sections;
	byte unknown140[4];
	s_168ce0_permutation *permutations;
};

// @retail 0x168ce0
void *collision_reference_get_data(s_collision_reference const *reference)
{
	void *result = NULL;

	switch (reference->type)
	{
	case 1:
		result = g_4e0340;
		break;
	case 3:
	{
		s_168ce0_bsp_view *bsp = (s_168ce0_bsp_view *)g_4e0348;

		result = bsp->sections[bsp->permutations[reference->permutation_index].section_index].data;
		break;
	}
	case 4:
	{
		s_lookup lookup;

		if (lookup.initialize(reference->object_handle))
		{
			result = function_1efd80((s_table_holder *)&lookup, reference->position);
		}
		break;
	}
	}
	return result;
}
struct s_bsp3d;
long function_14a280(s_bsp3d *bsp, real_point3d *point, long index);
real_point3d *function_142700(real_matrix4x3 const *matrix, real_point3d const *point, real_point3d *out);

struct s_168d60_instance;
struct s_168d60_bsp_view
{
	byte unknown000[0x13c];
	byte *sections;
	byte unknown140[4];
	struct s_168d60_instance *instances;
};

/* the structure's instanced geometry, as the point test reads it */
struct s_168d60_instance
{
	real_matrix4x3 matrix;
	short section_index;
	byte unknown36[0x3c - 0x36];
	real_point3d center;
	real radius;
	byte unknown4c[0x58 - 0x4c];
};

// @retail 0x168d60
bool collision_point_inside_instance(long instance_index, real_point3d const *point, dword flags)
{
	s_168d60_bsp_view *bsp = (s_168d60_bsp_view *)g_4e0348;
	s_168d60_instance *instance = &bsp->instances[instance_index];
	byte *section = bsp->sections + instance->section_index * 0xc8;

	if (collision_surface_test((s_collision_result_view const *)section, instance_index, flags))
	{
		real dx = instance->center.x - point->x;
		real dz = instance->center.z - point->z;
		real dy = instance->center.y - point->y;
		real radius = instance->radius;

		if (dx * dx + dz * dz + dy * dy <= radius * radius)
		{
			real_point3d local_point;

			function_142700(&instance->matrix, point, &local_point);
			if (function_14a280((s_bsp3d *)(section + 0x70), &local_point, 0) == NONE)
			{
				return true;
			}
		}
	}
	return false;
}