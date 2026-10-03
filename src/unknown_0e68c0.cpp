// @flags /O2 /Ob1 /Gr
/* UNKNOWN_0E68C0.CPP: a unit request with no arguments (an outside function
   lane I's handlers call) */

#include "cseries.h"
#include "slot_handler.h"
#include <string.h>

// @retail 0xe68c0
bool function_e68c0(long type, long unit_index)
{
	s_unit_request request;

	memset(&request, 0, sizeof(request));

	request.type = type;
	return function_e6900(unit_index, &request);
}
