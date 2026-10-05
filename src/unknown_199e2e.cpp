// @flags /O1 /Ob1 /Gr
/* UNKNOWN_199E2E.CPP: leaving both sessions, and hosting a new one. Retail
   calls both out of line from every caller (LTCG would otherwise inline
   0x199df9 into callers that pass constants), so they sit in their own /Ob1
   file (lane H) */

#include "unknown_11c920.h"
#include <xtl.h>

void network_session_manager_leave_session_a(bool close);
void network_session_manager_leave_session_b(bool close);
bool __stdcall network_session_manager_host_session(long mode, const XNKID *kid, const XNKEY *key);
bool network_session_manager_host_offline(void);
bool network_session_manager_host_online(void);
void function_199e2e(bool close);

/* leaves the sessions and hosts a new one */
// @retail 0x199df9
bool function_199df9(bool offline, bool system_link)
{
	function_199e2e(true);
	if (offline && !system_link)
		return network_session_manager_host_offline();
	else if (system_link)
		return network_session_manager_host_session(2, NULL, NULL);
	else
		return network_session_manager_host_online();
}

/* leaves both sessions */
// @retail 0x199e2e
void function_199e2e(bool close)
{
	network_session_manager_leave_session_a(close);
	network_session_manager_leave_session_b(close);
}
