// @flags /O2 /Ob0 /Gr
#include "unknown_058dd0.h"

// @retail 0x58d90
inline bool function_058d90(c_class_58d20 *s)
{
	bool result = false;
	switch (s->state)
	{
	case 2:
		result = true;
		break;
	case 3:
		break;
	case 4:
		result = true;
		break;
	case 5:
		break;
	case 6:
		result = true;
		break;
	case 7:
		result = s->flag7420;
		break;
	case 8:
		result = s->flag7420;
		break;
	}
	return result;
}
