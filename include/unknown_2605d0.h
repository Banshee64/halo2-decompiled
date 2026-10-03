#ifndef __UNKNOWN_2605D0_H__
#define __UNKNOWN_2605D0_H__

#include <string.h>

/* a candidate place function_261d20 lists for an actor (0x78 bytes) */
struct s_261d20_entry
{
	byte unknown00[0xc];
	real_point3d point;
	byte unknown18[0x78 - 0x18];
};

/* what the actor looks for (0x758 bytes; the first word is its kind) */
struct s_2605d0_request
{
	short type;
	byte unknown002[0x15 - 0x2];
	bool unknown015;
	byte unknown016[0x698 - 0x16];
	bool unknown698;
	byte unknown699;
	short unknown69a;
	byte unknown69c[0x758 - 0x69c];
};

/* the defaults of a request (inlined by its users) */
inline void request_initialize(s_2605d0_request *request)
{
	memset(request, 0, sizeof(*request));
	request->unknown015 = true;
	request->unknown69a = 1;
	request->unknown698 = true;
}

short __stdcall function_261d20(long actor_index, s_261d20_entry *entries, long maximum_count, s_2605d0_request const *request);
/* picks the reference the actor should follow, or g_470fa0 */
s_reference function_2605d0(long actor_index, s_2605d0_request const *request, long unknown, long unknown2, byte *scratch, bool *unknown3);

#endif
