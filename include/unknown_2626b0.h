#ifndef __UNKNOWN_2626B0_H__
#define __UNKNOWN_2626B0_H__

/* makes reference the one the actor follows (actor +0x418), remembering the
   one it gave up; returns the one it follows afterwards */
s_reference function_2626b0(long actor_index, s_reference reference, long other_actor_index, long unknown, bool unknown2, bool unknown3);
/* adds reference to the actor's history (actor +0x400) unless it is there */
void function_262800(long actor_index, s_reference reference, bool unknown);

#endif
