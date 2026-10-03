#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "unknown_19ec40.h"

// @flags /O2 /arch:SSE /Gr

/* UNKNOWN_19CAD0.CPP: the multiplayer globals' marker pairs: for each of the
   16 keys, the first markers of types 7 and 8 that carry it */

// @retail 0x19cb20
bool function_19cb20(s_marker_pair *pair, long key)
{
	long index;

	index = NONE;
	function_19ec40(NULL, 0.0f, 7, NONE, (short)key, 1, &index, 0.0f);
	pair->first = index;
	index = NONE;
	function_19ec40(NULL, 0.0f, 8, NONE, (short)key, 1, &index, 0.0f);
	pair->second = index;
	pair->unknown08 = 0;
	pair->unknown0c = 0;

	return pair->first != NONE && pair->second != NONE;
}

// @retail 0x19cad0
void function_19cad0()
{
	s_mp_globals *globals = g_4e9ae8;
	long key;

	globals->marker_pair_count = 0;
	for (key = 0; key < 16; key++)
	{
		if (function_19cb20(&globals->marker_pairs[globals->marker_pair_count], key))
			globals->marker_pair_count++;
	}
}
