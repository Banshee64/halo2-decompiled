/* HAVOK_REFERENCE.H: dropping a reference to a reference-counted Havok
   object, as game code does it */

#ifndef HAVOK_REFERENCE_H
#define HAVOK_REFERENCE_H

#include "cseries.h"

/* a reference-counted Havok object: its deleting destructor first in its
   vtable, its allocation size at +4 and its reference count at +6 */
class c_havok_reference_counted
{
public:
	virtual ~c_havok_reference_counted() {}

	word allocation_size;
	short reference_count;
};

/* whether the address lies in the memory the cache file was loaded into
   (unknown_279740.cpp) */
bool __cdecl function_279740(long value);

/* drops a reference, deleting the object with the last one; objects that
   came with the cache file are never deleted, their count goes back to 1 */
__forceinline void havok_reference_remove(c_havok_reference_counted *object)
{
	object->reference_count--;
	if (object->reference_count == 0)
	{
		if (function_279740((long)object))
		{
			object->reference_count = 1;
		}
		else
		{
			delete object;
		}
	}
}

#endif
