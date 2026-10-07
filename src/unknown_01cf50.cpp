// @flags /O2 /Ob1 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "globals.h"
#include <xtl.h>
#include <string.h>
#include <math.h>
#include <xmmintrin.h>
#include "unknown_058ee0.h"
#include "unknown_163110.h"

bool function_48b00(point3f const *point, real radius, real *screen, real *extent);
void function_1cf50();

struct s_unknown_01dcc0
{
    byte unknown00[4];
    long sub_header[5];
    long elements[4][6];
    long element_count;
    byte unknown7c[4];
    long width, height;
    void *data;
    byte unknown8c[8];
    bool flag94;
    byte flag95;
    byte unknown96[2];
};
extern s_unknown_01dcc0 g_4b4b58[39];
bool function_01dd60(long index, long *width, long *height);
void function_14bc0(short index, short element, bool use_depth);

PRIVATE __forceinline void write_texture_corners(real u, real v)
{
    D3DDevice_SetVertexData2f(1, u, v);
    D3DDevice_SetVertexData2f(2, u, v);
    D3DDevice_SetVertexData2f(3, u, v);
    D3DDevice_SetVertexData2f(4, u, v);
}

// @retail 0x15ec0
void __stdcall function_15ec0(long index)
{
    long width = 0, height = 0;
    long count;
    if (g_4b4b58[index].data && !g_4b4b58[index].flag95)
        count = g_4b4b58[index].element_count;
    else count = index == 0 || index == 3 ? 1 : 0;
    function_01dd60(index, &width, &height);
    for (long level = 0; level < count; ++level)
    {
        real x = (real)width, y = (real)height;
        real ratio = g_485ad4.hi / (g_485ad4.hi - g_485ad4.lo);
        real z = (ratio - g_485ad4.lo * ratio) * 16777215.0f;
        if (z < 0.0f) z = 0.0f;
        else if (z > 16777215.0f) z = 16777215.0f;
        real w = (1.0f / g_485ad4.hi) * 16777215.0f;
        if (w < 0.0f) w = 0.0f;
        else if (w > 16777215.0f) w = 16777215.0f;
        function_14bc0((short)index, (short)level, false);
        D3DDevice_Begin(D3DPT_TRIANGLESTRIP);
        real fraction = count > 1 ? (real)level / (count - 1) : 0.0f;
        D3DDevice_SetVertexData4f(5, 0.0f, 0.0f, 0.0f, fraction);
        D3DDevice_SetVertexData4f(6, 0.0f, 0.0f, 0.0f, 0.0f);
        write_texture_corners(0.0f, 0.0f);
        D3DDevice_SetVertexData4f(0, 0.0f, 0.0f, z, w);
        write_texture_corners(1.0f, 0.0f);
        D3DDevice_SetVertexData4f(0, x, 0.0f, z, w);
        write_texture_corners(0.0f, 1.0f);
        D3DDevice_SetVertexData4f(0, 0.0f, y, z, w);
        write_texture_corners(1.0f, 1.0f);
        D3DDevice_SetVertexData4f(0, x, y, z, w);
        D3DDevice_End();
        width /= 2;
        height /= 2;
    }
}

bool function_015b10(long index, D3DPalette **out);
const long g_43e8e8[4] = { 32, 64, 128, 256 };

// @retail 0x1d3f0
bool function_1d3f0(long index, void const *colors, D3DPalette **out)
{
	D3DPalette *palette = NULL;
	bool result = function_015b10(index, &palette);
	if (result && colors)
	{
		D3DCOLOR *destination = D3DPalette_Lock2(palette, 0);
		if (destination)
			memcpy(destination, colors, g_43e8e8[index] * sizeof(D3DCOLOR));
		D3DPalette_Unlock(palette);
	}
	*out = palette;
	return result;
}

// @retail 0x15780
void function_15780(long stage, long mode)
{
	D3DDevice_SetTextureStageState(stage, D3DTSS_MAXANISOTROPY, 0);
	D3DDevice_SetTextureStageState(stage, D3DTSS_MIPMAPLODBIAS, 0);
	D3DDevice_SetTextureStageState(stage, D3DTSS_MAXMIPLEVEL, 0);
	D3DDevice_SetTextureStageState(stage, D3DTSS_COLORSIGN, 0);
	D3DDevice_SetTextureStageState(stage, D3DTSS_ALPHAKILL, 0);
	switch (mode)
	{
	case 0:
		D3DDevice_SetTextureStageState(stage, D3DTSS_ADDRESSU, 1);
		D3DDevice_SetTextureStageState(stage, D3DTSS_ADDRESSV, 1);
		D3DDevice_SetTextureStageState(stage, D3DTSS_ADDRESSW, 1);
		D3DDevice_SetTextureStageState(stage, D3DTSS_MAGFILTER, 2);
		D3DDevice_SetTextureStageState(stage, D3DTSS_MINFILTER, 2);
		D3DDevice_SetTextureStageState(stage, D3DTSS_MIPFILTER, 2);
		break;
	case 1: case 5:
		D3DDevice_SetTextureStageState(stage, D3DTSS_ADDRESSU, 3);
		D3DDevice_SetTextureStageState(stage, D3DTSS_ADDRESSV, 3);
		D3DDevice_SetTextureStageState(stage, D3DTSS_ADDRESSW, 3);
		D3DDevice_SetTextureStageState(stage, D3DTSS_MAGFILTER, 2);
		D3DDevice_SetTextureStageState(stage, D3DTSS_MINFILTER, 2);
		D3DDevice_SetTextureStageState(stage, D3DTSS_MIPFILTER, 2);
		break;
	case 2: case 6:
		D3DDevice_SetTextureStageState(stage, D3DTSS_ADDRESSU, 4);
		D3DDevice_SetTextureStageState(stage, D3DTSS_ADDRESSV, 4);
		D3DDevice_SetTextureStageState(stage, D3DTSS_ADDRESSW, 4);
		D3DDevice_SetTextureStageState(stage, D3DTSS_MAGFILTER, 2);
		D3DDevice_SetTextureStageState(stage, D3DTSS_MINFILTER, 2);
		D3DDevice_SetTextureStageState(stage, D3DTSS_MIPFILTER, 2);
		break;
	case 3: case 7:
		D3DDevice_SetTextureStageState(stage, D3DTSS_ADDRESSU, 3);
		D3DDevice_SetTextureStageState(stage, D3DTSS_ADDRESSV, 3);
		D3DDevice_SetTextureStageState(stage, D3DTSS_ADDRESSW, 3);
		D3DDevice_SetTextureStageState(stage, D3DTSS_MAGFILTER, 1);
		D3DDevice_SetTextureStageState(stage, D3DTSS_MINFILTER, 1);
		D3DDevice_SetTextureStageState(stage, D3DTSS_MIPFILTER, 0);
		break;
	case 4:
		D3DDevice_SetTextureStageState(stage, D3DTSS_ADDRESSU, 3);
		D3DDevice_SetTextureStageState(stage, D3DTSS_ADDRESSV, 3);
		D3DDevice_SetTextureStageState(stage, D3DTSS_ADDRESSW, 3);
		D3DDevice_SetTextureStageState(stage, D3DTSS_MAGFILTER, 2);
		D3DDevice_SetTextureStageState(stage, D3DTSS_MINFILTER, 2);
		D3DDevice_SetTextureStageState(stage, D3DTSS_MIPFILTER, 0);
		break;
	default: __assume(0);
	}
}

struct s_buffer_pair
{
	long first;
	long second;
};

s_buffer_pair g_5093c0;

void *g_5093b0;

short bitmap_get_mipmap_count(short width, short height, short depth, short format,
	bool linear, short maximum_levels);
bool function_0158f0(byte linear, short format, long width, long height,
	long levels, long usage_index, D3DTexture **out);
bool function_0159b0(long edge, short format, long levels, long usage_index,
	D3DCubeTexture **out);
bool function_015a60(short format, long width, long height, long depth,
	long levels, long usage_index, D3DVolumeTexture **out);

// @retail 0x1d000
bool function_1d000(byte *bitmap)
{
	short width = *(short *)(bitmap + 4);
	short height = *(short *)(bitmap + 6);
	char depth = *(char *)(bitmap + 8);
	short format = *(short *)(bitmap + 0xc);
	bool linear = (bitmap[0xe] & 0x10) != 0;
	void **texture = (void **)(bitmap + 0x50);
	*texture = NULL;
	short levels = bitmap_get_mipmap_count(width, height, depth, format, linear,
		*(short *)(bitmap + 0x14));
	*(short *)(bitmap + 0x14) = levels;
	if (g_5093b0)
	{
		switch (*(short *)(bitmap + 0xa))
		{
		case 0:
			return function_0158f0(linear, format, width, height, levels, 0,
				(D3DTexture **)texture);
		case 1:
			return function_015a60(format, width, height, depth, levels, 0,
				(D3DVolumeTexture **)texture);
		default:
			return function_0159b0(width, format, levels, 0,
				(D3DCubeTexture **)texture);
		}
	}
	return true;
}

struct s_1c330_output
{
	long stream;
	long offset;
	long format;
	byte flag_c, flag_d;
	short unknown0e;
};

long function_35790(long format);
long function_35850(long format);

// @retail 0x1c330
void function_1c330(byte *context, s_1c330_output *output)
{
	(void)&context;
	(void)&output;
	memset(output, 0, 16 * sizeof(*output));
	long i;
	for (i = 0; i < 16; ++i)
		output[i].format = 2;
	long mapping[21];
	memset(mapping, 0xff, sizeof(mapping));
	byte *selection = context + *(long *)(context + 0x80) * 16;
	long tag = *(long *)(selection + 4);
	if (tag != NONE)
	{
		byte *definition = g_4e3b44[tag & 0xffff].bytes;
		byte *entry = *(byte **)(definition + 8) + *(long *)(selection + 8) * 0x1c;
		dword count = *(dword *)(entry + 4);
		dword j = 0;
		if (count > 0)
		{
			word *indices = *(word **)(entry + 8);
			do
			{
				mapping[*indices] = j;
				++j;
				++indices;
			} while (j < count);
		}
	}
	else
	{
		mapping[0] = 0;
		mapping[3] = 3;
		mapping[14] = 9;
	}
	for (long stream = 0; stream < *(long *)(context + 0x2d0) ||
		(*(long *)(context + 0x2d0) == 0 && (dword)stream < 16); ++stream)
	{
		char *source = *(char **)(context + 0x8c + stream * 4);
		if (source)
		{
			long offset = 0;
			for (dword element = 0; element < 10; ++element)
			{
				char *pair = source + 1 + element * 2;
				if (pair[0] >= 0)
				{
					long target = mapping[pair[0]];
					if (target >= 0)
					{
						s_1c330_output *entry = &output[target];
						entry->stream = stream;
						entry->offset = offset;
						entry->format = function_35790(pair[1]);
						entry->flag_c = false;
						entry->flag_d = false;
					}
					long size = function_35850(pair[1]);
					offset = (byte)size + offset;
				}
				else if (pair[0] == -2)
					offset += pair[1];
				else
					break;
			}
			*(char **)(context + 0xcc + stream * 4) = source;
			*(char **)(context + 0x8c + stream * 4) = NULL;
			context[0x20c] = 0;
		}
	}
}
long g_5093c8;
long g_5093cc;

// @retail 0x13d50
void function_13d50(s_buffer_pair const *values, long mode, long count)
{
	/* The count remains on the stack in retail. */
	(void)&count;
	long first = values->first;
	long second = values->second;
	g_5093c8 = mode;
	g_5093c0.first = first;
	g_5093c0.second = second;
	g_5093cc = count;
}

/* the texture stages: [0] the textures set on the device (0x51f3c8), [1] the
   textures wanted (0x51f3d8) */
IDirect3DBaseTexture8 *g_51f3c8[2][4];
long g_5093d8;
dword g_487288[17][32];
D3DSurface *g_51f3f4;
D3DSurface *g_51f3f8;
byte g_51f3fc;

extern D3DResource *g_509374, *g_509378, *g_50937c, *g_509380;
D3DTexture *g_509354[2];
D3DSurface *g_50935c, *g_509360, *g_509364, *g_509370;
D3DTexture *g_509368, *g_50936c;
D3DSurface *g_509384;
D3DTexture *g_50938c;
D3DSurface *g_509390, *g_509394, *g_509398, *g_50939c, *g_5093a0, *g_5093a4, *g_5093a8;
short g_485602;

void function_1cf50(void);

struct s_363a0_vertex
{
	real x, y, u, v;
	dword color;
};

// @retail 0x363a0
void __stdcall function_363a0(void *vertices)
{
	if (g_485602 == 0)
	{
		function_1cf50();
		D3DDevice::Begin(D3DPT_TRIANGLEFAN);
		s_363a0_vertex const *vertex = (s_363a0_vertex *)vertices;
		for (long i = 0; i < 4; ++i, ++vertex)
		{
			D3DDevice_SetVertexDataColor(9, vertex->color);
			D3DDevice_SetVertexData2f(3, vertex->u, vertex->v);
			D3DDevice_SetVertexData2f(0, vertex->x, vertex->y);
		}
		D3DDevice::End();
	}
}

// @retail 0x36880
void function_36880(color4f const *color, s_short_rectangle const *rectangle)
{
	(void)&rectangle;
	function_1cf50();
	D3DDevice::Begin(D3DPT_TRIANGLEFAN);
	D3DDevice_SetVertexData4f(9, color->red, color->green, color->blue, color->alpha);
	for (short i = 0; i < 4; ++i)
	{
		bool top = i < 2;
		bool right = i == 1 || i == 2;
		short y = top ? rectangle->top : rectangle->bottom;
		short x = right ? rectangle->right : rectangle->left;
		D3DDevice_SetVertexData2f(0, (real)x, (real)y);
	}
	D3DDevice::End();
}

// @retail 0x14980
void function_14980(void)
{
	if (g_50935c)
	{
		D3DResource_Release(g_50935c);
		g_50935c = NULL;
	}
	if (g_509360)
	{
		D3DResource_Release(g_509360);
		g_509360 = NULL;
	}
	if (g_509364)
	{
		D3DResource_Release(g_509364);
		g_509364 = NULL;
	}
	long i = 0;
	do
	{
		if (g_509354[i])
		{
			if (!VirtualFree(g_509354[i], 0, MEM_RELEASE)) GetLastError();
			g_509354[i] = NULL;
		}
		++i;
	} while (i < 2);
	if (g_509368)
	{
		if (!VirtualFree(g_509368, 0, MEM_RELEASE)) GetLastError();
		g_509368 = NULL;
	}
	if (g_50936c)
	{
		if (!VirtualFree(g_50936c, 0, MEM_RELEASE)) GetLastError();
		g_50936c = NULL;
	}
	if (g_509370)
	{
		if (!VirtualFree(g_509370, 0, MEM_RELEASE)) GetLastError();
		g_509370 = NULL;
	}
}
real g_4670c8 = 1.0f;
extern byte g_485607, g_5093fc;
extern word g_485648, g_48564a, g_48564c, g_48564e;
extern __int64 g_485aa0;
long g_485af4[4], g_485b04[4];

void *function_01dcf0(long index);
void *function_01dd20(long index, long element);
void *function_01dcc0(long index);
bool function_01dd60(long index, long *width, long *height);
long function_25960(void);
bool function_1cd30(D3DSurface *target, D3DSurface *depth, bool flag);

// @retail 0x14bc0
void function_14bc0(short index, short element, bool use_depth)
{
	D3DSurface *target = NULL;
	D3DSurface *depth = NULL;
	bool flag = false;
	switch (index)
	{
	case 0: target = g_50935c; depth = g_509364; flag = true; break;
	case 4: target = g_509360; flag = true; break;
	case 16: case 29: target = (D3DSurface *)g_509378; break;
	case 17: target = (D3DSurface *)g_509380; break;
	case 3: target = g_509370; depth = g_509364; flag = true; break;
	case 1: target = (D3DSurface *)function_01dcf0(1); depth = g_509364; break;
	case 18: target = (D3DSurface *)function_01dcf0(18); depth = g_509364; break;
	case 9:
		target = g_509384;
		depth = (D3DSurface *)function_01dcf0(9);
		target->Size = depth->Size;
		target->Data = ((D3DSurface *)function_01dcf0(9))->Data;
		break;
	case 10: case 11: case 12: case 25: case 26: case 27:
		target = (D3DSurface *)function_01dcf0(index); depth = NULL; break;
	case 13: target = (D3DSurface *)function_01dd20(13, element); depth = NULL; break;
	case 19: target = (D3DSurface *)function_01dcf0(19); depth = NULL; break;
	case 20: target = (D3DSurface *)function_01dcf0(20); depth = NULL; break;
	case 23: depth = NULL; break;
	case 5: case 6: case 7: case 8: case 15:
		target = (D3DSurface *)function_01dcf0(index); depth = g_509364; break;
	case 33: target = g_509390; depth = g_5093a8; break;
	case 34: target = g_509394; depth = g_5093a8; break;
	case 35: target = g_509398; depth = g_5093a8; break;
	case 36: target = g_50939c; depth = g_5093a8; break;
	case 37: target = g_5093a0; depth = g_5093a8; break;
	case 38: target = g_5093a4; depth = g_5093a8; break;
	case 22: target = (D3DSurface *)function_01dcf0(22); depth = NULL; break;
	case 21: target = (D3DSurface *)function_01dcf0(21); depth = NULL; break;
	case 14: break;
	default: __assume(0);
	}
	function_1cd30(target, use_depth ? depth : NULL, flag);
	D3DVIEWPORT8 viewport;
	if (g_485602 == 2)
	{
		viewport.X = 0; viewport.Y = 0;
		viewport.Width = 128; viewport.Height = 128;
	}
	else if (index == 0 || index == 2 || index == 1 || index == 18 || index == function_25960())
	{
		real scale;
		if (index == function_25960())
		{
			if (g_485607)
			{
				viewport.X = 0; viewport.Y = 0;
				viewport.Width = 640; viewport.Height = 480;
				goto viewport_ready;
			}
			scale = 1.0f;
		}
		else
		{
			if (g_485607 && !g_5093fc)
				scale = g_4670c8 < 0.0625f ? 0.0625f : g_4670c8 > 1.0f ? 1.0f : g_4670c8;
			else
				scale = 1.0f;
		}
		viewport.X = (long)((short)g_48564a * (double)scale);
		viewport.Y = (long)((short)g_485648 * (double)scale);
		viewport.Width = (long)(((short)g_48564e - (short)g_48564a) * (double)scale);
		viewport.Height = (long)(((short)g_48564c - (short)g_485648) * (double)scale);
	}
	else
	{
		D3DSURFACE_DESC desc;
		D3DSurface_GetDesc(target, &desc);
		viewport.X = 0; viewport.Y = 0;
		viewport.Width = desc.Width; viewport.Height = desc.Height;
	}
viewport_ready:
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;
	D3DDevice_SetViewport(&viewport);
}

// @retail 0x14f60
void function_14f60(short stage, short index)
{
	D3DBaseTexture *texture = NULL;
	long width = 0, height = 0;
	function_01dd60(index, &width, &height);
	g_485af4[stage] = width;
	g_485b04[stage] = height;
	switch (index)
	{
	case 0: texture = g_509354[(g_485aa0 - 1) % 2]; break;
	case 4: texture = g_509354[g_485aa0 % 2]; break;
	case 16: case 29: texture = (D3DBaseTexture *)g_509374; break;
	case 17: texture = (D3DBaseTexture *)g_50937c; break;
	case 3: texture = g_509368; break;
	case 24: texture = g_50936c; break;
	case 1: texture = (D3DBaseTexture *)function_01dcc0(1); break;
	case 18: texture = (D3DBaseTexture *)function_01dcc0(18); break;
	case 19: texture = (D3DBaseTexture *)function_01dcc0(19); break;
	case 23: texture = g_509354[(g_485aa0 - 1) % 2]; break;
	case 13: texture = (D3DBaseTexture *)function_01dcc0(13); break;
	case 20: texture = (D3DBaseTexture *)function_01dcc0(20); break;
	case 33: case 34: case 35: case 36: case 37: case 38: texture = g_50938c; break;
	case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12: case 15: case 21: case 22: case 25: case 26: case 27:
		texture = (D3DBaseTexture *)function_01dcc0(index); break;
	case 14: break;
	default: __assume(0);
	}
	g_51f3c8[1][stage] = texture;
}

struct s_mask_offset
{
	short x;
	short y;
};

s_mask_offset const g_43f188[16] =
{
	{0, 0}, {2, 2}, {0, 2}, {2, 0},
	{1, 1}, {3, 3}, {3, 1}, {1, 3},
	{0, 1}, {2, 3}, {3, 0}, {1, 2},
	{1, 0}, {3, 2}, {0, 3}, {2, 1}
};

// @retail 0x1c1b0
void function_1c1b0(void)
{
	memset(g_487288, 0, sizeof(g_487288));
	for (long count = 0; count <= 16; count++)
	{
		long x;
		dword *mask = g_487288[count];
		for (x = 0; x < 32; x += 4)
		{
			long base = x;
			for (long rows = 8; rows > 0; rows--, base += 128)
			{
				for (long i = 0; i < count; i++)
				{
					long bit = g_43f188[i].y * 32 + base + g_43f188[i].x;
					mask[bit / 32] = mask[bit / 32] | (1 << (bit % 32));
				}
			}
		}
	}
}

// @retail 0x1cd30
bool function_1cd30(D3DSurface *target, D3DSurface *depth, bool flag)
{
	if (g_51f3f4 != target || g_51f3f8 != depth)
	{
		if (g_51f3f4 && target && target->Common == g_51f3f4->Common && target->Format == g_51f3f4->Format && target->Size == g_51f3f4->Size)
			D3DDevice_SetRenderTargetFast(target, depth, 0);
		else
			D3DDevice_SetRenderTarget(target, depth);
		g_51f3f4 = target;
		g_51f3f8 = depth;
		g_51f3fc = flag;
	}
	return true;
}

// @retail 0x1c290
dword *function_1c290(real value)
{
	real scaled = value * 16.0f;
	long index;
	__asm
	{
		fld scaled
		fistp index
	}
	if (index < 0)
		index = 0;
	else if (index > 16)
		index = 16;
	return g_487288[index];
}

struct s_01b050_shader_state
{
	byte field_0000[0x1424];
	D3DPIXELSHADERDEF program;
	bool changed;
};

// @retail 0x1b050
void function_1b050(s_01b050_shader_state *state)
{
	D3DDevice_SetPixelShaderProgram(&state->program);
	state->changed = false;
}

// @retail 0x15180
void function_15180(D3DPIXELSHADERDEF const *program)
{
	D3DDevice_SetPixelShaderProgram(program);
}

// @retail 0x151c0
void function_151c0(real const *constants)
{
	D3DDevice_SetVertexShaderConstantFast(-46, constants, 3);
	g_5093d8 = 0;
}

// @retail 0x1ccf0
bool function_1ccf0(D3DPIXELSHADERDEF const *program)
{
	D3DDevice_SetPixelShaderProgram(program);
	return true;
}

// @retail 0x1cf50
void function_1cf50()
{
	IDirect3DBaseTexture8 **wanted = g_51f3c8[1];
	for (long stage = 0; stage < 4; wanted++, stage++)
	{
		IDirect3DBaseTexture8 **current = wanted - 4;
		if (*current != *wanted)
		{
			D3DDevice_SetTexture(stage, *wanted);
			*current = *wanted;
		}
	}
}

// @retail 0x1cf80
void function_1cf80(void)
{
	for (long stage = 0; stage < 4; stage++)
	{
		D3DDevice_SetTexture(stage, g_51f3c8[1][stage]);
		g_51f3c8[0][stage] = g_51f3c8[1][stage];
	}
}

// @retail 0x1c8a0
bool __stdcall function_1c8a0(D3DPRIMITIVETYPE type, word const *indices, long count)
{
	function_1cf50();
	D3DDevice_DrawIndexedVertices(type, count, indices);
	return true;
}

struct s_597d0_object
{
	byte unknown00[0x741c];
	long field_741c;
};

// @retail 0x597d0
bool function_597d0(s_597d0_object **out)
{
	bool result = false;
	long mode = 0;
	if (g_527330.initialized)
		mode = g_527330.state;

	switch (mode)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		result = false;
		if (g_527330.initialized)
		{
			s_597d0_object *object = g_527330.session_a;
			if (object->field_741c)
			{
				if (out)
					*out = object;
				result = true;
			}
		}
		break;
	case 7:
	case 8:
	case 9:
		result = false;
		if (g_527330.initialized)
		{
			s_597d0_object *object = g_527330.session_b;
			if (object->field_741c)
			{
				if (out)
					*out = object;
				result = true;
			}
		}
		break;
	}
	return result;
}


struct s_shader_entry
{
	long field_00;
	dword input_count;
	word *inputs;
	long byte_count;
	dword *program;
	byte field_14[8];
};

struct s_shader_tag
{
	long field_00;
	dword entry_count;
	s_shader_entry *entries;
};

struct s_shader_program
{
	dword const *program;
	long field_04;
	dword byte_count;
	long field_0c;
	real field_10;
};

dword const g_43f2b0[85] =
{
	0x00152078, 0x00000000, 0x0056e000, 0x7c2a1000, 0x2ca00000,
	0x00000000, 0x0056e0aa, 0x7c021000, 0x23a00000, 0x00000000,
	0x00570000, 0x8c2a1000, 0x2cb00000, 0x00000000, 0x0096e615,
	0x38aaf856, 0x9ca00000, 0x00000000, 0x0096e601, 0x39fefaae,
	0x93a00000, 0x00000000, 0x00970615, 0x38ab1856, 0xdcb00000,
	0x00000000, 0x00d6201b, 0x08363800, 0x20b08800, 0x00000000,
	0x00d6401b, 0x08365800, 0x20b04800, 0x00000000, 0x00d6601b,
	0x08367800, 0x20b02800, 0x00000000, 0x00d6801b, 0x08369800,
	0x20b01800, 0x00000000, 0x02575215, 0xa42b586e, 0x6ca0f81c,
	0x00000000, 0x065740ab, 0xa5575bff, 0x13a10000, 0x00000000,
	0x00576015, 0xb42b7800, 0x2cb00000, 0x00000000, 0x00770015,
	0xa40012fe, 0x3ca00000, 0x00000000, 0x007720ab, 0xa4001006,
	0x73a00000, 0x00000000, 0x00772015, 0xb40012fe, 0x7cb00000,
	0x00000000, 0x0041401a, 0xc4355800, 0x20b0e800, 0x00000000,
	0x0056a015, 0xa42ab800, 0x2090c848, 0x00000000, 0x0056c0bf,
	0xa42ad800, 0x20a0c850, 0x00000000, 0x0056c015, 0xb57ed800,
	0x20a0c858, 0x00000000, 0x0081601a, 0xc5fe286a, 0xf0b0e801,
};

s_shader_program g_4670ec[1] =
{
	{ g_43f2b0, NONE, 0x154, NONE, 1.0f }
};

// @retail 0x1c2e0
dword const *function_1c2e0(long tag, long index, long *count)
{
	dword const *result;
	if (tag != NONE)
	{
		s_shader_entry *entry = &((s_shader_tag *)g_4e3b44[tag & 0xffff].bytes)->entries[index];
		result = entry->program;
		if (count)
			*count = entry->byte_count >> 4;
	}
	else
	{
		result = g_4670ec[index].program;
		if (count)
			*count = g_4670ec[index].byte_count >> 4;
	}
	return result;
}

// @retail 0x1cb20
long function_1cb20(long mode, dword index, long tag)
{
	/* The candidate index remains a stack argument in retail. */
	dword const *index_reference = &index;
	switch (mode)
	{
	case 1: return 1;
	case 2: return 2;
	case 3:
		if (*index_reference + 2 < ((s_shader_tag *)g_4e3b44[tag & 0xffff].bytes)->entry_count)
			return *index_reference + 2;
	case 0: return 0;
	default: __assume(0);
	}
}

// @retail 0x1cb70
dword function_1cb70(long tag, long index)
{
	dword result = 0;
	s_shader_entry *entry = &((s_shader_tag *)g_4e3b44[tag & 0xffff].bytes)->entries[index];
	for (dword i = 0; i < entry->input_count; i++)
		result |= 1 << entry->inputs[i];
	return result;
}

struct s_shader_binding
{
	long field_00;
	long tag;
	long index;
	long field_0c;
};

struct s_shader_stream
{
	long field_00;
	long field_04;
	long field_08;
};

struct s_shader_cache
{
	s_shader_binding bindings[8];
	long active;
	long field_84;
	bool field_88;
	byte field_89[3];
	byte const *wanted[16];
	byte const *current[16];
	byte field_10c[0x100];
	bool descriptors_changed;
	byte field_20d[3];
	s_shader_stream streams[16];
	long stream_count;
	bool streams_changed;
	byte field_2d5[3];
};

// @retail 0x1c710
void __stdcall function_1c710(void *memory)
{
	s_shader_cache *state = (s_shader_cache *)memory;
	bool changed = false;
	if (state->descriptors_changed)
		function_1c330((byte *)state, (s_1c330_output *)state->field_10c);
	if (state->descriptors_changed || state->streams_changed)
	{
		D3DDevice_SetVertexShaderInputDirect(
			state->stream_count > 0 ? (D3DVERTEXATTRIBUTEFORMAT *)state->field_10c : NULL,
			state->stream_count,
			state->stream_count > 0 ? (D3DSTREAM_INPUT *)state->streams : NULL);
		state->streams_changed = false;
		changed = true;
	}
	if (state->field_88 || state->descriptors_changed)
	{
		changed = true;
		D3DDevice_SelectVertexShaderDirect((D3DVERTEXATTRIBUTEFORMAT *)state->field_10c, state->field_84);
		state->field_88 = false;
	}
	state->stream_count = 0;
	if (changed)
		state->descriptors_changed = false;
}

// @retail 0x1c4a0
void function_1c4a0(s_shader_cache *state)
{
	state->streams_changed = false;
	state->descriptors_changed = false;
	state->stream_count = 0;
	for (long i = 0; i < 8; i++)
	{
		state->bindings[i].field_0c = NONE;
		state->bindings[i].tag = NONE;
		state->bindings[i].index = NONE;
	}
	state->active = NONE;
	for (long j = 0; j < 16; j++)
		state->wanted[j] = NULL;
	for (long k = 0; k < 16; k++)
	{
		state->streams[k].field_08 = 0;
		state->streams[k].field_04 = 0;
		state->streams[k].field_00 = 0;
	}
}

// @retail 0x1c6b0
void function_1c6b0(void *memory)
{
	s_shader_cache *state = (s_shader_cache *)memory;
	state->stream_count = 0;
	state->streams_changed = true;
	state->descriptors_changed = true;
	for (long i = 0; i < 16; i++)
	{
		state->streams[i].field_08 = 0;
		state->streams[i].field_04 = 0;
		state->streams[i].field_00 = 0;
	}
	state->descriptors_changed = true;
	memset(state->current, 0, sizeof(state->current));
	memset(state->wanted, 0, sizeof(state->wanted));
}

// @retail 0x1c590
void function_1c590(s_shader_cache *state, long tag, long index)
{
	long count;
	dword const *program = function_1c2e0(tag, index, &count);
	state->active = 0;
	if (state->bindings[0].tag != tag || state->bindings[0].index != index)
	{
		state->bindings[0].field_00 = 0;
		state->bindings[0].tag = tag;
		state->bindings[0].index = index;
		state->bindings[0].field_0c = 0;
		D3DDevice_LoadVertexShaderProgram(program, 0);
		state->field_84 = 0;
		state->field_88 = true;
		memset(state->current, 0, sizeof(state->current));
		state->descriptors_changed = true;
		for (long i = 0; i < 16; i++)
		{
			state->streams[i].field_08 = 0;
			state->streams[i].field_04 = 0;
			state->streams[i].field_00 = 0;
		}
		state->streams_changed = true;
		state->stream_count = 0;
	}
}

// @retail 0x1c620
void function_1c620(s_shader_cache *state, long a, long b, long c, byte const *descriptor)
{
	long index = state->stream_count;
	if (state->streams[index].field_04 != b || state->streams[index].field_00 != a || state->streams[index].field_08 != c)
	{
		state->streams[index].field_08 = c;
		state->streams[index].field_04 = b;
		state->streams[index].field_00 = a;
		state->streams_changed = true;
	}
	state->wanted[index] = descriptor;
	if (state->current[index] != descriptor)
		state->descriptors_changed = true;
	state->stream_count++;
}

byte g_485af1;

extern byte g_51f0f0[0x2d8];
extern byte *g_485a80;

struct s_shader_slot
{
	dword unknown00;
	long tag;
};

// @retail 0x1cbb0
dword function_1cbb0(long mode, dword index, long tag)
{
	long const *mode_reference = &mode;
	long selected = function_1cb20(*mode_reference, index, tag);
	function_1c590((s_shader_cache *)g_51f0f0, tag, selected);
	return function_1cb70(tag, selected);
}

// @retail 0x1cbe0
dword function_1cbe0(long mode, dword index, long slot_index)
{
	(void)&mode;
	(void)&index;
	s_shader_slot *slot = &(*(s_shader_slot **)(g_485a80 + 0x5c))[slot_index];
	long selected = function_1cb20(mode, index, slot->tag);
	function_1c590((s_shader_cache *)g_51f0f0, slot->tag, selected);
	return function_1cb70(slot->tag, selected);
}

// @retail 0x1cc30
dword function_1cc30(long index)
{
	s_shader_slot *slot = &(*(s_shader_slot **)(g_485a80 + 0x5c))[index];
	function_1c590((s_shader_cache *)g_51f0f0, slot->tag, 0);
	return function_1cb70(slot->tag, 0);
}

// @retail 0x1c7f0
void __stdcall function_1c7f0(long index)
{
	/* The binding index occupies a stack slot in retail. */
	long const *reference = &index;
	s_shader_binding *binding = &((s_shader_cache *)g_51f0f0)->bindings[*reference & 0xffff];
	binding->field_0c = NONE;
	binding->tag = NONE;
	binding->index = NONE;
}

#include "physical_memory.h"

byte g_51f3f0;
s_physical_object *g_487b08;

PRIVATE bool __stdcall shader_block_busy(long index)
{
	return false;
}

// @retail 0x1c810
void function_1c810(void)
{
	g_51f3f0 = false;
	g_51f3f4 = NULL;
	g_51f3f8 = NULL;
	g_51f3fc = false;
	memset(g_51f3c8[1], 0, sizeof(g_51f3c8[1]));
	memset(g_51f3c8[0], 0, sizeof(g_51f3c8[0]));
	g_487b08 = physical_memory_new("vertex shader lruv cache", 0x88, 0, 8,
		function_1c7f0, shader_block_busy, NULL, g_468758);
}



// @retail 0x1e8c0
long __stdcall function_1e8c0(char const *key)
{
	/* This key is passed on the stack by the cache callback interface. */
	char const *const *reference = &key;
	return (*reference)[3] * 59 + (*reference)[2] * 53 + (*reference)[1] * 43 + (*reference)[0] * 17;
}

struct s_cache_key
{
	word key;
	word flags;
};

// @retail 0x1e8f0
long __stdcall function_1e8f0(s_cache_key const *a, s_cache_key const *b)
{
	/* Both pointers are stack arguments in the cache callback interface. */
	s_cache_key const *const *local_6e666f = &a;
	s_cache_key const *const *right_reference = &b;
	word right_flags = (*right_reference)->flags;
	word left_flags = (*local_6e666f)->flags;
	if ((bool)(right_flags & 1) == (bool)(left_flags & 1) && (*local_6e666f)->key == (*right_reference)->key &&
		(short)((left_flags ^ right_flags) & ~1) == 0)
		return 1;
	return 0;
}

struct s_cache_record
{
	long count;
	byte unknown04[8];
	byte active;
	byte unknown0d[0x63];
};

struct s_cache_record_state
{
	long count;
	dword unknown04;
	s_cache_record *records;
	void *buffer;
	byte unknown10;
	bool available;
	byte unknown12[2];
};

s_cache_record_state g_4b6280;

struct hash_table;
extern hash_table *g_51f400;

struct hash_node;
hash_node *function_13e2d0(hash_table *table, void *key);

struct s_cache_lookup_key
{
	short index;
	word flag : 1;
	word part : 15;
	byte unknown04[16];
};

// @retail 0x1e280
void *function_1e280(short index, bool flag, long part)
{
	(void)&part;
	s_cache_lookup_key key;
	key.index = index;
	key.flag = flag;
	key.part = (word)part;
	hash_node *node = function_13e2d0(g_51f400, &key);
	if (node && *(byte **)node)
		return *(byte **)node + 4;
	return 0;
}

struct s_cache_hash_view
{
	byte unknown00[0x34];
	c_data_allocator *allocator;
};

// @retail 0x1e310
void function_1e310(void)
{
	if (g_4b6280.records)
	{
		if (!VirtualFree(g_4b6280.records, 0, MEM_RELEASE)) GetLastError();
		if (!VirtualFree(g_4b6280.buffer, 0, MEM_RELEASE)) GetLastError();
	}
	((s_cache_hash_view *)g_51f400)->allocator->deallocate(g_51f400);
	g_51f400 = 0;
}

// @retail 0x1e2d0
s_cache_record *function_1e2d0(void)
{
	s_cache_record *result = 0;
	if (g_4b6280.count < 1024)
	{
		result = &g_4b6280.records[g_4b6280.count];
		result->count = 0;
		g_4b6280.count++;
		result->active = 0;
	}
	else if (g_4b6280.available)
		g_4b6280.available = false;
	return result;
}

// @retail 0x1cdd0
void function_1cdd0(real const *bounds, byte flags)
{
	/* The reference retains the flag argument's retail stack placement. */
	byte const *flags_reference = &flags;
	real constants[12];
	constants[0] = 1.0f;
	constants[1] = 1.0f;
	constants[2] = 1.0f;
	constants[3] = 1.0f;
	constants[4] = 0.0f;
	constants[5] = 0.0f;
	constants[6] = 0.0f;
	constants[7] = 0.0f;
	constants[8] = 1.0f;
	constants[9] = 1.0f;
	constants[10] = 0.0f;
	constants[11] = 0.0f;
	bool active = false;
	if ((*flags_reference & 3) && bounds)
	{
		active = true;
		if (*flags_reference & 1)
		{
			constants[0] = (bounds[1] - bounds[0]) * 0.5f;
			constants[1] = (bounds[3] - bounds[2]) * 0.5f;
			constants[2] = (bounds[5] - bounds[4]) * 0.5f;
			constants[3] = 1.0f;
			constants[4] = (bounds[1] + bounds[0]) * 0.5f;
			constants[5] = (bounds[2] + bounds[3]) * 0.5f;
			constants[6] = (bounds[4] + bounds[5]) * 0.5f;
			constants[7] = 0.0f;
		}
		if (*flags_reference & 2)
		{
			constants[8] = (bounds[7] - bounds[6]) * 0.5f;
			constants[9] = (bounds[9] - bounds[8]) * 0.5f;
			constants[10] = (bounds[6] + bounds[7]) * 0.5f;
			constants[11] = (bounds[9] + bounds[8]) * 0.5f;
		}
	}
	if (active || g_485af1)
	{
		D3DDevice_SetVertexShaderConstantFast(74, constants, 3);
		g_485af1 = active;
	}
}

byte const g_43f408[63][21] =
{
	{ 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x01, 0x00, 0x02, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x02, 0x00, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x03, 0x00, 0x02, 0x01, 0x04, 0xfe, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x04, 0x00, 0x0e, 0x01, 0x04, 0xfe, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x05, 0x00, 0x02, 0x01, 0x05, 0x02, 0x05, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x06, 0x00, 0x0e, 0xfe, 0x02, 0x01, 0x05, 0x02, 0x05, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x07, 0x00, 0x02, 0x01, 0x06, 0xfe, 0x01, 0x02, 0x06, 0xfe, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x08, 0x00, 0x0e, 0x01, 0x06, 0x02, 0x06, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x09, 0x00, 0x02, 0x01, 0x07, 0x02, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x0a, 0x00, 0x0e, 0xfe, 0x02, 0x01, 0x07, 0x02, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x0b, 0x01, 0x04, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x0c, 0x01, 0x05, 0x02, 0x05, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x0d, 0x01, 0x06, 0x02, 0x06, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x0e, 0x01, 0x07, 0x02, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x0f, 0x0a, 0x02, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x10, 0x0a, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x11, 0x0a, 0x02, 0x0b, 0x04, 0xfe, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x12, 0x0a, 0x0e, 0x0b, 0x04, 0xfe, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x13, 0x0d, 0x04, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x14, 0x00, 0x02, 0xfe, 0x04, 0x10, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x15, 0x00, 0x0e, 0xfe, 0x02, 0x10, 0x0f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x16, 0x00, 0x02, 0x01, 0x04, 0xfe, 0x03, 0x10, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x17, 0x00, 0x0e, 0x01, 0x04, 0xfe, 0x01, 0x10, 0x0f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x18, 0x03, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x19, 0x03, 0x0d, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x1a, 0x04, 0x02, 0x05, 0x02, 0x06, 0x02, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x1b, 0x04, 0x10, 0x05, 0x10, 0x06, 0x10, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x1c, 0x07, 0x02, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x1d, 0x07, 0x10, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x1e, 0x09, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x1f, 0x09, 0x0d, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x20, 0x03, 0x01, 0x04, 0x10, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x21, 0x03, 0x0d, 0x04, 0x10, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x22, 0x03, 0x01, 0x04, 0x10, 0x05, 0x10, 0x06, 0x10, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x23, 0x03, 0x0d, 0x04, 0x10, 0x05, 0x10, 0x06, 0x10, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x24, 0x00, 0x01, 0x03, 0x01, 0x0e, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x25, 0x00, 0x03, 0x03, 0x01, 0x0e, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x26, 0x00, 0x03, 0x01, 0x01, 0x02, 0x01, 0x03, 0x01, 0x04, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x27, 0x00, 0x02, 0x03, 0x0d, 0x0e, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x28, 0x00, 0x02, 0x03, 0x01, 0x0e, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x29, 0x00, 0x02, 0x03, 0x02, 0x0e, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x2a, 0x00, 0x03, 0x01, 0x02, 0x02, 0x00, 0x03, 0x03, 0x04, 0x02, 0x05, 0x01, 0x06, 0x03, 0x07, 0x03, 0x09, 0x11, 0xff, 0x00 },
	{ 0x2b, 0x00, 0x0b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x2c, 0x00, 0x02, 0x0e, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x2d, 0x00, 0x02, 0x03, 0x10, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x2e, 0x0e, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x2f, 0x0e, 0x06, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x30, 0x08, 0x10, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x31, 0x00, 0x03, 0x03, 0x01, 0x0e, 0x11, 0x0f, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x32, 0x00, 0x02, 0x04, 0x02, 0x06, 0x02, 0x05, 0x02, 0x03, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x33, 0x00, 0x02, 0x04, 0x10, 0x06, 0x10, 0x05, 0x10, 0x03, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x34, 0x00, 0x02, 0x03, 0x01, 0x09, 0x01, 0x0e, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x35, 0x00, 0x02, 0x03, 0x0d, 0x0e, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x36, 0x00, 0x02, 0x11, 0x02, 0x12, 0x02, 0x03, 0x01, 0x0e, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x37, 0x00, 0x02, 0x11, 0x10, 0x12, 0x10, 0x03, 0x0d, 0x0e, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x38, 0x13, 0x00, 0x14, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x39, 0x13, 0x08, 0x14, 0x0f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x3a, 0x00, 0x01, 0x01, 0x01, 0x05, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x3b, 0x00, 0x02, 0x04, 0x02, 0x06, 0x02, 0x05, 0x02, 0x03, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x3c, 0x00, 0x02, 0x04, 0x02, 0x06, 0x02, 0x05, 0x02, 0x03, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x3d, 0x00, 0x02, 0x04, 0x10, 0x06, 0x10, 0x05, 0x10, 0x03, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x3e, 0x00, 0x03, 0x03, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
};


struct s_stream_description
{
    byte format;
    byte stride;
    byte unknown02[6];
    long offset;
    long unknown0c;
    long buffer;
};

// @retail 0x1cda0
void function_1cda0(s_stream_description const *stream)
{
    function_1c620((s_shader_cache *)g_51f0f0, stream->buffer, stream->stride,
        stream->offset, g_43f408[stream->format]);
}

extern word g_485ac0;
extern byte g_485ac2;
byte g_485ac3, g_485ac4, g_485ac5;
bool g_485ac6;

// @retail 0x123b0
void function_123b0(void)
{
    dword standard = XGetVideoStandard();
    dword flags = XGetVideoFlags();
    if (standard == 3)
        g_485ac0 = (byte)flags & 0x40 ? 60 : 50;
    byte wide = (byte)((flags >> 4) & 1);
    byte low = (byte)(flags & 1);
    flags &= 8;
    g_485ac2 = low;
    g_485ac3 = wide;
    g_485ac6 = (long)flags != 0;
    g_485ac5 = flags ? 1 : 0;
    g_485ac4 = standard == 3;
}

long g_467004 = NONE;

// @retail 0x13cd0
void function_13cd0(void)
{
    g_467004 = NONE;
    memset(g_51f3c8[1], 0, sizeof(g_51f3c8[1]));
    function_1cf50();
    D3DDevice_SetIndices(0, 0);
    D3DDevice_SetPixelShader(0);
}

// @retail 0x14ac0
void function_14ac0(void)
{
    D3DVIEWPORT8 viewport;
    D3DVIEWPORT8 saved;
    viewport.X = 0;
    viewport.Y = 0;
    viewport.Width = 640;
    viewport.Height = 480;
    viewport.MinZ = 0.0f;
    viewport.MaxZ = 1.0f;
    D3DDevice_GetViewport(&saved);
    D3DDevice_SetViewport(&viewport);
    D3DDevice_Clear(0, 0, 0xf0, 0, 0.0f, 0);
    D3DDevice_SetViewport(&saved);
}

long g_4858b8;

// @retail 0x1d4b0
void function_1d4b0(long format, bool alternate, bool *linear, long *result)
{
	*linear = false;
	switch (format)
	{
	case 0x10: case 0x11: case 0x12: case 0x13: case 0x16: case 0x17:
	case 0x1b: case 0x1c: case 0x1d: case 0x1e: case 0x1f: case 0x20:
	case 0x35: case 0x37: case 0x3d: case 0x3e: case 0x3f: case 0x40: case 0x41:
		*linear = true;
		break;
	}
	*result = NONE;
	switch (format)
	{
	case 25: *result = 0; break;
	case 0: *result = 1; break;
	case 1: *result = 2; break;
	case 26: *result = 3; break;
	case 5: *result = 6; break;
	case 2: *result = 8; break;
	case 4: *result = 9; break;
	case 7: *result = 10; break;
	case 6: *result = 11; break;
	case 12: *result = 14; break;
	case 14: *result = 15; break;
	case 15: *result = 16; break;
	case 11: *result = alternate ? 17 : 18; break;
	}
}
real g_485adc, g_485ae0;

extern byte *g_50934c;
extern double g_4858a0;

// @retail 0x137a0
void __stdcall function_137a0(real *first, real *second)
{
    real now = (real)g_4858a0;
    real *times;
    real *a;
    real *b;
    if (*g_50934c)
    {
        times = (real *)(g_50934c + 0x1c);
        a = (real *)(g_50934c + 0x24);
        b = (real *)(g_50934c + 0x2c);
    }
    else
    {
        times = (real *)(g_50934c + 4);
        a = (real *)(g_50934c + 0xc);
        b = (real *)(g_50934c + 0x14);
    }
    real x, y;
    if (now <= times[0])
    {
        x = a[0];
        y = b[0];
    }
    else if (!(now > times[1]) && times[1] - times[0] > 0.0001f)
    {
        real fraction = (now - times[0]) / (times[1] - times[0]);
        x = (a[1] - a[0]) * fraction + a[0];
        y = (b[1] - b[0]) * fraction + b[0];
    }
    else
    {
        x = a[1];
        y = b[1];
    }
    *first = x;
    *second = y;
}

// @retail 0x15720
void __stdcall function_15720(real x, real y)
{
    (void)&x;
    (void)&y;
    x = g_485adc;
    y = g_485ae0;
    real const *near_plane = &x;
    real const *far_plane = &y;
    D3DDevice_SetDepthClipPlanes(*near_plane, *far_plane, D3DSDCP_SET_VERTEXPROGRAM_PLANES);
}

byte g_4b6290;
extern dword g_4850c8;

// @retail 0x13630
bool function_13630(void)
{
    g_4b6290 = false;
    if (g_4850c8)
    {
        g_485adc = 0.0f;
        g_4858b8 = 0;
        g_485ae0 = 16777215.0f;
        function_14bc0(0, 0, true);
        function_15720(0.0f, 0.0f);
    }
    return true;
}

byte g_4858bc;
dword g_4b843c;

// @retail 0x4a780
void function_4a780(void)
{
    g_4858bc = false;
    g_4b843c = 0;
    D3DDevice_SetRenderState(D3DRS_STENCILENABLE, 0);
    D3DDevice_SetScissors(0, FALSE, 0);
    function_15720(0.0f, 0.0f);
    function_14bc0((short)g_4858b8, 0, true);
}

byte g_485b48[0x1fc0];
byte g_4670bc = true;
struct s_render_reset_state;
void function_16b10(s_render_reset_state *state);

// @retail 0x1bbd0
void function_1bbd0(void)
{
    g_4670bc = true;
    function_16b10((s_render_reset_state *)g_485b48);
}

typedef void (__stdcall *t_render_pair_callback)(long, long);
struct s_render_pair_request
{
    long first;
    long second;
    t_render_pair_callback callback;
};

// @retail 0x48e40
void __stdcall function_48e40(s_render_pair_request const *request)
{
    (void)&request;
    g_4670bc = true;
    function_16b10((s_render_reset_state *)g_485b48);
    request->callback(request->first, request->second);
}

struct s_frame_offset
{
    point3f position;
    vector3f forward;
    vector3f up;
};
extern s_frame_offset g_485618;
byte g_55e6c8;

// @retail 0x47870
void function_47870(long first, long second, point3f const *position, t_render_pair_callback callback)
{
    (void)&first;
    (void)&second;
    if (callback)
    {
        s_cache_record *record = function_1e2d0();
        if (record)
        {
            real x = position->x - g_485618.position.x;
            real y = position->y - g_485618.position.y;
            real z = position->z - g_485618.position.z;
            record->count = 3;
            *(real *)record->unknown04 = 0.0f - (g_485618.forward.k * z + g_485618.forward.j * y + g_485618.forward.i * x);
            *(void (__stdcall **)(s_render_pair_request const *))((byte *)record + 0x20) = function_48e40;
            *(point3f *)((byte *)record + 0x64) = *position;
            s_render_pair_request *request = (s_render_pair_request *)((byte *)record + 0x24);
            request->first = first;
            request->second = second;
            request->callback = callback;
        }
        else if (!g_55e6c8)
            g_55e6c8 = true;
    }
}


struct s_shader_constant_state
{
    byte unknown00[0x1630];
    real constants[16][4];
    long unknown1730;
    dword changed;
};

struct s_18d70_state
{
	byte unknown00[0x1530];
	real values[16][4];
	byte unknown1630[0x100];
	dword changed;
};

// @retail 0x18d70
void function_18d70(s_18d70_state *state)
{
	dword remaining = state->changed;
	while (remaining)
	{
		long index;
		__asm
		{
			bsf ecx, remaining
			mov index, ecx
		}
		D3DDevice_SetVertexData4f(index, state->values[index][0], state->values[index][1],
			state->values[index][2], state->values[index][3]);
		remaining &= ~(1 << index);
		state->changed &= ~(1 << index);
	}
}

// @retail 0x18e80
void function_18e80(s_shader_constant_state *state)
{
    dword remaining = state->changed;
    while (remaining)
    {
        long index;
        __asm
        {
            bsf ecx, remaining
            mov index, ecx
        }
        D3DDevice_SetVertexShaderConstantFast(index - 78, state->constants[index], 1);
        remaining &= ~(1 << index);
        state->changed &= ~(1 << index);
    }
}

extern s_record_pool *g_509434;
extern long g_4c1bd0;

// @retail 0x3d270
void function_3d270(void)
{
	g_509434 = data_new_inlined("cached object render states", 256, 256, 0, g_510c2c);
	g_4c1bd0 = NONE;
}

typedef dword (__stdcall *t_cache_hash)(void const *);
typedef bool (__stdcall *t_cache_compare)(void const *, void const *);
hash_table *function_13e1a0(char const *name, long data_size, long bucket_count,
    t_cache_hash hash, t_cache_compare compare, long maximum_count, c_data_allocator *allocator);

// @retail 0x1e110
bool function_1e110(void)
{
    volatile bool result = true;
    void *buffer = VirtualAlloc(0, 0x1c000, 0x101000, PAGE_READWRITE);
    if (!buffer) GetLastError();
    g_4b6280.records = (s_cache_record *)buffer;
    buffer = VirtualAlloc(0, 0x800, 0x101000, PAGE_READWRITE);
    if (!buffer) GetLastError();
    g_4b6280.buffer = buffer;
    g_4b6280.available = true;
    if (!g_4b6280.records)
        result = false;
    g_4b6280.count = 0;
    g_4b6280.unknown04 = 0;
    g_4b6280.unknown10 = 0;
    g_51f400 = function_13e1a0("transparent planes", 16, 4096,
        (t_cache_hash)function_1e8c0, (t_cache_compare)function_1e8f0, 2048, g_468758);
    return result;
}

bool function_13e270(hash_table *table, void *key, void const *data);

struct s_cache_source_entries
{
    byte unknown00[0x158];
    long count;
    s_cache_lookup_key *entries;
};

// @retail 0x1e1c0
void function_1e1c0(void)
{
    s_cache_source_entries *source = (s_cache_source_entries *)g_4e0348;
    if (g_51f400)
    {
        for (long i = 0; i < source->count; ++i)
        {
            s_cache_lookup_key *key = &source->entries[i];
            function_13e270(g_51f400, key, key->unknown04);
        }
    }
}

struct s_shader_transform_row
{
    real x, y, z;
    long unused;
};

struct s_shader_transform_entry
{
    short first;
    word count;
};

struct s_shader_transform_table
{
    long unknown00;
    s_shader_transform_entry entries[16];
    s_shader_transform_row transforms[1][3];
};

s_shader_transform_table *g_485a5c;
extern byte g_485af0;

// @retail 0x151e0
void function_151e0(long index)
{
    if (index != NONE)
    {
        s_shader_transform_table *data = g_485a5c;
        s_shader_transform_row *transform = data->transforms[data->entries[index].first];
        long count = data->entries[index].count * 3;
        D3DDevice_SetVertexShaderConstantFast(-46, transform, count);
        g_485af0 = true;
    }
    else
    {
        s_shader_transform_row transform[3];
        transform[0].x = 1.0f;
        transform[0].y = 0.0f;
        transform[0].z = 0.0f;
        transform[0].unused = 0;
        transform[1].x = 0.0f;
        transform[1].y = 1.0f;
        transform[1].z = 0.0f;
        transform[1].unused = 0;
        transform[2].x = 0.0f;
        transform[2].y = 0.0f;
        transform[2].z = 1.0f;
        transform[2].unused = 0;
        D3DDevice_SetVertexShaderConstantFast(-46, transform, 3);
    }
}


typedef void (__stdcall *t_1e4e0_callback)(void *);

struct s_1e4e0_record
{
	long type;
	real depth;
	long flags;
	byte active;
	byte unknown0d[0x13];
	void (__stdcall *callback)(void *);
	byte payload[0x40];
	point3f position;
};

extern point3f g_4b9da0;
extern dword g_4b8348;
extern real g_4b8494;
void function_35aa0(void);
typedef bool (__stdcall *t_sort_2byte_compare_function)(word, word, void const *);
void sort_2byte(word *elements, unsigned long count, void *unused, t_sort_2byte_compare_function compare, void const *context);

struct s_depth_sort_record
{
	long type;
	real depth;
	long flags;
	byte active;
	byte unknown0d[3];
	plane3f plane;
	byte unknown20[0x44];
	point3f position;
};

// @retail 0x1e700
bool __stdcall function_1e700(word first, word second, void const *context)
{
	s_depth_sort_record const *a = (s_depth_sort_record const *)context + (short)first;
	s_depth_sort_record const *b = (s_depth_sort_record const *)context + (short)second;
	long comparison = a->type - b->type;
	if (!comparison)
	{
		if (a->active)
		{
			real point = b->position.z * a->plane.k;
			point += b->position.y * a->plane.j;
			point += b->position.x * a->plane.i;
			// Retail rereads these record fields in camera-dot-product order.
			plane3f const volatile *camera_plane = &a->plane;
			real camera = camera_plane->k * g_4b9da0.z;
			camera = camera_plane->j * g_4b9da0.y + camera;
			camera = camera_plane->i * g_4b9da0.x + camera;
			long camera_side = a->plane.d > camera ? 1 : 0;
			long point_side = a->plane.d > point ? 1 : 0;
			comparison = camera_side != point_side ? 1 : -1;
		}
		else if (b->active)
		{
			real point = a->position.z * b->plane.k;
			point += a->position.y * b->plane.j;
			point += a->position.x * b->plane.i;
			plane3f const volatile *camera_plane = &b->plane;
			real camera = camera_plane->k * g_4b9da0.z;
			camera = camera_plane->j * g_4b9da0.y + camera;
			camera = camera_plane->i * g_4b9da0.x + camera;
			long camera_side = b->plane.d > camera ? 1 : 0;
			long point_side = b->plane.d > point ? 1 : 0;
			comparison = camera_side == point_side ? 1 : -1;
		}
		else if (a->depth > b->depth)
			comparison = 1;
		else if (b->depth > a->depth)
			comparison = -1;
		else
			comparison = a->flags - b->flags;
	}
	return comparison > 0;
}

// @retail 0x1e370
void function_1e370(void)
{
	long scratch;
	function_35aa0();
	long first = g_4b6280.unknown04;
	word *indices = (word *)g_4b6280.buffer;
	for (long i = first; i < g_4b6280.count; ++i)
		indices[i - first] = (word)i;
	sort_2byte(indices, g_4b6280.count - first, &scratch, function_1e700, g_4b6280.records);
	for (long i = 0; i < g_4b6280.count - (long)g_4b6280.unknown04; ++i)
	{
		s_1e4e0_record *record = (s_1e4e0_record *)&g_4b6280.records[((word *)g_4b6280.buffer)[i]];
		if (record->callback)
			record->callback(record->payload);
	}
	g_4b8348 = 0;
	D3DDevice_SetRenderState(D3DRS_STIPPLEENABLE, 0);
	if (!(fabs(g_4b8494 - 1.0f) < 0.0001f))
	{
		D3DDevice_SetStipple(function_1c290(1.0f));
		g_4b8494 = 1.0f;
	}
}

// @retail 0x1e4e0
bool function_1e4e0(point3f const *position, t_1e4e0_callback callback, void const *data, long size)
{
	(void)&callback;
	(void)&data;
	bool result = false;
	if (g_4b6280.count < 1024)
	{
		g_4b6280.records[g_4b6280.count].count = 0;
		s_1e4e0_record *record = (s_1e4e0_record *)&g_4b6280.records[g_4b6280.count];
		record->active = false;
		vector3f delta;
		delta.i = position->x - g_485618.position.x;
		delta.j = position->y - g_485618.position.y;
		delta.k = position->z - g_485618.position.z;
		++g_4b6280.count;
		if (size)
			memcpy(record->payload, data, size);
		record->type = 3;
		record->flags = 0;
		record->callback = callback;
		real depth = g_485618.forward.k * delta.k;
		depth += g_485618.forward.j * delta.j;
		depth += g_485618.forward.i * delta.i;
		record->depth = 0.0f - depth;
		record->position = *position;
		result = true;
	}
	else if (g_4b6280.available)
		g_4b6280.available = false;
	return result;
}


void __stdcall function_35b00(void *payload);

// @retail 0x1e5e0
bool function_1e5e0(point3f const *position, long size, long a, long b, long c,
	long d, long e, long f, long g, void const *data)
{
	(void)&a; (void)&b; (void)&c; (void)&d;
	(void)&e; (void)&f; (void)&g; (void)&data;
	bool result = false;
	if (g_4b6280.count < 1024)
	{
		s_1e4e0_record *record = (s_1e4e0_record *)&g_4b6280.records[g_4b6280.count++];
		record->type = 0;
		record->active = false;
		long *payload = (long *)record->payload;
		payload[0] = a;
		payload[1] = b;
		payload[2] = c;
		payload[3] = d;
		payload[4] = e;
		payload[5] = f;
		payload[6] = g;
		if (size > 0)
			memcpy(payload + 7, data, size);
		record->type = 3;
		record->flags = 0;
		record->callback = function_35b00;
		vector3f delta;
		delta.k = position->z - g_485618.position.z;
		delta.j = position->y - g_485618.position.y;
		delta.i = position->x - g_485618.position.x;
		real depth = g_485618.forward.k * delta.k;
		depth += g_485618.forward.j * delta.j;
		depth += g_485618.forward.i * delta.i;
		record->depth = 0.0f - depth;
		record->position = *position;
		result = true;
	}
	else if (g_4b6280.available)
		g_4b6280.available = false;
	return result;
}

DWORD const g_43fdc0[5] = {1, 2, 5, 8, 6};
bool g_47fe85 = true;
typedef void (__stdcall *t_4b220_fill)(void *, long, void *);

// @retail 0x4b220
long function_4b220(long mode, long primitive, long stride, t_4b220_fill fill, void *context, long count)
{
	long result = NONE;
	long const *mode_reference = &mode;
	(void)&primitive;
	(void)&stride;
	(void)&fill;
	(void)&context;
	if (!*mode_reference)
	{
		long bytes = count * stride;
		dword words = (dword)bytes >> 2;
		long requested = words + 5;
		if (requested < 2048)
		{
			function_1cf50();
			DWORD *push = D3DDevice_BeginPush(requested);
			*push++ = D3DPUSH_ENCODE(D3DPUSH_SET_BEGIN_END, 1);
			*push++ = g_43fdc0[primitive];
			*push++ = D3DPUSH_ENCODE(D3DPUSH_INLINE_ARRAY | D3DPUSH_NOINCREMENT_FLAG, words);
			fill(push, bytes, context);
			push += words;
			*push++ = D3DPUSH_ENCODE(D3DPUSH_SET_BEGIN_END, 1);
			*push++ = 0;
			D3DDevice_EndPush(push);
		}
		else if (g_47fe85)
			g_47fe85 = false;
	}
	return result;
}

real *table_entry_data(long handle);

// @retail 0x152a0
void function_152a0(long handle, long index)
{
    if (index != NONE)
    {
        s_shader_transform_table *data = (s_shader_transform_table *)table_entry_data(handle);
        s_shader_transform_row *transform = data->transforms[data->entries[index].first];
        long count = data->entries[index].count * 3;
        D3DDevice_SetVertexShaderConstantFast(-46, transform, count);
        g_485af0 = true;
    }
    else
    {
        s_shader_transform_row transform[3];
        transform[0].x = 1.0f;
        transform[0].y = 0.0f;
        transform[0].z = 0.0f;
        transform[0].unused = 0;
        transform[1].x = 0.0f;
        transform[1].y = 1.0f;
        transform[1].z = 0.0f;
        transform[1].unused = 0;
        transform[2].x = 0.0f;
        transform[2].y = 0.0f;
        transform[2].z = 1.0f;
        transform[2].unused = 0;
        D3DDevice_SetVertexShaderConstantFast(-46, transform, 3);
    }
}

long function_4cb80(long element_index, long tag_index);

struct s_render_transform_record
{
	dword unknown00;
	long tag;
	long handle;
	dword flags;
	byte unknown10[0xc];
	transform4x3f const *transform;
};

// @retail 0x4d830
void function_4d830(s_render_transform_record const *record)
{
	if (record->transform)
	{
		real const *matrix = (real const *)record->transform;
		real constants[12];
		constants[0] = matrix[1];
		constants[1] = matrix[4];
		constants[2] = matrix[7];
		constants[3] = matrix[10];
		constants[4] = matrix[2];
		constants[5] = matrix[5];
		constants[6] = matrix[8];
		constants[7] = matrix[11];
		constants[8] = matrix[3];
		constants[9] = matrix[6];
		constants[10] = matrix[9];
		constants[11] = matrix[12];
		D3DDevice_SetVertexShaderConstant(-46, constants, 3);
		g_5093d8 = 0;
	}
	else if (record->handle != NONE)
	{
		byte *data = (byte *)table_entry_data(record->handle);
		long size = 0;
		if (*(word *)data > 0)
			size = *(word *)data * 48 + 0x44;
		for (long i = size / 32; i > 0; --i, data += 32)
			_mm_prefetch((char const *)data, _MM_HINT_T0);
		dword flags = record->flags;
		if ((flags & 0xe0000000) == 0x20000000)
		{
			long index = function_4cb80((flags >> 9) & 0x1ff, record->tag);
			if (index != NONE)
			{
				s_shader_transform_table *table = (s_shader_transform_table *)table_entry_data(record->handle);
				s_shader_transform_row *constants;
				if (table->entries[flags & 15].count == 1)
					constants = table->transforms[table->entries[flags & 15].first];
				else
					constants = table->transforms[index];
				if (constants)
				{
					D3DDevice_SetVertexShaderConstant(-46, constants, 3);
					g_5093d8 = 0;
					return;
				}
			}
		}
		function_152a0(record->handle, flags & 15);
	}
}

real g_4b9fa4, g_4b9ff8, g_4b9f18, g_4b9f9c;

PRIVATE __forceinline real maximum_25ca0(real first, real second)
{
    real result = second;
    if (first > second)
        result = first;
    return result;
}

// @retail 0x25ca0
void function_25ca0(bool enabled)
{
    real divisor = maximum_25ca0(0.0001f, g_4b9fa4);
    real value = g_4b9ff8;
    value *= 1.0f / divisor;
    value = 0.0f - value;
    real constants[4];
    constants[0] = value < 0.0f ? 0.0f : value > 1.0f ? 1.0f : value;
    constants[1] = 0.0f;
    constants[2] = g_4b9f18;
    if (enabled)
    {
        constants[3] = g_4b9f9c;
        D3DDevice_SetVertexShaderConstant(-81, constants, 1);
    }
    else
    {
        constants[3] = 0.0f;
        D3DDevice_SetVertexShaderConstant(-81, constants, 1);
    }
}

// @retail 0x1cd90
void function_1cd90(void)
{
	function_1c710(g_51f0f0);
}

PRIVATE __forceinline void select_immediate_descriptor(long format)
{
	byte const *descriptor = g_43f408[format];
	function_1c6b0(g_51f0f0);
	s_shader_cache *state = (s_shader_cache *)g_51f0f0;
	state->wanted[0] = descriptor;
	if (state->current[0] != descriptor)
		state->descriptors_changed = true;
	function_1c710(g_51f0f0);
}

// @retail 0x1ccb0
void function_1ccb0(long format)
{
	select_immediate_descriptor(format);
}

// @retail 0x1cc60
void function_1cc60(long index)
{
	function_1c590((s_shader_cache *)g_51f0f0, NONE, index);
	select_immediate_descriptor(g_4670ec[index].field_0c);
}

// @retail 0x40870
bool function_40870(void)
{
	function_1e370();
	g_4b6280.count = g_4b6280.unknown04;
	return true;
}

struct s_queued_material_payload
{
	void (__stdcall *begin)(void *);
	t_4b220_fill fill;
	void (__stdcall *end)(void *);
	long stride;
	long format;
	long primitive;
	long count;
	byte data[1];
};

// @retail 0x35b00
void __stdcall function_35b00(void *payload)
{
	g_4670bc = true;
	function_16b10((s_render_reset_state *)g_485b48);
	s_queued_material_payload *request = (s_queued_material_payload *)payload;
	if (request->begin)
		request->begin(request->data);
	select_immediate_descriptor(request->format);
	function_1c710(g_51f0f0);
	function_1cf50();
	function_4b220(0, request->primitive, request->stride, request->fill, request->data, request->count);
	if (request->end)
		request->end(request->data);
}

typedef bool (__stdcall *t_1f3a0_callback)(s_363a0_vertex *vertices, void *context);

// @retail 0x1f3a0
void function_1f3a0(short const *rectangle, real x, real y, long width, long height,
    short u_offset, short v_offset, real scale, dword color,
    t_1f3a0_callback callback, void *context)
{
    s_363a0_vertex vertices[4];
    short u = rectangle[2] + u_offset;
    short v = rectangle[3] + v_offset;
    vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = color;
    vertices[1].x = vertices[2].x = (real)width * scale + x;
    vertices[0].x = vertices[3].x = x;
    vertices[2].y = vertices[3].y = (real)height * scale + y;
    vertices[0].u = vertices[3].u = (real)u;
    vertices[1].u = vertices[2].u = (real)(u + width);
    vertices[0].v = vertices[1].v = (real)v;
    vertices[0].y = vertices[1].y = y;
    vertices[2].v = vertices[3].v = (real)(v + height);
    if (!callback || callback(vertices, context))
        function_363a0(vertices);
}

void __stdcall function_423c0(void *payload);
void __stdcall function_4f010(void *payload);
void __stdcall function_508d0(void *payload);
extern vector3f g_4b9dac;

struct s_42760_entry
{
    real depth;
    point3f position;
    byte unknown10[0x10];
    long type;
};
s_42760_entry g_4c152c[32];
long g_4c19ac;

// @retail 0x42760
void function_42760(long flags)
{
    long count = g_4c19ac;
    long i = 0;
    if (count > 0)
    {
      do
      {
        s_42760_entry *entry = &g_4c152c[i];
        if (entry->type == 3)
        {
            if (g_4b6280.count < 1024)
            {
                s_1e4e0_record *record = (s_1e4e0_record *)&g_4b6280.records[g_4b6280.count];
                record->type = 0;
                record->active = false;
                ++g_4b6280.count;
                record->type = 3;
                record->flags = 0;
                record->callback = function_423c0;
                real *depth = &record->depth;
                point3f *position = &record->position;
                if (depth) *depth = entry->depth;
                if (position) *position = entry->position;
                ((long *)record->payload)[0] = i;
                ((long *)record->payload)[1] = flags;
                *depth = 0.0f - *depth;
            }
            else if (g_4b6280.available)
                g_4b6280.available = false;
        }
        ++i;
      } while (i < count);
    }
}

struct s_42850_payload
{
    long a, b;
    point3f position;
    vector3f first, second;
    real scale, width;
    vector3f third;
};

// @retail 0x42850
void function_42850(long a, long b, point3f const *position,
    vector3f const *first, vector3f const *second, real scale, real width,
    vector3f const *third)
{
    if (g_4b6280.count < 1024)
    {
        s_1e4e0_record *record = (s_1e4e0_record *)&g_4b6280.records[g_4b6280.count++];
        record->type = 0;
        record->active = false;
        s_42850_payload *payload = (s_42850_payload *)record->payload;
        payload->a = a;
        payload->b = b;
        payload->position = *position;
        payload->first = *first;
        payload->second = *second;
        payload->scale = scale;
        payload->width = width;
        payload->third = *third;
        vector3f delta;
        delta.i = position->x - g_4b9da0.x;
        delta.j = position->y - g_4b9da0.y;
        delta.k = position->z - g_4b9da0.z;
        record->type = 3;
        record->flags = 0;
        record->callback = function_4f010;
        real depth = g_4b9dac.k * delta.k;
        depth += g_4b9dac.j * delta.j;
        depth += g_4b9dac.i * delta.i;
        record->depth = 0.0f - depth;
        record->position = *position;
    }
    else if (g_4b6280.available)
        g_4b6280.available = false;
}

struct s_429a0_payload
{
    byte type, opacity;
    byte unknown02[2];
    long a, b, c;
    point3f position, endpoint;
    vector3f first, second;
};

// @retail 0x429a0
void function_429a0(byte type, long a, long b, long c, point3f const *position,
    point3f const *endpoint, vector3f const *first, vector3f const *second, real opacity)
{
    if (g_4b6280.count < 1024)
    {
        s_1e4e0_record *record = (s_1e4e0_record *)&g_4b6280.records[g_4b6280.count++];
        record->type = 0;
        record->active = false;
        s_429a0_payload *payload = (s_429a0_payload *)record->payload;
        payload->type = type;
        payload->a = a;
        payload->b = b;
        payload->c = c;
        payload->position = *position;
        payload->endpoint = *(endpoint ? endpoint : position);
        payload->first = *first;
        payload->second = *second;
        long value = (long)(opacity * 256.0f);
        payload->opacity = (byte)(value < 0 ? 0 : value > 255 ? 255 : value);
        vector3f delta;
        delta.i = position->x - g_4b9da0.x;
        delta.j = position->y - g_4b9da0.y;
        delta.k = position->z - g_4b9da0.z;
        record->type = 3;
        record->flags = 0;
        record->callback = function_508d0;
        real depth = g_4b9dac.k * delta.k;
        depth += g_4b9dac.j * delta.j;
        depth += g_4b9dac.i * delta.i;
        record->depth = 0.0f - depth;
        record->position = *position;
    }
    else if (g_4b6280.available)
        g_4b6280.available = false;
}

struct s_1c8c0_stream
{
    byte format, stride;
    byte unknown02[2];
    long offset;
    byte unknown08[8];
    long buffer;
};

PRIVATE __forceinline dword stream_register_mask(byte const *descriptor)
{
    dword mask = 0;
    signed char const *item = (signed char const *)descriptor + 1;
    while (*item != -1)
    {
        if (*item >= 0)
            mask |= 1 << *item;
        item += 2;
    }
    return mask;
}

// @retail 0x1c8c0
bool function_1c8c0(byte const *definition, dword mask,
    s_1c8c0_stream const *third, s_1c8c0_stream const *second, s_1c8c0_stream const *first)
{
    long count = 0;
    if (*(long const *)(definition + 0x38) > 0)
    {
      short i = 0;
      do
      {
        s_stream_description const *stream = (s_stream_description const *)(*(byte *const *)(definition + 0x3c) + i * 32);
        mask &= ~stream_register_mask(g_43f408[stream->format]);
        function_1c620((s_shader_cache *)g_51f0f0, stream->buffer, stream->stride, stream->offset, g_43f408[stream->format]);
        ++count;
        ++i;
      } while (i < *(long const *)(definition + 0x38));
    }
    if (first)
    {
        function_1c620((s_shader_cache *)g_51f0f0, first->buffer, first->stride, first->offset, g_43f408[first->format]);
        mask &= ~stream_register_mask(g_43f408[first->format]);
        ++count;
    }
    if (second)
    {
        function_1c620((s_shader_cache *)g_51f0f0, second->buffer, second->stride, second->offset, g_43f408[48]);
        mask &= ~stream_register_mask(g_43f408[48]);
        ++count;
    }
    if (third)
    {
        function_1c620((s_shader_cache *)g_51f0f0, third->buffer, third->stride, third->offset, g_43f408[third->format]);
        mask &= ~stream_register_mask(g_43f408[third->format]);
        ++count;
    }
    if (count > 0)
        function_1c710(g_51f0f0);
    return mask == 0;
}

// @retail 0x1caa0
bool function_1caa0(long tag, byte const *selection, word const *kind, byte const *definition,
    s_1c8c0_stream const *third, s_1c8c0_stream const *second, s_1c8c0_stream const *first)
{
    long mode = *(word const *)(selection + 0x14);
    signed char index;
    switch (*kind)
    {
    case 1: index = (signed char)selection[0x10]; break;
    case 2: index = (signed char)selection[0x10]; break;
    case 3: index = (signed char)selection[0x10]; break;
    case 4: index = (signed char)selection[0x11]; break;
    case 5: index = (signed char)selection[0x11]; break;
    default: index = (signed char)selection[0x10]; break;
    }
    long shader = function_1cb20(mode, index, tag);
    function_1c590((s_shader_cache *)g_51f0f0, tag, shader);
    dword mask = function_1cb70(tag, shader);
    return function_1c8c0(definition, mask, third, second, first);
}

long g_509350;
const dword g_47ffd8[53] = {
    0x00000001, 0x00000000, 0x00020000, 0x00000000, 0x00000000, 0x80000007, 0x00000000, 0x007bbef0,
    0x00000cf2, 0x00377101, 0x000000ff, 0x00001fff, 0x00001fff, 0x000000ff, 0x00084208, 0x0007ec87,
    0x1f030700, 0x1f030700, 0x03030300, 0x00000017, 0x00000017, 0x0000001f, 0x00001000, 0x00001000,
    0x00000200, 0x00002000, 0x00000000, 0x00000004, 0x501502f9, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};
dword g_484ce8[53];

// @retail 0x12490
bool function_12490()
{
    bool result = true;
    D3DPRESENT_PARAMETERS parameters;
    memset(&parameters, 0, sizeof(parameters));
    g_509350 = 1;
    parameters.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
    parameters.Windowed = false;
    parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    parameters.EnableAutoDepthStencil = true;
    parameters.AutoDepthStencilFormat = D3DFMT_D24S8;
    parameters.BackBufferFormat = D3DFMT_A8R8G8B8;
    parameters.FullScreen_RefreshRateInHz = (short)g_485ac0;
    parameters.BackBufferWidth = 640;
    parameters.BackBufferHeight = 480;
    parameters.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    if (g_485ac2)
        parameters.Flags |= D3DPRESENTFLAG_WIDESCREEN;
    if (g_485ac6)
        parameters.Flags |= D3DPRESENTFLAG_PROGRESSIVE;
    D3DDevice *device = NULL;
    Direct3D_CreateDevice(0, D3DDEVTYPE_HAL, NULL, D3DCREATE_HARDWARE_VERTEXPROCESSING, &parameters, &device);
    g_5093b0 = device;
    if (!g_5093b0)
    {
        g_5093b0 = NULL;
        return false;
    }
    memcpy(g_484ce8, g_47ffd8, sizeof(g_484ce8));
    return result;
}

struct s_type_7ba8e9;
long function_136490(s_type_7ba8e9 const *bitmap);

// @retail 0x1d2f0
bool __stdcall function_1d2f0(D3DSurface *surface, byte *bitmap)
{
    volatile bool result = true;
    D3DSURFACE_DESC description;
    D3DSurface_GetDesc(surface, &description);
    memset(bitmap, 0, 0x74);
    *(dword *)bitmap = 0x6269746d;
    *(short *)(bitmap + 4) = (short)description.Width;
    *(short *)(bitmap + 6) = (short)description.Height;
    bitmap[8] = 1;
    *(short *)(bitmap + 0xa) = 0;
    bool linear;
    long format;
    function_1d4b0(description.Format, false, &linear, &format);
    if (format == NONE)
        return false;
    *(short *)(bitmap + 0xc) = (short)format;
    if (!linear)
        *(word *)(bitmap + 0xe) |= 1;
    else
        *(word *)(bitmap + 0xe) |= 0x10;
    if ((short)format >= 14 && (short)format <= 16)
        *(word *)(bitmap + 0xe) |= 2;
    if ((short)format == 18)
        *(word *)(bitmap + 0xe) |= 4;
    *(short *)(bitmap + 0x14) = 1;
    D3DLOCKED_RECT locked;
    D3DSurface_LockRect(surface, &locked, NULL, D3DLOCK_NOOVERWRITE);
    *(void **)(bitmap + 0x54) = locked.pBits;
    *(long *)(bitmap + 0x34) = function_136490((s_type_7ba8e9 const *)bitmap);
    return result;
}

D3DSurface g_484f50;

// @retail 0x14850
bool function_14850()
{
    volatile bool result = true;
    D3DTexture *texture;
    function_0158f0(0, 11, 64, 64, 0, 1, &texture);
    g_509374 = texture;
    g_509378 = D3DTexture_GetSurfaceLevel2(texture, 0);
    function_0158f0(0, 11, 64, 64, 0, 1, &texture);
    g_50937c = texture;
    g_509380 = D3DTexture_GetSurfaceLevel2(texture, 0);
    if (!g_509374 || !g_509378 || !g_50937c || !g_509380)
        result = false;
    g_484f50.Common = 0x50001;
    g_484f50.Data = 0;
    g_484f50.Lock = 0;
    g_484f50.Format = 0x11229;
    g_484f50.Size = 0x1f1ff1ff;
    g_484f50.Parent = NULL;
    g_509384 = &g_484f50;
    return result;
}

struct short_rect_pair
{
    struct { short v0, v1, v2, v3; } a, b;
};
extern short_rect_pair g_485a8a;
extern short g_485aca;
struct s_128c0_settings
{
    byte depth_format, interval_flag;
    short interval;
    long quality, extra;
};
word g_485ac8;
long g_485acc, g_485ad0;
long g_467008, g_46700c;
bool g_5093bc;

// @retail 0x128c0
bool function_128c0(s_128c0_settings const *settings)
{
    g_485ac8 = *(word const *)settings;
    g_485aca = settings->interval;
    g_485acc = settings->quality;
    g_485ad0 = settings->extra;
    volatile bool result = true;
    D3DPRESENT_PARAMETERS parameters;
    memset(&parameters, 0, sizeof(parameters));
    parameters.Windowed = false;
    parameters.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
    parameters.EnableAutoDepthStencil = true;
    parameters.AutoDepthStencilFormat = (D3DFORMAT)(D3DFMT_D24S8 + ((byte)g_485ac8 != 0));
    parameters.BackBufferFormat = D3DFMT_A8R8G8B8;
    parameters.BackBufferWidth = g_485a8a.a.v3 - g_485a8a.a.v1;
    parameters.BackBufferHeight = g_485a8a.a.v2 - g_485a8a.a.v0;
    switch (g_485aca)
    {
    case 0:
        parameters.SwapEffect = D3DSWAPEFFECT_FLIP;
        parameters.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
        break;
    case 1:
        parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
        parameters.FullScreen_PresentationInterval = 1 + ((g_485ac8 >> 8) ? 0x80000000 : 0);
        break;
    case 2:
        parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
        parameters.FullScreen_PresentationInterval = 2 + ((g_485ac8 >> 8) ? 0x80000000 : 0);
        break;
    default:
        parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
        g_485aca = 1;
        parameters.FullScreen_PresentationInterval = 0;
        break;
    }
    parameters.FullScreen_RefreshRateInHz = (short)g_485ac0;
    if (!g_5093b0)
    {
        D3DDevice *device = NULL;
        Direct3D_CreateDevice(0, D3DDEVTYPE_HAL, NULL, D3DCREATE_HARDWARE_VERTEXPROCESSING, &parameters, &device);
        g_5093b0 = device;
    }
    else
        D3DDevice_Reset(&parameters);
    if (!g_5093b0)
    {
        g_5093b0 = NULL;
        return false;
    }
    memcpy(g_484ce8, g_47ffd8, sizeof(g_484ce8));
    if (g_485acc < 0 || g_485acc > 5)
        g_485acc = 3;
    g_467008 = NONE;
    g_46700c = NONE;
    g_5093bc = true;
    return result;
}

// @retail 0x13d80
void function_13d80()
{
    if (g_5093c8 && (short)(g_5093c0.first >> 16) >= 0 && (short)g_5093c0.first >= 0 &&
        (short)(g_5093c0.second >> 16) <= 640 && (short)g_5093c0.second <= 480)
    {
        D3DSurface *surface = D3DDevice_GetBackBuffer2(0);
        D3DSURFACE_DESC description;
        D3DSurface_GetDesc(surface, &description);
        if (description.Size == description.Width * description.Height * 4)
        {
            D3DLOCKED_RECT locked;
            D3DSurface_LockRect(surface, &locked, NULL, 0);
            locked.pBits = (void *)((dword)locked.pBits | 0xf0000000);
            if (locked.pBits)
            {
                long width = (short)(g_5093c0.second >> 16) - (short)(g_5093c0.first >> 16);
                long height = (short)g_5093c0.second - (short)g_5093c0.first;
                byte const *source = (byte const *)g_5093c8;
                byte *row = (byte *)locked.pBits + (short)g_5093c0.first * locked.Pitch;
                for (long y = 0; y < height; ++y)
                {
                    dword *pixel = (dword *)row + (short)(g_5093c0.first >> 16);
                    for (long x = 0; x < width; ++x)
                    {
                        long value = source[x] * g_5093cc;
                        byte level = (byte)(value < 0 ? 0 : value > 255 ? 255 : value);
                        *pixel++ = (((((dword)level << 8) | level) << 8 | level) << 8) | level;
                    }
                    row += locked.Pitch;
                    source += width;
                }
            }
            D3DSurface_UnlockRect(surface);
        }
        D3DResource_Release(surface);
    }
}

extern D3DResource *g_485ae4, *g_485ae8, *g_485aec;

// @retail 0x1d0e0
bool function_1d0e0()
{
    D3DTexture *texture;
    if (!function_0158f0(0, 9, 4, 4, 0, 0, &texture))
    {
        g_485ae4 = texture;
        return false;
    }
    g_485ae4 = texture;
    D3DVolumeTexture *volume;
    if (!function_015a60(9, 4, 4, 4, 0, 0, &volume))
    {
        g_485ae8 = volume;
        return false;
    }
    g_485ae8 = volume;
    D3DCubeTexture *cube;
    if (!function_0159b0(4, 9, 0, 0, &cube))
    {
        g_485aec = cube;
        return false;
    }
    g_485aec = cube;
    bool result = true;
    if (!g_485ae4 || !g_485ae8 || !g_485aec)
        return false;
    word colors[2] = { 0x0f00, 0xf0f0 };
    D3DLOCKED_RECT rectangle;
    D3DTexture_LockRect((D3DTexture *)g_485ae4, 0, &rectangle, NULL, 0);
    for (long i = 0; i < 16; ++i)
        ((word *)rectangle.pBits)[i] = colors[i & 1];
    D3DTexture_UnlockRect((D3DTexture *)g_485ae4, 0);
    D3DLOCKED_BOX box;
    D3DVolumeTexture_LockBox((D3DVolumeTexture *)g_485ae8, 0, &box, NULL, 0);
    for (long j = 0; j < 64; ++j)
        ((word *)box.pBits)[j] = colors[j & 1];
    D3DVolumeTexture_UnlockBox((D3DVolumeTexture *)g_485ae8, 0);
    for (long face = 0; face < 6; ++face)
    {
        D3DCubeTexture_LockRect((D3DCubeTexture *)g_485aec, (D3DCUBEMAP_FACES)face, 0, &rectangle, NULL, 0);
        for (long k = 0; k < 16; ++k)
            ((word *)rectangle.pBits)[k] = colors[k & 1];
        D3DCubeTexture_UnlockRect((D3DCubeTexture *)g_485aec, (D3DCUBEMAP_FACES)face, 0);
    }
    return result;
}

extern D3DPIXELSHADERDEF g_484f68;
extern long g_4b6298;
extern bool g_4b6294;
void function_0222d0(D3DRENDERSTATETYPE state, dword value);

// @retail 0x1e930
void __stdcall function_1e930(long opaque)
{
    memset(&g_484f68, 0, sizeof(g_484f68));
    g_484f68.PSTextureModes = 0;
    g_484f68.PSCombinerCount = 1;
    g_484f68.PSFinalCombinerInputsABCD = 4;
    function_0222d0(D3DRS_CULLMODE, 0);
    function_0222d0(D3DRS_COLORWRITEENABLE, 0x10101);
    function_0222d0(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    function_0222d0(D3DRS_ZENABLE, 2);
    function_0222d0(D3DRS_ZBIAS, 8);
    function_0222d0(D3DRS_STENCILENABLE, 0);
    if ((byte)opaque)
    {
        function_0222d0(D3DRS_ALPHABLENDENABLE, 0);
        function_0222d0(D3DRS_ALPHATESTENABLE, 0);
        function_0222d0(D3DRS_ZWRITEENABLE, 1);
    }
    else
    {
        function_0222d0(D3DRS_ALPHABLENDENABLE, 1);
        function_0222d0(D3DRS_ALPHATESTENABLE, 0);
        function_0222d0(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        function_0222d0(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        function_0222d0(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        function_0222d0(D3DRS_ZWRITEENABLE, 0);
        g_484f68.PSFinalCombinerInputsEFG = 0x1400;
    }
    function_1c590((s_shader_cache *)g_51f0f0, *(long *)(*(byte **)(g_485a80 + 0x5c) + 0x14), 0);
    function_1c710(g_51f0f0);
    function_15180(&g_484f68);
    function_14bc0((short)g_4858b8, 0, true);
    function_1cf50();
    g_4b6298 = 0;
    g_4b6294 = true;
}

extern real g_48565c;
real g_48568c[46];
bool g_55e6bc;

// @retail 0x12d50
void function_12d50(real const *projection, bool scaled, real *output)
{
    real constants[16];
    D3DDevice_SetDepthClipPlanes((scaled ? 0.15f : 1.0f) * g_485adc, g_485ae0,
        D3DSDCP_SET_VERTEXPROGRAM_PLANES);
    if (!projection)
        projection = g_48568c;
    if (!output)
        output = constants;
    real scale = 16777215.0f / g_48565c;
    for (long i = 0; i < 4; ++i)
    {
        output[i * 4] = (projection[38 + i] * projection[3] + projection[1] * projection[30 + i]
            + projection[34 + i] * projection[2]) * scale;
        output[i * 4 + 1] = (projection[38 + i] * projection[6] + projection[30 + i] * projection[4]
            + projection[34 + i] * projection[5]) * scale;
        output[i * 4 + 2] = (projection[38 + i] * projection[9] + projection[8] * projection[34 + i]
            + projection[30 + i] * projection[7]) * scale;
        output[i * 4 + 3] = (projection[38 + i] * projection[12] + projection[11] * projection[34 + i]
            + projection[30 + i] * projection[10]) * scale;
        output[i * 4 + 3] += projection[42 + i] * scale;
    }
    if (scaled)
        for (long j = 0; j < 16; ++j)
            output[j] *= 0.15f;
    if (output == constants && scaled != g_55e6bc)
    {
        D3DDevice_SetVertexShaderConstant(-96, output, 4);
        g_55e6bc = scaled;
    }
}

// @retail 0x14600
bool function_14600()
{
    volatile bool result = true;
    g_50935c = D3DDevice_GetBackBuffer2(0);
    g_509360 = D3DDevice_GetBackBuffer2(1);
    g_509364 = D3DDevice_GetDepthStencilSurface2();
    if (g_50935c && g_509360 && g_509364)
    {
        void *memory = VirtualAlloc(NULL, 20, 0x101000, PAGE_READWRITE);
        if (!memory) GetLastError();
        g_509354[0] = (D3DTexture *)memory;
        memory = VirtualAlloc(NULL, 20, 0x101000, PAGE_READWRITE);
        if (!memory) GetLastError();
        g_509354[1] = (D3DTexture *)memory;
        if (g_509354[0] && g_509354[1])
        {
            for (long i = 0; i < 2; ++i)
            {
                D3DTexture *texture = g_509354[i];
                texture->Common = 0x40001;
                texture->Data = (i ? g_509360 : g_50935c)->Data;
                texture->Lock = 0;
                texture->Size = 0x271df27f;
                texture->Format = 0x11229;
            }
        }
        else
            result = false;
    }
    else
        result = false;
    void *memory = VirtualAlloc(NULL, 24, 0x101000, PAGE_READWRITE);
    if (!memory) GetLastError();
    g_509370 = (D3DSurface *)memory;
    if (g_509370)
    {
        *g_509370 = *g_50935c;
        g_509370->Data = g_509364->Data;
    }
    else
        result = false;
    memory = VirtualAlloc(NULL, 20, 0x101000, PAGE_READWRITE);
    if (!memory) GetLastError();
    g_509368 = (D3DTexture *)memory;
    if (g_509368)
    {
        g_509368->Common = 0x40001;
        g_509368->Data = g_509364->Data;
        g_509368->Lock = 0;
        g_509368->Size = 0x271df27f;
        g_509368->Format = 0x11229;
    }
    else
        result = false;
    memory = VirtualAlloc(NULL, 20, 0x101000, PAGE_READWRITE);
    if (!memory) GetLastError();
    g_50936c = (D3DTexture *)memory;
    if (g_50936c)
    {
        g_50936c->Common = 0x40001;
        g_50936c->Data = g_509364->Data;
        g_50936c->Lock = 0;
        g_50936c->Size = 0x271df27f;
        g_50936c->Format = 0x13f29;
    }
    else
        result = false;
    return result;
}

// @retail 0x1eb00
void function_1eb00()
{
    function_0222d0(D3DRS_CULLMODE, 0);
    function_0222d0(D3DRS_ALPHABLENDENABLE, 0);
    function_0222d0(D3DRS_ALPHATESTENABLE, 0);
    function_0222d0(D3DRS_ZENABLE, 0);
    function_0222d0(D3DRS_ZBIAS, 0);
    function_1c590((s_shader_cache *)g_51f0f0, *(long *)(*(byte **)(g_485a80 + 0x5c) + 0x6c), 0);
    function_1c710(g_51f0f0);
    real x = 1.0f / (short)(g_48564e - g_48564a);
    real y = 1.0f / (short)(g_48564c - g_485648);
    real constants[20];
    constants[0] = x * 2.0f;
    constants[1] = 0.0f;
    constants[2] = 0.0f;
    constants[3] = -1.0f - x;
    constants[4] = 0.0f;
    constants[5] = y * -2.0f;
    constants[6] = 0.0f;
    constants[7] = y + 1.0f;
    constants[8] = 0.0f;
    constants[9] = 0.0f;
    constants[10] = 0.0f;
    constants[11] = 0.5f;
    constants[12] = 0.0f;
    constants[13] = 0.0f;
    constants[14] = 0.0f;
    constants[15] = 1.0f;
    constants[16] = 1.0f;
    constants[17] = 1.0f;
    constants[18] = 0.0f;
    constants[19] = 1.0f;
    D3DDevice_SetVertexShaderConstant(81, constants, 5);
    memset(&g_484f68, 0, sizeof(g_484f68));
    g_484f68.PSTextureModes = 0;
    g_484f68.PSCombinerCount = 1;
    g_484f68.PSFinalCombinerInputsABCD = 4;
    D3DDevice_SetPixelShaderProgram(&g_484f68);
    function_1cf50();
    g_4b6298 = 0;
    g_4b6294 = true;
}

// @retail 0x36580
void function_36580()
{
    g_4670bc = true;
    function_16b10((s_render_reset_state *)g_485b48);
    function_0222d0(D3DRS_COLORWRITEENABLE, 0x10101);
    function_0222d0(D3DRS_ALPHABLENDENABLE, 1);
    function_0222d0(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    function_0222d0(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    function_0222d0(D3DRS_ZENABLE, 0);
    function_0222d0(D3DRS_ZBIAS, 0);
    function_0222d0(D3DRS_CULLMODE, 0);
    real x = 1.0f / (short)(g_48564e - g_48564a);
    real y = 1.0f / (short)(g_48564c - g_485648);
    real constants[20];
    constants[0] = x * 2.0f;
    constants[1] = 0.0f;
    constants[2] = 0.0f;
    constants[3] = -1.0f - x;
    constants[4] = 0.0f;
    constants[5] = y * -2.0f;
    constants[6] = 0.0f;
    constants[7] = y + 1.0f;
    constants[8] = 0.0f;
    constants[9] = 0.0f;
    constants[10] = 0.0f;
    constants[11] = 0.5f;
    constants[12] = 0.0f;
    constants[13] = 0.0f;
    constants[14] = 0.0f;
    constants[15] = 1.0f;
    constants[16] = 1.0f;
    constants[17] = 1.0f;
    constants[18] = 0.0f;
    constants[19] = 1.0f;
    D3DDevice_SetVertexShaderConstant(81, constants, 5);

    real extra[24];
    memset(extra, 0, sizeof(extra));
    D3DDevice_SetVertexShaderConstant(86, extra, 6);
    memset(&g_484f68, 0, sizeof(g_484f68));
    g_484f68.PSTextureModes = 0;
    g_484f68.PSCombinerCount = 1;
    g_484f68.PSRGBInputs[0] = 0x20040000;
    g_484f68.PSAlphaInputs[0] = 0x20140000;
    g_484f68.PSRGBOutputs[0] = 0xc0;
    g_484f68.PSAlphaOutputs[0] = 0xc0;
    g_484f68.PSFinalCombinerInputsABCD = 0xc;
    g_484f68.PSFinalCombinerInputsEFG = 0x1c00;
    D3DDevice_SetPixelShaderProgram(&g_484f68);
    function_1cc60(0);
}


extern byte g_4c1a18;

// @retail 0x36560
void function_36560()
{
    if (g_4c1a18)
    {
        function_1e930(1);
        g_4c1a18 = 0;
    }
}

// @retail 0x12420
bool function_12420()
{
    function_123b0();
    bool result = function_12490();
    if (result)
    {
        D3DDevice_Swap(0);
        if (g_5093b0)
        {
            D3DDevice_Release();
            g_5093b0 = NULL;
        }
    }
    return result;
}



struct s_44940_entry
{
    dword unknown00;
    long tag;
    dword unknown08;
    dword flags;
    byte unknown10[0x10];
};
extern s_44940_entry g_4ba138[850];
extern byte g_485a75, g_485a76;
void function_15370(short mode);

void function_3c650(byte const *state);
void function_3d000(real const *state);

// @retail 0x467c0
short __stdcall function_467c0(short first, short second, short count)
{
    if (count > 0)
    {
        function_0222d0(D3DRS_CULLMODE, 0x901);
        function_0222d0(D3DRS_COLORWRITEENABLE, 0x10101);
        function_0222d0(D3DRS_ALPHABLENDENABLE, 1);
        function_0222d0(D3DRS_SRCBLEND, 0x304);
        function_0222d0(D3DRS_DESTBLEND, 0);
        function_0222d0(D3DRS_BLENDOP, 0x8006);
        function_0222d0(D3DRS_ALPHATESTENABLE, 0);
        function_0222d0(D3DRS_ZENABLE, 0);
        function_0222d0(D3DRS_ZBIAS, 0);
        function_1c590((s_shader_cache *)g_51f0f0, *(long *)(*(byte **)(g_485a80 + 0x5c) + 0xc), 0);
        function_1c710(g_51f0f0);
        real constants[32] = {
            1.0f,0.0f,0.0f,-0.0078125f, 0.0f,1.0f,0.0f,-0.0078125f,
            1.0f,0.0f,0.0f,0.0078125f, 0.0f,1.0f,0.0f,0.0078125f,
            1.0f,0.0f,0.0f,-0.0078125f, 0.0f,1.0f,0.0f,0.0078125f,
            1.0f,0.0f,0.0f,0.0078125f, 0.0f,1.0f,0.0f,-0.0078125f
        };
        D3DDevice_SetVertexShaderConstant(18, constants, 8);
        D3DPIXELSHADERDEF program;
        memset(&program, 0, sizeof(program));
        program.PSAlphaOutputs[0] = 0xc00;
        program.PSRGBOutputs[0] = 0xc00;
        program.PSRGBOutputs[1] = 0xc00;
        program.PSTextureModes = 0x8421;
        program.PSCombinerCount = 2;
        program.PSConstant0[0] = 0xff000000;
        program.PSAlphaInputs[0] = 0x08a009a0;
        program.PSRGBInputs[0] = 0x0aa00ba0;
        program.PSRGBInputs[1] = 0x1c110c11;
        program.PSFinalCombinerInputsABCD = 0xc;
        g_484f68 = program;
        D3DDevice_SetPixelShaderProgram(&program);
        for (short i = 0; i < count; ++i)
        {
            short source = i & 1 ? second : first;
            short target = i & 1 ? first : second;
            for (short stage = 0; stage < 4; ++stage)
            {
                function_14f60(stage, source);
                D3DDevice_SetTextureStageState(stage, D3DTSS_ADDRESSU, 4);
                D3DDevice_SetTextureStageState(stage, D3DTSS_ADDRESSV, 4);
                D3DDevice_SetTextureStageState(stage, D3DTSS_MAGFILTER, 2);
                D3DDevice_SetTextureStageState(stage, D3DTSS_MINFILTER, 2);
                D3DDevice_SetTextureStageState(stage, D3DTSS_MIPFILTER, 1);
                D3DDevice_SetTextureStageState(stage, D3DTSS_MIPMAPLODBIAS, 0);
                D3DDevice_SetTextureStageState(stage, D3DTSS_MAXMIPLEVEL, 0);
                D3DDevice_SetTextureStageState(stage, D3DTSS_MAXANISOTROPY, 0);
                D3DDevice_SetTextureStageState(stage, D3DTSS_COLORSIGN, 0);
                D3DDevice_SetTextureStageState(stage, D3DTSS_ALPHAKILL, 0);
            }
            function_14bc0(target, 0, false);
            program.PSConstant0[0] = (i > 0 ? 0x7fu : 0xffu) << 24;
            g_484f68 = program;
            D3DDevice_SetPixelShaderProgram(&program);
            function_1cf50();
            D3DDevice_Begin(D3DPT_TRIANGLEFAN);
            D3DDevice_SetVertexData2s(3, 0, 0);
            D3DDevice_SetVertexData4f(0, 0.53125f, 0.53125f, 16777215.0f, 16777215.0f);
            D3DDevice_SetVertexData2s(3, 1, 0);
            D3DDevice_SetVertexData4f(0, 64.53125f, 0.53125f, 16777215.0f, 16777215.0f);
            D3DDevice_SetVertexData2s(3, 1, 1);
            D3DDevice_SetVertexData4f(0, 64.53125f, 64.53125f, 16777215.0f, 16777215.0f);
            D3DDevice_SetVertexData2s(3, 0, 1);
            D3DDevice_SetVertexData4f(0, 0.53125f, 64.53125f, 16777215.0f, 16777215.0f);
            D3DDevice_End();
        }
        function_14bc0((short)g_4858b8, 0, true);
    }
    return count & 1 ? second : first;
}

// @retail 0x4d640
void __stdcall function_4d640(short index, long mode, bool setup)
{
    (void)&index;
    s_44940_entry *entry = &g_4ba138[index];
    if (entry->unknown10[10] != 0xff)
    {
        real opacity = (real)entry->unknown10[10] * (1.0f / 255.0f);
        if (!(fabs(g_4b8494 - opacity) < 0.0001f))
        {
            D3DDevice_SetStipple(function_1c290(opacity));
            g_4b8494 = opacity;
        }
    }
    if (entry->unknown00 & 0x800)
        function_12d50(NULL, true, NULL);
    if (mode == 1 || mode == 3)
    {
        if (g_485a75 || g_485a76)
            function_15370(0);
        else if (entry->unknown00 & 0x4000)
            function_15370(2);
    }
    if (setup)
    {
        byte *structure = (byte *)g_4e0348;
        if (*(long *)(structure + 0x214) > 0)
        {
            byte *state = *(byte **)(structure + 0x218);
            function_3c650(state);
            function_3d000((real const *)state);
        }
    }
}

// @retail 0x4d720
void function_4d720(short index, long mode)
{
    s_44940_entry *entry = &g_4ba138[index];
    if (entry->unknown10[10] != 0xff && !(fabs(g_4b8494 - 1.0f) < 0.0001f))
    {
        D3DDevice_SetStipple(function_1c290(1.0f));
        g_4b8494 = 1.0f;
    }
    if (entry->unknown00 & 0x800)
        function_12d50(NULL, false, NULL);
    if ((mode == 1 || mode == 3) && ((entry->unknown00 & 0x4000) || g_485a75 || g_485a76))
        function_15370(1);
}



#include "visibility_slot.h"
typedef long (__stdcall *visibility_draw_callback)(s_slot *slot, byte *data);
void __stdcall function_20bb0(long player, dword mask, long const *indices, long count, bool query, visibility_draw_callback draw);
long __stdcall function_2e540(s_slot *slot, byte *data);
extern long g_4b9ed4;
extern long g_485898;
extern vector3f g_4b9dac;

// @retail 0x2e540
long __stdcall function_2e540(s_slot *slot, byte *data)
{
    long result = 0;
    long tag = *(long *)data;
    point3f position = *(point3f *)(data + 4);
    vector3f offset;
    offset.i = data[0x10] * (2.0f / 255.0f) - 1.0f;
    offset.j = data[0x11] * (2.0f / 255.0f) - 1.0f;
    offset.k = data[0x12] * (2.0f / 255.0f) - 1.0f;
    byte *definition = g_4e3b44[tag & 0xffff].bytes;
    real scale = definition[0x28] & 0x40 ? *(real *)(data + 0x14) : 1.0f;
    if (tag != NONE && !(definition[0x28] & 2))
    {
        real radius = *(real *)(definition + 0x10) * scale;
        if ((*(dword *)slot & 0x38) == 0x20)
        {
            real sizes[7] = { 1.0f, 0.5f, 0.25f, 0.125f, 0.0625f, 0.03125f, 0.015625f };
            radius *= sizes[*(short *)(definition + 0x16)];
        }
        switch (*(short *)(definition + 0x14))
        {
        case 0:
            position.x += -radius * g_4b9dac.i;
            position.y += -radius * g_4b9dac.j;
            position.z += -radius * g_4b9dac.k;
            break;
        case 1:
            position.x += offset.i * (radius * 1.4142135381698608f);
            position.y += offset.j * (radius * 1.4142135381698608f);
            position.z += offset.k * (radius * 1.4142135381698608f);
            break;
        }
        real screen[4], extent[2];
        if (function_48b00(&position, radius, screen, extent))
        {
            if (extent[0] < 2.0f) extent[0] = 2.0f;
            else if (extent[0] > 128.0f) extent[0] = 128.0f;
            if (extent[1] < 2.0f) extent[1] = 2.0f;
            else if (extent[1] > 128.0f) extent[1] = 128.0f;
            real left = (real)floor((double)screen[0] - extent[0]);
            real top = (real)floor((double)screen[1] - extent[1]);
            real right = (real)floor((double)screen[0] + extent[0]);
            real bottom = (real)floor((double)screen[1] + extent[1]);
            real area = (bottom - top) * (right - left);
            if (area > 0.0f && area <= 2147483648.0f)
            {
                if (screen[2] < 0.0f) screen[2] = 0.0f;
                else if (screen[2] > 16777215.0f) screen[2] = 16777215.0f;
                if (screen[3] < 0.0f) screen[3] = 0.0f;
                else if (screen[3] > 16777215.0f) screen[3] = 16777215.0f;
                D3DDevice_Begin(D3DPT_TRIANGLEFAN);
                D3DDevice_SetVertexData4f(0, left, top, screen[2], screen[3]);
                D3DDevice_SetVertexData4f(0, right, top, screen[2], screen[3]);
                D3DDevice_SetVertexData4f(0, right, bottom, screen[2], screen[3]);
                D3DDevice_SetVertexData4f(0, left, bottom, screen[2], screen[3]);
                D3DDevice_End();
                result = (long)area;
            }
        }
    }
    return result;
}

// @retail 0x2dd30
void function_2dd30()
{
    g_4670bc = true;
    function_16b10((s_render_reset_state *)g_485b48);
    function_14bc0(g_485602, 0, true);
    function_15370(0);
    if (!(fabs(g_4b8494 - 1.0f) < 0.0001f))
    {
        D3DDevice_SetStipple(function_1c290(1.0f));
        g_4b8494 = 1.0f;
    }
    function_0222d0(D3DRS_ALPHABLENDENABLE, 0);
    function_0222d0(D3DRS_COLORWRITEENABLE, 0);
    function_0222d0(D3DRS_ALPHATESTENABLE, 0);
    function_0222d0(D3DRS_ZWRITEENABLE, 0);
    function_0222d0(D3DRS_ZENABLE, 2);
    function_0222d0(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    function_0222d0(D3DRS_CULLMODE, 0);
    function_0222d0(D3DRS_STENCILENABLE, 0);
    function_0222d0(D3DRS_ZBIAS, 0);
    function_1cf50();
    D3DPIXELSHADERDEF program;
    memset(&program, 0, sizeof(program));
    program.PSCombinerCount = 0x11001;
    program.PSFinalCombinerInputsABCD = 0;
    program.PSFinalCombinerInputsEFG = 0;
    g_484f68 = program;
    D3DDevice_SetPixelShaderProgram(&program);
    function_1c590((s_shader_cache *)g_51f0f0, *(long *)(*(byte **)(g_485a80 + 0x5c) + 0x54), 0);
    function_1c710(g_51f0f0);
    if (g_485898 < 4 || g_485898 > 7)
        function_20bb0(g_4b9ed4, 0x1f, NULL, 0, true, function_2e540);
}

short function_1358c0(short format);
void *function_135a30(s_type_7ba8e9 const *bitmap, short mipmap_index, short x, short y);

// @retail 0x13f10
void function_13f10(long bitmap_value, long front_value)
{
    byte *bitmap = (byte *)bitmap_value;
    bool front = (byte)front_value != 0;
    if (bitmap && *(void **)(bitmap + 0x54))
    {
        short_rect_pair rectangle_pair = g_485a8a;
        short right = (short)(rectangle_pair.a.v1 + *(short *)(bitmap + 4));
        if (rectangle_pair.a.v3 <= right) right = rectangle_pair.a.v3;
        short bottom = (short)(rectangle_pair.a.v0 + *(short *)(bitmap + 6));
        if (rectangle_pair.a.v2 <= bottom) bottom = rectangle_pair.a.v2;
        long width = right - rectangle_pair.a.v1;
        long height = bottom - rectangle_pair.a.v0;
        short format = *(short *)(bitmap + 0xc);
        if ((format == 11 || format == 10) && *(short *)(bitmap + 0x14) == 0)
        {
            D3DSurface *surface = front ? D3DDevice_GetRenderTarget2() : D3DDevice_GetBackBuffer2(0);
            D3DSURFACE_DESC description;
            D3DSurface_GetDesc(surface, &description);
            if (description.Size == description.Width * description.Height * 4)
            {
                D3DResource_BlockUntilNotBusy(surface);
                D3DLOCKED_RECT locked;
                D3DSurface_LockRect(surface, &locked, NULL, 0);
                byte *source = (byte *)((dword)locked.pBits | 0xf0000000);
                if (source)
                {
                    long size = function_1358c0(format) * width / 8;
                    for (long y = 0; y < height; ++y)
                    {
                        memcpy(function_135a30((s_type_7ba8e9 *)bitmap, 0, 0, (short)y), source, size);
                        source += locked.Pitch;
                    }
                }
            }
            if (surface) D3DResource_Release(surface);
        }
    }
    function_13d80();
    D3DDevice_SetBackBufferScale(1.0f, 1.0f);
    D3DDevice_Swap(0);
    D3DDevice_SetBackBufferScale(1.0f, 1.0f);
    if (g_467008 != g_485acc || g_46700c != g_485ad0)
    {
        D3DDevice_SetFlickerFilter(g_485acc);
        D3DDevice_SetSoftDisplayFilter(g_485ad0);
        g_46700c = g_485ad0;
        g_467008 = g_485acc;
    }
    ++g_485aa0;
}



// @retail 0x12fa0
void function_12fa0(real const *projection, byte const *camera, bool scaled, long mode)
{
    real width = (real)(*(short const *)(camera + 0x36) - *(short const *)(camera + 0x32));
    real height = (real)(*(short const *)(camera + 0x34) - *(short const *)(camera + 0x30));
    real left = (real)*(short const *)(camera + 0x32);
    real top = (real)*(short const *)(camera + 0x30);
    real scale = 1.0f;
    if (scaled && !g_5093fc)
        scale = g_4670c8 < 0.0625f ? 0.0625f : g_4670c8 > 1.0f ? 1.0f : g_4670c8;
    real bias = 0.03125f;
    if (mode == 2)
    {
        width = height = 128.0f;
        left = top = 0.0f;
        scale = 1.0f;
        bias = 0.0f;
    }
    real constants[48];
    function_12d50(projection, false, constants);
    constants[16] = projection[14];
    constants[17] = projection[15];
    constants[18] = projection[16];
    constants[19] = 1.0f;
    constants[20] = projection[17];
    constants[21] = projection[18];
    constants[22] = projection[19];
    constants[23] = 0.5f;
    constants[24] = projection[20];
    constants[25] = projection[21];
    constants[26] = projection[22];
    constants[27] = 2.0f;
    constants[28] = ((real const *)camera)[0];
    constants[29] = ((real const *)camera)[1];
    constants[30] = ((real const *)camera)[2];
    constants[31] = 767.8125f;
    constants[32] = width * 0.5f * scale;
    constants[33] = 0.0f;
    constants[34] = 0.0f;
    constants[35] = ((width + 1.0f) * 0.5f + left) * scale + bias;
    constants[36] = 0.0f;
    constants[37] = height * -0.5f * scale;
    constants[38] = 0.0f;
    constants[39] = ((height + 1.0f) * 0.5f + top) * scale + bias;
    constants[40] = width * 0.5f * scale;
    constants[41] = height * -0.5f * scale;
    constants[42] = 16777215.0f;
    constants[43] = 0.0f;
    constants[44] = (width * 0.5f + left) * scale + 0.5f;
    constants[45] = (height * 0.5f + top) * scale + 0.5f;
    constants[46] = 0.0f;
    constants[47] = 0.0f;
    D3DDevice_SetVertexShaderConstant(-96, constants, 12);
}


real *table_entry_data(long handle);
long function_184000(long bit, long index);

// @retail 0x45ce0
bool function_45ce0(short index, byte *out, short part, short transform_index)
{
    s_44940_entry *entry = &g_4ba138[index];
    byte *definition;
    if (entry->tag != NONE)
    {
        byte *tag = g_4e3b44[entry->tag & 0xffff].bytes;
        byte *section = *(byte **)(tag + 0x28) + ((entry->flags >> 9) & 0x1ff) * 0x5c;
        definition = *(byte **)(section + 0x34);
    }
    else
    {
        byte *structure = (byte *)g_4e0348;
        byte *section;
        if (!(entry->unknown00 & 0x1000))
            section = *(byte **)(structure + 0xa0) + ((entry->flags >> 9) & 0x1ff) * 0xb0;
        else
        {
            byte *instance = *(byte **)(structure + 0x144) + ((entry->flags >> 18) & 0x7ff) * 0x58;
            section = *(byte **)(structure + 0x13c) + *(short *)(instance + 0x34) * 0xc8;
        }
        definition = *(byte **)(section + 0x50);
    }
    if (!definition) return false;
    byte *record = *(byte **)(definition + 4) + part * 0x48;
    *(real *)(out + 0x10) = 0.0f;
    *(real *)(out + 0x28) = *(real *)(record + 0x2c) > 0.0f ? *(real *)(record + 0x2c) : 1.0f;
    byte *material;
    bool visible = true;
    if (entry->tag != NONE)
        material = *(byte **)(g_4e3b44[entry->tag & 0xffff].bytes + 0x64) + *(short *)(record + 4) * 32;
    else
    {
        material = *(byte **)((byte *)g_4e0348 + 0xa8) + *(short *)(record + 4) * 32;
        long instance = (entry->flags >> 18) & 0x7ff;
        if (instance == 0x7ff) instance = NONE;
        long bit = material[0x1c] == 0xff ? NONE : material[0x1c];
        visible = (byte)function_184000(bit, instance) != 0;
    }
    *(long *)out = *(long *)(material + 0xc);
    out[0x14] = false;
    if (!visible) return visible;
    if (entry->tag != NONE)
    {
        long section_index = (entry->flags >> 9) & 0x1ff;
        if (section_index == 0xff) return false;
        byte *tag = g_4e3b44[entry->tag & 0xffff].bytes;
        byte *section = *(byte **)(tag + 0x28) + section_index * 0x5c;
        byte *transforms = (byte *)table_entry_data(entry->unknown08);
        short selected = *(short *)(section + 0x2c);
        byte *transform;
        if (selected != NONE)
        {
            if (tag[4] & 4) selected = ((short *)(transforms + 4))[transform_index * 2];
            transform = transforms + 0x44 + selected * 0x30;
        }
        else transform = transforms + 0x44;
        *(real *)(out + 4) = *(real *)(transform + 0xc);
        *(real *)(out + 8) = *(real *)(transform + 0x1c);
        *(real *)(out + 0xc) = *(real *)(transform + 0x2c);
        long bias = entry->unknown10[11] & 15;
        if (bias) *(real *)(out + 0x10) = (bias - 5) * 0.001f;
    }
    else if (entry->unknown00 & 0x1000)
    {
        byte *instance = *(byte **)((byte *)g_4e0348 + 0x144) + ((entry->flags >> 18) & 0x7ff) * 0x58;
        void *plane = function_1e280(*(short *)(instance + 0x34), true, part);
        out[0x14] = plane != NULL;
        if (plane) memcpy(out + 0x18, plane, 16);
        *(point3f *)(out + 4) = *(point3f *)(instance + 0x3c);
        if (out[0x14])
        {
            real x = *(real *)(out + 0x18), y = *(real *)(out + 0x1c), z = *(real *)(out + 0x20);
            *(real *)(out + 0x18) = *(real *)(instance + 0x1c) * z + *(real *)(instance + 0x10) * y + *(real *)(instance + 4) * x;
            *(real *)(out + 0x1c) = *(real *)(instance + 0x20) * z + *(real *)(instance + 0x14) * y + *(real *)(instance + 8) * x;
            *(real *)(out + 0x20) = *(real *)(instance + 0x24) * z + *(real *)(instance + 0x18) * y + *(real *)(instance + 0xc) * x;
            *(real *)(out + 0x24) = *(real *)(out + 0x24) * *(real *)instance +
                *(real *)(instance + 0x30) * *(real *)(out + 0x20) + *(real *)(instance + 0x2c) * *(real *)(out + 0x1c) +
                *(real *)(instance + 0x28) * *(real *)(out + 0x18);
        }
    }
    else
    {
        void *plane = function_1e280((short)((entry->flags >> 9) & 0x1ff), false, part);
        out[0x14] = plane != NULL;
        if (plane) memcpy(out + 0x18, plane, 16);
        *(point3f *)(out + 4) = *(point3f *)(record + 0x10);
    }
    return visible;
}



long g_485870;
real g_485874, g_485878, g_48587c, g_485880, g_485884;
dword __cdecl pack_color4f(color4f const *color);

// @retail 0x40890
void function_40890(void)
{
	if (!g_485870) return;
	color4f color, inverse;
	color.alpha = inverse.alpha = g_485878 * g_485874;
	color.red = g_48587c * g_485874;
	color.green = g_485880 * g_485874;
	color.blue = g_485884 * g_485874;
	inverse.red = (1.0f - g_48587c) * g_485874;
	inverse.green = (1.0f - g_485880) * g_485874;
	inverse.blue = (1.0f - g_485884) * g_485874;
	dword packed = pack_color4f(&color);
	dword constant = pack_color4f(&inverse);
	function_0222d0(D3DRS_CULLMODE, 0x901);
	function_0222d0(D3DRS_COLORWRITEENABLE, 0x10101);
	function_0222d0(D3DRS_ALPHABLENDENABLE, 1);
	dword local_617c45, rgb_inputs;
	switch (g_485870)
	{
	case 1: case 2:
		function_0222d0(D3DRS_SRCBLEND, 1);
		function_0222d0(D3DRS_DESTBLEND, g_485870 == 1 ? 0x303 : 1);
		function_0222d0(D3DRS_BLENDOP, g_485870 == 1 ? 0x8006 : 0x800b);
		constant = packed;
		rgb_inputs = 0x1200000;
		local_617c45 = 0x11200000;
		break;
	case 3: case 4: case 5:
		function_0222d0(D3DRS_SRCBLEND, 0x307);
		function_0222d0(D3DRS_DESTBLEND, 0x8002);
		function_0222d0(D3DRS_BLENDOP, g_485870 == 3 ? 0x8008 : g_485870 == 4 ? 0x8007 : 0x8006);
		function_0222d0(D3DRS_BLENDCOLOR, packed);
		constant = packed;
		rgb_inputs = g_485870 == 3 ? 0x1201140 : g_485870 == 4 ? 0x1201120 : 0x1411120;
		local_617c45 = 0;
		break;
	default:
		function_0222d0(D3DRS_SRCBLEND, 1);
		function_0222d0(D3DRS_DESTBLEND, 0x8002);
		function_0222d0(D3DRS_BLENDOP, 0x8006);
		function_0222d0(D3DRS_BLENDCOLOR, constant);
		rgb_inputs = 0x11200000;
		local_617c45 = 0;
		break;
	}
	function_0222d0(D3DRS_ALPHATESTENABLE, 0);
	function_0222d0(D3DRS_ZENABLE, 0);
	function_0222d0(D3DRS_ZBIAS, 0);
	function_1cc30(13);
	function_1c710(g_51f0f0);
	real x = 1.0f / (short)(g_48564e - g_48564a);
	real y = 1.0f / (short)(g_48564c - g_485648);
	real constants[20] = {
		2.0f * x, 0, 0, -1.0f - x,
		0, -2.0f * y, 0, 1.0f + y,
		0, 0, 0, 0.5f,
		0, 0, 0, 1.0f,
		0, 0, 0, 1.0f
	};
	D3DDevice_SetVertexShaderConstant(81, constants, 5);
	D3DPIXELSHADERDEF program;
	memset(&program, 0, sizeof(program));
	program.PSAlphaInputs[0] = local_617c45;
	program.PSRGBInputs[0] = rgb_inputs;
	program.PSCombinerCount = 1;
	program.PSConstant0[0] = constant;
	program.PSAlphaOutputs[0] = 0xc00;
	program.PSRGBOutputs[0] = 0xc00;
	program.PSFinalCombinerInputsABCD = 0xc;
	program.PSFinalCombinerInputsEFG = 0x1c00;
	g_484f68 = program;
	function_15180(&program);
	short width = g_48564e - g_48564a;
	short height = g_48564c - g_485648;
	function_1cf50();
	D3DDevice_Begin(D3DPT_QUADLIST);
	D3DDevice_SetVertexData2s(0, 0, 0);
	D3DDevice_SetVertexData2s(0, width, 0);
	D3DDevice_SetVertexData2s(0, width, height);
	D3DDevice_SetVertexData2s(0, 0, height);
	D3DDevice_End();
}

bool function_48b00(point3f const *point, real radius, real *screen, real *extent);
void function_1cf50();

// @retail 0x484b0
void __stdcall function_484b0(point3f const *point, real depth, real width, real height, real cosine, real sine, dword color)
{
    real screen[4], extent[2];
    if (width > 0.0f && height > 0.0f && function_48b00(point, 1.0f, screen, extent))
    {
        real ratio = g_485ad4.hi / (g_485ad4.hi - g_485ad4.lo);
        real z = ((ratio * depth - g_485ad4.lo * ratio) / depth) * 16777215.0f;
        if (z < 0.0f) z = 0.0f;
        else if (z > 16777215.0f) z = 16777215.0f;
        real w = (depth / g_485ad4.hi) * 16777215.0f;
        if (w < 0.0f) w = 0.0f;
        else if (w > 16777215.0f) w = 16777215.0f;
        function_1cf50();
        D3DDevice_Begin(D3DPT_TRIANGLEFAN);
        D3DDevice_SetVertexDataColor(5, color);
        D3DDevice_SetVertexData2f(2, 1.0f, 0.0f);
        real wc = width * cosine;
        real hs = height * sine;
        real ws = width * sine;
        real hc = height * cosine;
        real a = (wc - hs) * extent[0];
        real b = (ws + hc) * extent[1];
        real x = screen[0] + a;
        real y = screen[1] - b;
        D3DDevice_SetVertexData2f(1, x, y);
        D3DDevice_SetVertexData4f(0, x, y, z, w);
        real c = (hs + wc) * extent[0];
        real d = (ws - hc) * extent[1];
        D3DDevice_SetVertexData2f(2, 1.0f, 1.0f);
        x = screen[0] + c;
        y = screen[1] - d;
        D3DDevice_SetVertexData2f(1, x, y);
        D3DDevice_SetVertexData4f(0, x, y, z, w);
        D3DDevice_SetVertexData2f(2, 0.0f, 1.0f);
        x = screen[0] - a;
        y = screen[1] + b;
        D3DDevice_SetVertexData2f(1, x, y);
        D3DDevice_SetVertexData4f(0, x, y, z, w);
        D3DDevice_SetVertexData2f(2, 0.0f, 0.0f);
        x = screen[0] - c;
        y = screen[1] + d;
        D3DDevice_SetVertexData2f(1, x, y);
        D3DDevice_SetVertexData4f(0, x, y, z, w);
        D3DDevice_End();
    }
}

// @retail 0x480a0
void __stdcall function_480a0(point3f const *point, real width, real height, real cosine, real sine, dword color)
{
    real screen[4], extent[2];
    if (width > 0.0f && height > 0.0f && function_48b00(point, 1.0f, screen, extent))
    {
        function_1cf50();
        D3DDevice_Begin(D3DPT_TRIANGLEFAN);
        D3DDevice_SetVertexDataColor(9, color);
        real wc = width * cosine;
        real hs = height * sine;
        real ws = width * sine;
        real hc = height * cosine;
        real a = (wc - hs) * extent[0];
        real b = (ws + hc) * extent[1];
        D3DDevice_SetVertexData2s(3, 1, 0);
        D3DDevice_SetVertexData4f(0, screen[0] + a, screen[1] - b, screen[2], screen[3]);
        real c = (hs + wc) * extent[0];
        real d = (ws - hc) * extent[1];
        D3DDevice_SetVertexData2s(3, 1, 1);
        D3DDevice_SetVertexData4f(0, screen[0] + c, screen[1] - d, screen[2], screen[3]);
        D3DDevice_SetVertexData2s(3, 0, 1);
        D3DDevice_SetVertexData4f(0, screen[0] - a, screen[1] + b, screen[2], screen[3]);
        D3DDevice_SetVertexData2s(3, 0, 0);
        D3DDevice_SetVertexData4f(0, screen[0] - c, screen[1] + d, screen[2], screen[3]);
        D3DDevice_End();
    }
}
