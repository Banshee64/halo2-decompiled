/* OBJECT_ITERATOR.H: the iterator over the object table (0xbae80, 0xbaeb0) */

#ifndef OBJECT_ITERATOR_H
#define OBJECT_ITERATOR_H

#include "cseries.h"

struct s_object;

struct s_object_iterator
{
	dword type_mask;
	byte flags;
	byte unknown05;
	short index;
	long object_index;
	long signature;
};

void function_bae80(s_object_iterator *iterator, dword type_mask, byte flags);
s_object *function_baeb0(s_object_iterator *iterator);

#endif
