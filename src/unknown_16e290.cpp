// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_16E290.CPP: requests for the geometry blocks of the structure bsp
   and of render models (geometry_cache.h), and two small initializers */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "geometry_cache.h"

#include <string.h>

bool function_12dcb0(s_geometry_block_info *block);

extern dword g_4e6494;
extern real_vector3d *g_4687b0;

/* a structure bsp, as read here */
struct s_16e290_cluster
{
	byte unknown00[0xc];
	s_geometry_block_info block;
	byte unknown_end[0x38 - 0xc - sizeof(s_geometry_block_info)];
};

struct s_16e290_index
{
	word unknown0;
	word cluster_index;
	byte unknown4[8];
};

struct s_16e290_bsp
{
	byte unknown00[4];
	long checksum;
	byte unknown08[0x1c - 0x8];
	long unknown1c;
	byte unknown20[0x40 - 0x20];
	long cluster_count;
	s_16e290_cluster *clusters;
	byte unknown48[0x54 - 0x48];
	s_16e290_index *indices54;
	byte unknown58[0x64 - 0x58];
	s_16e290_index *indices64;
};

struct s_16e290_bsp_globals
{
	byte unknown00[0x80];
	long bsp_count;
	s_16e290_bsp *bsp;
};

struct s_16e290_match_view
{
	byte unknown0[8];
	long checksum;
};

s_16e290_bsp_globals *g_4e0344;

/* the object with a geometry block at +0x28 and a result at +0x50 */
struct s_16e290_resource
{
	byte unknown00[0x28];
	s_geometry_block_info block;
	byte unknown_pad[0x50 - 0x28 - sizeof(s_geometry_block_info)];
	long value;
};

// @retail 0x16e290
long function_16e290(s_16e290_resource *resource)
{
	long result = 0;

	if (!g_4e6494 || function_12de70(&resource->block, 0))
	{
		result = resource->value;
	}
	return result;
}

#define PIN(x, lo, hi) ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

// @retail 0x16e5a0
void function_16e5a0(long cluster_index, short bsp_index)
{
	if (bsp_index == g_4686c4)
	{
		s_16e290_bsp *bsp = g_4e0344->bsp;

		if (cluster_index != NONE && PIN(cluster_index, 0, bsp->cluster_count - 1) == cluster_index)
		{
			function_12dcb0(&bsp->clusters[cluster_index].block);
		}
	}
}

/* the render model sections, as read here */
struct s_16e290_section
{
	byte unknown00[0x28];
	s_geometry_block_info block;
	byte unknown_pad[0xc8 - 0x28 - sizeof(s_geometry_block_info)];
};

struct s_16e290_permutation
{
	byte unknown00[0x34];
	short section_index;
	byte unknown36[0x58 - 0x36];
};

struct s_16e290_render_model
{
	byte unknown000[0x13c];
	s_16e290_section *sections;
	byte unknown140[4];
	s_16e290_permutation *permutations;
};

// @retail 0x16e770
void function_16e770(long render_model_index, long permutation_index)
{
	s_16e290_render_model *render_model = (s_16e290_render_model *)g_4e3b44[render_model_index & 0xffff].bytes;

	s_16e290_section *section = &render_model->sections[render_model->permutations[permutation_index].section_index];

	function_12dcb0(&section->block);
}

// @retail 0x16e890
bool function_16e890(long index)
{
	bool result = true;

	if (g_4e0344 && g_4e0344->bsp_count > 0 && g_4e0348)
	{
		s_16e290_bsp *bsp = g_4e0344->bsp;

		if (bsp->unknown1c != NONE && bsp->checksum == ((s_16e290_match_view *)g_4e0348)->checksum)
		{
			result = function_12dcb0(&bsp->clusters[bsp->indices64[index].cluster_index].block);
		}
	}
	return result;
}

// @retail 0x16e8f0
bool function_16e8f0(long index)
{
	bool result = true;

	if (g_4e0344 && g_4e0344->bsp_count > 0 && g_4e0348)
	{
		s_16e290_bsp *bsp = g_4e0344->bsp;

		if (bsp->unknown1c != NONE && bsp->checksum == ((s_16e290_match_view *)g_4e0348)->checksum)
		{
			result = function_12dcb0(&bsp->clusters[bsp->indices54[index].cluster_index].block);
		}
	}
	return result;
}

/* a 0x44 byte state cleared with the default vectors */
struct s_16f460
{
	byte unknown00[0x28];
	real scale;
	real_vector3d forward;
	real_vector3d up;
	byte unknown44[0xac - 0x44];
};

extern real g_54e854;

// @retail 0x16f460
void function_16f460(s_16f460 *state)
{
	memset(state, 0, sizeof(s_16f460));
	state->forward = *g_4687a8;
	state->up = *g_4687b0;
	state->scale = g_54e854;
}

bool g_4ea935;
extern byte g_4ea936;
extern long g_4e64a0;
extern long g_4e64a4;
long g_4e64ac;
long g_4e6470;
long g_4e6474;

// @retail 0x16f200
void function_16f200(void)
{
	if (g_4ea935)
	{
		if (g_4e6948->state != 2)
		{
			g_4e64a4 = 1;
			g_4e64a0 = 3;
			g_4e64ac = 0;
			g_4e6470 = 3;
			g_4e6474 = 1;
		}
		g_4ea935 = false;
	}
	if (g_4ea936)
	{
		if (g_4e6948->state != 2)
		{
			g_4e64a4 = 0;
			g_4e64a0 = 3;
			g_4e64ac = 0;
			g_4e6470 = 3;
			g_4e6474 = 0;
		}
		g_4ea936 = false;
	}
}
