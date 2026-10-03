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
	real value10;
	real value14;
	byte unknown18[0x58 - 0x18];
	word word58;
	byte unknown5a[0x108 - 0x5a];
	byte flag108;
	byte unknown109[3];
	real value10c;
	byte unknown110[0x140 - 0x110];
};

s_entry_155760 g_4e8c44[1];
byte g_51ec10;
long g_4e8c3c;

void function_23c110(void);
void function_23cbb0(void);
void function_16c840(void);
void function_23d090(void);
void function_23de50(void);

// @retail 0x155710
void function_155710(long index)
{
	if (g_4e8c3c == 0)
	{
		g_4e8c44[index].word58 = 0;
		g_4e8c44[index].value10 = 0.0f;
		g_4e8c44[index].value14 = 0.0f;
		g_4e8c44[index].proc = function_23c110;
		g_4e8c44[index].value10c = 1.0f;
		g_4e8c44[index].flag108 = 0;
	}
}

// @retail 0x155d60
bool function_155d60(long index)
{
	return g_4e8c44[index].proc == function_23de50;
}

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
