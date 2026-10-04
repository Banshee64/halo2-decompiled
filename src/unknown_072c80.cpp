// @flags /O2 /Gr
/* UNKNOWN_072C80.CPP: the two searches of the session manager (0x477088):
   each holds two online tasks; the first one also knows the matchmaking
   state (decompiled by lane D for the session manager, outside its region) */

#include "cseries.h"
#include "unknown_058dd0.h"

void function_6b640(long task_index);

struct s_session_search
{
	long unknown00;
	s_session_owner *owner;
	bool active;
	bool unknown09;
	bool unknown0a;
	byte unknown0b;
	long task0c;
	long value10;
	long task14;
	long value18;
	long value1c;
	long value20;
	byte unknown24[8];
	long type;
	bool unknown30;
	byte unknown31[0x9c - 0x31];
};

struct s_session_searches
{
	bool initialized;
	s_session_search search_a;
	c_session_state_matchmaking *matchmaking;
	s_session_search search_b;
};

s_session_searches g_477088;

static inline void session_search_initialize(s_session_search *search, s_session_owner *owner, long type)
{
	search->owner = owner;
	search->unknown0a = false;
	search->task0c = NONE;
	search->value10 = 0;
	search->task14 = NONE;
	search->value18 = 0;
	search->value1c = NONE;
	search->value20 = 0;
	search->type = type;
	search->unknown30 = false;
	search->unknown09 = false;
	search->active = true;
}

static inline void session_search_dispose(s_session_search *search)
{
	if (search->task0c != NONE)
	{
		function_6b640(search->task0c);
		search->task0c = NONE;
	}
	if (search->task14 != NONE)
	{
		function_6b640(search->task14);
		search->task14 = NONE;
	}
	search->active = false;
}

// @retail 0x72c80
void session_searches_initialize(s_session_owner *owner, c_session_state_matchmaking *matchmaking)
{
	session_search_initialize(&g_477088.search_a, owner, 4);
	g_477088.matchmaking = matchmaking;
	session_search_initialize(&g_477088.search_b, owner, 3);
	g_477088.initialized = true;
}

// @retail 0x72d30
void session_searches_dispose(void)
{
	session_search_dispose(&g_477088.search_a);
	session_search_dispose(&g_477088.search_b);
	g_477088.initialized = false;
}
