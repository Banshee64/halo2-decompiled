/* GLOBALS.H: globals shared by more than one source file (defined in
src/globals.cpp) */

#ifndef GLOBALS_H
#define GLOBALS_H

#include "real_math.h"

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

/* g_4e0300: the object header data. Offset 0x44 is the array of object
   headers (12 bytes each: 8 unknown bytes, then the object pointer); batches
   2-3 (s_obj_array) and 2-6 (s_object_header) view the same array */
struct s_obj_array;
struct s_object_header;
struct s_object_header_data
{
	byte unknown00[0x24];
	long header_size;
	byte unknown28[0x10];
	long maximum_count;
	byte unknown3c[8];
	union
	{
		s_obj_array *table;
		s_object_header *headers;
	};
};

extern s_object_header_data *g_4e0300;

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
	byte unknownc8[0x170 - 0xc8];
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
extern real g_5476c4;

/* g_4cf78c: a datum array (0b49a0) */
struct s_datum_array;
extern s_datum_array *g_4cf78c;

/* g_4d87f8: the allocator interface (slots 0, 1, 5 and 10 are used) and the
   count of live blocks, shared by 08ad30 and 097d80 */
class c_allocator
{
public:
	virtual void release(void *block, long size) {}
	virtual void get_info(void *block, void *info) {}
	virtual void slot2() {}
	virtual void slot3() {}
	virtual void slot4() {}
	virtual void *allocate(long size, long a, long b) { return 0; }
	virtual void slot6() {}
	virtual void slot7() {}
	virtual void slot8() {}
	virtual void slot9() {}
	virtual void compact(long size) {}
};

struct s_allocator_globals
{
	c_allocator *allocator;
	long count;
};

extern s_allocator_globals *g_4d87f8;

/* g_4f55f0: a datum array (clumps, 26b230; slot owners, 1a8080) */
struct s_data_array;
extern s_data_array *g_4f55f0;

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

#endif