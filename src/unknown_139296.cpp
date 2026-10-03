// @flags /O1 /Oi /arch:SSE /Gr
/* UNKNOWN_139296.CPP: per-player interface state (built for size) */

#include "cseries.h"
#include "globals.h"
#include <string.h>

struct s_player_view
{
	byte unknown00[0x28];
	short user_index;
};

struct s_user_interface_state
{
	real value00;
	byte unknown04[0x6c - 0x04];
};

struct s_510c4c;
extern s_510c4c *g_510c4c;

real g_4e69c0[4];

struct s_4e69d0
{
	long indices[7];
	byte unknown1c[0x270 - 0x1c];
};

s_4e69d0 g_4e69d0[4];

// @retail 0x139296
void function_139296(long user_index, real value)
{
	if (user_index >= 0 && user_index < 4)
	{
		g_4e69c0[user_index] = value;
	}
}

// @retail 0x1392a9
real function_1392a9(long user_index)
{
	if (user_index >= 0 && user_index < 4)
	{
		return g_4e69c0[user_index];
	}
	return 1.0f;
}

// @retail 0x13a6e8
void function_13a6e8(long player_index, real amount)
{
	s_player_view *player = (s_player_view *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c);
	long user_index = player->user_index;
	if (user_index != NONE)
	{
		((s_user_interface_state *)((byte *)g_510c4c + user_index * sizeof(s_user_interface_state)))->value00 -= amount;
	}
}

// @retail 0x13ac42
void function_13ac42(long index)
{
	memset(&g_4e69d0[index], 0, sizeof(s_4e69d0));
	g_4e69d0[index].indices[0] = NONE;
	g_4e69d0[index].indices[1] = NONE;
	g_4e69d0[index].indices[2] = NONE;
	g_4e69d0[index].indices[3] = NONE;
	g_4e69d0[index].indices[4] = NONE;
	g_4e69d0[index].indices[5] = NONE;
	g_4e69d0[index].indices[6] = NONE;
}

struct s_name_buffer
{
	wchar_t name[256];
};

void function_08cc20(s_name_buffer *buffer, const wchar_t *name);

struct s_510c4c_view
{
	byte unknown000[0x1bc];
	byte *strings;
	byte unknown1c0[0x1d2 - 0x1c0];
	bool flag1d2;
};

// @retail 0x13934d
void function_13934d(s_name_buffer *buffer, long string_id)
{
	byte *strings = ((s_510c4c_view *)g_510c4c)->strings;
	if (strings)
	{
		wchar_t const *name;
		switch (string_id)
		{
		case 0xe42d:
			name = (wchar_t const *)(strings + 0x1bc);
			break;
		case 0xe42e:
			name = (wchar_t const *)(strings + 0x208);
			break;
		case 0xe42f:
			name = (wchar_t const *)(strings + 0x134);
			break;
		case 0xe430:
			name = (wchar_t const *)(strings + 0x176);
			break;
		default:
			name = NULL;
			break;
		}
		if (name)
		{
			function_08cc20(buffer, name);
			return;
		}
	}
	buffer->name[0] = 0;
}

long function_155760(long index);
bool function_155d60(long index);
extern long g_4b9ed8;
bool g_4f55e2;

// @retail 0x13939b
bool function_13939b()
{
	long index = g_4b9ed8;
	return (function_155760(index) != 3 || function_155d60(index)) &&
		function_155760(index) != 2 &&
		((s_510c4c_view *)g_510c4c)->flag1d2 &&
		!(g_4e6948->state == 1 ? g_4f55e2 : false);
}

short g_4b9dd4;
short g_4b9dd6;
extern short g_4b9dd0;
extern short g_4b9dd2;
byte function_016a90();

// @retail 0x13a690
long function_13a690(long mode)
{
	long result = 0;
	short width = g_4b9dd6 - g_4b9dd2;
	short top = g_4b9dd0;
	short bottom = g_4b9dd4;

	if (width < 640 || (short)(bottom - top) < 480)
	{
		if (width < 640 && (short)(bottom - top) < 480)
		{
			result = 2;
		}
		else
		{
			result = 1;
			if (function_016a90() && mode == 3)
			{
				result = 2;
			}
		}
	}

	return result;
}