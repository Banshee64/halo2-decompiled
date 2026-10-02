// @flags /O2 /Gr
/* UNKNOWN_147090.CPP */

#include "cseries.h"

struct s_147090_list
{
	byte unknown00[0x18];
	long size;
	byte *data;
	long count;
};

s_147090_list *g_47989c;
s_147090_list *g_4798a0;

static long list_total(s_147090_list *list)
{
	long total = 0;
	long count = list->count;

	if (count > 0)
	{
		long *p = (long *)(list->data + list->size) - 1;
		do
		{
			total += *p;
			p -= 2;
			count--;
		}
		while (count);
	}
	return total;
}

// @retail 0x147090
long function_147090(void)
{
	long a = 0;

	if (g_47989c)
		a = list_total(g_47989c);

	return (g_4798a0 ? list_total(g_4798a0) : 0) + a;
}
