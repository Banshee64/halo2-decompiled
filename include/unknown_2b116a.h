/* UNKNOWN_2B116A.H: shared types of the screens and lists of
   src/unknown_2b116a.cpp */

#ifndef UNKNOWN_2B116A_H
#define UNKNOWN_2B116A_H

#include "cseries.h"
#include "data_array.h"

/* an iterator over a list's items (the item, then the data iterator) */
struct s_list_item_iterator
{
	byte *item;
	s_data_iterator iterator;
};

bool function_2b2327(s_list_item_iterator *iterator);

#endif
