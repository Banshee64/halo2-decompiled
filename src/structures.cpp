// @flags /O2 /arch:SSE /Gr
/* STRUCTURES.CPP: queries of the structure bsp's render geometry: the
   lightmap triangle under a collision point */

#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include "globals.h"
#include "geometry_cache.h"

/* a range of a section's triangles: the cluster (or instance definition)
   it is in, and its first triangle reference (8 bytes) */
struct s_structure_surface_range
{
	short cluster_index;
	short count;
	long first;
};

/* a triangle reference: its first index in the section's strip, its
   lightmap part, and its collision surface (8 bytes) */
struct s_structure_triangle_reference
{
	word first_index;
	word part_index;
	long surface_index;
};

/* a collision bsp's surfaces, as read here: the plane first (8 bytes) */
struct s_structure_collision_surface
{
	short plane_index;
	byte unknown02[6];
};

struct s_structure_collision_bsp
{
	long surface_count;
	s_structure_collision_surface *surfaces;
	byte unknown08[0x40 - 0x08];
};

/* a section's vertices: the count and the data, at an offset in the
   section's block (0x20 bytes) */
struct s_structure_vertex_block
{
	word unknown00;
	word count;
	byte unknown04[4];
	long offset;
	byte *data;
	byte unknown10[0x20 - 0x10];
};

/* a section of render geometry (0x44 bytes): its parts (the list
   function_16e1b0 searches), its strip indices and its vertices */
struct s_structure_section
{
	long part_count;
	void *parts;
	byte unknown08[0x20 - 0x08];
	long strip_index_count;
	word *strip_indices;
	byte unknown28[0x38 - 0x28];
	long vertex_block_count;
	s_structure_vertex_block *vertex_blocks;
	byte unknown40[4];
};

/* a cluster (0xb0 bytes) and an instanced geometry definition (0xc8 bytes)
   both start their geometry the same way */
struct s_structure_cluster_view
{
	byte unknown00[0x28];
	s_geometry_block_info field_28;
	byte unknown4c[4];
	s_structure_section *sections;
	byte unknown54[0xb0 - 0x54];
};

struct s_structure_instanced_geometry_definition
{
	byte unknown00[0x28];
	s_geometry_block_info field_28;
	byte unknown4c[4];
	s_structure_section *sections;
	byte unknown54[0x74 - 0x54];
	s_structure_collision_surface *surfaces;
	byte unknown78[0xb8 - 0x78];
	long surface_range_count;
	s_structure_surface_range *surface_ranges;
	byte unknownc0[4];
	s_structure_triangle_reference *triangle_references;
};

struct s_structure_instance
{
	transform4x3f matrix;
	short definition_index;
	byte unknown36[0x58 - 0x36];
};

struct s_structure_bsp_view
{
	byte unknown000[0x18];
	s_structure_collision_bsp *collision_bsps;
	byte unknown01c[0x30 - 0x1c];
	s_structure_surface_range *surface_ranges;
	byte unknown034[0x50 - 0x34];
	s_structure_triangle_reference *triangle_references;
	byte unknown054[0xa0 - 0x54];
	s_structure_cluster_view *clusters;
	byte unknown0a4[0x13c - 0xa4];
	s_structure_instanced_geometry_definition *instanced_geometry_definitions;
	byte unknown140[4];
	s_structure_instance *instances;
};

/* a collision result, as read here */
struct s_structure_collision_result
{
	long type;
	byte unknown04[4];
	point3f point;
	byte unknown14[0x3c - 0x14];
	long instance_index;
	byte unknown40[0x4c - 0x40];
	long surface_range_index;
	byte unknown50[4];
	long plane_index;
};

/* the lightmap triangle found */
struct s_structure_lightmap_triangle
{
	long cluster_index;
	long instance_index;
	byte unknown08[0x18 - 0x08];
	long unknown18;
	byte unknown1c[4];
	long part_index;
	long part_offset;
	long lightmap_part_index;
	byte unknown2c[4];
	real u;
	real v;
};

struct s_16e1b0_list;
void function_16e1b0(long value, s_16e1b0_list const *list, long *unknown, long *range_index, long *offset);
point3f *function_142700(transform4x3f const *matrix, point3f const *point, point3f *out);
bool function_11e800(point3f const *a, point3f const *b, point3f const *c, point3f const *p, real *u, real *v);

static inline bool structure_section_get_vertex(s_structure_section const *section, long index, point3f *point)
{
	s_structure_vertex_block const *block = &section->vertex_blocks[0];
	point3f const *vertices = (point3f const *)(block->data + block->offset);

	if (vertices && index >= 0 && index < block->count)
	{
		*point = vertices[index];
		return true;
	}
	return false;
}

// @retail 0x14ac60
bool structure_get_lightmap_triangle(s_structure_collision_result const *collision, s_structure_lightmap_triangle *triangle)
{
	bool result = false;
	point3f point = collision->point;
	s_structure_bsp_view *bsp = (s_structure_bsp_view *)g_4e0348;
	long surface_range_index = collision->surface_range_index;
	long plane_index = collision->plane_index & 0x7fff;
	long instance_index = collision->instance_index;
	s_structure_instanced_geometry_definition *definition = NULL;
	s_structure_section *section = NULL;
	s_structure_surface_range *range;

	if (collision->type == 1)
	{
		range = &bsp->surface_ranges[surface_range_index];
		s_structure_cluster_view *cluster = &bsp->clusters[range->cluster_index];

		if (function_12de70(&cluster->field_28, 3))
		{
			section = &cluster->sections[0];
		}
	}
	else if (collision->type == 3)
	{
		s_structure_instance *instance = &bsp->instances[instance_index];

		definition = &bsp->instanced_geometry_definitions[instance->definition_index];
		if (definition->surface_range_count == 0)
		{
			return false;
		}
		range = &definition->surface_ranges[surface_range_index];
		function_142700(&instance->matrix, &point, &point);
		if (function_12de70(&definition->field_28, 3))
		{
			section = &definition->sections[0];
		}
	}

	triangle->unknown18 = 0;
	if (section)
	{
		for (long i = range->first; i < range->first + range->count; i++)
		{
			s_structure_triangle_reference *reference = instance_index == NONE ?
				&bsp->triangle_references[i] :
				&definition->triangle_references[i];

			if (reference->surface_index == NONE)
			{
				continue;
			}

			s_structure_collision_surface *surface = instance_index == NONE ?
				&bsp->collision_bsps[0].surfaces[reference->surface_index] :
				&definition->surfaces[reference->surface_index];

			if (surface->plane_index != plane_index)
			{
				continue;
			}

			word *indices = &section->strip_indices[reference->first_index];
			long unknown;
			long part_index;
			long part_offset;
			point3f vertices[3];
			bool valid = true;

			function_16e1b0(reference->first_index, (s_16e1b0_list const *)section, &unknown, &part_index, &part_offset);
			for (long vertex = 0; vertex < 3; vertex++)
			{
				valid = valid && structure_section_get_vertex(section, indices[vertex], &vertices[vertex]);
			}
			if (valid && function_11e800(&vertices[0], &vertices[1], &vertices[2], &point, &triangle->u, &triangle->v))
			{
				triangle->part_offset = part_offset;
				triangle->lightmap_part_index = reference->part_index;
				triangle->part_index = part_index;
				triangle->unknown18 = unknown;
				triangle->cluster_index = range->cluster_index;
				triangle->instance_index = collision->instance_index;
				result = true;
				break;
			}
		}
	}
	return result;
}
