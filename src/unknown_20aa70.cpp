// @flags /O2 /Gr /arch:SSE
#include "cseries.h"
#include <xtl.h>
#include <string.h>
#include "crc.h"
#include "real_math.h"

dword g_4e6080;
dword g_4e6084;
dword g_4e608c;
byte *g_51e9ec;
dword g_51e9f0;
void *g_51e9f4;

struct s_anim_header
{
	signed char unknown0;
	signed char unknown1;
	short unknown2;
	short unknown4;
	short unknown6;
	byte unknown8[4];
	long unknownc;
};

struct s_anim_data
{
	byte *base;
	s_anim_header *header;
	byte unknown8;
	byte type;
	short count;
};

struct s_vec4 { real_vector3d v; real w; };
struct real_vector2d_copy
{
	real i, j;
};

real_vector3d *g_4687a4;
real_vector3d *g_468788;

// @retail 0x20aa70
void function_20aa70(real_vector3d *out, s_anim_data *data, long index_, real *w)
{
	short index = (short)index_;
	s_anim_header *h;
	byte *p;

	*out = *g_4687a4;
	*w = 0.0f;
	switch (data->type)
	{
	case 1:
		h = data->header;
		p = data->base + h->unknown6 + h->unknown1 + index * 8 + h->unknownc + h->unknown0;
		*(real_vector2d_copy *)out = *(real_vector2d_copy *)p;
		break;
	case 2:
		h = data->header;
		p = data->base + h->unknown6 + h->unknown1 + index * 12 + h->unknownc + h->unknown0;
		*(real_vector2d_copy *)out = *(real_vector2d_copy *)p;
		*w = ((real *)p)[2];
		break;
	case 3:
		h = data->header;
		p = data->base + h->unknown6 + h->unknown1 + index * 16 + h->unknownc + h->unknown0;
		*out = *(real_vector3d *)p;
		*w = ((real *)p)[3];
		break;
	}
}
// @retail 0x20ab60
void function_20ab60(real_vector3d *sum, s_anim_data *data, real *w)
{
	s_anim_header *h;
	byte *p;
	long i;
	short j;

	*sum = *g_4687a4;
	*w = 0.0f;
	switch (data->type)
	{
	case 1:
		for (i = 0; i < data->count; i++)
		{
			h = data->header;
			j = (short)i;
			p = data->base + h->unknown6 + h->unknown1 + j * 8 + h->unknownc + h->unknown0;
			sum->i = ((real *)p)[0] + sum->i;
			sum->j = ((real *)p)[1] + sum->j;
		}
		break;
	case 2:
		for (i = 0; i < data->count; i++)
		{
			h = data->header;
			j = (short)i;
			p = data->base + h->unknown6 + h->unknown1 + j * 12 + h->unknownc + h->unknown0;
			sum->i = ((real *)p)[0] + sum->i;
			sum->j = ((real *)p)[1] + sum->j;
			*w = ((real *)p)[2] + *w;
		}
		break;
	case 3:
		for (i = 0; i < data->count; i++)
		{
			h = data->header;
			j = (short)i;
			p = data->base + h->unknown6 + h->unknown1 + j * 16 + h->unknownc + h->unknown0;
			sum->i += ((real *)p)[0];
			sum->j += ((real *)p)[1];
			sum->k += ((real *)p)[2];
			*w += ((real *)p)[3];
		}
		break;
	}
	if (data->count - 1 > 0)
	{
		sum->i /= (real)data->count - 1.0f;
		sum->j /= (real)data->count - 1.0f;
		sum->k /= (real)data->count - 1.0f;
		*w /= (real)data->count;
	}
}

// @retail 0x20ad40
void function_20ad40(s_anim_data *data, real_vector3d *a, real_vector3d *b, long index)
{
	s_anim_header *h;
	byte *p;

	*a = *g_468788;
	*b = *g_4687a4;
	h = data->header;
	if (h->unknown4 != 0)
	{
		p = data->base + (h->unknown2 + h->unknown6 + h->unknown1 + h->unknownc + h->unknown0);
		p += index * 12;
		*a = *(real_vector3d *)p;
		if (index + 1 < data->count)
		{
			b->i = ((real *)p)[3] - ((real *)p)[0];
			b->j = ((real *)p)[4] - ((real *)p)[1];
			b->k = ((real *)p)[5] - ((real *)p)[2];
		}
	}
}

// @retail 0x20adf0
void function_20adf0(s_anim_data *data, byte *dest)
{
	s_anim_header *h;
	byte *src;
	long n;
	long i;

	h = data->header;
	if (h->unknown1)
	{
		src = data->base + h->unknown6 + h->unknownc + h->unknown0;
		n = h->unknown1 / 3;
		for (i = 0; i < n; i++)
		{
			dest[i] |= src[i];
			dest[i] |= src[n + i];
			dest[i] |= src[2 * n + i];
		}
	}
	h = data->header;
	if (h->unknown0)
	{
		if (h->unknown6)
		{
			src = data->base + h->unknown6 + h->unknownc;
			n = h->unknown0 / 3;
			for (i = 0; i < n; i++)
			{
				dest[i] |= src[i];
				dest[i] |= src[n + i];
				dest[i] |= src[2 * n + i];
			}
		}
	}
}
// @retail 0x20aee0
void function_20aee0(void)
{
	long size = 0x1880;
	dword offset = g_4e6080 + g_4e6084;
	g_4e6084 += 0x1880;
	crc_checksum_buffer(&g_4e608c, &size, 4);
	g_51e9f0 = offset;
	void *memory = VirtualAlloc(NULL, 0x1880, 0x101000, PAGE_READWRITE);
	if (!memory)
	{
		GetLastError();
	}
	g_51e9f4 = memory;
	g_51e9ec = (byte *)memory;
}

// @retail 0x20af50
void function_20af50(void)
{
	if (!VirtualFree(g_51e9f4, 0, MEM_RELEASE))
	{
		GetLastError();
	}
	g_51e9f4 = NULL;
	g_51e9f0 = 0;
	g_51e9ec = NULL;
}

// @retail 0x20af90
void function_20af90(void)
{
	memset(g_51e9ec, 0, 0x1880);
}
