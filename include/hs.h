/* HS.H: the scripting system's value type */

#ifndef HS_H
#define HS_H

#include "cseries.h"

union script_value
{
	dword d;
	real r;
	short s;
	word w;
	byte b;
};

#endif
