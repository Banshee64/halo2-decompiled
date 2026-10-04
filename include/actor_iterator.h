#ifndef __ACTOR_ITERATOR_H__
#define __ACTOR_ITERATOR_H__

/* ACTOR_ITERATOR.H: iterating the actors (g_4f55f0); actor_iterator_next is
   0x1e46c0 (unknown_1e46c0.cpp), actor_iterator_new is inlined everywhere */

#include "cseries.h"
#include "data_array.h"
#include "globals.h"

struct s_actor_iterator
{
	void *actor;
	s_data_iterator iterator;
	bool active_only;
	long actor_index;
};

static inline void actor_iterator_new(s_actor_iterator *iterator, bool active_only)
{
	if (g_4f55d0->active)
	{
		iterator->iterator.data = g_4f55f0;
		iterator->iterator.index = NONE;
		iterator->iterator.datum_index = NONE;
		iterator->active_only = active_only;
	}
}

void *actor_iterator_next(s_actor_iterator *iterator);

#endif
