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

/* hkArrays: data, size, capacity (the top bit set when the array does not
   own its storage); Havok's thread memory (g_480118) frees the storage */
struct s_havok_component_element60
{
	byte unknown[0x60];
};

struct s_havok_component_element0c
{
	byte unknown[0xc];
};

struct s_havok_component_element48
{
	byte unknown[0x48];
};

struct s_havok_component_element08
{
	byte unknown[0x8];
};

struct s_havok_array60
{
	s_havok_component_element60 *data;
	long size;
	long capacity_and_flags;

	~s_havok_array60()
	{
		if (!(capacity_and_flags & 0x80000000))
		{
			g_480118->allocate((long)data, (capacity_and_flags & 0x7fffffff) * sizeof(s_havok_component_element60), 0x12);
		}
	}
};

struct s_havok_array0c
{
	s_havok_component_element0c *data;
	long size;
	long capacity_and_flags;

	~s_havok_array0c()
	{
		if (!(capacity_and_flags & 0x80000000))
		{
			g_480118->allocate((long)data, (capacity_and_flags & 0x7fffffff) * sizeof(s_havok_component_element0c), 0x12);
		}
	}
};

struct s_havok_array48
{
	s_havok_component_element48 *data;
	long size;
	long capacity_and_flags;

	~s_havok_array48()
	{
		if (!(capacity_and_flags & 0x80000000))
		{
			g_480118->allocate((long)data, (capacity_and_flags & 0x7fffffff) * sizeof(s_havok_component_element48), 0x12);
		}
	}
};

/* allocated on its own from Havok's thread memory */
struct s_havok_array08
{
	s_havok_component_element08 *data;
	long size;
	long capacity_and_flags;

	~s_havok_array08()
	{
		if (!(capacity_and_flags & 0x80000000))
		{
			g_480118->allocate((long)data, (capacity_and_flags & 0x7fffffff) * sizeof(s_havok_component_element08), 0x12);
		}
	}

	static void operator delete(void *block)
	{
		g_480118->allocate((long)block, sizeof(s_havok_array08), 0x12);
	}
};
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
	s_havok_array60 unknown70;
	s_havok_array0c unknown7c;
	s_havok_array48 unknown88;
	s_havok_array08 *unknown94;
	long unknown98;
	long unknown9c;

	~s_havok_component();
	void initialize(long object_index);
};



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
	component->unknown70.data = NULL;
	component->unknown70.size = 0;
	component->unknown70.capacity_and_flags = 0x80000000;
	component->unknown7c.data = NULL;
	component->unknown7c.size = 0;
	component->unknown7c.capacity_and_flags = 0x80000000;
	component->unknown88.data = NULL;
	component->unknown88.size = 0;
	component->unknown88.capacity_and_flags = 0x80000000;
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

long *g_51e9a0;

// @retail 0x1cf0b0
long havok_component_new(long object_index)
{
	s_data_array *components = (s_data_array *)g_51e9b8;
	long component_index = datum_new(components);
	s_havok_component *component = &((s_havok_component *)components->data)[component_index & 0xffff];
	if (component)
	{
		component->initialize(object_index);
	}
	havok_object_count(object_index);
	return component_index;
}
/* the object, its header and definition as 0x1cf8b0 reads them */
struct s_havok_vehicle_definition
{
	byte unknown000[0x1ec];
	dword unknown1ec_0 : 19;
	dword unknown1ec_19 : 1;
	dword unknown1ec_20 : 12;
};

struct s_havok_vehicle
{
	long definition_index;
	byte unknown004[0x134 - 0x4];
	dword unknown134_0 : 1;
	dword unknown134_1 : 1;
	dword unknown134_2 : 30;
	byte unknown138[0x248 - 0x138];
	long unknown248;
};

struct s_havok_component_object_header
{
	short identifier;
	byte flags;
	byte type;
	byte unknown04[4];
	s_havok_vehicle *object;
};

struct s_havok_friction
{
	byte unknown00[8];
	real friction;
	long unknown0c;
};

struct s_havok_material
{
	byte unknown00[0x3c];
	real friction;
	long unknown40;
};

// @retail 0x1cf8b0
void havok_component_friction_get(long component_index, s_havok_friction *result, s_havok_material const *material)
{
	s_havok_component *component = &((s_havok_component *)((s_data_array *)g_51e9b8)->data)[component_index & 0xffff];
	s_havok_component_object_header *header = &((s_havok_component_object_header *)g_4e0300->data)[component->object_index & 0xffff];
	real friction;

	if (((1 << header->type) & 2) && TEST_FIELD_BIT(((s_havok_vehicle_definition *)g_4e3b44[header->object->definition_index & 0xffff].bytes)->unknown1ec_19) &&
		(header->object->unknown248 != NONE || TEST_FIELD_BIT(header->object->unknown134_1)))
	{
		friction = 0.0f;
	}
	else
	{
		friction = material->friction;
	}
	result->friction = friction;
	result->unknown0c = material->unknown40;
}
/* data_next_absolute_index (unknown_16b570.cpp, built /Ob1), which retail
   inlines here */
static inline long havok_data_next_absolute_index(s_data_array *data, long index)
{
	long result = NONE;

	if (index >= 0)
	{
		for (; index < data->high_water_index; index++)
		{
			if (data->bitmap[index >> 5] & (1 << (index & 0x1f)))
			{
				result = index;
				break;
			}
		}
	}
	return result;
}

/* data_iterator_next (unknown_16b570.cpp), inlined */
static inline byte *havok_data_iterator_next(s_data_iterator *iterator)
{
	s_data_array *data = iterator->data;
	long index = havok_data_next_absolute_index(data, iterator->index + 1);
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

inline s_havok_component *havok_component_get(long component_index)
{
	return &((s_havok_component *)((s_data_array *)g_51e9b8)->data)[component_index & 0xffff];
}

void function_1d1260(s_havok_component *component);
void __stdcall function_1d01c0(s_havok_component *component);

// @retail 0x1cf9f0
s_havok_component::~s_havok_component()
{
	if (unknown04 & 0x20)
	{
		function_1d1260(this);
	}
	function_1d01c0(this);
	if (unknown94)
	{
		delete unknown94;
		unknown94 = NULL;
	}
}

// @retail 0x1cec80
void havok_components_dispose(void)
{
	s_data_iterator iterator;

	iterator.data = (s_data_array *)g_51e9b8;
	iterator.index = NONE;
	iterator.datum_index = NONE;
	while (havok_data_iterator_next(&iterator))
	{
		havok_component_get(iterator.datum_index)->~s_havok_component();
	}
	((s_data_array *)g_51e9b8)->valid = false;
}

// @retail 0x1cf220
void havok_component_delete(long component_index)
{
	s_havok_component *component = havok_component_get(component_index);
	long object_index = component->object_index;
	s_havok_object *object;

	component->~s_havok_component();
	datum_delete((s_data_array *)g_51e9b8, component_index);
	object = havok_object_get(object_index);
	if (TEST_FIELD_BIT(object->havok_flag))
	{
		object->havok_flag = 0;
		(*g_51e9a0)--;
	}
}
