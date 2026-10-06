#include "unknown_11c920.h"
#include <xtl.h>
#include <xonline.h>
#include <string.h>

// @flags /O1 /Oi /Gr

extern bool g_51ec90;
bool function_24aae8();
DWORD online_get_users(XONLINE_USER *users);

// @retail 0x24ab0c
bool __stdcall function_24ab0c(long controller, XONLINE_USER *user, char const *name)
{
	long const *local_controller = &controller;
	bool result = false;
	if (function_24aae8())
	{
		g_51ec90 = true;
		if (*name)
		{
			XONLINE_USER users[16];
			long count = (word)online_get_users(users);
			for (long i = 0; i < count; ++i)
			{
				XONLINE_USER *entry = &users[i];
				char const *entry_name = "";
				if (entry)
					entry_name = (char const *)entry + 0xc;
				if (strcmp(entry_name, name) == 0)
				{
					*user = *entry;
					result = true;
				}
			}
		}
	}
	return result;
}
