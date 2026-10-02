/* GLOBALS.H: globals shared by more than one source file (defined in
src/globals.cpp) */

#ifndef GLOBALS_H
#define GLOBALS_H

#include "real_math.h"

/* the game time globals; unknown02 is a scale read by firing position code */
struct s_game_time_globals
{
	byte unknown00[2];
	short unknown02;
	real rate;
	long game_time;
};

extern s_game_time_globals *g_510c54;

/* g_4e0300: the object header data. Offset 0x44 is the array of object
   headers (12 bytes each: 8 unknown bytes, then the object pointer); batches
   2-3 (s_obj_array) and 2-6 (s_object_header) view the same array */
struct s_obj_array;
struct s_object_header;
struct s_object_header_data
{
	byte unknown00[0x44];
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
struct s_tag_instance
{
	byte unknown00[8];
	union
	{
		s_animation_tag_data *data;
		bitmap_group *group;
	};
	byte unknown0c[4];
};

extern s_tag_instance *g_4e3b44;

/* a default point and vector, shared by the camera and animation code
   (a vector in 2-7, a point in 2-3: three reals either way) */
extern real_point3d *g_468788;

#endif
