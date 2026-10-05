#include "unknown_11c920.h"
#include "globals.h"

// @flags /O2 /arch:SSE /Gr

struct s_decal_sprite_definition
{
	word flags;
	byte unknown02[0x8c - 2];
	long bitmap_index;
	byte unknown90[0xa4 - 0x90];
	real pixels_per_unit;
};

struct s_decal_sprite_frame
{
	short bitmap_index;
	byte unknown02[6];
	box2f bounds;
	point2f origin;
};

struct s_decal_sprite_sequence
{
	byte unknown00[0x34];
	long frame_count;
	s_decal_sprite_frame *frames;
};

struct s_decal_bitmap_size
{
	byte unknown00[4];
	short width;
	short height;
	byte unknown08[0x74 - 8];
};

struct s_decal_bitmap_group
{
	byte unknown00[0x3c];
	long sequence_count;
	s_decal_sprite_sequence *sequences;
	long bitmap_count;
	s_decal_bitmap_size *bitmaps;
};

// @retail 0x17cb00
bool function_17cb00(s_decal_sprite_definition const *definition, short sequence_index, short frame_index,
	real scale, box2f *texture_bounds, box2f *position_bounds)
{
	if (definition->bitmap_index != NONE)
	{
		s_decal_bitmap_group *group = (s_decal_bitmap_group *)g_4e3b44[definition->bitmap_index & 0xffff].bytes;

		if (sequence_index < group->sequence_count)
		{
			s_decal_sprite_sequence *sequence = &group->sequences[sequence_index];

			if (frame_index < sequence->frame_count)
			{
				s_decal_sprite_frame *frame = &sequence->frames[frame_index];

				if (frame->bitmap_index < group->bitmap_count)
				{
					s_decal_bitmap_size *bitmap = &group->bitmaps[frame->bitmap_index];
					real aspect = 1.0f;

					*texture_bounds = frame->bounds;
					if (definition->flags & 0x100)
					{
						aspect = (frame->bounds.x1 - frame->bounds.x0) / (frame->bounds.y1 - frame->bounds.y0);
						aspect *= (real)bitmap->height / (real)bitmap->width;
					}

					real factor = scale / definition->pixels_per_unit;
					real width = (real)bitmap->width * factor;
					real height = (real)bitmap->height * factor * aspect;

					position_bounds->x0 = (0.0f - frame->origin.x) * width;
					position_bounds->x1 = (frame->bounds.x1 - frame->origin.x - frame->bounds.x0) * width;
					position_bounds->y0 = (0.0f - frame->origin.y) * height;
					position_bounds->y1 = (frame->bounds.y1 - frame->origin.y - frame->bounds.y0) * height;
					return true;
				}
			}
		}
	}
	return false;
}
