#include <math.h>
#include <new.h>
#include "cseries.h"
#include "globals.h"
#include "unknown_19ec40.h"
#include "unknown_1efac0.h"
#include "unknown_2420a0.h"
#include "marker_list.h"

// @flags /O2 /arch:SSE /Gr

/* ---- globals ---- */
s_slot_table *g_51ec80;
long g_4b9ed8;

/* the two colors (alpha, then red, green and blue) of the markers drawn by
   function_243ed0 */
real_argb_color g_468c80[2] =
{
	{ 1.0f, 0.4588235318660736f, 0.729411780834198f, 1.0f },
	{ 1.0f, 0.8078431487083435f, 0.5607843399047852f, 0.8705882430076599f },
};

/* ---- views of other data ---- */
struct s_slot_object
{
	byte unknown00[0x17e];
	short slot;
};

struct s_slot_object_header
{
	byte unknown00[8];
	s_slot_object *object;
};

struct s_player_view
{
	byte unknown00[0x88];
	byte type;
	byte unknown89[0x21c - 0x89];
};

long function_19f3c0(long, long);

/* ---- the game engine class whose vtable is at 0x459d18 ---- */
struct s_marker_update;

class c_game_engine_markers
{
public:
	virtual void v0() {}
	virtual void v1() {}
	virtual void v2() {}
	virtual void v3() {}
	virtual bool v4(long index);
	virtual void v5() {}
	virtual void v6() {}
	virtual void v7() {}
	virtual void v8() {}
	virtual void v9() {}
	virtual void v10() {}
	virtual void v11() {}
	virtual void v12() {}
	virtual void v13() {}
	virtual void v14() {}
	virtual void v15() {}
	virtual void v16() {}
	virtual void v17() {}
	virtual void v18() {}
	virtual void v19() {}
	virtual void v20() {}
	virtual void v21() {}
	virtual void v22() {}
	virtual void v23() {}
	virtual void v24() {}
	virtual void v25() {}
	virtual void v26() {}
	virtual void v27() {}
	virtual void v28() {}
	virtual void v29() {}
	virtual void v30() {}
	virtual void v31() {}
	virtual void v32() {}
	virtual void v33() {}
	virtual void v34() {}
	virtual void v35() {}
	virtual void v36() {}
	virtual long v37(long player_index, byte *flag);
	virtual void v38() {}
	virtual void v39() {}
	virtual void v40() {}
	virtual void v41() {}
	virtual void v42(dword mask, dword *changed, s_marker_update *update) {}
	virtual void v43() {}
	virtual void v44() {}
	virtual void v45(dword *flags, long, s_marker_update *update);
};

/* the copy of the slot table a client keeps */
struct s_marker_update
{
	byte unknown00[0x24];
	long l24;
	byte unknown28[4];
	short a[9];
	short b[9];
	byte c[9];
	byte unknown59[3];
	s_long_triple triples[3];
};

// @retail 0x2420a0
bool c_game_engine_markers::v4(long index)
{
	long none = NONE;

	if (g_51ec80->a[index] == none && g_51ec80->b[index] == none)
		return false;
	return true;
}

// @retail 0x2420d0
long c_game_engine_markers::v37(long player_index, byte *flag)
{
	s_player_view *player = (s_player_view *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c);
	long result = NONE;

	*flag = 1;
	if (*((byte *)player + 0xc0) != 0xff)
	{
		if (function_19f3c0(player_index, 1) != NONE)
			result = (g_4e6948->mode_180 == 9) * 8 + 0xb;
	}

	return result;
}

// @retail 0x242140
real function_242140(long object_index)
{
	s_slot_object *object = ((s_slot_object_header *)g_4e0300->data)[object_index & 0xffff].object;
	real result = 0.0f;
	long slot = object->slot;
	short scale = g_4e6948->scale_a;
	real divisor = (real)(scale == 0 ? 1 : scale);

	if (slot != NONE)
	{
		if (g_51ec80->flags[slot] & 8)
		{
			result = 1.0f;
		}
		else
		{
			short count = g_51ec80->e[slot];
			long n = count > 0 ? count : 0;

			result = g_510c54->rate / divisor * (real)n;
		}
	}

	return result;
}

/* the slot (0..8) whose first or second identifier is the value, or 9 */
PRIVATE long slot_find(long value)
{
	long i;

	for (i = 0; i < 9; i++)
	{
		if (g_51ec80->b[i] == value || g_51ec80->c[i] == value)
			break;
	}

	return i;
}

PRIVATE long slot_name(long slot)
{
	if (slot < 9)
		return (short)g_4e9ae8->name.c[slot];
	return NONE;
}

// @retail 0x242b60
long function_242b60(long value)
{
	return slot_name(slot_find(value));
}

// @retail 0x243400
void c_game_engine_markers::v45(dword *flags, long, s_marker_update *update)
{
	dword changed = 0;
	dword mask = *flags & 0x1f;
	s_slot_table *g;
	long i;

	if (mask)
		v42(mask, &changed, update);

	g = g_51ec80;
	if (*flags & 0x20)
	{
		long value = g->l1f4;

		if (update->l24 != value)
		{
			update->l24 = value;
			changed |= 0x20;
		}
	}

	if ((*flags >> 7) & 1)
	{
		for (i = 0; i < 9; i++)
		{
			short value = g->d[i];

			if (update->a[i] != value)
			{
				update->a[i] = value;
				changed |= 0x80;
			}
		}
	}

	if (*flags & 0x100)
	{
		for (i = 0; i < 9; i++)
		{
			short value = g->e[i];

			if (update->b[i] != value)
			{
				update->b[i] = value;
				changed |= 0x100;
			}
		}
	}

	if (*flags & 0x200)
	{
		for (i = 0; i < 9; i++)
		{
			byte value = g->flags[i];

			if (update->c[i] != value)
			{
				update->c[i] = value;
				changed |= 0x200;
			}
		}
	}

	if (*flags & 0x400)
	{
		i = 0;
		do
		{
			long a = g->triples[i].a == NONE ? NONE : g->triples[i].a & 0xffff;
			long b = g->triples[i].b == NONE ? NONE : g->triples[i].b & 0xffff;
			long c = g->triples[i].c == NONE ? NONE : g->triples[i].c & 0xffff;

			if (update->triples[i].a != a)
			{
				update->triples[i].a = a;
				changed |= 0x400;
			}
			if (update->triples[i].b != b)
			{
				update->triples[i].b = b;
				changed |= 0x400;
			}
			if (update->triples[i].c != c)
			{
				update->triples[i].c = c;
				changed |= 0x400;
			}
			i++;
		}
		while (i < 3);
	}

	*flags = changed;
}

/* true when a marker with these flags applies in the current team mode */
#define MARKER_APPLIES(flags, options) \
	(((flags) & 1) && (options)->team_mode != 1 && (options)->team_mode != 2 || \
	 ((flags) & 2) && (options)->team_mode == 1 || \
	 ((flags) & 4) && (options)->team_mode == 2)

// @retail 0x243b10
long function_243b10(bool team_only, long key_b, long *second)
{
	s_game_options_view *options = g_4e6948;
	long results[8];
	long key_a;

	if (team_only)
		key_a = options->mode_180 != 9 ? 0 : 2;
	else
		key_a = (options->mode_180 == 9) * 2 + 1;

	long count = function_19ec40(0, 0.0f, (short)key_a, (short)key_b, 0, 8, results, 0.0f);
	long result = NONE;
	long i = 0;

	if (count > 0)
	{
		do
		{
			long index = results[i];
			s_marker_entry *entry = g_4e0350->marker_entries + index;

			if (MARKER_APPLIES(entry->flags, options))
			{
				if (result == NONE)
				{
					result = index;
				}
				else
				{
					if (second)
						*second = results[i];
					break;
				}
			}
			i++;
		}
		while (i < count);
	}

	return result;
}

// @retail 0x243c00
void function_243c00(long slot)
{
	long k;

	for (k = 0; k < 2; k++)
	{
		long marker_index = k == 0 ? g_51ec80->b[slot] : g_51ec80->c[slot];

		if (marker_index != NONE)
		{
			long results[8];
			long count = function_19ec40(0, 0.0f, (short)((g_4e6948->mode_180 == 9) * 2 + 1), (short)slot, (short)(k + 1), 8, results, 0.0f);
			real_point3d point = g_4e0350->marker_entries[marker_index].position;
			real_point3d *bounds_a = &g_51ec80->bounds[0][slot];
			real_point3d *bounds_b = &g_51ec80->bounds[1][slot];
			long j;

			if (k == 0)
			{
				bounds_a->x = 0.5f;
				bounds_a->y = 0.0f;
				bounds_a->z = 0.0f;
			}
			else
			{
				bounds_b->x = 0.5f;
				bounds_b->y = 0.0f;
				bounds_b->z = 0.0f;
			}

			for (j = 0; j < count; j++)
			{
				s_marker_entry *other = &g_4e0350->marker_entries[results[j]];

				if (MARKER_APPLIES(other->flags, g_4e6948))
				{
					real_point3d other_point = other->position;
					real dx = point.x - other_point.x;
					real dy = point.y - other_point.y;
					real distance = (real)sqrt(dx * dx + dy * dy);
					real below = point.z - other_point.z;
					real above = other_point.z - point.z;

					if (k == 0)
					{
						bounds_a->x = distance > bounds_a->x ? distance : bounds_a->x;
						bounds_a->y = bounds_a->y > below ? bounds_a->y : below;
						bounds_a->z = bounds_a->z > above ? bounds_a->z : above;
					}
					else
					{
						bounds_b->x = distance > bounds_b->x ? distance : bounds_b->x;
						bounds_b->y = bounds_b->y > below ? bounds_b->y : below;
						bounds_b->z = bounds_b->z > above ? bounds_b->z : above;
					}
				}
			}

			if (k == 0)
			{
				bounds_a->y += 0.1f;
				bounds_a->z += 0.9f;
			}
			else
			{
				bounds_b->y += 0.1f;
				bounds_b->z += 0.9f;
			}
		}
	}
}

/* ---- the marker list ---- */
PRIVATE s_color_bits *marker_color()
{
	real_argb_color *color = &g_468c80[0];

	if (g_4b9ed8 != NONE)
	{
		long player_index = g_4e8c20->entries[g_4b9ed8];

		if (player_index != NONE)
		{
			s_player_view *player = (s_player_view *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c);

			if (player->type == 1 || player->type == 3)
				color = &g_468c80[1];
		}
	}

	return (s_color_bits *)&color->red;
}

// @retail 0x243ed0
bool function_243ed0(s_marker_list *list, real_point3d const *position)
{
	list->b0 = 1;
	list->b1 = 0;
	list->l4 = 1;
	list->position = *position;
	list->r14 = 0.0f;
	list->r18 = 0.1f;
	list->r1c = 0.0f;
	list->l20 = NONE;
	list->color24 = *marker_color();
	list->color30 = *marker_color();
	list->r3c = 1.0f;
	list->r40 = 1.0f;
	list->count = 1;
	list->items[0].a = *marker_color();
	list->items[0].b = *marker_color();
	list->items[0].kind = 0;
	list->items[0].r = 1.0f;
	list->items[0].index = NONE;
	return true;
}

// @retail 0x2440a0
bool function_2440a0(s_marker_list *list, real_point3d const *point, long object_index)
{
	s_slot_object *object = ((s_slot_object_header *)g_4e0300->data)[object_index & 0xffff].object;

	if (list->count < 2 && object->slot != NONE)
	{
		s_game_options_view *options = g_4e6948;
		long divisor = 1;

		if (options->divisor)
			divisor = options->divisor;
		long scale = options->scale_b == 0 ? 5 : options->scale_b;
		long value = NONE;

		list->items[list->count].a = *(s_color_bits *)point;
		list->items[list->count].b = *(s_color_bits *)point;
		list->items[list->count].kind = (options->mode_180 == 9) + 1;
		list->items[list->count].index = NONE;

		short slot = object->slot;
		if (slot >= 0 && slot < 9)
			value = g_51ec80->d[slot];

		if (options->mode_180 == 9 && (g_51ec80->flags[slot] & 2))
		{
			if (value == NONE)
				list->items[list->count].r = 0.0f;
			else
				list->items[list->count].r = (real)value / (real)scale;
		}
		else
		{
			if (value == NONE)
				list->items[list->count].r = 0.0f;
			else
				list->items[list->count].r = (real)value / (real)divisor;
		}

		list->b0 = 1;
		list->count++;
	}

	return true;
}

/* ---- the object whose destructor is at 0x244930 ---- */
struct c_marker_base : c_a
{
	dword d8;
	dword dc;

	c_marker_base(dword other)
	{
		dc = other;
		unknown06 = 1;
		d8 = 0;
	}
};

struct c_marker_object : c_marker_base
{
	dword d10;

	c_marker_object(dword value, dword other) : c_marker_base(other)
	{
		d10 = value;
	}

	void operator delete(void *block)
	{
		g_480118->allocate((long)block, ((c_a *)block)->flags, 0x22);
	}
};

// @retail 0x244930 deleting c_marker_object

class c_marker_object_factory
{
public:
	dword unknown04[2];
	dword value0c;

	virtual c_marker_object *create(dword value, void *memory);
	virtual void s1() {}
	virtual void s2() {}
	virtual void s3() {}
};

// @retail 0x244bb0
c_marker_object *c_marker_object_factory::create(dword value, void *memory)
{
	return new (memory) c_marker_object(value, value0c);
}

/* ---- the list of lines drawn by a marker ---- */
struct s_line_a
{
	long a;
	long b;
	long c;
	byte d;
	byte e;
	short f;
	real_point3d position;
	real r1c;
};

struct s_line_b
{
	long a;
	long b;
	long c;
	byte d;
	byte e;
	short f;
	real_point3d position;
	real r1c;
	real r20;
	real r24;
	real r28;
};

struct s_line_list
{
	short count_a;
	short count_b;
	byte unknown04[4];
	s_line_a lines_a[0x100];
	s_line_b lines_b[0x100];
};

// @retail 0x244ca0
void function_244ca0(s_line_list *list, long a, long b, long c, byte d, byte e, short f, real_point3d const *position, real height, real radius)
{
	if (list->count_a < 0x100)
	{
		s_line_a *line = &list->lines_a[list->count_a++];

		line->a = a;
		line->b = b;
		line->d = d;
		line->e = e;
		line->c = c;
		line->f = f;
		line->position = *position;
		line->r1c = radius;
	}

	if (height > 0.0f)
	{
		real z = position->z - height;

		if (list->count_a < 0x100)
		{
			s_line_a *line = &list->lines_a[list->count_a++];

			line->a = a;
			line->b = b;
			line->c = c;
			line->d = d;
			line->e = e;
			line->f = f;
			line->position.x = position->x;
			line->position.y = position->y;
			line->position.z = z;
			line->r1c = radius;
		}

		if (list->count_b < 0x100)
		{
			s_line_b *line = &list->lines_b[list->count_b++];

			line->a = a;
			line->b = b;
			line->c = c;
			line->d = d;
			line->e = e;
			line->f = f;
			line->position.x = position->x;
			line->position.y = position->y;
			line->position.z = z;
			line->r1c = 0.0f;
			line->r20 = 0.0f;
			line->r24 = height;
			line->r28 = radius;
		}
	}
}
