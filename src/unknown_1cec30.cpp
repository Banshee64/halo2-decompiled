// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_1CEC30.CPP: the havok components (0x1cec30..0x1cffxx): a data
   array of 0x200 components of 0xa0 bytes, one per object with a Havok
   rigid body, and the properties the game keeps on the rigid bodies */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"
#include "unknown_1cec30.h"

/* the havok components (unknown_183c60.cpp has its own view of the data) */
struct s_manager_globals;
extern s_manager_globals *g_51e9b8;

c_data_allocator *g_468758;

struct s_havok_component
{
	short identifier;
	byte unknown02[2];
	dword unknown04;
	long object_index;
	long unknown0c;
	long unknown10;
	real unknown14;
	char unknown18;
	char unknown19;
	bool unknown1a;
	bool unknown1b;
	bool unknown1c;
	byte unknown1d[3];
	long unknown20;
	byte unknown24[0x70 - 0x24];
	s_havok_array unknown70;
	s_havok_array unknown7c;
	s_havok_array unknown88;
	void *unknown94;
	long unknown98;
	long unknown9c;

	void initialize(long object_index);
};

inline void havok_array_construct(s_havok_array *array)
{
	array->data = NULL;
	array->size = 0;
	array->capacity_and_flags = 0x80000000;
}

inline long havok_entity_property_get(hkEntity const *entity, dword key)
{
	long i;

	for (i = 0; i < entity->m_property_count; i++)
	{
		if (entity->m_properties[i].m_key == key)
		{
			return entity->m_properties[i].m_value.m_data;
		}
	}
	return 0;
}

inline bool havok_entity_property_exists(hkEntity const *entity, dword key)
{
	long i;

	for (i = 0; i < entity->m_property_count; i++)
	{
		if (entity->m_properties[i].m_key == key)
		{
			return true;
		}
	}
	return false;
}


// @retail 0x1cec30
void havok_components_initialize(void)
{
	g_51e9b8 = (s_manager_globals *)data_new_inlined("havok components", 0x200, sizeof(s_havok_component), 4, g_468758);
}

// @retail 0x1cf280
long havok_entity_component_index_get(hkEntity const *entity)
{
	return havok_entity_property_get(entity, HAVOK_PROPERTY_COMPONENT_INDEX);
}

// @retail 0x1cf2b0
void havok_entity_component_index_set(hkEntity *entity, hkPropertyValue component_index)
{
	if (havok_entity_property_exists(entity, HAVOK_PROPERTY_COMPONENT_INDEX))
	{
		entity->removeProperty(HAVOK_PROPERTY_COMPONENT_INDEX);
	}
	entity->addProperty(HAVOK_PROPERTY_COMPONENT_INDEX, component_index);
}

// @retail 0x1cf300
long havok_entity_property_2002_get(hkEntity const *entity)
{
	return havok_entity_property_get(entity, HAVOK_PROPERTY_2002);
}

// @retail 0x1cf330
void havok_entity_property_2002_set(hkEntity *entity, hkPropertyValue value)
{
	if (havok_entity_property_exists(entity, HAVOK_PROPERTY_2002))
	{
		entity->removeProperty(HAVOK_PROPERTY_2002);
	}
	entity->addProperty(HAVOK_PROPERTY_2002, value);
}

// @retail 0x1cf380
long __cdecl havok_entity_property_2003_get(hkEntity const *entity)
{
	return havok_entity_property_get(entity, HAVOK_PROPERTY_2003);
}

// @retail 0x1cf3b0
void havok_entity_property_2003_set(hkEntity *entity, hkPropertyValue value)
{
	if (havok_entity_property_exists(entity, HAVOK_PROPERTY_2003))
	{
		entity->removeProperty(HAVOK_PROPERTY_2003);
	}
	entity->addProperty(HAVOK_PROPERTY_2003, value);
}

// @retail 0x1cf950
void s_havok_component::initialize(long object_index)
{
	s_havok_component *component = this;
	real seconds;
	long ticks;

	component->object_index = object_index;
	component->unknown04 = 0;
	component->unknown0c = NONE;
	seconds = g_510c54->ticks_per_second * 0.35f;
	__asm
	{
		fld seconds
		fistp ticks
	}
	component->unknown18 = NONE;
	component->unknown19 = NONE;
	component->unknown20 = NONE;
	component->unknown14 = 0.0f;
	component->unknown1a = false;
	component->unknown1b = false;
	component->unknown1c = false;
	component->unknown10 = -ticks;
	havok_array_construct(&component->unknown70);
	havok_array_construct(&component->unknown7c);
	havok_array_construct(&component->unknown88);
	component->unknown94 = NULL;
	component->unknown98 = 0;
	component->unknown9c = 0;
}

// @retail 0x1cfdf0
bool havok_component_unknown10_recent(s_havok_component const *component)
{
	long game_time = g_510c54->game_time;
	real seconds = g_510c54->ticks_per_second * 0.35f;
	long ticks;

	__asm
	{
		fld seconds
		fistp ticks
	}
	return game_time - component->unknown10 < ticks;
}

// @retail 0x1cfe30
void havok_component_unknown10_expire(s_havok_component *component)
{
	long game_time = g_510c54->game_time;
	real seconds = g_510c54->ticks_per_second * 0.35f;
	long ticks;

	__asm
	{
		fld seconds
		fistp ticks
	}
	component->unknown10 = game_time - ticks + 1;
}

/* the objects as the havok components see them: a flag word at +0xc0 */
struct s_havok_object
{
	byte unknown000[0xc0];
	word havok_flag : 1;
	word unknownc0 : 15;
};

struct s_havok_object_header
{
	byte unknown00[8];
	s_havok_object *object;
};

long *g_51e9a0;

// @retail 0x1cf0b0
long havok_component_new(long object_index)
{
	s_data_array *components = (s_data_array *)g_51e9b8;
	long component_index = datum_new(components);
	s_havok_component *component = &((s_havok_component *)components->data)[component_index & 0xffff];
	s_havok_object *object;

	if (component)
	{
		component->initialize(object_index);
	}
	object = ((s_havok_object_header *)g_4e0300->data)[object_index & 0xffff].object;
	if (!TEST_FIELD_BIT(object->havok_flag))
	{
		object->havok_flag = 1;
		(*g_51e9a0)++;
	}
	return component_index;
}