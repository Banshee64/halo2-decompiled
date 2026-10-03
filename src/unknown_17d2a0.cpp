// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_17D2A0.CPP: the decals (entry 37 of the lifecycle table) */

#include "cseries.h"
#include "globals.h"
#include "game_state.h"
#include <string.h>
#include <xtl.h>

D3DResource *g_509444;

/* a decal (g_4ea950, 0x40 bytes) */
struct s_decal_datum
{
	short salt;
	byte flag0 : 1;
	byte flag1 : 1;
	byte : 6;
	char cell_x;
	long definition_index;
	short unknown08;
	short cell_y;
	real_point3d position;
	long creation_time;
	real lifetime;
	real fade_time;
	union
	{
		dword color;
		struct
		{
			byte unknown24[3];
			byte alpha;
		};
	};
	byte unknown28[8];
	long previous_index;
	long next_index;
	long first_index;
	long next_in_group_index;
};

/* the decal cells in the game state (g_4ea94c, 0x3c2c bytes) */
struct s_decal_globals
{
	long cells[7][512];
	long unassigned_index;
	long fading_count;
	long permanent_count;
	byte unknown380c[0x3c2c - 0x380c];
};

s_decal_globals *g_4ea94c;

/* the first decal of each decal definition (unknown_03d380.cpp) */
extern dword g_4c8798[256];

void crc_checksum_buffer(dword *crc_reference, void const *buffer, long buffer_size);
void function_43890(void);
void function_43990(void);
/* the resource manager of the decals (g_509448; unknown_13d170.cpp) */
struct s_resource_manager;

struct s_decal_resource_manager
{
	byte unknown00[0x38];
	long generation;
	long first;
	long last;
	long limits[8];
};

void function_13d230(s_resource_manager *manager);
void function_13d830(s_resource_manager *manager, long handle);
void function_13d2b0(s_game_proc_table_509448 *table);
void __stdcall function_23aad0(long a, long b, long c);
void function_17d5f0(bool permanent);
void function_17d860(long decal_index);

struct s_bsp3d;
extern s_bsp3d *g_4e033c;
long function_14a280(s_bsp3d *bsp, real_point3d *point, long index);

struct s_decal_structure_leaf
{
	short cluster_index;
	byte unknown02[6];
};

struct s_decal_structure_bsp
{
	byte unknown00[0x30];
	s_decal_structure_leaf *leaves;
};

#define DECAL(index) ((s_decal_datum *)g_4ea950->data + ((index) & 0xffff))

// @retail 0x17d220
void decals_initialize(void)
{
	g_4ea950 = data_new_inlined("decals", 0x400, sizeof(s_decal_datum), 0, g_510c2c);

	long size = sizeof(s_decal_globals);
	byte *base = game_state_globals.base_address + game_state_globals.cpu_allocation_size;

	game_state_globals.cpu_allocation_size += sizeof(s_decal_globals);
	crc_checksum_buffer(&game_state_globals.allocation_size_checksum, &size, 4);
	g_4ea94c = (s_decal_globals *)base;
	function_43890();
}

// @retail 0x17d2a0
void decals_dispose(void)
{
	g_4ea950 = 0;
	if (g_509444)
	{
		D3DResource_Release(g_509444);
		g_509444 = 0;
	}
	g_509448->allocator->deallocate(g_509448);
}

// @retail 0x17d2e0
void decals_initialize_for_new_map(void)
{
	s_decal_globals *globals = g_4ea94c;
	s_data_array *decals = g_4ea950;

	memset(globals, 0, sizeof(*globals));
	memset(globals->cells, 0xff, sizeof(globals->cells));
	globals->unassigned_index = NONE;
	globals->fading_count = 0;
	globals->permanent_count = 0;
	decals->valid = true;
	data_delete_all(decals);
}

// @retail 0x17d330
void decals_dispose_from_old_map(void)
{
	function_17d5f0(true);
	function_13d2b0(g_509448);
	g_4ea950->valid = false;
}

// @retail 0x17ce00
void decal_link(short cell_x, short cell_y, long decal_index)
{
	long *cell = &g_4ea94c->cells[0][(cell_x << 9) + cell_y];
	long next_index = *cell;
	s_decal_datum *decal = DECAL(decal_index);

	decal->previous_index = NONE;
	decal->next_index = next_index;
	decal->cell_y = cell_y;
	decal->cell_x = (char)cell_x;
	decal->unknown08 = NONE;
	if (next_index != NONE)
		DECAL(next_index)->previous_index = decal_index;
	*cell = decal_index;
}

// @retail 0x17d100
long __stdcall function_17d100(long cell_y, long source_index)
{
	long decal_index = datum_new(g_4ea950);

	if (decal_index != NONE)
	{
		s_decal_datum *decal = DECAL(decal_index);
		s_decal_datum *source = DECAL(source_index);
		short salt = decal->salt;

		*decal = *source;
		decal->salt = salt;
		decal->first_index = source_index;
		decal->next_in_group_index = NONE;
		if (source->flag1)
			g_4ea94c->permanent_count++;
		if (source->flag0)
			g_4ea94c->fading_count++;
		decal_link(source->cell_x, (short)cell_y, decal_index);
	}
	return decal_index;
}

// @retail 0x17d350
void decals_update_locations(void)
{
	if (g_4ea950->valid)
	{
		long decal_index = g_4ea94c->unassigned_index;

		while (decal_index != NONE)
		{
			s_decal_datum *decal = DECAL(decal_index);
			long next_index = decal->next_index;

			if (g_4686c4 != NONE)
			{
				long leaf_index = function_14a280(g_4e033c, &decal->position, 0);

				if (leaf_index != NONE)
				{
					short cluster_index = ((s_decal_structure_bsp *)g_4e0348)->leaves[leaf_index].cluster_index;

					if (cluster_index != NONE)
					{
						if (next_index != NONE)
							DECAL(next_index)->previous_index = decal->previous_index;
						if (decal->previous_index != NONE)
							DECAL(decal->previous_index)->next_index = decal->next_index;
						else
							g_4ea94c->unassigned_index = decal->next_index;
						decal_link(decal->cell_x, cluster_index, decal_index);
					}
				}
			}
			decal_index = next_index;
		}
	}
}

// @retail 0x17d5f0
void function_17d5f0(bool permanent)
{
	s_data_array *decals = g_4ea950;

	if (decals->valid)
	{
		s_decal_globals *globals = g_4ea94c;
		long index = NONE;
		s_decal_datum *decal;

		for (;;)
		{
			index = data_find_index(decals, index + 1);
			if (index == NONE)
				break;
			decal = (s_decal_datum *)(decals->data + decals->size * index);
			if (!decal)
				break;
			if (decal->flag0)
			{
				decal->flag0 = false;
				globals->fading_count--;
			}
			if (permanent && decal->flag1)
			{
				decal->flag1 = false;
				globals->permanent_count--;
			}
		}
	}
}

// @retail 0x17d5e0
void __stdcall decals_render(long a, long b, long c)
{
	function_23aad0(a, b, c);
}

// @retail 0x17ccd0
bool function_17ccd0(long decal_index)
{
	s_decal_datum *decal = DECAL(decal_index);
	real age = (real)(g_510c54->game_time - decal->creation_time) * g_510c54->rate;
	bool result = false;

	decal->alpha = 0xff;
	if (!decal->flag1)
	{
		if (decal->lifetime != 0.0f && !(decal->lifetime > age))
		{
			if (decal->flag0)
			{
				decal->flag0 = false;
				g_4ea94c->fading_count--;
				for (long index = decal->next_in_group_index; index != NONE; )
				{
					s_decal_datum *member = DECAL(index);

					if (member->flag0)
					{
						member->flag0 = false;
						g_4ea94c->fading_count--;
					}
					index = member->next_in_group_index;
				}
			}
			function_13d830((s_resource_manager *)g_509448, decal->definition_index);
			return true;
		}
		if (decal->lifetime > 0.0f && decal->fade_time > 0.0f)
		{
			real remaining = decal->lifetime - age;

			if (decal->fade_time > remaining)
			{
				real alpha = remaining / decal->fade_time * 256.0f;

				if (0.0f > alpha)
					alpha = 0.0f;
				else if (alpha > 255.0f)
					alpha = 255.0f;
				decal->color = ((long)alpha << 24) | (decal->color & 0xffffff);
			}
		}
	}
	return result;
}

// @retail 0x17cc60
void function_17cc60(long decal_index)
{
	s_decal_datum *decal = DECAL(decal_index);

	if (decal->first_index == decal_index)
	{
		byte alpha = decal->alpha;
		long index = decal->next_in_group_index;

		if (!function_17ccd0(decal_index))
		{
			while (index != NONE)
			{
				s_decal_datum *member = DECAL(index);

				index = member->next_in_group_index;
				member->color = (alpha << 24) | (member->color & 0xffffff);
			}
		}
	}
}

// @retail 0x17d520
void function_17d520(void)
{
	function_17d5f0(false);

	s_decal_resource_manager *manager = (s_decal_resource_manager *)g_509448;

	if (manager->generation == 0x7fffffff)
		function_13d230((s_resource_manager *)manager);
	else
		manager->generation++;
	for (long i = 0; i < 8; i++)
		manager->limits[i] = 0x7fffffff;

	s_data_array *decals = g_4ea950;
	long index = NONE;

	for (;;)
	{
		index = data_find_index(decals, index + 1);
		if (index == NONE)
			break;

		s_decal_datum *decal = (s_decal_datum *)(decals->data + decals->size * index);

		if (decal->first_index == ((decal->salt << 16) | index) && !decal->flag1)
			function_13d830((s_resource_manager *)g_509448, decal->definition_index);
	}
}

// @retail 0x17d710
void function_17d710(short cluster_index)
{
	if (g_4ea950->valid)
	{
		for (short i = 0; i < 7; i++)
		{
			long decal_index;

			if (cluster_index == NONE)
			{
				if (i != 0)
					continue;
				decal_index = g_4ea94c->unassigned_index;
			}
			else
			{
				decal_index = g_4ea94c->cells[i][cluster_index];
			}
			while (decal_index != NONE)
			{
				s_decal_datum *decal = DECAL(decal_index);
				long next_index = decal->next_index;

				if (decal->flag1 && decal->first_index == decal_index)
				{
					decal->flag1 = false;
					g_4ea94c->permanent_count--;
					for (long index = decal->next_in_group_index; index != NONE; )
					{
						s_decal_datum *member = DECAL(index);

						if (member->flag1)
						{
							member->flag1 = false;
							g_4ea94c->permanent_count--;
						}
						index = member->next_in_group_index;
					}
					function_13d830((s_resource_manager *)g_509448, decal->definition_index);
				}
				decal_index = next_index;
			}
		}
	}
}

// @retail 0x17d810
void decal_delete_group(long decal_index)
{
	long next_index = DECAL(decal_index)->next_in_group_index;

	function_17d860(decal_index);
	while (next_index != NONE)
	{
		long index = next_index;

		next_index = DECAL(index)->next_in_group_index;
		function_17d860(index);
	}
}

// @retail 0x17d860
void function_17d860(long decal_index)
{
	s_decal_datum *decal = DECAL(decal_index);

	if (decal->next_index != NONE)
		DECAL(decal->next_index)->previous_index = decal->previous_index;
	if (decal->previous_index != NONE)
		DECAL(decal->previous_index)->next_index = decal->next_index;
	else if (decal->cell_y == NONE)
		g_4ea94c->unassigned_index = decal->next_index;
	else
		g_4ea94c->cells[0][(decal->cell_x << 9) + decal->cell_y] = decal->next_index;
	datum_delete(g_4ea950, decal_index);
}
