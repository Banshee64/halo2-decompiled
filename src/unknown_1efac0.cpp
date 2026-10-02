#include <string.h>
#include "cseries.h"
#include "globals.h"
#include "unknown_1efac0.h"

// @flags /O2 /Gr

c_allocator *g_480118;
real g_45dbd8;

/* ---- c_d: a 12-byte object with a virtual destructor ---- */
struct c_d : c_a
{
	dword unknown08;

	virtual ~c_d();
	virtual void *v3() { return 0; }
	virtual long v5();
	virtual void v6() {}
	virtual real v7(long);
	virtual void v8(byte *, long, long);
};

// @retail 0x1efaf0
c_d::~c_d()
{
}

// @retail 0x1efad0
long c_d::v5()
{
	return 0x1b;
}

// @retail 0x1efac0
real c_d::v7(long)
{
	return g_45dbd8;
}

// @retail 0x1efae0
void c_d::v8(byte *a, long, long)
{
	*a = 0;
}

/* ---- object / tag lookups ---- */
struct s_tag_ref_data
{
	byte unknown00[0xc];
	long ref0c;
	byte unknown10[0x38 - 0x10];
	long ref38;
};

struct s_tag_instance_view
{
	byte unknown00[8];
	s_tag_ref_data *data;
	byte unknown0c[4];
};

struct s_object_view
{
	long tag_index;
	byte unknown04[0x116 - 4];
	short offset116;
	byte unknown118[2];
	short offset11a;
};

struct s_object_header_view
{
	byte unknown00[8];
	s_object_view *object;
};

struct s_object_data_view
{
	byte unknown00[0x44];
	s_object_header_view *headers;
};

struct s_lookup
{
	long handle;
	s_tag_ref_data *tag_a;
	s_tag_ref_data *tag_b;
	void *pointer_a;
	void *pointer_b;

	bool initialize(long handle);
};

// @retail 0x1efb40
bool s_lookup::initialize(long object_handle)
{
	s_object_data_view *header_data = (s_object_data_view *)g_4e0300;
	s_tag_instance_view *tags = (s_tag_instance_view *)g_4e3b44;
	s_object_view *object = header_data->headers[object_handle & 0xffff].object;
	s_tag_ref_data *definition = tags[object->tag_index & 0xffff].data;
	bool result = false;

	if (definition->ref38 != NONE)
	{
		s_tag_ref_data *a = tags[definition->ref38 & 0xffff].data;

		if (a->ref0c != NONE)
		{
			s_tag_ref_data *b = tags[a->ref0c & 0xffff].data;

			void *pb = (byte *)object + object->offset116;
			void *pa = (byte *)object + object->offset11a;

			pointer_a = pa;
			handle = object_handle;
			tag_a = a;
			tag_b = b;
			pointer_b = pb;
			result = true;
		}
	}

	return result;
}

/* ---- a three-level table of items, walked by a packed position ---- */
struct s_item
{
	byte first;
	byte unknown01[3];
	byte data[0x40];
};

struct s_sub
{
	long unknown00;
	long count;
	s_item *items;
	long unknown0c;
	long unknown10;
};

struct s_group
{
	long unknown00;
	long count;
	s_sub *subs;
};

struct s_table
{
	byte unknown00[0x1c];
	long count;
	s_group *groups;
};

struct s_table_holder
{
	byte unknown00[8];
	s_table *table;
};

// @retail 0x1efd80
void *function_1efd80(s_table_holder *holder, dword position)
{
	s_group *group = &holder->table->groups[position >> 24];
	s_sub *sub = &group->subs[(position >> 16) & 0xff];

	return sub->items[(position >> 8) & 0xff].data;
}

/* ---- the iterator ---- */
struct s_iterator
{
	s_table *table;
	signed char bytes[16];
	union
	{
		dword position;
		struct
		{
			byte first;
			byte item;
			byte sub;
			byte group;
		};
	};
	void *current;

	bool advance();
};

// @retail 0x1efca0
bool s_iterator::advance()
{
	long g = group;
	long sub_index = sub;
	long item_index;
	s_sub *s = 0;
	bool next = true;

	if (position != NONE)
	{
		s = &table->groups[g].subs[sub_index];

		if (s && item + 1 < s->count)
		{
			item_index = item + 1;
			next = false;
		}
	}

	if (next)
	{
		s_table *t = table;
		long count = t->count;

		for (;;)
		{
			g = g == NONE ? 0 : g + 1;

			if (g >= count)
			{
				g = NONE;
				break;
			}

			s_group *group_pointer = &t->groups[g];

			sub_index = bytes[g];
			if (sub_index >= 0 && sub_index < group_pointer->count)
			{
				s = &group_pointer->subs[sub_index];

				if (s->count > 0)
				{
					item_index = 0;
					break;
				}
			}
		}
	}

	if (g == NONE)
	{
		current = 0;
		position = NONE;
	}
	else
	{
		s_item *item_pointer = &s->items[item_index];

		position = ((((((sub_index & 0xff) | (g << 8)) << 8) | (item_index & 0xff)) << 8) | (item_pointer->first & 0xff));
		current = item_pointer->data;
	}

	return current != 0;
}

/* ---- the table of selected values ---- */
struct s_mix_item
{
	byte unknown00[5];
	byte value;
	byte unknown06[2];
};

struct s_mix_entry
{
	byte unknown00[4];
	byte id;
	byte unknown05[3];
	long count;
	s_mix_item *items;
};

struct s_mix_source
{
	byte unknown00[0x70];
	long count;
	s_mix_entry *entries;
};

struct s_mix_values
{
	signed char values[16];
};

struct s_mix_output
{
	long id;
	s_mix_values values;
	long position;
};

// @retail 0x1efbd0
void function_1efbd0(dword id, s_mix_output *output, s_mix_source *source, signed char *indices)
{
	output->id = id;
	output->position = NONE;

	if (!indices)
	{
		memset(&output->values, 0, sizeof(output->values));
	}
	else if (!source)
	{
		output->values = *(s_mix_values *)indices;
	}
	else
	{
		long count = 16;
		long i;

		if (source->count <= 16)
			count = source->count;

		memset(&output->values, 0xff, sizeof(output->values));

		for (i = 0; i < count; i++)
		{
			s_mix_entry *entry = &source->entries[i];
			long index = indices[i];

			if (entry->id != 0xff && index >= 0 && index < entry->count)
			{
				s_mix_item *item = &entry->items[index];
				byte value = item->value;

				if (value != 0xff)
					output->values.values[(signed char)entry->id] = value;
			}
		}
	}
}
