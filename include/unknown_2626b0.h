#ifndef __UNKNOWN_2626B0_H__
#define __UNKNOWN_2626B0_H__

/* what a reference names (0x20 bytes) */
struct s_262b40_result
{
	byte unknown00[0xe];
	word flags;
	short unknown10;
	byte unknown12[0x20 - 0x12];
};

s_262b40_result *function_262b40(s_reference reference);

/* makes reference the one the actor follows (actor +0x418), remembering the
   one it gave up; returns the one it follows afterwards */
s_reference function_2626b0(long actor_index, s_reference reference, long other_actor_index, byte *scratch, bool unknown2, bool unknown3);
/* adds reference to the actor's history (actor +0x400) unless it is there */
void function_262800(long actor_index, s_reference reference, bool unknown);

#endif
