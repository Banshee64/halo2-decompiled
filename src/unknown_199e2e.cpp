// @flags /O1 /Ob1 /Gr
/* UNKNOWN_199E2E.CPP: leaving both sessions. Retail calls it out of line
   from every caller, so it sits in its own /Ob1 file (lane H) */

#include "cseries.h"

void network_session_manager_leave_session_a(bool close);
void network_session_manager_leave_session_b(bool close);

/* leaves both sessions */
// @retail 0x199e2e
void function_199e2e(bool close)
{
	network_session_manager_leave_session_a(close);
	network_session_manager_leave_session_b(close);
}
