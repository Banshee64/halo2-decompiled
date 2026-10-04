/* ONLINE_PRESENCE.H: the presence an account publishes (unknown_06b590.cpp)
   and what it is built from */

#ifndef ONLINE_PRESENCE_H
#define ONLINE_PRESENCE_H

#include "unknown_11c920.h"

/* the presence an account publishes, packed into a dword */
struct s_online_presence_source
{
	long state;
	long unknown04;
	long unknown08;
	short minutes_a;
	short minutes_b;
};

union s_online_presence
{
	struct
	{
		dword magic : 16;
		dword time : 8;
		dword state : 8;
	};
	struct
	{
		dword unused : 28;
		dword unknown04 : 2;
		dword unknown08 : 2;
	};
};

void online_presence_build(s_online_presence *presence, const s_online_presence_source *source);

#endif
