// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_092E00.CPP: a link's list of connection entries (lane D, outside its
   regions: 0x88650 calls it) */

#include "cseries.h"
#include "network_connection.h"

// @retail 0x92e00
bool link_remove_entry(s_link *link, long id)
{
	long count = link->entry_count;
	bool result = false;

	for (long i = 0; i < link->entry_count; i++)
	{
		if (link->entries[i].id == id)
		{
			link->entry_count = count - 1;
			result = true;
			if (i < link->entry_count)
				link->entries[i] = link->entries[link->entry_count];
			break;
		}
	}
	return result;
}
