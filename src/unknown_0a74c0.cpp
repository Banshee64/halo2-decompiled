// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0A74C0.CPP: the check that two vectors form an orthonormal pair
   (lane M, for 0xdf380) */

#include "cseries.h"
#include "real_math.h"

bool valid_real_normal3d(real_vector3d const *vector);
bool valid_realcmp(real a, real b);

// @retail 0xa74c0
bool valid_real_vector3d_axes2(real_vector3d const *forward, real_vector3d const *up)
{
	return valid_real_normal3d(forward) && valid_real_normal3d(up) && valid_realcmp(dot_product3d(forward, up), 0.0f);
}
