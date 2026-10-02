/* GLOBALS.H: globals shared by more than one source file (defined in
src/globals.cpp) */

#ifndef GLOBALS_H
#define GLOBALS_H

#include "real_math.h"
#include "data_array.h"

/* the game time globals (0x24 bytes); ticks_per_second is read by
   firing position code; scale is 1.0 once initialized */
struct s_game_time_globals
{
	bool active;
	byte unknown01;
	short ticks_per_second;
	real rate;
	long game_time;
	real scale;
	byte unknown10[0x10];
	long unknown20;
};

extern s_game_time_globals *g_510c54;

/* g_4e0300: the object header data, a data array (data_array.h) whose
   elements (12 bytes each: 8 unknown bytes, then the object pointer) each
   source file views through its own s_object_header */
extern s_data_array *g_4e0300;

/* g_4e6948: the game options. 016a90 reads the state at +8, 03d380 and
   072c70 the mode at +0xc, 146240 the ticks per second at +0xe, the session
   states (058dd0) the ids and positions at +0x10..+0x20 and the flag at
   +0x1120 (also read by 096e90) */
struct s_game_options_view
{
	byte unknown00;
	byte flag;
	short index;
	byte unknown04[4];
	long state;
	char mode;
	byte unknown0d;
	short ticks_per_second;
	long id_a;
	long id_b;
	byte unknown18[4];
	long position_a;
	long position_b;
	byte unknown24[0x184 - 0x24];
	struct
	{
		dword bit0 : 1;
		dword bit1 : 1;
		dword bit2 : 1;
		dword unknown : 9;
		dword bit12 : 1;
		dword bit13 : 1;
	} flags184;
	byte unknown188[0x22c - 0x188];
	byte flags22c;
	byte unknown22d[3];
	short s230;
	short s232;
	short s234;
	short s236;
	byte unknown238[0x1120 - 0x238];
	byte flag1120;
};

extern s_game_options_view *g_4e6948;

/* the multiplayer globals (g_4e9ae8): the engine index at +0xc14 selects the
   engine object in g_55e4d0; value24 is read by 0a45d0; the 16 slot
   identifiers at +0x2c are read by the entity definitions of 09a5e0 */
struct s_name18
{
	word c[9];
};

struct s_player_info
{
	byte b0;
	byte unknown01[3];
	real_point3d v;
	word w10;
	word w12;
	byte b14;
	byte unknown15[3];
};

struct s_stats
{
	long l[9];
};

struct s_mp_globals
{
	byte unknown00[6];
	word w6;
	word w8;
	word wa;
	word wc;
	word we;
	s_name18 name;
	byte unknown22[2];
	dword value24;
	byte unknown28[4];
	long slots[16];
	short w6c;
	word w6e;
	byte unknown70[0xe0 - 0x70];
	word we0;
	byte unknowne2[0xfc - 0xe2];
	s_stats stats;
	byte unknown120[0x558 - 0x120];
	s_player_info players[1];
	byte unknown570[0x6dc - 0x570];
	long l6dc[4];
	byte unknown6e0[0xc08 - 0x6ec];
	bool bc08;
	byte unknownc09[0xc14 - 0xc09];
	long engine_index;
	byte unknownc18[0x84];
};

extern s_mp_globals *g_4e9ae8;

/* the engine objects, indexed by g_4e9ae8->engine_index (engine_peer.h) */
class c_engine_peer;
extern c_engine_peer *g_55e4d0[256];

/* g_4e8c20: a table of indices (entries at +0xc) */
struct s_index_table
{
	byte unknown00[0xc];
	long entries[4];
};

extern s_index_table *g_4e8c20;

/* globals shared between the game state lifecycle callbacks (batch 24-1) and
   the game state code (03d380) */
struct s_simulation_world;
struct s_47f048_object;
extern byte g_4cf770;
extern s_simulation_world *g_4cf77c;
extern s_47f048_object *g_4cf780;
extern dword g_4701ec;
extern byte *g_4e8c34;
extern long *g_510c70;
extern s_data_array *g_4ed28c;
extern s_data_array *g_4ea950;
extern s_data_array *g_4ee4e4;
extern s_data_array *g_4ee4e8;

/* g_4e9188: the Bink state; the memory callbacks are registered by 155ea0 */
struct s_bink_globals
{
	byte initialized;
	byte flag1;
	byte unknown02[0xde];
};

extern s_bink_globals g_4e9188;

/* g_509448 (decals) and g_557c6c: tables of callbacks at the start, then the
   allocator that built them */
struct s_game_proc_table_509448
{
	byte unknown00[0x20];
	void (*proc20)(void);
	void (*proc24)(void);
	byte unknown28[0x6c - 0x28];
	c_data_allocator *allocator;
};

struct s_game_proc_table_557c6c
{
	byte unknown00[0x2c];
	void (*proc2c)(void);
	void (*proc30)(void);
	c_data_allocator *allocator;
};

extern s_game_proc_table_509448 *g_509448;
extern s_game_proc_table_557c6c *g_557c6c;

/* g_4e3b44: the tag instances (16 bytes each: the data pointer is at +8);
   batches 2-5 (animation tag data) and an earlier bitmap batch view the same
   data pointer with different types */
struct bitmap_group;
struct s_animation_tag_data;
struct s_sound_tag_data;
struct s_palette_tag_data;
struct s_tag_flags;
struct s_tag_instance
{
	byte unknown00[8];
	union
	{
		s_tag_flags *flags;
		s_animation_tag_data *data;
		bitmap_group *group;
		s_palette_tag_data *palette;
		s_sound_tag_data *sound;
		byte *bytes;
	};
	byte unknown0c[4];
};

extern s_tag_instance *g_4e3b44;

/* a default point and vector, shared by the camera and animation code
   (a vector in 2-7, a point in 2-3: three reals either way) */
extern real_point3d *g_468788;

/* g_4e034c: the globals holding a tag header at +0xc0 (only used when the
   pointer at +0xc0 is set; the one at +0xc4 is read in that case), used by
   the animation (2-1) and sound (2-2) code */
struct s_tag_header
{
	byte unknown00[4];
	dword datum_index;
};

struct s_table_a;
struct s_table_b;

struct s_tag_header_globals
{
	byte unknown00[0xc0];
	s_tag_header *header;
	s_tag_header *header_alt;
	byte unknownc8[0x16c - 0xc8];
	long index;
	void *a_valid;
	s_table_a *a;
	void *b_valid;
	s_table_b *b;
};

extern s_tag_header_globals *g_4e034c;

/* g_4687a4: a default vector, read by the camera (2-10) and animation code */
extern real_vector3d *g_4687a4;

/* the rasterizer state flags, shared by 030290 and 0494b0 */
extern dword g_4ba014;

/* the animation sampling state, shared by the channel decoders of 28c510
   and 28cdb0 (each defines its own view of the animation data and output) */
struct s_animation_data;
struct s_animation_output;
extern s_animation_data *g_504480;
extern dword g_504464;
extern real g_50446c;
extern long g_5044b4;
extern long g_5044b8;
extern long g_5044bc;
extern s_animation_output *g_5044c0;

/* the object type definitions, indexed by object type (108a90, 108fd0) */
struct s_object_type_definition;
extern s_object_type_definition *g_468630[16];

/* the game state allocator: game_state_globals (game_state.h) is at 0x4e6080 */

/* the default axis (a vector) and the pi constant of the vector math
   (11cc90, 11d180) */
extern real_vector3d *g_4687a8;
extern real_vector3d *g_4687ac;
extern real_vector3d *g_4687b0;
extern c_data_allocator *g_46875c;
extern real g_5476c4;

/* data arrays (data_array.h): g_4cf78c (0b49a0), g_4f55f0 (clumps, 26b230;
   slot owners, 1a8080), g_502420 and g_50241c (clumps, 26b230), g_502418
   (26bda0), g_4e8c24 (the players; 0699a0, 072c70, 096e90) and the arrays of
   03d380 */
extern s_data_array *g_4cf78c;
extern s_data_array *g_502418;
extern s_data_array *g_50241c;

/* g_4e8c24: the players (elements of 0x21c bytes) */
extern s_data_array *g_4e8c24;

/* g_4d87f8: the allocator interface (slots 0, 1, 5, 10 and 13 are used; get_info
   returns whether the block was found) and the count of live blocks, shared by
   081f80, 08ad30, 08b110, 096e90 and 097d80 */
class c_allocator
{
public:
	virtual void release(void *block, long size) {}
	virtual bool get_info(void *block, void *info) { return false; }
	virtual void slot2() {}
	virtual void slot3() {}
	virtual void slot4() {}
	virtual void *allocate(long size, long a, long b) { return 0; }
	virtual void slot6() {}
	virtual void slot7() {}
	virtual void slot8() {}
	virtual void slot9() {}
	virtual void compact(long size) {}
	virtual void slot11() {}
	virtual void slot12() {}
	virtual void dispose(long flags) {}
};

struct s_allocator_globals
{
	c_allocator *allocator;
	long count;
	byte unknown08;

	~s_allocator_globals()
	{
		if (allocator)
		{
			allocator->dispose(1);
			allocator = 0;
		}
	}
};

extern s_allocator_globals *g_4d87f8;

/* g_4eca60: the identifiers in the slots of the multiplayer globals' slot
   table (09a5e0, 183c60) */
extern long g_4eca60[8];

/* g_480118: another allocator (1efac0) */
extern c_allocator *g_480118;

/* a real zero, read by 16ae0 and 1efac0 */
extern real g_45dbd8;

/* g_4f55f0: a datum array (clumps, 26b230; slot owners, 1a8080) */
extern s_data_array *g_4f55f0;

/* g_502420: a datum array (clump objects; 26b230, 26bda0) */
extern s_data_array *g_502420;

/* g_4e0350: the globals with the entry table at +0x10c (1eb8a0, 19c1d0) and
   the palette sources at +0x214 (0158f0) */
struct s_unknown_entry;
struct s_palette_source;
struct s_palette_source_globals
{
	byte unknown00[0x10c];
	s_unknown_entry *entries;
	byte unknown110[0x104];
	s_palette_source *sources;
};

extern s_palette_source_globals *g_4e0350;

/* g_4e0348: the match globals. The entries at +0x10 are read by 1ee340; the
   count and match entries at +0x22c are read by 0bfd20. */
struct s_tag_block_entry
{
	byte unknown00[8];
	short value08;
	byte unknown0a[6];
	short value10;
	byte unknown12[2];
};

struct s_match_entry
{
	dword key;
	short subkey;
	byte byte_06;
	byte byte_07;
	byte unknown08[0x54];
};

struct s_match_globals
{
	byte unknown00[0x10];
	s_tag_block_entry *entries;
	byte unknown14[0x218];
	long count;
	s_match_entry *match_entries;
};

extern s_match_globals *g_4e0348;

/* input-device state, shared by 196d20 (the whole array) and 1969d0.
   g_511000 holds 4 entries of 0xa4 bytes. 1969d0 reads the array from
   0x511034 (g_511000 + 0x34) with its own layout (s_input_device_view,
   which runs past the entry into the next one), through input_device(). */
struct s_input_vector
{
	dword v[4];
};

struct s_input_entry_state
{
	byte unknown00[4];
	byte active;
	byte unknown05[0x4f];
	s_input_vector vector;
	byte unknown64[0x2c];
	char value_90;
	byte unknown91[0xb];
	short value_9c;
	byte unknown9e[6];
};

struct s_input_counter
{
	word value : 15;
	word flag : 1;
};

struct s_input_device_view
{
	byte active;
	byte unknown01;
	byte unknown02[0xe];
	byte data[0x7c];
	char slot;
	byte unknown8d[0x13];
	char button;
	byte unknownA1;
	short axis;
};

extern byte g_510ca0;
extern byte g_510cb0;
extern byte g_510cb1;
extern dword g_510e2c;
extern dword g_510e30;
extern s_input_counter g_511c90[0x1b5 * 4];
extern s_input_counter g_511c4e[0x1b5 * 4];
extern s_input_counter g_515294[0x1000];
extern s_input_entry_state g_511000[4];

inline s_input_device_view *input_device(long index)
{
	return (s_input_device_view *)((byte *)g_511000 + 0x34) + index;
}

/* g_4cef68: the creation weights of the object types, 0x4c bytes per entry (turret and vehicle entity definitions) */
struct s_creation_weight
{
	real weight;
	long field4;
	byte unknown08[0x44];
};

extern s_creation_weight g_4cef68[1];

/* g_51ebd4: the sound permutation tables (219110: the sets, chances, entries
   and bit data) and the entries at +0x4c (03d380) */
struct s_permutation_set
{
	byte unknown00[8];
	short first_index;
	short count;
};

struct s_permutation_chance
{
	byte unknown00[2];
	word chance;
	byte unknown04[12];
};

struct s_sound_globals
{
	byte unknown00[0x24];
	s_permutation_set *sets;
	byte unknown28[4];
	s_permutation_chance *chances;
	byte unknown30[4];
	byte *entries34;
	byte unknown38[4];
	byte *bits;
	byte unknown40[0x4c - 0x40];
	byte *entries;
};

extern s_sound_globals *g_51ebd4;

/* the current palette source index (0158f0, 03d380) */
extern short g_4686c4;

/* the physical memory heap: a block index, and per block the lowest allowed
   address and the current top (0b3d30, and the PHYSICAL_MEMORY_ALLOCATE
   macro of unknown_053310.h) */
extern long g_4e6420;
extern long g_4e642c[2];
extern long g_4e6440[2];

/* the time source: when g_510548 is set, g_51054c is the current time
   (otherwise GetTickCount is used); read by 058dd0 and 08b110 */
extern byte g_510548;
extern long g_51054c;

/* the allocator the data arrays of 106 and 116 are built through, the cloth data array (1169f0) and the prop state data array (25d690) */
extern c_data_allocator *g_510c2c;
extern s_data_array *g_4e0338;
extern s_data_array *g_4e0320;
extern s_data_array *g_4cf8d8;
extern bool g_4cf8d4;
extern s_data_array *g_502414;

#endif
