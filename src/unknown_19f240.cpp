#include "unknown_11c920.h"
#include "globals.h"
#include "slot_owner.h"
#include "game_engine_events.h"

// @flags /O2 /Gr

/* an iterator over the players (g_4e8c24) that skips the players whose flag
   at +2 is set (its first parameter is passed as a pointer to longs, as the
   callers declare it) */
struct s_player_iterator
{
	byte *datum;
	s_record_pool *data;
	long datum_index;
	long index;
};

/* the players (0x21c bytes each) and their units' item slots */
struct s_player_record
{
	byte unknown00[0x2c];
	long unit_index;
};

struct s_item
{
	long tag_index;
};

struct s_unit_object
{
	byte unknown00[0x218];
	long items[4];
};

struct s_item_object_header
{
	short identifier;
	byte unknown02[6];
	void *object;
};

struct s_item_tag
{
	byte unknown00[0x290];
	short type;
};

struct s_object_header
{
	byte unknown00[8];
	byte *object;
};

/* record_pool_iterator_step, as these iterators inline it: once with its call to
   function_16bc00, then with that inlined too */
static inline byte *player_iterator_first(s_record_pool_iterator *iterator)
{
	s_record_pool *data = iterator->data;
	long index = function_16bc00(data, iterator->index + 1);
	byte *result;

	if (index != NONE)
	{
		result = data->data + data->size * index;
		iterator->index = index;
		iterator->datum_index = (*(short *)result << 16) | index;
	}
	else
	{
		iterator->index = data->maximum_count;
		iterator->datum_index = NONE;
		result = 0;
	}
	return result;
}

#include <xtl.h>
#include <math.h>
#include "effects.h"
#include "flexible_surface_calls.h"

extern point3f g_4b9da0;
extern byte *g_485a80;
void function_1ccb0(long format);
void function_1cd90();
void function_1cf50();

class c_polygon_material_reference_19f
{
public:
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void slot3() = 0;
    virtual void slot4() = 0;
    virtual void slot5() = 0;
    virtual void slot6() = 0;
    virtual void slot7() = 0;
    virtual void slot8() = 0;
    virtual void slot9() = 0;
    virtual byte *reference() = 0;
};

PRIVATE inline byte *polygon_tag_19f(long index)
{
    return g_4e3b44[index & 0xffff].bytes;
}

PRIVATE inline byte *polygon_pointer_19f(byte *base, long offset)
{
    return *(byte **)(base + offset);
}

PRIVATE inline real polygon_distance_19f(real x, real y)
{
    return (real)sqrt(x * x + y * y);
}

PRIVATE inline long polygon_round_19f(real value)
{
    long result;
    __asm
    {
        fld value
        fistp result
    }
    return result;
}

// @retail 0x19f680
void function_19f680(long tag, long group, long pass, long variant, void *context,
    point2f const *vertices, long count, point3f const *center, real radius,
    real perimeter, point3f const *color, real height)
{
    real distance = polygon_distance_19f(center->x - g_4b9da0.x,
        center->y - g_4b9da0.y) - radius;
    real fade = distance < 0.0f ? 0.0f : distance > 1.5f * radius ? 1.5f * radius : distance;
    fade = 1.0f - fade / (1.5f * radius);
    if (fade > 0.0f)
    {
        real spread = fade * radius * 2.0f;
        real maximum_distance = radius - 0.0001f + spread;
        function_1bd50(0);
        function_1cdd0(0, 0);
        function_4b2d0(tag, group, variant, ((byte *)context)[4], pass);
        long material = tag == NONE ? *(long *)(g_485a80 + 0xd0) : tag;
        byte *reference;
        dword kind = *(dword *)&g_4e3b44[(short)material];
        if (kind == 0x5052544d || kind == 0x70727433)
            reference = ((c_polygon_material_reference_19f *)function_137bd0(material))->reference();
        else
            reference = polygon_pointer_19f(polygon_tag_19f(material), 0x24);
        byte *groups = polygon_pointer_19f(polygon_tag_19f(*(long *)reference), 0x5c);
        long group_offset = *(word *)(polygon_pointer_19f(groups, 4) + group * 10) & 0x1ff;
        long variant_offset = *(word *)(polygon_pointer_19f(groups, 0xc) + (group_offset + variant) * 2) & 0x1ff;
        long shader = *(long *)(polygon_pointer_19f(groups, 0x14) + (variant_offset + pass) * 10 + 4);
        byte *attributes = polygon_pointer_19f(polygon_tag_19f(*(long *)(polygon_pointer_19f(
            polygon_pointer_19f(polygon_tag_19f(shader), 0x20), 4) + 0x100)), 8);
        long attribute_count = *(long *)(attributes + 4);
        short *types = *(short **)(attributes + 8);
        long uv = NONE;
        for (long i = 0; i < attribute_count; ++i)
        {
            if (types[i] == 3)
            {
                uv = i;
                break;
            }
        }
        DWORD old_cull;
        D3DDevice_GetRenderState(D3DRS_CULLMODE, &old_cull);
        function_1ccb0(59);
        function_1cd90();
        function_1cf50();
        D3DDevice_Begin(D3DPT_TRIANGLESTRIP);
        real repetitions = perimeter / (height * 1.2f);
        long rounded = polygon_round_19f(repetitions);
        real texture_scale = (real)(rounded < 2 ? 2 : polygon_round_19f(repetitions)) / perimeter;
        long period = g_510c54->field_2_3 * 2;
        real texture_position = (real)(g_510c54->game_time % period) / (real)period;
        for (long step = 0; step <= count; ++step)
        {
            long index = step == count ? 0 : step;
            real alpha = (maximum_distance - polygon_distance_19f(vertices[index].x - g_4b9da0.x,
                vertices[index].y - g_4b9da0.y)) / (maximum_distance - spread);
            alpha = alpha < 0.0f ? 0.0f : alpha > 1.0f ? 1.0f : alpha;
            point3f bottom = { vertices[index].x, vertices[index].y, center->z };
            point3f top = bottom;
            top.z += height;
            D3DDevice_SetVertexData4f(3, color->x, color->y, color->z, alpha);
            if (uv != NONE)
                D3DDevice_SetVertexData2f(uv, texture_position, 1.0f);
            D3DDevice_SetVertexData4f(0, bottom.x, bottom.y, bottom.z, 1.0f);
            if (uv != NONE)
                D3DDevice_SetVertexData2f(uv, texture_position, 0.0f);
            D3DDevice_SetVertexData4f(0, top.x, top.y, top.z, 1.0f);
            long previous = index == 0 ? count - 1 : step - 1;
            texture_position -= polygon_distance_19f(vertices[previous].x - vertices[index].x,
                vertices[previous].y - vertices[index].y) * texture_scale;
        }
        D3DDevice_End();
        D3DDevice_SetRenderState(D3DRS_CULLMODE, old_cull);
    }
}

// @retail 0x19f240
bool function_19f240(long *iterator_)
{
	s_player_iterator *iterator = (s_player_iterator *)iterator_;
	s_record_pool_iterator *data_iterator = (s_record_pool_iterator *)&iterator->data;

	iterator->datum = player_iterator_first(data_iterator);
	while (iterator->datum && (iterator->datum[2] & 2))
		iterator->datum = data_iterator_next_inlined(data_iterator);

	return iterator->datum != 0;
}

// @retail 0x19f300
bool function_19f300(long *iterator_)
{
	s_player_iterator *iterator = (s_player_iterator *)iterator_;
	s_record_pool_iterator *data_iterator = (s_record_pool_iterator *)&iterator->data;

	iterator->datum = player_iterator_first(data_iterator);
	while (iterator->datum && ((s_player_record *)iterator->datum)->unit_index == NONE)
		iterator->datum = data_iterator_next_inlined(data_iterator);

	return iterator->datum != 0;
}

// @retail 0x19f3c0
long function_19f3c0(long player_index, long type)
{
	s_player_record *player = (s_player_record *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c);
	long result = NONE;

	if (player->unit_index != NONE)
	{
		s_object_header *headers = (s_object_header *)g_4e0300->data;
		s_unit_object *unit = (s_unit_object *)headers[player->unit_index & 0xffff].object;
		long i;

		for (i = 0; i < 4; i++)
		{
			long item_index = unit->items[i];

			if (item_index != NONE)
			{
				s_item *item = (s_item *)headers[item_index & 0xffff].object;
				s_item_tag *tag = (s_item_tag *)g_4e3b44[item->tag_index & 0xffff].bytes;

				if (tag->type == type)
				{
					result = item_index;
					break;
				}
			}
		}
	}

	return result;
}

bool function_15eaf0();

/* the players (0x21c bytes each), as the score events see them */
struct s_score_player
{
	byte unknown00[0xc0];
	char team;
	byte unknownc1[0x21c - 0xc1];
};

/* the event initialize and set cause player functions of unknown_19de80.cpp,
   which retail inlines here */
static inline void score_event_initialize(s_event *event, long type, long subtype)
{
	event->type = type;
	event->subtype = subtype;
	event->a = NONE;
	event->cause_player_index = NONE;
	event->cause_team = NONE;
	event->effect_player_index = NONE;
	event->effect_team = NONE;
	event->f = 0;
	event->g = NONE;
}

static inline void score_event_set_cause_player(s_event *event, long player_index)
{
	event->cause_player_index = player_index;
	event->cause_team = ((s_score_player *)g_4e8c24->data)[player_index & 0xffff].team;
}

/* announces the points a player has left to win */
// @retail 0x19f470
void function_19f470(long player_index, long score)
{
	if (g_4e6948->score_to_win - score == 10)
	{
		s_event event;

		score_event_initialize(&event, 0, function_15eaf0() ? 0x30 : 0x2f);
		score_event_set_cause_player(&event, player_index);
		if (g_4e6948->mode != 4)
		{
			function_a7c50(&event);
			function_19eb30(&event);
		}
	}
	if (g_4e6948->score_to_win - score == 30)
	{
		s_event event;

		score_event_initialize(&event, 0, function_15eaf0() ? 10 : 9);
		score_event_set_cause_player(&event, player_index);
		if (g_4e6948->mode != 4)
		{
			function_a7c50(&event);
			function_19eb30(&event);
		}
	}
	if (g_4e6948->score_to_win - score == 60)
	{
		s_event event;

		score_event_initialize(&event, 0, function_15eaf0() ? 8 : 7);
		score_event_set_cause_player(&event, player_index);
		if (g_4e6948->mode != 4)
		{
			function_a7c50(&event);
			function_19eb30(&event);
		}
	}
}
// @retail 0x1a6fe0
short function_1a6fe0(long owner_index, short type)
{
	short result = NONE;
	s_slot_owner_entry *owner = (s_slot_owner_entry *)(g_4f55f0->data + (owner_index & 0xffff) * sizeof(s_slot_owner_entry));
	short count = owner->current;
	short i;

	for (i = 0; i <= count; i++)
	{
		if (owner->slots[i].type == type)
		{
			result = i;
			break;
		}
	}

	return result;
}
