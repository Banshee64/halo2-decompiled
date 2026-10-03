// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "globals.h"

/* the globals of g_4e034c, as 1e96a0 reads them: the table of reals at +0xfc
   has four columns per row */
struct s_1e96a0_globals
{
	byte unknown00[0xf8];
	void *table_valid;
	real *table;
};

// @retail 0x1e96a0
real function_1e96a0(short column, short row)
{
	s_1e96a0_globals *globals = (s_1e96a0_globals *)g_4e034c;
	real result = 1.0f;

	if (globals && globals->table_valid && globals->table)
	{
		if (column < 0)
			column = 0;
		else if (column > 3)
			column = 3;
		result = globals->table[row * 4 + column];
	}
	return result;
}
