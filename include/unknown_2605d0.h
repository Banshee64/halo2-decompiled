#ifndef __UNKNOWN_2605D0_H__
#define __UNKNOWN_2605D0_H__

#include <string.h>

/* a candidate place function_261d20 lists for an actor (0x78 bytes) */
struct s_261d20_entry
{
	byte unknown00[0xc];
	real_point3d point;
	byte unknown18[0x30 - 0x18];
	real distance_squared;
	byte unknown34[0x78 - 0x34];
};

/* what the actor looks for (0x758 bytes; the first word is its kind) */
struct s_2605d0_request
{
	short type;
	byte unknown002[0x8 - 0x2];
	real unknown008;
	real unknown00c;
	real unknown010;
	byte unknown014;
	bool unknown015;
	byte unknown016[0x18 - 0x16];
	/* retail keeps requests 8 byte aligned on the stack */
	__int64 unknown018;
	byte unknown020[0x56 - 0x20];
	bool unknown056;
	bool unknown057;
	byte unknown058[0x5b - 0x58];
	bool unknown05b;
	byte unknown05c[0x698 - 0x5c];
	bool unknown698;
	byte unknown699;
	short unknown69a;
	bool unknown69c;
	byte unknown69d[0x758 - 0x69d];
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
