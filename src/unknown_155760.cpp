// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_155760.CPP: entry classification */

#include "cseries.h"
#include "globals.h"

struct s_entry_155760
{
	byte unknown00[4];
	real value;
	byte unknown08[4];
	void *proc;
	byte unknown10[0x140 - 0x10];
};

s_entry_155760 g_4e8c44[1];
byte g_51ec10;

void function_23c110(void);
void function_23cbb0(void);
void function_16c840(void);
void function_23d090(void);

// @retail 0x155760
long function_155760(long index)
{
	s_entry_155760 *entry = g_4e8c44 + index;
	long result = 3;
	if (entry->proc == function_23c110)
	{
		if (entry->value == g_45dbd8)
		{
			result = 0;
		}
	}
	else if (entry->proc == function_23cbb0)
	{
		result = 1;
	}
	else if (entry->proc == function_16c840 || (entry->proc == function_23d090 && g_51ec10))
	{
		result = 2;
	}
	return result;
}
