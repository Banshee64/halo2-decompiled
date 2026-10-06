// @flags /O2 /Gr
#include "unknown_11c920.h"
#include "globals.h"
#include "havok_reference.h"
#include "unknown_1cec30.h"
#include "object_list.h"

/* Interfaces whose destructors restore their retail virtual tables. */
struct s_278a70_event;
struct s_278da0_group;
class c_library_30c470
{
public:
	void detach(void *callback);
};
class c_interface_2777d0
{
public:
	virtual ~c_interface_2777d0();
	virtual void slot1(hkEntity *entity) { }
	virtual void slot2(hkEntity *entity) { }
};

class c_interface_2777f0
{
public:
	virtual ~c_interface_2777f0();
	virtual void slot1() { }
	virtual void slot2(s_278a70_event *event) { }
	virtual void slot3() { }
	virtual void slot4(void *context);
	virtual void slot5() { }
	virtual long slot6() { return 0; }
	long unknown04;
	c_library_30c470 *library;
};

class c_interface_278b40
{
public:
	virtual ~c_interface_278b40();
	virtual void slot1(s_278da0_group *group);
	virtual void slot2(s_278da0_group *group);
};

// @retail 0x2777d0 deleting
c_interface_2777d0::~c_interface_2777d0()
{
}

// @retail 0x277800 deleting c_interface_2777f0
// @retail 0x2777f0
c_interface_2777f0::~c_interface_2777f0()
{
}

// @retail 0x278b40 deleting
c_interface_278b40::~c_interface_278b40()
{
}

void __cdecl function_2d91d0(void *array, long element_size);

struct s_277890_buffer
{
	long *data;
	long count;
	long capacity;

	__forceinline void append(long value)
	{
		if (count == (capacity & 0x7fffffff))
			function_2d91d0(this, sizeof(long));
		data[count++] = value;
	}

	~s_277890_buffer()
	{
		if (!(capacity & 0x80000000))
			g_480118->allocate((long)data, (capacity & 0x7fffffff) * sizeof(long), 0x12);
	}
};

class c_interface_277890 : public c_interface_2777d0
{
public:
	long unknown04;
	s_277890_buffer buffer;
	byte unknown14[8];
	c_havok_reference_counted reference;
	virtual void slot1(hkEntity *entity);
	virtual void slot2(hkEntity *entity);

};

// @retail 0x277890 deleting c_interface_277890

class c_callback_277820_target
{
public:
	virtual void slot0() {}
	virtual void slot1() {}
	virtual void slot2() {}
	virtual bool test(void *first, void *second) { return false; }
};

class c_callback_277820
{
public:
	virtual void slot0() {}
	virtual void slot1() {}
	virtual void slot2() {}
	virtual void slot3() {}
	virtual hkBool test(void *first, void *second);
	long unknown04;
	c_callback_277820_target *target;
};

// @retail 0x277820
hkBool c_callback_277820::test(void *first, void *second)
{
	hkBool result;
	result.m_bool = target->test(first, second);
	return result;
}

struct s_278300_references
{
	byte unknown00[8];
	c_havok_reference_counted **data;
	long count;
	long capacity;
	long index;
};

// @retail 0x278300
void function_278300(s_278300_references *references)
{
	while (references->index + 1 < references->count)
	{
		references->index++;
		havok_reference_remove(references->data[references->index]);
	}
	references->index = NONE;
	references->count = 0;
}

void function_b7360(long object_index);
void function_226a60(long component_index);

// @retail 0x277990
void c_interface_277890::slot2(hkEntity *entity)
{
	long index = havok_entity_property_get(entity, 0x2001);
	if (index != NONE)
	{
		s_havok_component *component = havok_component_get(index);
		component->unknown04 &= ~8;
		function_b7360(component->object_index);
		function_226a60(index);
	}
}

void __cdecl function_2d91d0(void *array, long element_size);
void havok_component_rigid_body_state_update(long index, s_havok_component *component);

__forceinline long property_2778e0(hkEntity const *entity)
{
	long count = entity->m_property_count;
	for (long i = 0; i < count; i++)
	{
		if (entity->m_properties[i].m_key == 0x2001)
			return entity->m_properties[i].m_value.m_data;
	}
	return 0;
}

// @retail 0x2778e0
void c_interface_277890::slot1(hkEntity *entity)
{
	if (unknown14[4])
	{
		((c_havok_reference_counted *)entity)->reference_count++;
		buffer.append((long)entity);
	}
	long index = property_2778e0(entity);
	if (index != NONE)
	{
		s_havok_component *component = havok_component_get(index);
		if ((bool)component->flag5)
		{
			for (long i = 0; i < component->rigid_bodies.size; i++)
				havok_component_rigid_body_state_update(i, component);
		}
	}
}

struct s_279670_object
{
	byte unknown00[0xb8];
	long platform_index;
	long unknownbc;
	word flag0 : 1;
	word flag1 : 1;
	word flag2 : 1;
	word others : 13;
};

struct s_279670_header
{
	byte unknown00[8];
	s_279670_object *object;
};

// @retail 0x279670
bool __cdecl function_279670(hkEntity *entity)
{
	long index = havok_entity_property_get(entity, 0x2001);
	bool result = true;
	if (index != NONE)
	{
		long object_index = havok_component_get(index)->object_index;
		s_279670_object *object = ((s_279670_header *)g_4e0300->data)[object_index & 0xffff].object;
		if ((bool)object->flag1 || (bool)object->flag2)
		{
			long platform_index = object->platform_index;
			bool found = false;
			if (platform_index != NONE)
			{
				for (long i = 0; i < g_5107f0->object_count; i++)
				{
					if (g_5107f0->object_indices[i] == platform_index)
					{
						found = true;
						break;
					}
				}
			}
			result = !found;
		}
	}
	return result;
}

struct s_278a70_body
{
	byte unknown00[0x18];
	long type;
	long unknown1c;
	hkEntity *entity;
};

struct s_278a70_event
{
	long unknown00;
	s_278a70_body *body;
};

class c_callback_2789b0 : public c_havok_reference_counted, public c_interface_2777f0
{
public:
	virtual void slot2(s_278a70_event *event);
	static void operator delete(void *block)
	{
		g_480118->allocate((long)block, ((c_callback_2789b0 *)block)->allocation_size, 0x10);
	}
};

// @retail 0x278ae0
void c_interface_2777f0::slot4(void *context)
{
	library->detach(((byte *)this - 8) ? this : NULL);
	c_havok_reference_counted *object = (c_havok_reference_counted *)((byte *)this - 8);
	havok_reference_remove(object);
}

// @retail 0x278a70
void c_callback_2789b0::slot2(s_278a70_event *event)
{
	if (event->body->type == 1)
	{
		hkEntity *entity = event->body->entity;
		if (entity)
		{
			long index = havok_entity_property_get(entity, 0x2001);
			if (index != NONE)
			{
				s_havok_component *component = havok_component_get(index);
				component->flags12 &= ~1;
			}
		}
	}
}

class c_278da0_resource
{
public:
	virtual void slot0() {}
	virtual void slot1() {}
	virtual void slot2() {}
	virtual void slot3() {}
	virtual void finish() {}
	virtual void release() {}
	long unknown04;
	c_278da0_resource *owner;
};

class c_278da0_kind
{
public:
	virtual void slot0() {}
	virtual void slot1() {}
	virtual void slot2() {}
	virtual void slot3() {}
	virtual void slot4() {}
	virtual void slot5() {}
	virtual void slot6() {}
	virtual long type() { return 0; }
};

class c_library_311690
{
public:
	void update(long value);
	byte unknown00[0x54];
	c_278da0_kind *kind;
};

struct s_278da0_entry
{
	byte *first;
	byte *second;
	c_278da0_resource *resource;
	long unknown0c;
};

struct s_278da0_group
{
	byte unknown00[0x20];
	class c_library_30c190 *context;
	byte unknown24[0x3c - 0x24];
	c_library_311690 **items;
	long item_count;
	byte unknown44[0x24];
	s_278da0_entry *entries;
	long count;
};

__forceinline hkRigidBody *body_278da0(byte *contact)
{
	s_278a70_body *body = (s_278a70_body *)(contact - 0x10);
	return body->type == 1 ? (hkRigidBody *)body->entity : NULL;
}

// @retail 0x278da0
void c_interface_278b40::slot2(s_278da0_group *group)
{
	for (long i = 0; i < group->count; i++)
	{
		s_278da0_entry *entry = &group->entries[i];
		if (body_278da0(entry->first)->m_fixed || body_278da0(entry->second)->m_fixed)
		{
			c_278da0_resource *resource = entry->resource;
			if (resource)
			{
				c_278da0_resource *owner = resource->owner;
				resource->release();
				owner->finish();
			}
			group->entries[i].resource = NULL;
		}
	}
	for (long i = 0; i < group->item_count; i++)
	{
		if (group->items[i]->kind->type() == 2)
			group->items[i]->update(1);
	}
}

class c_278b60_filter
{
public:
	virtual hkBool test0(s_278a70_body *first, s_278a70_body *second) { return hkBool(); }
	virtual hkBool test1(s_278a70_body *first, s_278a70_body *second) { return hkBool(); }
};

struct s_278b60_filter
{
	byte unknown00[8];
	byte interface08[4];
};

struct s_278b60_dispatch;

class c_278b60_factory
{
public:
	virtual void slot0() {}
	virtual c_278da0_resource *create(s_278a70_body *first, s_278a70_body *second,
		s_278b60_dispatch *dispatch) { return NULL; }
};

typedef c_278da0_resource *(__cdecl *function_278b60_create)(s_278a70_body *, s_278a70_body *,
	s_278b60_dispatch *, c_278da0_resource *);

struct s_278b60_table
{
	byte unknown00[0xc];
	c_278b60_factory *factories[64];
	byte unknown10c[0x80];
	function_278b60_create callbacks[1024];
};

struct s_278b60_dispatch
{
	s_278b60_table *table;
};

class c_library_30c190
{
public:
	c_278b60_filter *filter();
	byte unknown00[0xcc];
	s_278b60_dispatch *dispatch;
	s_278b60_filter *filter_object;
};

class c_278b60_shape
{
public:
	virtual void slot0() {}
	virtual void slot1() {}
	virtual void slot2() {}
	virtual void slot3() {}
	virtual void slot4() {}
	virtual long type() { return 0; }
};

// @retail 0x278b60
void c_interface_278b40::slot1(s_278da0_group *group)
{
	for (long i = 0; i < group->count; i++)
	{
		s_278da0_entry *entry = &group->entries[i];
		if (!entry->resource)
		{
			s_278a70_body *first = (s_278a70_body *)(entry->first - 0x10);
			s_278a70_body *second = (s_278a70_body *)(entry->second - 0x10);
			hkRigidBody *first_body = first->type == 1 ? (hkRigidBody *)first->entity : NULL;
			hkRigidBody *second_body = second->type == 1 ? (hkRigidBody *)second->entity : NULL;
			s_278b60_filter *filter = group->context->filter_object;
			if (filter && !((c_278b60_filter *)filter->interface08)->test0(first, second).m_bool)
			{
				group->entries[i].resource = NULL;
				continue;
			}
			s_278b60_dispatch *dispatch = group->context->dispatch;
			long first_type = *((char *)first_body + 0x48);
			long second_type = *((char *)second_body + 0x48);
			c_278da0_resource *owner = dispatch->table->factories[first_type * 8 + second_type]->create(first, second, dispatch);
			dispatch = group->context->dispatch;
			s_278b60_table *table = dispatch->table;
			long first_shape = (*(c_278b60_shape **)first)->type();
			long second_shape = (*(c_278b60_shape **)second)->type();
			function_278b60_create create = table->callbacks[first_shape * 32 + second_shape];
			s_278da0_entry *destination = &group->entries[i];
			destination->resource = create(first, second, dispatch, owner);
			c_278da0_resource **resource = &group->entries[i].resource;
			if (*resource && (*resource)->owner)
			{
				hkRigidBody *active_first = first->type == 1 ? (hkRigidBody *)first->entity : NULL;
				hkRigidBody *active_second = second->type == 1 ? (hkRigidBody *)second->entity : NULL;
				if ((active_first->m_motion->getType() != 6 && !active_first->m_fixed) ||
					(active_second->m_motion->getType() != 6 && !active_second->m_fixed))
				{
					if (group->context->filter()->test1(first, second).m_bool)
						group->entries[i].unknown0c = 0;
					else
						group->entries[i].unknown0c = NONE;
				}
				else
					group->entries[i].unknown0c = NONE;
			}
			else
			{
				*resource = NULL;
				owner->finish();
			}
		}
	}
	for (long i = 0; i < group->item_count; i++)
	{
		if (group->items[i]->kind->type() == 1)
			group->items[i]->update(2);
	}
}

long function_147090(void);
void *g_55e508;
void *g_55e50c;
long g_55e504;
long g_55e500;
bool g_55e4fc;

struct s_278e80_state
{
	byte unknown00[0xae];
	bool changed;
};

// @retail 0x278e60
void __cdecl function_278e60(void *context)
{
	g_55e508 = context;
	g_55e504 = function_147090();
}

// @retail 0x278e80
void __cdecl function_278e80(s_278e80_state *state)
{
	g_55e508 = NULL;
	if (function_147090() - g_55e504)
	{
		state->changed = true;
		g_55e4fc = true;
	}
}

// @retail 0x278eb0
void __cdecl function_278eb0(void *context)
{
	g_55e50c = context;
	g_55e500 = function_147090();
}

// @retail 0x278ed0
void __cdecl function_278ed0(s_278e80_state *state)
{
	g_55e50c = NULL;
	if (function_147090() - g_55e500)
	{
		state->changed = true;
		g_55e4fc = true;
	}
}

struct s_2797a0_group
{
	byte unknown00[0x40];
	long count;
};

struct s_2797a0_groups
{
	byte unknown00[8];
	s_2797a0_group **first;
	long first_count;
	long unknown10;
	s_2797a0_group **second;
	long second_count;
};

struct s_2797a0_iterator
{
	byte unknown00;
	bool second;
	byte unknown02[2];
	long group;
	long index;
	s_2797a0_groups *groups;
	long mode;
};

// @retail 0x2797a0
bool function_2797a0(s_2797a0_iterator *iterator)
{
	bool result = false;
	if (iterator->group == NONE)
	{
		iterator->group = 0;
		iterator->index = 0;
		if (iterator->mode == 1)
			iterator->second = true;
	}
	else
		iterator->index++;
	if (!iterator->second)
	{
		s_2797a0_groups *groups = iterator->groups;
		while (iterator->group < iterator->groups->first_count)
		{
			if (iterator->index < groups->first[iterator->group]->count)
			{
				result = true;
				break;
			}
			iterator->group++;
			iterator->index = 0;
		}
		if (!result)
		{
			iterator->second = true;
			iterator->group = 0;
			iterator->index = 0;
		}
	}
	if (iterator->second && (iterator->mode == 2 || iterator->mode == 1))
	{
		s_2797a0_groups *groups = iterator->groups;
		while (iterator->group < iterator->groups->second_count)
		{
			if (iterator->index < groups->second[iterator->group]->count)
			{
				result = true;
				break;
			}
			iterator->group++;
			iterator->index = 0;
		}
	}
	return result;
}
