/* SIMULATION_WORLD.H: the simulation world (g_4cf77c, 0x1220 bytes at
   0x544b20): its views, the 16 players and 16 actors it tracks, and the
   queue of 0x404c-byte blocks it buffers (lane D) */

#ifndef SIMULATION_WORLD_H
#define SIMULATION_WORLD_H

#include "cseries.h"

/* a machine's 6-byte address, compared with memcmp */
struct s_machine_address
{
	byte bytes[6];
};

/* a view of the world onto one remote machine; type 1/3 views face a remote
   authority, type 2/4 views a remote client */
class c_simulation_view
{
public:
	byte unknown00[2];
	short type;
	byte unknown04[0x14 - 0x4];
	s_machine_address address;
	byte unknown1a[2];
	long unknown1c;
	byte unknown20[0x2c - 0x20];
	long state;
	byte unknown30[0x3c - 0x30];
	long unknown3c;
	byte unknown40[0x75 - 0x40];
	byte flag75;
	byte unknown76[0x7c - 0x76];
	dword player_mask;
};

/* a player the world tracks (0x88 bytes) */
struct s_simulation_world_player
{
	long player_index;
	long unknown04;
	long unknown08;
	byte unknown0c[0x20 - 0xc];
	long unknown20;
	byte unknown24;
	bool flag25;
	byte unknown26[0x88 - 0x26];
};

/* an actor the world tracks (0x90 bytes) */
struct s_simulation_world_actor
{
	long actor_index;
	long unknown04;
	long unknown08;
	byte unknown0c[4];
	long time;
	dword state[0x1f];
};

/* one buffered block (0x404c bytes: its contents, then the link) */
struct s_simulation_block_data
{
	long size;
	byte bytes[0x4048 - 4];
};

struct s_simulation_block
{
	s_simulation_block_data data;
	s_simulation_block *next;
};

class c_simulation_world
{
public:
	byte unknown00[8];
	long state;
	byte unknown0c[0x18 - 0xc];
	long unknown18;
	byte unknown1c[0x2e - 0x1c];
	byte flag2e;
	byte unknown2f[0x40 - 0x2f];
	c_simulation_view *views[15];
	s_simulation_world_player players[16];
	s_simulation_world_actor actors[16];
	byte flag11fc;
	byte unknown11fd[3];
	long buffer_size;
	byte *buffer;
	byte unknown1208[4];
	long unknown120c;
	long unknown1210;
	long block_count;
	s_simulation_block *first_block;
	s_simulation_block *last_block;

	void delete_all_players(void);
	void delete_all_actors(void);
};

#endif
