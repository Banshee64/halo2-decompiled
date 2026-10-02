/* UNKNOWN_0494B0.H: structures used by the render view code at 0x494b0 */

#ifndef UNKNOWN_0494B0_H
#define UNKNOWN_0494B0_H

#include "real_math.h"

struct s_view_shape_0
{
	byte unknown00[0xc];
	real f0c;
};

struct s_view_shape_1
{
	byte unknown00[4];
	real_vector3d v4;
	byte unknown10[4];
	real f14;
	real f18;
	byte unknown1c[0xc];
	real f28;
	real f2c;
};

struct s_view_source
{
	union
	{
		long type;
		short type_low;
	};
	byte unknown04[0x18];
	real_vector3d v1c;
	real f28;
	real_point3d v2c;
	union
	{
		s_view_shape_0 shape0;
		s_view_shape_1 shape1;
	};
};

struct s_view_camera
{
	byte unknown00[0x28];
	real_vector3d v28;
	real_vector3d v34;
	byte unknown40[0x2c];
	real f6c;
	real f70;
};

struct s_view_flags
{
	dword flags;
	byte unknown04[0x8a];
	short w8e;
};

struct real_vector4d
{
	real i, j, k, l;
};

struct s_view_result
{
	real_point3d p0;
	real f0c;
	real_point3d p10;
	byte unknown1c[4];
	real_vector3d v20;
	real f2c;
	real_vector3d v30;
	real f3c;
	real_vector3d v40;
	real f4c;
	real_vector3d v50;
	real f5c;
	real f60;
	real f64;
	real f68;
	real f6c;
	real f70;
	real f74;
	real f78;
};

#endif
