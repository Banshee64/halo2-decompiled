#ifndef __UNKNOWN_2729B0_H__
#define __UNKNOWN_2729B0_H__

/* a starting location of the scenario (0x7c bytes) */
struct s_2729b0_starting_location
{
	byte unknown00[0x20];
	short variant_index;
	byte unknown22[0x7c - 0x22];
};

/* a character variant (function_272a00): the default index at +0x24, then
   for each block of the character tag an index plus one, or 0 (or less) for
   the default */
struct s_character_variant
{
	byte unknown00[0x24];
	short default_index;
	byte unknown26[0x28 - 0x26];
	char block_indices[11];
};

/* the character variant of an actor's squad or starting location, or 0 */
void *function_2729b0(s_2729b0_starting_location *location);
void *function_272a00(long actor_index);
short function_272ab0(long actor_index);
short function_272ad0(s_character_variant const *variant);

#endif
