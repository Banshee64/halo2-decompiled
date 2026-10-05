// @flags /O1 /Ob2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "unknown_19b516.h"

// @retail 0x190262
long function_190262(long value)
{
	long result;

	switch (value)
	{
	case NONE:
		result = 0;
		break;
	case 0:
		result = 1;
		break;
	case 1:
		result = 2;
		break;
	case 2:
		result = 3;
		break;
	default:
		result = NONE;
		break;
	}
	return result;
}

