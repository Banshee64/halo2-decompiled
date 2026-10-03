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
