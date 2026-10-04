/* UNKNOWN_0494B0.H: structures used by the render view code at 0x494b0 */

#ifndef UNKNOWN_0494B0_H
#define UNKNOWN_0494B0_H

#include "unknown_0259d0.h"

struct s_view_shape_0
{
	byte unknown00[0xc];
	real f0c;
};

struct s_view_shape_1
{
	byte unknown00[4];
	vector3f v4;
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
	vector3f v1c;
	real f28;
	point3f v2c;
	union
	{
		s_view_shape_0 shape0;
		s_view_shape_1 shape1;
	};
};

struct s_view_camera
{
	byte unknown00[0x28];
	vector3f v28;
	vector3f v34;
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

struct vector4f
{
	real i, j, k, l;
};

struct s_view_result
{
	point3f p0;
	real f0c;
	point3f p10;
	byte unknown1c[4];
	vector3f v20;
	real f2c;
	vector3f v30;
	real f3c;
	vector3f v40;
	real f4c;
	vector3f v50;
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
