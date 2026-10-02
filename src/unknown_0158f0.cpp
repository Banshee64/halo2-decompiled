// @flags /O2 /Gr
/* UNKNOWN_0158F0.CPP: D3D texture and palette creation (the D3D header
   allocation is inlined from the SDK's D3DDevice_CreateTexture2) */

#include "cseries.h"
#include <xtl.h>
#include "globals.h"

long g_450768[8][24];
dword g_43e8a8[12];
long g_43e8d8[6];

/* the palette tag data: a list of palette groups, each pointing at 256-color
   palettes (0x400 bytes apart) that are loaded into D3D palette headers */
struct s_palette_group
{
	byte unknown00[8];
	long palette_count;
	dword palettes;
	byte unknown10[0x58];
};

struct s_palette_tag_data
{
	byte unknown00[0x80];
	long group_count;
	s_palette_group *groups;
};

/* one entry of the global array at +0x214; the palette tag index is at +0x1c */
struct s_palette_source
{
	byte unknown00[0x1c];
	long tag_index;
	byte unknown20[0x24];
};

struct s_palette_source_globals
{
	byte unknown00[0x214];
	s_palette_source *sources;
};

D3DPalette *g_484dbc;
long g_484dc0[4];
D3DPalette g_484dd0[32];
long g_467000;
long g_5093ac;
short g_4686c4;
s_palette_source_globals *g_4e0350;
dword **g_407488;


// @retail 0x158f0
bool function_0158f0(
	byte a,
	short b,
	long width,
	long height,
	long levels,
	long usage_index,
	D3DTexture **out)
{
	D3DTexture *texture = D3DDevice_CreateTexture2(width, height, 1, levels + 1, g_43e8a8[usage_index], (D3DFORMAT)g_450768[a][b], D3DRTYPE_TEXTURE);
	if (texture == NULL)
	{
		*out = NULL;
		return false;
	}
	*out = texture;
	return true;
}

// @retail 0x159b0
bool function_0159b0(
	long edge,
	short b,
	long levels,
	long usage_index,
	D3DCubeTexture **out)
{
	D3DCubeTexture *texture = (D3DCubeTexture *)D3DDevice_CreateTexture2(edge, edge, 1, levels + 1, g_43e8a8[usage_index], (D3DFORMAT)g_450768[0][b], D3DRTYPE_CUBETEXTURE);
	if (texture == NULL)
	{
		*out = NULL;
		return false;
	}
	*out = texture;
	return true;
}

// @retail 0x15a60
bool function_015a60(
	short a,
	long width,
	long height,
	long depth,
	long levels,
	long usage_index,
	D3DVolumeTexture **out)
{
	D3DVolumeTexture *texture = (D3DVolumeTexture *)D3DDevice_CreateTexture2(width, height, depth, levels + 1, g_43e8a8[usage_index], (D3DFORMAT)g_450768[0][a], D3DRTYPE_VOLUMETEXTURE);
	if (texture == NULL)
	{
		*out = NULL;
		return false;
	}
	*out = texture;
	return true;
}

// @retail 0x15b10
bool function_015b10(
	long index,
	D3DPalette **out)
{
	D3DPalette *palette = D3DDevice_CreatePalette2((D3DPALETTESIZE)g_43e8d8[index]);
	if (palette == NULL)
	{
		*out = NULL;
		return false;
	}
	*out = palette;
	return true;
}

// @retail 0x15b70
void function_015b70(void)
{
	long stage;
	long group_index;
	long palette_total = 0;
	s_palette_tag_data *tag_data;

	for (stage = 0; stage < 4; stage++)
	{
		D3DDevice_SetPalette(stage, g_484dbc);
		g_484dc0[stage] = NONE;
	}

	long tag_index = g_4e0350->sources[g_4686c4].tag_index;
	g_467000 = NONE;
	g_5093ac = 0;
	if (tag_index != NONE)
	{
		tag_data = (s_palette_tag_data *)g_4e3b44[tag_index & 0xffff].data;
		for (group_index = 0; group_index < tag_data->group_count; group_index++)
		{
			s_palette_group *group = &tag_data->groups[group_index];
			for (long i = 0; i < group->palette_count; i++)
			{
				if (palette_total >= 0 && palette_total < 32)
				{
					D3DPalette *palette = &g_484dd0[palette_total];
					palette->Data = (group->palettes + i * 0x400) & 0x7fffffff;
					palette->Common = 0x30001;
					((D3DResource *)palette)->Lock = 0;
					palette_total++;
				}
			}
		}
		g_5093ac = palette_total;
	}
}

// @retail 0x15c90
void function_015c90(long stage, long index)
{
	if (index != g_467000 && index >= 0 && index < g_5093ac && index != g_484dc0[stage])
	{
		D3DDevice_SetPalette(stage, &g_484dd0[index]);
		g_484dc0[stage] = index;
		g_467000 = index;
	}
}

// @retail 0x15cd0
dword function_015cd0(long index, dword entry)
{
	dword result = 0xff4060;

	if (index >= 0 && index < g_5093ac)
	{
		dword *palette = (dword *)(g_484dd0[index].Data | 0x80000000);
		if (palette)
		{
			result = palette[entry & 0xff];
		}
	}
	return result;
}

// @retail 0x15d00
bool function_015d00(long count, dword **out)
{
	if (count >= 0x800)
	{
		*out = NULL;
		return false;
	}
	*out = D3DDevice_BeginPush(count);
	return true;
}
