// stubs for game functions not decompiled yet, called by damage.cpp
#include "cseries.h"

/* compares two string ids for bsearch_elements */
// @stub 0x122cf0
long __stdcall function_122cf0(const void *a, const void *b, const void *context) { return *(long *)a - *(long *)b; }
/* the difficulty multiplier of a team (kind 1 body, 2 shield); retail passes
   both arguments in registers and returns in xmm0 */
// @stub 0x1e9720
real function_1e9720(long kind, short team) { return 1.0f; }
