#include "unknown_11c920.h"

// @flags /O1 /Gr

class c_slots_215541
{
public:
	void function_215541(void *entry);
	long unknown00;
	void *entries[8];
};

// @retail 0x215541
void c_slots_215541::function_215541(void *entry)
{
	for (long i = 0; i < 8; i++)
	{
		if (!entries[i])
		{
			entries[i] = entry;
			break;
		}
	}
}
