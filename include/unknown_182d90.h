#ifndef UNKNOWN_182D90_H
#define UNKNOWN_182D90_H

#include "unknown_11c920.h"
#include <xmmintrin.h>

struct s_extent_transform
{
	__m128 rows[4];
	void compose(s_extent_transform const *a, s_extent_transform const *b);
};

#endif
