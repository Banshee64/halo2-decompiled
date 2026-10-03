// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_213380.CPP: the end of a content signature calculation. Decompiled
   by lane L: the preferences file (global_preferences.cpp) begins the
   calculation inline and calls this with a register argument. */

#include "cseries.h"
#include <xtl.h>

/* the signature calculation in progress */
HANDLE g_470024 = INVALID_HANDLE_VALUE;

// @retail 0x213380
bool signature_calculate_end(XCALCSIG_SIGNATURE *signature)
{
	long result = XCalculateSignatureEnd(g_470024, signature) == ERROR_SUCCESS;

	g_470024 = INVALID_HANDLE_VALUE;
	return result;
}
