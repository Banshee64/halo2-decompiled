// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_130F60.CPP */

#include "unknown_11c920.h"
#include "globals.h"
#include <math.h>
#include <string.h>

/* the scenario's palette entries (8 bytes each): a tag reference */
struct s_scenario_palette_entry
{
	byte unknown00[4];
	long tag_index;
};

/* the palette sources at +0x214 (0x44 bytes each) */
struct s_palette_source_view
{
	byte unknown00[0x3c];
	byte flags;
	byte unknown3d[3];
	short palette_index;
	byte unknown42[2];
};

struct s_scenario_palette_view
{
	byte unknown000[8];
	long palette_count;
	s_scenario_palette_entry *palette;
	byte unknown010[0x214 - 0x10];
	s_palette_source_view *sources;
};

struct s_palette_owner
{
	byte unknown00[0x6c];
	char palette_index;
	byte unknown6d;
	char default_palette_index;
};

struct s_palette_tag_view
{
	byte unknown00[0x10];
	byte flags;
};

// @retail 0x130f60
long function_130f60(s_palette_owner const *owner, long *palette_index)
{
	s_scenario_palette_view *scenario = (s_scenario_palette_view *)g_4e0350;
	long result = NONE;
	*palette_index = NONE;

	if (owner->palette_index != NONE)
	{
		if (owner->palette_index >= 0 && owner->palette_index < scenario->palette_count)
		{
			s_scenario_palette_entry *entry = &scenario->palette[owner->palette_index];
			*palette_index = owner->palette_index;
			return entry->tag_index;
		}
	}
	else
	{
		if (owner->default_palette_index != NONE && owner->default_palette_index >= 0 && owner->default_palette_index < scenario->palette_count)
		{
			s_scenario_palette_entry *entry = &scenario->palette[owner->default_palette_index];
			if (entry->tag_index != NONE && !(((s_palette_tag_view *)g_4e3b44[entry->tag_index & 0xffff].bytes)->flags & 0x10))
			{
				*palette_index = owner->default_palette_index;
				return entry->tag_index;
			}
		}
	}

	s_palette_source_view *source = &scenario->sources[g_4686c4];
	if (source->flags & 1)
	{
		short index = source->palette_index;
		if (index != NONE && index >= 0 && index < scenario->palette_count)
		{
			s_scenario_palette_entry *entry = &scenario->palette[index];
			if (entry->tag_index != NONE)
			{
				*palette_index = index;
				result = entry->tag_index;
			}
		}
	}

	return result;
}

struct s_131030
{
	color3f field_0;
	real field_c;
	real field_10;
	real field_14;
	real field_18;
	real field_1c;
};

PRIVATE __forceinline void function_131031(s_131030 *arg_1, color3f const *arg_2, real arg_3, real arg_4, real arg_5, real arg_6)
{
	real local_1 = 0.0f > arg_6 ? 0.0f : (arg_6 > 1.0f ? 1.0f : arg_6);
	arg_1->field_0.red += arg_2->red * local_1;
	arg_1->field_0.green += arg_2->green * local_1;
	arg_1->field_0.blue += arg_2->blue * local_1;
	arg_1->field_c += arg_3 * local_1;
	arg_1->field_10 += arg_4 * local_1;
	arg_1->field_14 += arg_5 * local_1;
	arg_1->field_18 += local_1;
	arg_1->field_1c *= 1.0f - local_1;
}

PRIVATE __forceinline void function_131032(real arg_1, real arg_2, vector3f *arg_3)
{
	real local_1 = (real)cos(arg_2);
	arg_3->i = (real)cos(arg_1) * local_1;
	arg_3->j = (real)sin(arg_1) * local_1;
	arg_3->k = (real)sin(arg_2);
}

real function_11ce20(vector3f const *a, vector3f const *b);

// @retail 0x131030
void function_131030(long arg_1, vector3f const *arg_2, s_131030 *arg_3, s_131030 *arg_4, s_131030 *arg_5)
{
	(void)&arg_2;
	(void)&arg_5;
	byte *local_1 = g_4e3b44[arg_1 & 0xffff].bytes;
	memset(arg_3, 0, sizeof(*arg_3));
	arg_3->field_1c = 1.0f;
	memset(arg_4, 0, sizeof(*arg_4));
	arg_4->field_1c = 1.0f;
	memset(arg_5, 0, sizeof(*arg_5));
	arg_5->field_1c = 1.0f;
	for (long local_2 = 0; local_2 < *(long *)(local_1 + 0x78); local_2++)
	{
		byte *local_3 = *(byte **)(local_1 + 0x7c) + local_2 * 0x34;
		vector3f local_4 = *(vector3f *)local_3;
		vector3f local_5;
		if (local_4.i == 0.0f && local_4.j == 0.0f && local_4.k == 0.0f)
		{
			real local_6 = *(real *)(local_3 + 0xc) + 3.1415927410125732f;
			function_131032(*(real *)(local_3 + 0xc), *(real *)(local_3 + 0x10), &local_4);
			function_131032(local_6, 0.0f, &local_5);
		}
		else
		{
			local_5.i = -local_4.i;
			local_5.j = -local_4.j;
			local_5.k = 0.0f;
			real local_7 = (real)sqrt(local_5.j * local_5.j + local_5.i * local_5.i);
			if (!(0.0001f > (real)fabs(local_7)))
			{
				real local_8 = 1.0f / local_7;
				local_5.i *= local_8;
				local_5.j *= local_8;
				local_5.k *= local_8;
			}
		}
		if (local_4.k * local_4.k + local_4.j * local_4.j + local_4.i * local_4.i > 0.0f)
		{
			long local_9 = 0;
			do
			{
				real *local_10;
				vector3f const *local_11;
				switch (local_9)
				{
				case 0:
					local_10 = *(long *)(local_3 + 0x1c) > 0 ? *(real **)(local_3 + 0x20) : NULL;
					local_11 = &local_4;
					break;
				case 1:
					local_10 = *(long *)(local_3 + 0x24) > 0 ? *(real **)(local_3 + 0x28) : NULL;
					local_11 = &local_5;
					break;
				default:
					__assume(0);
				}
				real local_12 = function_11ce20(arg_2, local_11);
				if (local_10 && local_10[7] > local_10[6])
				{
					real local_13 = 0.0f > ((local_12 - local_10[7]) / (local_10[6] - local_10[7])) ? 0.0f : (((local_12 - local_10[7]) / (local_10[6] - local_10[7])) > 1.0f ? 1.0f : ((local_12 - local_10[7]) / (local_10[6] - local_10[7])));
					function_131031(arg_3, (color3f *)local_10, local_10[3], local_10[4], local_10[5], local_10[8] * local_13);
					function_131031(arg_4, (color3f *)local_10, local_10[3], local_10[4], local_10[5], local_10[9] * local_13);
					function_131031(arg_5, (color3f *)local_10, local_10[3], local_10[4], local_10[5], local_10[10] * local_13);
				}
				local_9++;
			} while (local_9 < 2);
		}
	}
	real local_14 = arg_3->field_1c * arg_4->field_1c;
	real local_15 = arg_5->field_1c;
	if (*(long *)(local_1 + 0x48) > 0)
	{
		real *local_16 = *(real **)(local_1 + 0x4c);
		function_131031(arg_3, (color3f *)local_16, local_16[3], local_16[4], local_16[5], local_14);
	}
	else
		arg_3->field_18 += local_14;
	if (*(long *)(local_1 + 0x50) > 0)
	{
		real *local_17 = *(real **)(local_1 + 0x54);
		function_131031(arg_4, (color3f *)local_17, local_17[3], local_17[4], local_17[5], local_14);
	}
	else
		arg_4->field_18 += local_14;
	if (*(long *)(local_1 + 0x58) > 0)
	{
		real *local_18 = *(real **)(local_1 + 0x5c);
		function_131031(arg_5, (color3f *)local_18, local_18[3], 0.0f, 1.0f, local_15);
	}
	else
		arg_5->field_18 += local_15;
}
