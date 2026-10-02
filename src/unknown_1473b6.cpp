// @flags /O1 /Gr
/* UNKNOWN_1473B6.CPP */

#include "cseries.h"

struct s_1473b6
{
	byte unknown00[8];
	long l8;
	long lc;
};

// @retail 0x1473b6
bool function_1473b6(s_1473b6 *p)
{
	return p->l8 != 0 || p->lc != 0;
}