/* UNKNOWN_19C1D0.H: the lookup tables of 19c1d0 */

#ifndef UNKNOWN_19C1D0_H
#define UNKNOWN_19C1D0_H

struct s_entry_a
{
	long key0;
	long key1;
	char name[0x100];
};

s_entry_a *function_19c270(long key0, long key1);
s_entry_a *function_19c320(const char *name);
long function_19c440(long key0, long key1);

#endif
