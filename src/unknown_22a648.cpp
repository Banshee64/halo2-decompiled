// @flags /O1 /arch:SSE /Gr
/* UNKNOWN_22A648.CPP: hud drawing (built for size; the file continues past
   0x22c000) */

#include "cseries.h"

// @retail 0x22a85b
long function_22a85b(
	char type)
{
	long result = NONE;

	switch (type)
	{
	case 0:
		result = 6;
		break;
	case 1:
		result = 8;
		break;
	}
	return result;
}
