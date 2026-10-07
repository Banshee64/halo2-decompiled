// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_02B400.CPP: vector math and rectangular grid placement */

#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include <math.h>
#include "globals.h"

struct s_masked_list;
typedef void (__stdcall *masked_list_proc)(long value, short mask);
void masked_list_iterate(s_masked_list const *list, long mask, masked_list_proc proc);
void __stdcall function_c3340(long light_index, long unused);
void function_3eec0();
s_masked_list *g_547f98;

// @retail 0x2c340
void function_2c340(bool enabled)
{
	if (enabled)
	{
		masked_list_iterate(g_547f98, 0xffff, (masked_list_proc)function_c3340);
		function_3eec0();
	}
}

struct s_2f800_source
{
	point3f position;
	byte unknown0c[0x14];
	vector3f forward;
	vector3f up;
	byte unknown38[0x14];
	real angle;
	real scale;
};

struct s_2f800_view
{
	point3f position;
	vector3f forward;
	vector3f up;
	bool flag24;
	byte unknown25[3];
	real angle;
	real scale;
	byte unknown30[0x10];
	real near_distance;
	real far_distance;
	byte unknown48[0x10];
	bool flag58;
};

struct s_2f800_size
{
	short width, height;
};

real g_5234dc;

struct s_2f970_view
{
	byte unknown00[0x30];
	short top, left, bottom, right;
	short outer_top, outer_left, outer_bottom, outer_right;
	byte unknown40[0x18];
	bool crop;
	byte unknown59[3];
	real center_x, center_y, crop_scale;
	bool offset;
	byte unknown69[3];
	real offset_x, offset_y;
};

bool g_4ba019;

// @retail 0x2f970
bool function_2f970(s_2f970_view const *view, real *out)
{
	real center_x = (view->outer_right + view->outer_left) * 0.5f;
	real center_y = (view->outer_top + view->outer_bottom) * 0.5f;
	real vertical_scale = 2.0f / (view->outer_bottom - view->outer_top);
	real horizontal_scale = (real)(view->bottom - view->top) / (view->right - view->left) * vertical_scale;
	out[0] = (view->left - center_x) * horizontal_scale;
	out[1] = (view->right - center_x) * horizontal_scale;
	out[2] = (center_y - view->bottom) * vertical_scale;
	out[3] = (center_y - view->top) * vertical_scale;
	g_4ba019 = view->crop || view->offset;
	if (g_4e0350)
		g_4ba019 |= *(short *)((byte *)g_4e0350 + 0x10) == 2;
	if (view->crop)
	{
		real width = out[1] - out[0];
		real height = out[3] - out[2];
		real x = (view->center_x + 1.0f) * 0.5f * width + out[0];
		real y = (view->center_y + 1.0f) * 0.5f * height + out[2];
		real half_width = view->crop_scale * width * 0.5f;
		real half_height = view->crop_scale * height * 0.5f;
		out[0] = x - half_width;
		out[2] = y - half_height;
		out[3] = half_height + y;
		out[1] = half_width + x;
	}
	else if (view->offset)
	{
		real x = (out[1] - out[0]) * (1.0f / 640.0f) * view->offset_x;
		real y = (out[3] - out[2]) * (1.0f / 480.0f) * view->offset_y;
		out[0] += x;
		real right = out[1] + x;
		out[2] -= y;
		out[3] -= y;
		out[1] = right;
	}
	return true;
}

// @retail 0x2f800
void function_2f800(s_2f800_source const *source, s_2f800_size const *first,
	s_2f800_size const *second, s_2f800_view *view)
{
	(void)&second;
	if (source)
	{
		view->position = source->position;
		view->forward = source->forward;
		view->up = source->up;
		view->angle = source->angle;
		view->scale = source->scale;
		real factor = 0.785f;
		if (first && second)
		{
			long ratio = first->width * second->height * 100 / (first->height * second->width);
			if (ratio == 200)
				factor = 0.5f;
			else if (ratio == 50)
				factor = 0.8f;
		}
		view->angle *= factor;
		view->scale *= factor;
	}
	else
	{
		view->position = *g_468788;
		view->forward = *g_4687a8;
		view->up = *g_4687b0;
		view->scale = 1.0f;
		view->angle = (real)(2.0 * atan2((double)(tan(g_5234dc * 0.5f) * 0.75f), 1.0));
	}
	view->flag24 = false;
	view->near_distance = g_485ad4.lo;
	view->far_distance = g_485ad4.hi;
	view->flag58 = false;
}

extern byte g_485ac2;
struct s_unknown_13bf00;
extern s_unknown_13bf00 *g_510c50;

struct s_projection_scalars
{
	box2f clip;
	real width, height;
	real half_width, half_height;
	real offset_x, offset_y;
	real scale_x, scale_y;
	real left, right, bottom, top;
};

// @retail 0x2fb70
void function_2fb70(s_2f800_view const *view, box2f const *clip, s_projection_scalars *out)
{
	short const *viewport = (short const *)view->unknown30;
	out->width = (real)(viewport[3] - viewport[1]);
	out->height = (real)(viewport[2] - viewport[0]);
	if (!clip)
	{
		out->clip.x0 = out->clip.y0 = -1.0f;
		out->clip.x1 = out->clip.y1 = 1.0f;
	}
	else
		out->clip = *clip;
	real half_width = (out->clip.x1 - out->clip.x0) * 0.5f;
	real half_height = (out->clip.y1 - out->clip.y0) * 0.5f;
	real offset_x = (out->clip.x1 + out->clip.x0) / half_width * -0.5f;
	real offset_y = (out->clip.y0 + out->clip.y1) / half_height * -0.5f;
	out->half_height = half_height;
	out->half_width = half_width;
	out->offset_x = offset_x;
	out->offset_y = offset_y;
	real tangent = (real)tan(view->angle * 0.5f);
	real aspect = out->width / out->height;
	if (!(g_4e6948 && g_4e6948->flag && g_4e6948->index != NONE && g_4e6948->state == 3) && g_485ac2)
	{
		aspect *= 4.0f / 3.0f;
		if (g_510c50 && ((byte *)g_510c50)[5])
			tangent = tangent * (4.0f / 3.0f) * 0.5625f;
	}
	else if (g_510c50 && ((byte *)g_510c50)[5])
	{
		real scale = out->height * (1.0f / 480.0f);
		out->clip.y0 *= scale;
		out->clip.y1 *= scale;
	}
	out->scale_x = 1.0f / (half_width * aspect * tangent);
	real inverse_x = 1.0f / out->scale_x;
	out->scale_y = 1.0f / (out->half_height * tangent);
	real left = 0.0f - (offset_x + 1.0f) * inverse_x;
	out->right = 0.0f - (offset_x - 1.0f) * inverse_x;
	real inverse_y = 1.0f / out->scale_y;
	real bottom = 0.0f - (out->offset_y + 1.0f) * inverse_y;
	real top = 0.0f - (out->offset_y - 1.0f) * inverse_y;
	out->left = left;
	out->bottom = bottom;
	out->top = top;
}

#define k_real_epsilon 0.0001f

// @retail 0x48e70
long function_48e70(real value)
{
	long bits = *(long *)&value;
	long exponent = (bits >> 23) & 255;
	long shift = 150 - exponent;
	long mantissa = ((bits & 0x7fffff) | 0x800000) >> shift;
	long sign = bits >> 31;
	long small = (exponent - 127) >> 31;
	long result = ((mantissa ^ sign) - sign) & ~small;
	long increment;
	if (!sign && bits && (small || (((1 << shift) - 1) & bits & 0x7fffff)))
		increment = 1;
	else
		increment = 0;
	return result + increment;
}

struct s_sphere_plane_volume
{
	point3f center;
	real radius;
	long count;
	plane3f *planes;
};

// @retail 0x38390
bool function_38390(point3f const *point, s_sphere_plane_volume const *volume)
{
	vector3f delta;
	delta.i = point->x - volume->center.x;
	delta.j = point->y - volume->center.y;
	delta.k = point->z - volume->center.z;
	real distance = delta.i * delta.i;
	distance += delta.k * delta.k;
	distance += delta.j * delta.j;
	bool result = false;
	if (distance < volume->radius * volume->radius)
	{
		bool outside = false;
		for (long i = 0; i < volume->count; ++i)
		{
			plane3f const *plane = &volume->planes[i];
			real distance_to_plane = point->x * plane->i + plane->j * point->y + plane->k * point->z - plane->d;
			if (distance_to_plane > k_real_epsilon)
			{
				outside = true;
				break;
			}
		}
		result = !outside;
	}
	return result;
}

// @retail 0x1c1a0
double __cdecl function_1c1a0(real value)
{
	return fabs(value);
}

// @retail 0x1d6d0
double __stdcall function_1d6d0(real value)
{
	return ceil(value);
}

// @retail 0x2b460
double __cdecl function_2b460(real value)
{
	return floor(value);
}

// @retail 0x2b480
double __stdcall function_2b480(real value)
{
	return floor(value);
}

/* Preserve the retail call boundary used by 0x2c2a70. */
__declspec(noinline) real normalize2d(point2f *v);

// @retail 0x2b400
real normalize2d(point2f *v)
{
	real m = (real)sqrt(v->x * v->x + v->y * v->y);
	if (!(fabs(m) < k_real_epsilon))
	{
		real inv = 1.f / m;
		v->x = v->x * inv;
		v->y = inv * v->y;
		return m;
	}
	return 0.f;
}

// @retail 0x4efb0
vector3f *function_4efb0(vector3f *v)
{
	real length_squared = v->i * v->i + v->j * v->j + v->k * v->k;
	if (length_squared != 0.0f)
	{
		real inverse = (real)(1.0f / sqrt(length_squared));
		v->i = (real)(v->i * inverse);
		v->j = (real)(v->j * inverse);
		v->k = (real)(v->k * inverse);
	}
	return v;
}

// @retail 0x461c0
long function_461c0(point3f const *a, point3f const *b, real radius)
{
	/* Retail receives the radius on the stack. */
	real const *radius_reference = &radius;
	vector3f delta;
	vector3d_from_points3d(a, b, &delta);
	real distance_squared = delta.i * delta.i;
	distance_squared += delta.k * delta.k;
	distance_squared += delta.j * delta.j;
	if (distance_squared <= *radius_reference * *radius_reference)
		return 1;
	return 0;
}

struct s_grid_pair
{
	short x;
	short y;
};

// @retail 0x2f600
void function_2f600(long count, long mode, s_grid_pair *out)
{
	long x = 1;
	long y = 1;
	bool narrow = mode == 1;
	while (x * y < count)
	{
		bool grow;
		if (narrow)
			grow = x < y;
		else
			grow = x <= y;
		if (grow)
			++x;
		else
		{
			x = 1;
			++y;
		}
	}
	out->x = (short)x;
	out->y = (short)y;
}

// @retail 0x2f640
void function_2f640(long index, s_grid_pair *extent, s_grid_pair const *dimensions,
	long count, long mode, s_grid_pair *out)
{
	extent->x = extent->y = 1;
	long width = dimensions->x;
	long area = width * dimensions->y;
	long offset = 0;
	if (count < area)
	{
		if (mode == 1)
		{
			if (!index)
				extent->x = 2;
			else
				offset = 1;
		}
		else if (!index)
			extent->y = 2;
		else
			offset = index / width;
	}
	long position = index + offset;
	out->x = (short)(position % dimensions->x);
	out->y = (short)(position / dimensions->x);
}


#include <string.h>
void function_141590(transform4x3f const *in, transform4x3f *out);

PRIVATE __forceinline void normalize_projection_axis(vector3f *axis)
{
    real length = (real)sqrt((double)axis->k * axis->k + (double)axis->j * axis->j + (double)axis->i * axis->i);
    if (!(fabs(length) < 0.0001f))
    {
        real inverse = 1.0f / length;
        axis->i *= inverse;
        axis->j *= inverse;
        axis->k *= inverse;
    }
}

// @retail 0x2fd90
void function_2fd90(s_2f800_view const *view, box2f const *clip, byte *out)
{
    s_projection_scalars scalars;
    function_2fb70(view, clip, &scalars);
    vector3f right, up, backward;
    right.i = view->up.k * view->forward.j - view->up.j * view->forward.k;
    right.j = view->forward.k * view->up.i - view->up.k * view->forward.i;
    right.k = view->up.j * view->forward.i - view->forward.j * view->up.i;
    up.i = view->forward.k * right.j - view->forward.j * right.k;
    up.j = view->forward.i * right.k - view->forward.k * right.i;
    up.k = view->forward.j * right.i - view->forward.i * right.j;
    backward.i = 0.0f - view->forward.i;
    backward.j = 0.0f - view->forward.j;
    backward.k = 0.0f - view->forward.k;
    normalize_projection_axis(&right);
    normalize_projection_axis(&up);
    normalize_projection_axis(&backward);
    transform4x3f *camera = (transform4x3f *)(out + 0x34);
    camera->forward = right;
    camera->left = up;
    camera->up = backward;
    camera->position = view->position;
    camera->scale = 1.0f;
    transform4x3f *inverse = (transform4x3f *)out;
    function_141590(camera, inverse);
    memcpy(out + 0x68, &scalars.left, 16);
    *(real *)(out + 0xb8) = scalars.scale_x * scalars.width * 0.5f;
    *(real *)(out + 0xbc) = scalars.scale_y * scalars.height * 0.5f;
    real x, y, z, distance;
    if (view->near_distance == 0.0f)
    {
        real const *plane = (real const *)view->unknown48;
        x = inverse->up.i * plane[2] + inverse->left.i * plane[1] + inverse->forward.i * plane[0];
        y = inverse->up.j * plane[2] + inverse->left.j * plane[1] + inverse->forward.j * plane[0];
        z = inverse->up.k * plane[2] + inverse->left.k * plane[1] + inverse->forward.k * plane[0];
        distance = plane[3] * inverse->scale + inverse->position.z * z + inverse->position.y * y + inverse->position.x * x;
    }
    else
    {
        x = y = 0.0f;
        z = 1.0f;
        distance = 0.0f - view->near_distance;
    }
    real reciprocal = 1.0f / z;
    real near_value = 0.0f - reciprocal * distance;
    real scale = (real)(view->far_distance / ((view->far_distance - (double)near_value) *
        (fabs((double)reciprocal * x) + fabs((double)reciprocal * y) + 1.0f)));
    real a = reciprocal * scale * x;
    real b = reciprocal * scale * y;
    real c = 0.0f - scale * near_value;
    if (c > 0.0f && view->near_distance == 0.0f)
    {
        a = 0.0f - a;
        b = 0.0f - b;
        c = 0.0f - c;
        scale = 0.0f - scale;
    }
    real *matrix = (real *)(out + 0x78);
    memset(matrix, 0, 64);
    matrix[0] = scalars.scale_x;
    matrix[5] = scalars.scale_y;
    matrix[8] = 0.0f - scalars.offset_x;
    matrix[9] = 0.0f - scalars.offset_y;
    matrix[2] = 0.0f - a;
    matrix[6] = 0.0f - b;
    matrix[10] = 0.0f - scale;
    matrix[11] = -1.0f;
    matrix[14] = c;
}

struct s_view_setup;
struct s_view_collection_1733e0;
struct s_predicted_resource_block;
struct s_planar_camera_source;
struct s_planar_camera
{
    bool disabled;
    byte unknown01[3];
    transform4x3f inverse;
    transform4x3f matrix;
    bool has_plane;
    byte unknown6d[3];
    real offset;
    plane3f plane;
    byte unknown84[4];
    real depth;
    bool valid;
    byte unknown8d[3];
    byte frustum[0x108];
    long count;
    point2f corners[4];
};
void function_441b0(s_planar_camera_source const *source, s_planar_camera *state, byte const *view);
void function_1726d0(s_view_setup *view, point3f const *position, vector3f const *forward, vector3f const *up);
void __stdcall function_173130(long camera_count, byte *cameras, long cluster_index, s_view_collection_1733e0 *collection);
bool function_16e5e0(s_predicted_resource_block const *block, short mode);
void function_146de0(void);
void function_146b80(void);
extern byte *g_510c44;
extern bool g_510c48;

// @retail 0x3f660
void function_3f660(transform4x3f const *matrix)
{
    bool restore = g_47989c != NULL;
    if (restore)
        function_146de0();
    byte *collection = g_510c44;
    g_510c48 = true;
    byte setup[0x54];
    function_1726d0((s_view_setup *)setup, (point3f const *)((byte const *)matrix + 4),
        (vector3f const *)((byte const *)matrix + 0x1c), (vector3f const *)((byte const *)matrix + 0x28));
    s_2f970_view view;
    function_2f800((s_2f800_source const *)setup, NULL, NULL, (s_2f800_view *)&view);
    view.top = view.left = 0;
    view.bottom = 480;
    view.right = 640;
    view.outer_top = view.top;
    view.outer_left = view.left;
    view.outer_bottom = view.bottom;
    view.outer_right = view.right;
    view.outer_left = (short)(view.outer_left + 48.0f);
    view.outer_top = (short)(view.outer_top + 48.0f);
    view.outer_right = (short)(view.outer_right - 48.0f);
    view.outer_bottom = (short)(view.outer_bottom - 48.0f);
    view.crop = false;
    view.center_x = view.center_y = 0.0f;
    view.offset = false;
    view.offset_x = view.offset_y = 0.0f;
    box2f clip;
    function_2f970(&view, (real *)&clip);
    byte projection[0xc0];
    function_2fd90((s_2f800_view *)&view, &clip, projection);
    s_planar_camera camera;
    function_441b0((s_planar_camera_source const *)&view, &camera, projection);
    function_173130(1, (byte *)&camera, *(short *)(setup + 0x10), (s_view_collection_1733e0 *)collection);
    byte *structure = (byte *)g_4e0348;
    for (long i = 0; i < *(short *)(collection + 0xa6c); ++i)
    {
        short cluster = *(short *)(collection + 0xa6e + i * 0x1a);
        byte *entry = *(byte **)(structure + 0xa0) + cluster * 0xb0;
        function_16e5e0((s_predicted_resource_block *)(entry + 0x84), 0);
    }
    g_510c48 = false;
    if (restore)
        function_146b80();
}
