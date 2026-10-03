// @flags /O2 /Gr
/* UNKNOWN_18EC20.CPP: saved game and map loading helpers: pushing the
   physical memory heap, and the kind of a saved game */

#include "cseries.h"
#include "globals.h"
#include "online_tasks.h"

long g_4ed294;

// @retail 0x18ec20
void function_18ec20(bool keep)
{
	g_4ed294 = 1;
	if (!keep)
	{
		long index = g_4e6420;
		g_4e642c[index + 1] = g_4e642c[index];
		g_4e6440[g_4e6420 + 1] = g_4e6440[g_4e6420];
		g_4e6420 = index + 1;
	}
}

struct s_saved_game_header
{
	long version;
	char type;
	byte unknown05[0x12c - 5];
	bool flag12c;
};

// @retail 0x18eeb0
long function_18eeb0(s_saved_game_header const *header)
{
	long result = 1;

	if (header && header->version == 1 && !(header->type >= 4 && header->type <= 5))
	{
		if (online_logon_connected())
		{
			result = (header->flag12c != 0) + 2;
		}
		else
		{
			result = (header->flag12c != 0) + 4;
		}
	}
	return result;
}
