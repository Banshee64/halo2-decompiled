// @flags /O2 /Ob1 /Gr
/* SIMULATION_WORLD.CPP: the simulation world (g_4cf77c) and the simulation
   globals' queries (lane D) */

#include "cseries.h"
#include <string.h>
#include "globals.h"
#include "simulation_world.h"

extern byte g_4cf771;
extern byte g_4cf772;

#define SIMULATION_WORLD ((c_simulation_world *)g_4cf77c)

typedef void (__stdcall *game_module_proc)(dword);
extern game_module_proc g_46e320[30];

/* an iteration over the world's views: the views whose type bit is set in mask */
struct s_view_iterator
{
	dword mask;
	long index;
};

// @retail 0x69470
bool world_next_view(c_simulation_world *world, s_view_iterator *iterator, c_simulation_view **out)
{
	bool result = false;

	while (iterator->index >= 0)
	{
		if (iterator->index >= 15)
			break;
		c_simulation_view *view = world->views[iterator->index++];
		if (view && (iterator->mask & (1 << view->type)))
		{
			*out = view;
			result = true;
			break;
		}
	}
	return result;
}


// @retail 0x67e10
bool function_67e10(c_simulation_world *world)
{
	return world->state != 3 && world->state != 5;
}

// @retail 0x68250
bool function_68250(void)
{
	bool result = false;
	if (g_4cf770)
	{
		if (g_4e6948 && g_4e6948->flag1120 && g_4e6948->index != NONE && !g_4cf772 && SIMULATION_WORLD->unknown18 == 4)
			result = true;
		else
			result = false;
	}
	return result;
}

// @retail 0x68290
bool function_68290(void)
{
	bool result = false;
	if (g_4cf770 && !g_4cf772)
	{
		c_simulation_world *world = SIMULATION_WORLD;
		if (world->state)
			result = world->unknown18 != 4;
	}
	return result;
}

// @retail 0x690d0
void function_690d0(c_simulation_world *world, long actor_index, const dword *state)
{
	if (world->state != 3 && world->state != 5)
	{
		s_simulation_world_actor *actor = &world->actors[actor_index];
		if (actor->actor_index != NONE)
		{
			actor->time = g_510c54->game_time;
			memcpy(actor->state, state, sizeof(actor->state));
		}
	}
}

// @retail 0x69580
void function_69580(c_simulation_world *world, long player_index)
{
	long absolute_index = (word)player_index;
	s_simulation_world_player *player = &world->players[absolute_index];
	player->unknown20 = 0;
	player->player_index = NONE;
	player->unknown04 = NONE;
	player->unknown08 = NONE;

	s_view_iterator iterator;
	c_simulation_view *view;
	iterator.mask = 0x14;
	iterator.index = 0;
	while (world_next_view(world, &iterator, &view))
	{
		if (view->unknown3c != NONE && view->flag75)
			view->player_mask &= ~(1 << absolute_index);
	}
}

// @retail 0x69610
void function_69610(c_simulation_world *world)
{
	for (short i = 0; i < 16; i++)
	{
		s_simulation_world_player *player = &world->players[i];
		if (player->player_index != NONE && player->flag25)
			player->flag25 = 0;
	}
}

// @retail 0x696d0
bool function_696d0(c_simulation_world *world, long player_index)
{
	s_simulation_world_player *player = &world->players[(word)player_index];
	bool result = false;
	if (player->player_index != NONE)
		result = player->flag25;
	return result;
}

// @retail 0x696f0
dword function_696f0(c_simulation_world *world)
{
	dword mask = 0;
	for (long i = 0; i < 16; i++)
	{
		s_simulation_world_player *player = &world->players[i];
		if (player->player_index != NONE && player->flag25)
			mask |= 1 << i;
	}
	return mask;
}

// @retail 0x6a480
dword function_6a480(c_simulation_world *world)
{
	dword mask = 0;
	for (long i = 0; i < 16; i++)
	{
		if (world->players[i].player_index != NONE)
			mask |= 1 << i;
	}

	s_view_iterator iterator;
	c_simulation_view *view;
	iterator.mask = 0x14;
	iterator.index = 0;
	while (world_next_view(world, &iterator, &view))
	{
		bool established = view->type == 2 ? view->state >= 5 : view->state >= 3;
		if (established)
			mask &= view->player_mask;
	}
	return mask;
}

// @retail 0x6a600
void c_simulation_world::delete_all_players(void)
{
	for (long i = 0; i < 16; i++)
	{
		s_simulation_world_player *player = &players[i];
		if (player->player_index != NONE)
		{
			player->player_index = NONE;
			player->unknown04 = NONE;
			player->unknown08 = NONE;
			player->unknown20 = 0;
		}
	}
}

// @retail 0x6a6f0
void c_simulation_world::delete_all_actors(void)
{
	for (long i = 0; i < 16; i++)
	{
		s_simulation_world_actor *actor = &actors[i];
		if (actor->actor_index != NONE)
		{
			actor->actor_index = NONE;
			actor->unknown04 = NONE;
			actor->unknown08 = 0;
		}
	}
}

// @retail 0x6a860
void function_6a860(c_simulation_world *world, long *size)
{
	for (short i = 0; i < 4; i++)
		g_46e320[i](0);
	world->flag11fc = 1;
	*size = 0x3fe000;
}

static inline bool world_buffering(c_simulation_world *world)
{
	return world->buffer_size != NONE;
}

// @retail 0x6a990
bool function_6a990(c_simulation_world *world, long size, const void *data, long offset)
{
	bool result = false;
	long state = world->state;
	if (state && (state == 3 || state == 5) && state != 4 && state != 5)
	{
		if (world_buffering(world) && world->buffer_size == offset)
		{
			byte *buffer = world->buffer;
			if (buffer && offset >= 0 && size > 0 && offset + size <= 0x40000)
			{
				memcpy(buffer + offset, data, size);
				world->buffer_size += size;
				result = true;
			}
		}
	}
	return result;
}

// @retail 0x6ab10
void function_6ab10(c_simulation_world *world)
{
	s_simulation_block *block = world->first_block;
	while (block)
	{
		s_simulation_block *next = block->next;
		long info;
		if (!g_4d87f8->allocator->get_info(block, &info))
			info = NONE;
		s_allocator_globals *globals = g_4d87f8;
		globals->allocator->release(block, NONE);
		globals->count--;
		block = next;
	}
	world->first_block = 0;
	world->last_block = 0;
	world->block_count = 0;
	world->unknown1210 = NONE;
	world->unknown120c = 0;
}

// @retail 0x6ab90
bool function_6ab90(c_simulation_world *world, const s_simulation_block_data *data)
{
	s_allocator_globals *globals = g_4d87f8;
	s_simulation_block *block = (s_simulation_block *)globals->allocator->allocate(sizeof(s_simulation_block), 0, 0);
	if (!block)
	{
		globals->allocator->compact(0);
		block = (s_simulation_block *)globals->allocator->allocate(sizeof(s_simulation_block), 0, 0);
	}
	if (block)
		globals->count++;
	if (!block)
		return false;

	if (world->last_block)
		world->last_block->next = block;
	else
		world->first_block = block;
	block->next = 0;
	world->block_count++;
	world->last_block = block;
	block->data = *data;
	world->unknown1210 = data->size;
	return true;
}

// @retail 0x6ac30
void function_6ac30(c_simulation_world *world, s_simulation_block_data *data)
{
	s_simulation_block *block = world->first_block;
	*data = block->data;
	world->first_block = block->next;
	if (world->last_block == block)
		world->last_block = 0;
	long info;
	g_4d87f8->allocator->get_info(block, &info);
	s_allocator_globals *globals = g_4d87f8;
	globals->allocator->release(block, NONE);
	globals->count--;
	world->block_count--;
	world->unknown120c++;
}

// @retail 0x6acb0
c_simulation_view *function_6acb0(c_simulation_world *world)
{
	s_view_iterator iterator;
	c_simulation_view *view = 0;
	iterator.mask = 0xa;
	iterator.index = 0;
	world_next_view(world, &iterator, &view);
	return view;
}

// @retail 0x6ace0
c_simulation_view *function_6ace0(c_simulation_world *world, long value)
{
	s_view_iterator iterator;
	c_simulation_view *view;
	iterator.mask = 0x14;
	iterator.index = 0;
	c_simulation_view *result = 0;
	while (world_next_view(world, &iterator, &view))
	{
		if (view->unknown1c == value)
		{
			result = view;
			break;
		}
	}
	return result;
}

// @retail 0x6ad40
c_simulation_view *function_6ad40(c_simulation_world *world, const s_machine_address *address)
{
	s_view_iterator iterator;
	c_simulation_view *view;
	iterator.mask = 0x14;
	iterator.index = 0;
	while (world_next_view(world, &iterator, &view))
	{
		s_machine_address view_address = view->address;
		if (!memcmp(&view_address, address, sizeof(s_machine_address)))
			return view;
	}
	return 0;
}

// @retail 0x6adc0
c_simulation_view *function_6adc0(c_simulation_world *world, long value)
{
	s_view_iterator iterator;
	c_simulation_view *view;
	iterator.mask = NONE;
	iterator.index = 0;
	c_simulation_view *result = 0;
	while (world_next_view(world, &iterator, &view))
	{
		if (view->unknown3c == value)
		{
			result = view;
			break;
		}
	}
	return result;
}
