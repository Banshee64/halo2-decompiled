#include "unknown_11c920.h"
#include "unknown_1946f0.h"

// @flags /O2 /Ob1 /arch:SSE /Gr

/* Compares the packed directions, then their decoded squared distance. */
// @retail 0x24f7b0
bool vector3f::quantized_equal(vector3f const *other) const
{
	long index = function_24f590(this);
	long other_index = function_24f590(other);
	bool local_0;
	if (index == other_index)
		local_0 = true;
	else
	{
		vector3f a;
		vector3f b;
		function_24f6b0(index, &a);
		function_24f6b0(other_index, &b);
		real x = a.i - b.i;
		real y = a.j - b.j;
		real z = a.k - b.k;
		local_0 = x * x + y * y + z * z < 0.05f;
	}
	return local_0;
}
