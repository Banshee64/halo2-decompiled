// @flags /O2 /Op /arch:SSE /Gr
/* UNKNOWN_0494B0.CPP: rasterizer render state helpers */

#include "cseries.h"
#include <xtl.h>
#include "globals.h"

PRIVATE bool test_bit(dword flags, long bit)
{
	return (flags >> bit) & 1;
}

dword g_4b8344;
dword g_4b8340;
dword g_4b8464;
dword g_4b8450;
dword g_4b846c;
dword g_4b8448;
dword g_4b8308;
dword g_5093b4;
byte g_47fe84;
dword g_4b8334;
dword g_4b8330;


real g_4c9798[36];
byte g_4b5144;
dword g_4b50c4;
dword g_4b50d8;
dword g_4b5130;
dword g_4b5134;

PRIVATE real normalize(real value, real lower, real upper)
{
	return (value - lower) / (upper - lower);
}

// @retail 0x494b0
void function_0494b0(real const *bounds, real const *window)
{
	real width = bounds[1] - bounds[0];
	real left = 1.0f - (window[0] - bounds[0]) / width;
	real right = 1.0f - (window[1] - bounds[0]) / width;
	real height = bounds[3] - bounds[2];
	real top = (window[2] - bounds[2]) / height;
	real bottom = (window[3] - bounds[2]) / height;
	real scale_x = 1.0f / (right - left);
	real scale_y = 1.0f / (bottom - top);

	g_4c9798[8] = 512.0f;
	g_4c9798[9] = 512.0f;
	g_4c9798[32] = scale_x;
	g_4c9798[33] = scale_y;
	g_4b50c4 = g_4b50d8 = g_4b5144 ? 0x1f1ff1ff : 0;
	g_4b5130 = g_4b5134 = 0x200;
	g_4c9798[10] = 16777215.0f;
	g_4c9798[11] = 1.0f;
	g_4c9798[34] = scale_x * left * -512.0f;
	g_4c9798[35] = scale_y * top * -512.0f;
	g_4ba014 |= 0x48;
	D3DDevice_SetVertexShaderConstantFast(-56, g_4c9798, 9);
}

// @retail 0x495e0
void function_0495e0(void)
{
	dword value = test_bit(g_4ba014, 4) ? 16 : 1;
	g_4b8344 = value;
	D3DDevice_SetRenderState(D3DRS_DEPTHCLIPCONTROL, value);
	value = g_47fe84 ? 2 : 0;
	g_4b8464 = value;
	D3DDevice_SetRenderState(D3DRS_MULTISAMPLEMODE, value);
	g_4b8340 = 1;
	D3DDevice_SetRenderState(D3DRS_SOLIDOFFSETENABLE, 1);
	real z_offset = 550.0f;
	g_4b8334 = *(dword *)&z_offset;
	D3DDevice_SetRenderState(D3DRS_POLYGONOFFSETZOFFSET, *(dword *)&z_offset);
	real z_slope = 2.0f;
	g_4b8330 = *(dword *)&z_slope;
	D3DDevice_SetRenderState(D3DRS_POLYGONOFFSETZSLOPESCALE, *(dword *)&z_slope);
}

// @retail 0x496a0
void function_0496a0(void)
{
	g_4b8344 = 0;
	D3DDevice_SetRenderState(D3DRS_DEPTHCLIPCONTROL, 0);
	g_4b8340 = 0;
	D3DDevice_SetRenderState(D3DRS_SOLIDOFFSETENABLE, 0);
	g_4b8464 = 0;
	D3DDevice_SetRenderState(D3DRS_MULTISAMPLEMODE, 0);
	g_4b8450 = 0;
	D3DDevice_SetRenderState(D3DRS_ZBIAS, 0);
}

// @retail 0x496f0
void function_0496f0(void)
{
	g_4b846c = 0x204;
	D3DDevice_SetRenderState(D3DRS_SHADOWFUNC, 0x204);
	g_4b8344 = 0x10;
	D3DDevice_SetRenderState(D3DRS_DEPTHCLIPCONTROL, 0x10);
	g_4b8448 = g_5093b4;
	D3DDevice_SetRenderState(D3DRS_CULLMODE, g_5093b4);
}

// @retail 0x49740
void function_049740(void)
{
	g_4b846c = 0x200;
	D3DDevice_SetRenderState(D3DRS_SHADOWFUNC, 0x200);
	g_4b8344 = 0;
	D3DDevice_SetRenderState(D3DRS_DEPTHCLIPCONTROL, 0);
	g_4b8308 = 0x1010101;
	D3DDevice_SetRenderState(D3DRS_COLORWRITEENABLE, 0x1010101);
}
