// @flags /O2 /Gr
/* UNKNOWN_122CF0.CPP: the comparison of two longs that bsearch_elements uses
   to find scenario object names (outside lane J's region; decompiled for
   src/unknown_0aa8e0.cpp) */

#include "cseries.h"

// @retail 0x122cf0
long __stdcall long_compare(const void *a, const void *b, const void *context)
{
	return *(long const *)a - *(long const *)b;
}
