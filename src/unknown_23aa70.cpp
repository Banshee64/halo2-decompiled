// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_23AA70.CPP: a pass over the structure's clusters */

#include "unknown_11c920.h"
#include "globals.h"

/* the structure's clusters (0xb0 bytes each), as this file reads them */
struct s_cluster_23aa
{
	byte unknown00[0x7c];
	short value7c;
	word count7e;
	byte unknown80[0xb0 - 0x80];
};

struct s_structure_23aa
{
	byte unknown00[0x9c];
	long cluster_count;
	s_cluster_23aa *clusters;
	byte unknowna4[0xf4 - 0xa4];
	long valuef4;
};

void function_17d710(short cluster_index);

/* calls 0x17d710 on every cluster that has a value7c and a count */
// @retail 0x23aa70
void function_23aa70(void)
{
	s_structure_23aa *structure = (s_structure_23aa *)g_4e0348;

	if (structure->valuef4)
	{
		for (short i = 0; i < structure->cluster_count; i++)
		{
			s_cluster_23aa *cluster = &structure->clusters[i];

			if (cluster->value7c != NONE && cluster->count7e > 0)
			{
				function_17d710(i);
			}
		}
	}
}
