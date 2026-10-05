// @flags /O1 /arch:SSE /Gr
/* UNKNOWN_236299.CPP: playing the user interface sounds */

#include "unknown_11c920.h"
#include "globals.h"
#include <math.h>
#include "unknown_030290.h"
#include "unknown_234c64.h"
#include "unknown_24b5bc.h"
#include "unknown_19b510.h"
#include <string.h>

#pragma intrinsic(memset)

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

struct s_type_954545;
s_type_954545 *function_148350(void);
void function_1a0180(long tag_index, long string_handle, word *buffer);

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
void function_23620d(long string_handle, word *buffer)
{
	buffer[0] = 0;
	if (string_handle != NONE)
	{
		s_user_interface_globals_strings *globals = (s_user_interface_globals_strings *)function_148350();
		if (globals && globals->string_list_index != NONE)
		{
			function_1a0180(globals->string_list_index, string_handle, buffer);
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

struct s_type_7ba8e9;
s_type_7ba8e9 *function_137610(long group_index, short frame_index, short sequence_index);

/* the user interface globals' cursor bitmap */
struct s_user_interface_globals_cursor
{
	byte unknown00[0x114];
	long cursor_bitmap_index;
};

/* a bitmap of the user interface globals' cursor bitmap group */
// @retail 0x236235
s_type_7ba8e9 *function_236235(short frame_index, short sequence_index)
{
	s_type_7ba8e9 *result = 0;
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
void function_2360c3(short_rectangle2d const *bounds, point3f *point)
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
	point3f p0;
	point3f p1;

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

/* how far a window's widgets slide in from: a quarter of the screen comes
   from its own corner, half of it from its own edge, the full screen from
   nowhere */
// @retail 0x2363d4
void function_2363d4(short_rectangle2d const *bounds, short *x, short *y)
{
	short bottom = bounds->bottom;
	short left = bounds->left;
	short top = bounds->top;
	short right = bounds->right;
	short width = right - left;
	short height = bottom - top;

	if (width == 320)
	{
		if (top == 0)
		{
			if (left == 0)
			{
				*x = 20;
				*y = -16;
			}
			else
			{
				*x = -20;
				*y = -16;
			}
		}
		else
		{
			if (left == 0)
			{
				*x = 20;
				*y = 16;
			}
			else
			{
				*x = -20;
				*y = 16;
			}
		}
	}
	else
	{
		*x = 0;
		if (height == 240)
		{
			if (top == 0)
			{
				*y = -16;
			}
			else
			{
				*y = 16;
			}
		}
		else
		{
			*y = 0;
		}
	}
}

/* a machine address */
struct s_machine_address
{
	byte bytes[6];
};

struct s_profile_record;
void function_18fd94(long index, dword *xuid, s_profile_record *record);

/* a signed in local player as the session is told of it (0xe4 bytes) */
struct s_local_player_entry
{
	bool valid;
	byte unknown01;
	short value02;
	long index;
	s_machine_address address;
	byte xuid[0x1c - 0xe];
	byte record[0xe4 - 0x1c];
};

/* a player slot's word at +4 */
struct s_player_slot_view_04
{
	byte unknown00[4];
	short value04;
	byte unknown06[0xc70 - 6];
};

/* lists the signed in local players with this machine's address */
// @retail 0x23654b
void __stdcall function_23654b(void *c, void *a, void *b)
{
	s_local_player_entry *entries = (s_local_player_entry *)c;
	s_machine_address *address = (s_machine_address *)a;
	long *count = (long *)b;

	*address = *(s_machine_address *)g_4cf7cc;
	long entry_count = 0;
	memset(entries, 0, 16 * sizeof(s_local_player_entry));
	s_local_player_entry *next_entry = entries;
	long index = 0;
	do
	{
		if (TEST_FIELD_BIT(((s_player_slot_sign_in_view *)g_54e8e0)[index].signed_in))
		{
			entry_count++;
			s_local_player_entry *entry = next_entry++;

			entry->valid = true;
			entry->address = *address;
			entry->value02 = ((s_player_slot_view_04 *)g_54e8e0)[index].value04;
			entry->index = index;
			function_18fd94(index, (dword *)entry->xuid, (s_profile_record *)entry->record);
		}
		index = next_controller_index(index);
	}
	while (index != NONE);
	*count = entry_count;
}

/* the user interface globals' dialogs: groups of dialogs that share their
   string list, defaults and flags */
struct s_dialog_entry
{
	long dialog_id;
	byte flags;
	byte unknown05;
	char choices;
	byte unknown07;
	long title;
	long message;
	long first_choice;
	long second_choice;
};

struct s_dialog_group
{
	byte unknown00[4];
	byte flags;
	byte unknown05;
	char choices;
	byte unknown07[5];
	long string_list_index;
	long title;
	long message;
	long first_choice;
	long second_choice;
	long dialog_count;
	s_dialog_entry *dialogs;
};

struct s_dialog_globals_view
{
	byte unknown00[0x88];
	long group_count;
	s_dialog_group *groups;
};

/* the definition of a dialog: its group's defaults overridden by its own
   (the definition of dialog 1 for an unknown dialog) */
// @retail 0x23661f
void function_23661f(s_dialog_definition *definition, long dialog_id)
{
	bool found = false;
	s_dialog_globals_view *globals = (s_dialog_globals_view *)function_148350();

	memset(definition, 0, sizeof(*definition));
	definition->string_list_index = NONE;
	definition->dialog_id = dialog_id;
	definition->title = 0;
	definition->message = 0;
	definition->screen_id = 7;
	if (globals)
	{
		for (long i = 0; !found && i < globals->group_count; i++)
		{
			s_dialog_group *group = &globals->groups[i];

			for (long j = 0; !found && j < group->dialog_count; j++)
			{
				s_dialog_entry *entry = &group->dialogs[j];

				if (entry->dialog_id == dialog_id)
				{
					found = true;
					if (group->string_list_index != NONE)
					{
						definition->string_list_index = group->string_list_index;
					}
					if (entry->title)
					{
						definition->title = entry->title;
					}
					else if (group->title)
					{
						definition->title = group->title;
					}
					if (entry->message)
					{
						definition->message = entry->message;
					}
					else if (group->message)
					{
						definition->message = group->message;
					}
					if (entry->first_choice)
					{
						definition->first_choice = entry->first_choice;
					}
					else if (group->first_choice)
					{
						definition->first_choice = group->first_choice;
					}
					if (entry->second_choice)
					{
						definition->second_choice = entry->second_choice;
					}
					else if (group->second_choice)
					{
						definition->second_choice = group->second_choice;
					}
					definition->choices = group->choices;
					if (entry->choices)
					{
						definition->choices = entry->choices;
					}
					if ((group->flags & 1) || (entry->flags & 1))
					{
						definition->screen_id = 0xf0;
					}
					else
					{
						definition->screen_id = 7;
					}
				}
			}
		}
	}
	if (!found && dialog_id != 1)
	{
		function_23661f(definition, 1);
	}
}
