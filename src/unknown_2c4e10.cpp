/* UNKNOWN_2C4E10.CPP: reading a recorded unit control from a recording, by
   the field lists of each recording version */

#include "unknown_11c920.h"
#include <string.h>

// @flags /O2 /Gr

/* how a type's bytes are swapped: its name, size and swap codes */
struct s_byte_swap_definition
{
	char const *name;
	long size;
	long const *codes;
	long signature;
	long unknown10;
};

/* a field of the recorded control: its size in the recording and its offset
   in the control (NONE when the field is skipped) */
struct s_recorded_field
{
	s_byte_swap_definition *definition;
	long size;
	long offset;
};

long const g_475b00[4] = { -100, 1, 1, -101 };
s_byte_swap_definition g_475b10 = { "byte", 1, g_475b00, 'bysw', 0 };
long const g_475b24[4] = { -100, 1, 2, -101 };
s_byte_swap_definition g_475b34 = { "word", 2, g_475b24, 'bysw', 0 };
long const g_475b48[4] = { -100, 1, 4, -101 };
s_byte_swap_definition g_475b58 = { "long", 4, g_475b48, 'bysw', 0 };
long const g_475b6c[2] = { -4, -4 };
s_byte_swap_definition g_475b74 = { "real_vector2d", 8, g_475b6c, 'bysw', 0 };
long const g_475b88[3] = { -4, -4, -4 };
s_byte_swap_definition g_475b94 = { "real_vector3d", 12, g_475b88, 'bysw', 0 };

/* the fields each recording version adds */
s_recorded_field g_475ba8[12] =
{
	{ &g_475b10, 4, 0 },
	{ &g_475b10, 2, 4 },
	{ &g_475b34, 4, 0x10 },
	{ &g_475b34, 2, 6 },
	{ &g_475b10, 1, 8 },
	{ &g_475b10, 1, 9 },
	{ &g_475b34, 2, NONE },
	{ &g_475b74, 8, 0x14 },
	{ &g_475b94, 12, 0x28 },
	{ &g_475b94, 12, 0x34 },
	{ &g_475b94, 12, 0x40 },
	{ 0, NONE, NONE },
};

s_recorded_field g_475c38[2] =
{
	{ &g_475b58, 4, 0x20 },
	{ 0, NONE, NONE },
};

s_recorded_field g_475c50[2] =
{
	{ &g_475b34, 2, 0xa },
	{ 0, NONE, NONE },
};

s_recorded_field g_475c68[2] =
{
	{ &g_475b34, 2, 0xc },
	{ 0, NONE, NONE },
};

s_recorded_field *g_475c80[4] = { g_475ba8, g_475c38, g_475c50, g_475c68 };

/* the recorded control (0x7c bytes), as the reader fills it */
struct playback_unit_control_view
{
	byte unknown00[0xc];
	short value0c;
	byte unknown0e[0x7c - 0xe];
};

// @retail 0x2c4e10
void function_2c4e10(playback_unit_control_view *control, byte const **cursor, byte version)
{
	memset(control, 0, sizeof(*control));
	control->value0c = NONE;
	for (short v = 0; v < (version > 1 ? version : 1); v++)
	{
		for (s_recorded_field const *field = g_475c80[v]; field->size != NONE; field++)
		{
			if (field->offset != NONE)
			{
				memcpy((byte *)control + field->offset, *cursor, field->size);
			}
			*cursor += field->size;
		}
	}
}
