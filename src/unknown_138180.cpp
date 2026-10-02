// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_138180.CPP: game session options (validate, compare, initialize) */

#include "cseries.h"
#include <string.h>
#include <stdlib.h>
#include <wchar.h>

#define MAXIMUM_PLAYERS_PER_SESSION 16
#define MAXIMUM_CONTROLLERS 4

struct s_session_machine
{
	byte address[6];
};

struct s_session_player_id
{
	byte index;
	byte unknown1;
	byte unknown2;
	byte unknown3;
	byte unknown4[8];
};

struct s_session_player
{
	byte active;
	byte flag1;
	short index;
	long controller;
	s_session_machine machine;
	s_session_player_id id;
	byte unknown1a[2];
	wchar_t name[32];
	byte unknown5c[16];
	byte unknown6c[0x98 - 0x6c];
	byte flag98;
	byte unknown99[0xe4 - 0x99];
};

struct s_session_options
{
	long type;
	char unknown4;
	byte unknown5;
	short unknown6;
	byte unknown8[0x1c - 0x8];
	char name[0x104];
	short unknown120;
	byte unknown122[0x12a - 0x122];
	short unknown12a;
	byte unknown12c;
	byte unknown12d[0x134 - 0x12d];
	byte unknown134[0x130];
	byte unknown264[4];
	long machine_mask;
	s_session_machine machines[MAXIMUM_PLAYERS_PER_SESSION];
	byte local_machine_valid;
	s_session_machine local_machine;
	byte unknown2d3;
	s_session_player players[MAXIMUM_PLAYERS_PER_SESSION];
};

dword g_4e61cc[MAXIMUM_CONTROLLERS];
byte g_440070[12];

static inline dword string_length_bounded(const char *string, dword maximum_length)
{
	const char *end = string;
	dword length = 0;
	do
	{
		if (!*end++)
		{
			break;
		}
		length++;
	} while (length < maximum_length);
	return length;
}

// @retail 0x138180
bool __stdcall function_138180(const s_session_options *options)
{
	dword i = 0;
	if (options->type < 0 || options->type >= 6)
	{
		return false;
	}
	if (options->unknown4 < 0 || options->unknown4 >= 6)
	{
		return false;
	}
	if (options->unknown6 <= 0 || options->unknown6 > 300)
	{
		return false;
	}
	if (string_length_bounded(options->name, sizeof(options->name)) == 0)
	{
		return false;
	}
	if (options->unknown120 < 0 || options->unknown120 >= 16)
	{
		return false;
	}

	dword mask = options->machine_mask;
	bool ok = (mask & 0xffff0000) == 0;
	for (; i < MAXIMUM_PLAYERS_PER_SESSION; i++)
	{
		if ((1 << i) & mask)
		{
			for (long j = i + 1; j < MAXIMUM_PLAYERS_PER_SESSION; j++)
			{
				if ((1 << j) & mask)
				{
					ok = ok && memcmp(&options->machines[i], &options->machines[j], sizeof(s_session_machine)) != 0;
				}
			}
		}
	}

	dword count = 0;
	for (i = 0; i < MAXIMUM_PLAYERS_PER_SESSION; i++)
	{
		const s_session_player *player = &options->players[i];
		if (player->active)
		{
			bool unique = ok && player->index >= 0 && player->index < MAXIMUM_CONTROLLERS && memcmp(&player->id, g_440070, sizeof(player->id)) != 0;
			count++;
			for (long j = i + 1; j < MAXIMUM_PLAYERS_PER_SESSION; j++)
			{
				if (options->players[j].active)
				{
					unique = unique && memcmp(&player->id, &options->players[j].id, sizeof(player->id)) != 0;
				}
			}
			if (player->flag1)
			{
				ok = unique && memcmp(&player->machine, g_440070, sizeof(player->machine)) == 0;
			}
			else
			{
				long found = NONE;
				for (long m = 0; m < MAXIMUM_PLAYERS_PER_SESSION; m++)
				{
					if (((1 << m) & mask) && memcmp(&options->machines[m], &player->machine, sizeof(s_session_machine)) == 0)
					{
						found = m;
						break;
					}
				}
				ok = unique && found != NONE;
			}
		}
	}

	switch (options->type)
	{
	case 1:
		ok = ok && options->unknown12a >= 0 && options->unknown12a < 4;
		if (options->unknown12c)
		{
			if (!ok)
			{
				return false;
			}
			if (count >= 2 && count <= 2)
			{
				return options->local_machine_valid != 0;
			}
			return false;
		}
		else
		{
			if (!ok || count != 1)
			{
				return false;
			}
			return options->local_machine_valid != 0;

		}
	case 2:
		if (!ok)
		{
			return false;
		}
		if (count >= 1 && count <= 16)
		{
			return options->local_machine_valid != 0;
		}
		return false;
	case 3:
		if (!ok)
		{
			return false;
		}
		if (count >= 1)
		{
			return options->local_machine_valid != 0;

		}
		return false;
	case 4:
		return ok;
	case 5:
		return ok;
	default:
		return false;
	}
}

// @retail 0x1384a0
bool __stdcall function_1384a0(const s_session_options *a, const s_session_options *b)
{
	if (strcmp(a->name, b->name) == 0 && a->type == b->type && a->unknown6 == b->unknown6)
	{
		dword masks[2];
		for (long k = 0; k < 2; k++)
		{
			const s_session_options *options = k == 0 ? a : b;
			masks[k] = 0;
			if (options->local_machine_valid)
			{
				for (long j = 0; j < MAXIMUM_PLAYERS_PER_SESSION; j++)
				{
					const s_session_player *player = &options->players[j];
					if (player->active && !player->flag1 && memcmp(&player->machine, &options->local_machine, sizeof(s_session_machine)) == 0)
					{
						masks[k] |= 1 << (byte)player->index;
					}
				}
			}
		}
		if (masks[0] == masks[1])
		{
			bool equal = true;
			switch (a->type)
			{
			case 1:
				equal = a->unknown12c == b->unknown12c && a->unknown12a == b->unknown12a;
				break;
			case 2:
				equal = memcmp(a->unknown134, b->unknown134, sizeof(a->unknown134)) == 0;
				break;
			}
			return equal;
		}
	}
	return false;
}

// @retail 0x138600
int __cdecl sort_controllers_ascending(const void *a, const void *b)
{
	short x = *(const short *)a;
	short y = *(const short *)b;
	if (x == NONE && y != NONE)
	{
		return 1;
	}
	if (y == NONE && x != NONE)
	{
		return -1;
	}
	if (x > y)
	{
		return 1;
	}
	return x < y ? -1 : 0;
}

static inline long controller_index_next(long index)
{
	long next = NONE;
	if (index >= 0 && index < MAXIMUM_CONTROLLERS - 1)
	{
		next = index + 1;
	}
	return next;
}

// @retail 0x138640
void __stdcall function_138640(long count, s_session_options *options)
{
	options->machine_mask = 1;
	memset(&options->machines[0], 0, sizeof(s_session_machine));
	options->local_machine = options->machines[0];
	options->local_machine_valid = 1;

	long order[MAXIMUM_CONTROLLERS];
	bool used[MAXIMUM_CONTROLLERS];
	long index;
	for (index = 0; index != NONE; index = controller_index_next(index))
	{
		used[index] = false;
		order[index] = NONE;
	}

	for (long i = 0; i < count; i++)
	{
		long controller = NONE;
		for (index = 0; index != NONE; index = controller_index_next(index))
		{
			if (g_4e61cc[(short)index] && !used[index])
			{
				controller = index;
				break;
			}
		}
		if (controller == NONE)
		{
			controller = 0;
		}
		if (used[controller])
		{
			for (index = 0; index != NONE; index = controller_index_next(index))
			{
				if (!used[index])
				{
					controller = index;
					break;
				}
			}
		}
		order[i] = controller;
		used[controller] = true;
	}
	qsort(order, MAXIMUM_CONTROLLERS, sizeof(long), sort_controllers_ascending);

	memset(options->players, 0, sizeof(options->players));
	for (long i = 0; i < count; i++)
	{
		s_session_player *player = &options->players[i];
		player->active = 1;
		player->flag1 = 0;
		player->machine = options->local_machine;
		player->controller = order[i];
		player->index = (short)i;
		memset(&player->id, 0, 12);
		player->id.index = (byte)i;
		player->id.unknown1 = 0xff;
		player->id.unknown2 = 0xff;
		player->id.unknown3 = 0xff;
		wcsncpy(player->name, L"", 0x1f);
		player->name[0x1f] = 0;
		memset(player->unknown5c, 0, sizeof(player->unknown5c));
		if (options->type == 2)
		{
			player->flag98 = (byte)i;
		}
		else
		{
			player->flag98 = 1;
		}
	}
}
