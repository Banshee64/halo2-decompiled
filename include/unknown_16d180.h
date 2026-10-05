/* UNKNOWN_16D180.H: the lookups of a model's variants, regions and
   permutations by name (src/unknown_16d180.cpp) */

#ifndef UNKNOWN_16D180_H
#define UNKNOWN_16D180_H

#include "unknown_11c920.h"

enum string_handle
{
	_string_id_none = 0
};

long function_16d180(long model_index, string_handle name);
long function_16d1d0(long model_index, string_handle name);
long function_16d220(long region_index, long model_index, string_handle name);

#endif
