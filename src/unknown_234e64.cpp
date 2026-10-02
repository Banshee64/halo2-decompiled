// @flags /O1 /Gr
/* UNKNOWN_234E64.CPP: a virtual clear method shared by three vtables (0x4599b8..) */

#include "cseries.h"

class c_clearable
{
public:
	virtual void clear();

	byte unknown04[4];
	long value08;
	long value0c;
	long values10[8];
	long value30;
	long value34;
};

// @retail 0x234e64
void c_clearable::clear()
{
	value08 = 0;
	value0c = 0;
	for (long i = 0; i < 8; i++)
	{
		values10[i] = 0;
	}
	value30 = 0;
	value34 = 0;
}
