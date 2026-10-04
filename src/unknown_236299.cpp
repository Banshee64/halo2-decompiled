// @flags /O1 /arch:SSE /Gr
/* UNKNOWN_236299.CPP: playing the user interface sounds */

#include "cseries.h"
#include "globals.h"
#include <math.h>
#include "unknown_030290.h"
#include "unknown_234c64.h"

struct s_interface_sound_reference
{
	long index;
	byte unknown04[4];
};

struct s_interface_sound_globals
{
	byte unknown00[0x94];
	s_interface_sound_reference sounds[13];
};

struct s_sound_tag_view
{
	byte flags;
	byte unknown01[7];
	short first_set;
	char set_count;
};

struct s_sound_set_view
{
	short name;
	byte unknown02[0xa];
};

struct s_sound_globals_view
{
	byte unknown00[0x14];
	long *names;
	byte unknown18[0xc];
	s_sound_set_view *sets;
};

struct s_sound_play
{
	long object_index;
	long tag_index;
	real scale;
	byte *set;
};

struct s_user_interface_globals;
s_user_interface_globals *function_148350(void);
void unicode_string_list_get_string(long tag_index, long string_id, word *buffer);

struct s_user_interface_globals_strings
{
	byte unknown00[0x11c];
	long string_list_index;
};

struct s_bitmap_view
{
	byte unknown00[0x74];
};

struct s_bitmap_group_view
{
	byte unknown00[0x44];
	long bitmap_count;
	s_bitmap_view *bitmaps;
};

void function_12360(s_bitmap_view *bitmap, real priority);
long function_11cae0(void);
byte __stdcall function_219070(long set_index);
/* unknown_189010.cpp */
struct s_sound_label_play;
long function_189760(s_sound_label_play const *play);
long function_1896c0(real scale, long tag_index);

/* a string of the user interface globals' string list */
// @retail 0x23620d
void function_23620d(long string_id, word *buffer)
{
	buffer[0] = 0;
	if (string_id != NONE)
	{
		s_user_interface_globals_strings *globals = (s_user_interface_globals_strings *)function_148350();
		if (globals && globals->string_list_index != NONE)
		{
			unicode_string_list_get_string(globals->string_list_index, string_id, buffer);
		}
	}
}

/* loads every bitmap of a bitmap tag */
// @retail 0x23625d
void function_23625d(long tag_index)
{
	if (tag_index != NONE)
	{
		s_bitmap_group_view *group = (s_bitmap_group_view *)g_4e3b44[tag_index & 0xffff].bytes;
		long count = group->bitmap_count;

		for (long i = 0; i < count; i++)
		{
			function_12360(&group->bitmaps[i], 0.0f);
		}
	}
}

// @retail 0x236299
void function_236299(long sound)
{
	s_interface_sound_globals *globals = (s_interface_sound_globals *)function_148350();
	if (globals)
	{
		long tag_index = sound < 13 ? globals->sounds[sound].index : NONE;
		if (tag_index != NONE)
		{
			s_sound_tag_view *tag = (s_sound_tag_view *)g_4e3b44[tag_index & 0xffff].bytes;
			if (tag->flags & 0x80)
			{
				long name;
				switch (function_11cae0())
				{
				case 0:
					name = 0x7000770;
					break;
				case 1:
					name = 0x8000771;
					break;
				case 2:
					name = 0x6000772;
					break;
				case 3:
					name = 0x6000773;
					break;
				case 4:
					name = 0x7000774;
					break;
				case 5:
					name = 0x7000775;
					break;
				case 6:
					name = 0x6000776;
					break;
				case 7:
					name = 0x7000777;
					break;
				case 8:
					name = 0xa000778;
					break;
				default:
					__assume(0);
				}

				for (long i = 0; i < tag->set_count; i++)
				{
					s_sound_globals_view *sound_globals = (s_sound_globals_view *)g_51ebd4;
					if (sound_globals->names[sound_globals->sets[tag->first_set + i].name] == name)
					{
						byte set[2];
						s_sound_play play;
						set[0] = (byte)i;
						set[1] = function_219070(i);
						play.object_index = NONE;
						play.tag_index = tag_index;
						play.scale = 1.f;
						play.set = set;
						function_189760((s_sound_label_play const *)&play);
						break;
					}
				}
			}
			else
			{
				function_1896c0(1.f, tag_index);
			}
		}
	}
}

struct bitmap_data;
bitmap_data *function_137610(long group_index, short frame_index, short sequence_index);

/* the user interface globals' cursor bitmap */
struct s_user_interface_globals_cursor
{
	byte unknown00[0x114];
	long cursor_bitmap_index;
};

/* a bitmap of the user interface globals' cursor bitmap group */
// @retail 0x236235
bitmap_data *function_236235(short frame_index, short sequence_index)
{
	bitmap_data *result = 0;
	s_user_interface_globals_cursor *globals = (s_user_interface_globals_cursor *)function_148350();

	if (globals && globals->cursor_bitmap_index != NONE)
	{
		result = function_137610(globals->cursor_bitmap_index, frame_index, sequence_index);
	}
	return result;
}

/* projects a point onto the screen: the window manager's depth moves it away
   from the eye, and x and y scale about the bounds' centre */
// @retail 0x2360c3
void function_2360c3(short_rectangle2d const *bounds, real_point3d *point)
{
	real half_width = (real)(bounds->right - bounds->left) * 0.5f;
	real half_height = (real)(bounds->bottom - bounds->top) * 0.5f;
	real depth = g_54d598.color14.green;
	real scale;

	if (fabsf(g_54d598.color14.green) < 0.0001f)
	{
		depth = 1.0f;
	}
	point->z += depth;
	if (point->z >= 0.0f)
	{
		point->z = point->z > 1.0f ? point->z : 1.0f;
	}
	else
	{
		point->z = -1.0f < point->z ? -1.0f : point->z;
	}
	scale = 1.0f / depth;
	scale = 1.0f / (scale * point->z);
	point->x = point->x * scale + half_width;
	point->y = half_height - point->y * scale;
	point->z -= depth;
}

/* projects both corners of a rectangle at one depth */
// @retail 0x23618e
s_float_rect *function_23618e(s_float_rect *rect, real depth, short_rectangle2d const *bounds)
{
	real_point3d p0;
	real_point3d p1;

	p0.x = rect->x0;
	p0.y = rect->y0;
	p0.z = depth;
	p1.x = rect->x1;
	p1.y = rect->y1;
	p1.z = depth;
	function_2360c3(bounds, &p0);
	function_2360c3(bounds, &p1);
	rect->x0 = p0.x;
	rect->y0 = p0.y;
	rect->x1 = p1.x;
	rect->y1 = p1.y;
	return rect;
}
