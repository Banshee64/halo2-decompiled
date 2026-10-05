// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_092E00.CPP: a link's list of connection entries (lane D, outside its
   regions: 0x88650 calls it) */

#include "unknown_11c920.h"
#include "unknown_0820f0.h"

// @retail 0x92e00
bool link_remove_entry(s_link *link, long id)
{
	bool result = false;

	for (long i = 0; i < link->entry_count; i++)
	{
		if (link->entries[i].id == id)
		{
			link->entry_count--;
			result = true;
			if (i < link->entry_count)
				link->entries[i] = link->entries[link->entry_count];
			goto done;
		}
	}
done:
	return result;
}
