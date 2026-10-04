/* UNKNOWN_03BCB0.H: predicting a bitmap's texture: unless the texture was
   used in the frames ahead of the cache's current one, prefetch the bitmap
   and its texture header and ask the texture cache for it, falling back to a
   request (src/unknown_03bcb0.cpp; retail also inlines it) */

#ifndef UNKNOWN_03BCB0_H
#define UNKNOWN_03BCB0_H

#include "cseries.h"
#include <xmmintrin.h>

struct s_bitmap_data;
struct D3DTexture;

/* the parts of a bitmap (0x74 bytes, xbox_texture_cache.cpp) read here */
struct s_bitmap_predict_view
{
	byte unknown00[0xe];
	word flags;
	byte unknown10[0x28 - 0x10];
	long data_offset;
	long data_offset1;
	long data_offset2;
	byte unknown34[0x50 - 0x34];
	D3DTexture *texture;
	byte unknown54[0x70 - 0x54];
	long last_frame;
};

extern long g_4e6488;

D3DTexture *texture_cache_bitmap_get_texture(s_bitmap_data *bitmap, dword flags, real bias);
bool function_12ce00(s_bitmap_data *bitmap, dword flags, real bias);

static __forceinline void bitmap_predict_inline(s_bitmap_predict_view *bitmap, dword flags)
{
	D3DTexture *texture = NULL;

	if (bitmap->last_frame > g_4e6488)
	{
		texture = bitmap->texture;
	}
	if (!texture)
	{
		_mm_prefetch((char const *)&bitmap->flags, _MM_HINT_T0);
		_mm_prefetch((char const *)&bitmap->data_offset, _MM_HINT_T0);
		_mm_prefetch((char const *)&bitmap->data_offset1, _MM_HINT_T0);
		_mm_prefetch((char const *)&bitmap->data_offset2, _MM_HINT_T0);
		_mm_prefetch((char const *)bitmap->texture, _MM_HINT_T0);
		if (!texture_cache_bitmap_get_texture((s_bitmap_data *)bitmap, flags, 0.0f))
		{
			function_12ce00((s_bitmap_data *)bitmap, flags, 0.0f);
		}
	}
}

void function_3bcb0(s_bitmap_data *bitmap);

#endif
