/* SIMULATION_WORLD.H: the simulation world (g_4cf77c, 0x1220 bytes at
   0x544b20): its views, the 16 players and 16 actors it tracks, and the
   queue of 0x404c-byte blocks it buffers (lane D) */

#ifndef SIMULATION_WORLD_H
#define SIMULATION_WORLD_H

#include "cseries.h"
#include "input_record.h"

/* a machine's 6-byte address, compared with memcmp */
struct s_machine_address
{
	byte bytes[6];
};

/* a player's 12-byte key */
typedef dword t_player_key[3];

class c_simulation_world;
class c_simulation_view;
struct s_network_observer;

/* the per-view baseline of the replicated state (0x4cd0 bytes, at +0x6060 of
   the view's distribution data) */
struct s_simulation_view_baseline
{
	c_simulation_view *view;
	bool unknown04;
	bool active;
	bool unknown06;
	byte unknown07;
	long sequence;
	s_input_record state;
	long time;
};

/* the replication data a view owns (src/simulation_view.cpp; only the fields
   the view code touches) */
struct s_simulation_view_data
{
	byte unknown00[0x30];
	byte handles[8];	/* a c_handle_table_450cd0 (unknown_096ed0.h) */
	byte unknown38;
	bool unknown39;
	bool unknown3a;
	byte unknown3b[0x5079 - 0x3b];
	bool unknown5079;
	byte unknown507a[0x6060 - 0x507a];
	s_simulation_view_baseline baseline;
};

/* a view of the world onto one remote machine; type 1/3 views face a remote
   authority, type 2/4 views a remote client */
class c_simulation_view
{
public:
	word unknown00;
	short type;
	long unknown04;
	s_simulation_view_data *data;
	c_simulation_world *world;
	long world_index;
	s_machine_address address;
	byte unknown1a[2];
	long unknown1c;
	s_network_observer *observer;
	long channel_index;
	long failure_reason;
	long state;
	long state_id;
	long remote_state;
	long remote_id;
	long unknown3c;
	long unknown40;
	byte unknown44[0x75 - 0x44];
	byte flag75;
	byte unknown76[2];
	bool flag78;
	byte unknown79[3];
	dword player_mask;
	long unknown80;
	long unknown84;
	bool unknown88;
	byte unknown89[3];
	long time8c;
	long unknown90;
	byte *buffer;
	long unknown98;
	long unknown9c;
	long unknowna0;
	byte unknowna4[0xac - 0xa4];
	long unknownac;
	long unknownb0;

	void initialize(long unknown04, short type, s_simulation_view_data *data, const s_machine_address *address, long unknown1c);
	bool channel_ready(void);
	void set_state(long state, long id);
	void fail(long reason);
	void update_established(void);
	void release_buffer(void);
	void detach(void);
	void set_unknown88(bool value);
	bool has_pending_entity(void);
	bool function_85cb0(void);
	void update_baseline(void);
	bool update_player_mask(dword player_mask, dword valid_mask, const t_player_key *keys);
	void send_player_update(dword controller_mask, const struct s_simulation_player_state *states);
	bool handle_player_update(bool failed, long a, long b, dword controller_mask, const struct s_simulation_player_state *states);

	bool established(void) const
	{
		return unknown3c != NONE && flag75;
	}
};

/* a player's simulation state (0x5c bytes) */
struct s_simulation_player_state
{
	dword data[0x17];
};

/* a player the world tracks (0x88 bytes) */
struct s_simulation_world_player
{
	long player_index;
	long unknown04;
	long unknown08;
	dword key[3];
	byte unknown18[0x20 - 0x18];
	long unknown20;
	bool flag24;
	bool flag25;
	byte unknown26[2];
	long state_time;
	s_simulation_player_state state;
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

/* one of the 16 player records of the world's owner (0xb4 bytes) */
struct s_simulation_owner_player
{
	dword key[3];
	bool flag0c;
	byte unknown0d[0xb4 - 0xd];
};

/* the 16 players a watcher knows of (simulation_players.cpp) */
struct s_player_collection
{
	dword player_mask;
	s_simulation_owner_player players[16];
};

/* what the world belongs to (the simulation watcher, g_4cf780): its valid
   players */
struct s_simulation_world_owner
{
	byte unknown00[0x1c];
	long unknown1c;
	byte unknown20[4];
	dword unknown24[0x18];
	byte unknown84[4];
	s_player_collection players;
	byte unknownbcc[0xc30 - 0xbcc];
	bool unknownc30;
};

dword simulation_player_collection_get_in_game_mask(const s_player_collection *collection);
bool simulation_watcher_player_valid(long player_index, const s_simulation_world_owner *watcher, const t_player_key *key);
bool simulation_world_player_valid(long player_index, c_simulation_world *world, const t_player_key *key);
dword function_696f0(c_simulation_world *world);
bool simulation_watcher_get_players(s_simulation_world_owner *watcher, long *unknown1c, dword *player_mask, dword *in_game_mask, dword *state, t_player_key *keys, bool force);

struct s_key_450d14;
void function_6a7f0(c_simulation_world *world, s_key_450d14 *key, dword controller_mask, const s_simulation_player_state *states);
void simulation_world_view_established(c_simulation_world *world, c_simulation_view *view, bool established);
void simulation_world_view_synchronized(c_simulation_world *world, c_simulation_view *view, bool synchronized);
void simulation_view_baseline_set_active(s_simulation_view_baseline *baseline, bool active);
void simulation_view_baseline_send(s_simulation_view_baseline *baseline);
void simulation_view_baseline_update(s_simulation_view_baseline *baseline);
void __stdcall simulation_view_buffer_disposed(byte *buffer, c_simulation_view *view);
void function_6b040(c_simulation_world *world);

/* a replicated entity (0x20 bytes) and the type definition that handles it */
struct s_simulation_entity
{
	long handle;
	short type;
	byte unknown06[0x20 - 6];
};

class c_simulation_entity_definition
{
public:
	virtual void v0() = 0;
	virtual void v1() = 0;
	virtual void v2() = 0;
	virtual void v3() = 0;
	virtual void v4() = 0;
	virtual void v5() = 0;
	virtual bool v6(s_simulation_entity *entity) = 0;
};

struct s_simulation_entity_definitions
{
	long count;
	c_simulation_entity_definition *definitions[1];
};

/* the entities the world replicates (0x400 of them) */
struct s_simulation_entity_database
{
	byte unknown00[0x10];
	s_simulation_entity_definitions *definitions;
	s_simulation_entity entities[0x400];
};

/* what the world distributes: the replicated handles (s_handle_peers,
   unknown_096ed0.h, 0x2048 bytes) and the entity database */
struct s_simulation_distribution
{
	byte peers[0x2048];
	byte unknown2048[0x2098 - 0x2048];
	s_simulation_entity_database entity_database;
};

class c_simulation_world
{
public:
	s_simulation_world_owner *owner;
	s_simulation_distribution *distribution;
	long state;
	byte unknown0c;
	s_machine_address local_address;
	byte unknown13[0x18 - 0x13];
	long unknown18;
	long unknown1c;
	long unknown20;
	bool flag24;
	bool flag25;
	byte unknown26[2];
	long unknown28;
	bool flag2c;
	byte unknown2d;
	byte flag2e;
	byte unknown2f;
	long unknown30;
	long time34;
	long unknown38;
	long view_count;
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
