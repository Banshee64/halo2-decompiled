#include "cseries.h"
#include "globals.h"
#include "data_array.h"

// @flags /O2 /Gr

/* the widget types (antenna, cloth, light volume...): a table of 0x38 byte
   entries keyed by the tag group, with a set of callbacks each */
struct s_widget_type
{
	long key;
	long unknown04;
	void (*initialize)(void);
	void (*initialize_for_new_map)(void);
	void (*dispose_from_old_map)(void);
	void *unknown14;
	long (__stdcall *create)(long arg, long object_index);
	void (__stdcall *dispose)(long handle);
	byte unknown20[0x38 - 0x20];
};

/* one widget attached to an object (12 bytes) */
struct s_widget
{
	word identifier;
	short type;
	long handle;
	long next;
};

struct s_widget_reference
{
	long key;
	long arg;
};

struct s_widget_object_tag_data
{
	byte unknown00[0x9c];
	long count;
	s_widget_reference *references;
};

struct s_widget_object_header
{
	short identifier;
	byte flags;
	byte type;
	byte unknown04[4];
	byte *object;
};

struct s_widget_object
{
	long tag_index;
	byte unknown04[0xdc - 4];
	long widget_head;
};

s_widget_type g_467498[3];

PRIVATE short widget_type_find(long key)
{
	short i;

	for (i = 0; i < 3; i++)
	{
		if (g_467498[i].key == key)
			return i;
	}

	return NONE;
}

// @retail 0xd4830
void widgets_initialize(void)
{
	short i;

	g_4e0320 = data_new_inlined("widget", 0x40, sizeof(s_widget), 0, g_510c2c);

	for (i = 0; i < 3; i++)
	{
		if (g_467498[i].initialize)
			g_467498[i].initialize();
	}
}

// @retail 0xd4890
void widgets_initialize_for_new_map(void)
{
	short i;

	g_4e0320->valid = 1;
	data_delete_all(g_4e0320);

	for (i = 0; i < 3; i++)
	{
		if (g_467498[i].initialize_for_new_map)
			g_467498[i].initialize_for_new_map();
	}
}

// @retail 0xd48d0
void widgets_dispose_from_old_map(void)
{
	short i;

	for (i = 0; i < 3; i++)
	{
		if (g_467498[i].dispose_from_old_map)
			g_467498[i].dispose_from_old_map();
	}

	g_4e0320->valid = 0;
}

// @retail 0xd4900
void object_widgets_new(long object_index)
{
	s_widget_object_header *header = (s_widget_object_header *)(g_4e0300->data + (object_index & 0xffff) * 12);
	s_widget_object *object = (s_widget_object *)header->object;
	s_widget_object_tag_data *tag_data = (s_widget_object_tag_data *)g_4e3b44[object->tag_index & 0xffff].bytes;
	short i;

	object->widget_head = NONE;

	for (i = 0; i < tag_data->count; i++)
	{
		s_widget_reference *reference = &tag_data->references[i];
		short type = widget_type_find(reference->key);

		if (type != NONE && reference->arg != NONE)
		{
			s_widget_type *widget_type = &g_467498[type];
			long widget_index = datum_new(g_4e0320);

			if (widget_index != NONE)
			{
				s_widget *widget = (s_widget *)(g_4e0320->data + (widget_index & 0xffff) * 12);

				widget->type = type;

				if (widget_type->create)
				{
					widget->handle = widget_type->create(reference->arg, object_index);

					if (widget->handle != NONE)
					{
						widget->next = object->widget_head;
						object->widget_head = widget_index;
					}
					else
					{
						datum_delete(g_4e0320, widget_index);
					}
				}
				else
				{
					widget->next = object->widget_head;
					object->widget_head = widget_index;
					widget->handle = NONE;
				}
			}
		}
	}
}

// @retail 0xd4a60
void object_widgets_delete(long object_index)
{
	s_widget_object_header *header = (s_widget_object_header *)(g_4e0300->data + (object_index & 0xffff) * 12);
	s_widget_object *object = (s_widget_object *)header->object;
	long widget_index = object->widget_head;

	while (widget_index != NONE)
	{
		s_widget *widget = (s_widget *)(g_4e0320->data + (widget_index & 0xffff) * 12);
		long next = widget->next;

		if (widget->handle != NONE)
			g_467498[widget->type].dispose(widget->handle);

		datum_delete(g_4e0320, widget_index);
		widget_index = next;
	}

	object->widget_head = NONE;
}

// @retail 0xd4ae0
void object_widget_delete(long object_index, long handle)
{
	if (handle != NONE)
	{
		s_widget_object_header *header = (s_widget_object_header *)(g_4e0300->data + (object_index & 0xffff) * 12);
		s_widget_object *object = (s_widget_object *)header->object;
		long widget_index = object->widget_head;

		if (widget_index != NONE)
		{
			s_widget *widget = (s_widget *)(g_4e0320->data + (widget_index & 0xffff) * 12);

			if (widget->handle == handle)
			{
				object->widget_head = widget->next;

				if (widget->handle != NONE)
					g_467498[widget->type].dispose(widget->handle);

				datum_delete(g_4e0320, widget_index);
			}
			else
			{
				long previous_index = widget_index;

				do
				{
					s_widget *previous = (s_widget *)(g_4e0320->data + (previous_index & 0xffff) * 12);
					long next_index = previous->next;

					if (next_index == NONE)
						break;

					widget = (s_widget *)(g_4e0320->data + (next_index & 0xffff) * 12);

					if (widget->handle == handle)
					{
						previous->next = widget->next;

						if (widget->handle != NONE)
							g_467498[widget->type].dispose(widget->handle);

						datum_delete(g_4e0320, next_index);
						break;
					}

					previous_index = next_index;
				}
				while (true);
			}
		}
	}
}