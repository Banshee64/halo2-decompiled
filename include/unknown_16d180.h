/* UNKNOWN_16D180.H: the lookups of a model's variants, regions and
   permutations by name (src/unknown_16d180.cpp) */

#ifndef UNKNOWN_16D180_H
#define UNKNOWN_16D180_H

#include "cseries.h"

enum string_id
{
	_string_id_none = 0
};

long function_16d180(long model_index, string_id name);
long model_find_region_by_name(long model_index, string_id name);
long model_find_permutation_by_name(long region_index, long model_index, string_id name);

#endif
