#ifndef __UNKNOWN_2605D0_H__
#define __UNKNOWN_2605D0_H__

/* a candidate place function_261d20 lists for an actor (0x78 bytes) */
struct s_261d20_entry
{
	byte unknown00[0xc];
	real_point3d point;
	byte unknown18[0x78 - 0x18];
};

/* what the actor looks for (the first word is its kind) */
struct s_2605d0_request
{
	short type;
};

short __stdcall function_261d20(long actor_index, s_261d20_entry *entries, long maximum_count, s_2605d0_request const *request);
/* picks the reference the actor should follow, or g_470fa0 */
s_reference function_2605d0(long actor_index, s_2605d0_request const *request, long unknown, long unknown2, long unknown3, long unknown4);

#endif
