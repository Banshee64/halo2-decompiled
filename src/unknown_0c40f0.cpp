// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0C40F0.CPP: setting a value on the data (g_4e031c) an object keeps
   for a tag. Decompiled by lane R for the effects (0x1771a0). */

#include "unknown_11c920.h"
#include "globals.h"
#include "data_array.h"

s_record_pool *g_4e031c;

struct s_c40f0_datum
{
	short salt;
	byte flags;
	byte field_3;
	real time;
	long tag_index;
	long object_index;
	real value;
	point3f point;
	byte entries[0xf0];
};

struct s_c40f0_definition
{
	byte unknown00[2];
	short unknown02;
};

// @retail 0xc3a40
void function_c3a40()
{
	g_4e031c = data_new_inlined("liquid", 64, 0x110, 0, g_510c2c);
}

// @retail 0xc3a80
void function_c3a80()
{
	s_record_pool *array = g_4e031c;
	array->valid = true;
	record_pool_release_all(array);
}

// @retail 0xc3aa0
void function_c3aa0()
{
	g_4e031c->valid = false;
}

// @retail 0xc3ab0
void function_c3ab0()
{
	if (g_4e031c)
		g_4e031c = 0;
}

// @retail 0xc3cc0
void __stdcall function_c3cc0(long index)
{
	s_record_pool *array = g_4e031c;
	if (array && array->valid && index != NONE)
		record_pool_release(array, index);
}

/* The lifecycle slots and deletion slot of the liquid callback table. */
void (*g_467510[4])() =
{
	function_c3a40, function_c3a80, function_c3aa0, function_c3ab0
};
void (__stdcall *g_467524)(long) = function_c3cc0;

// @retail 0xc4210
byte *function_c4210(long index, long element_index)
{
	s_record_pool *array = g_4e031c;
	byte *result = 0;
	if (array && array->valid && index != NONE)
	{
		s_c40f0_datum *liquid = (s_c40f0_datum *)array->data + (index & 0xffff);
		result = (byte *)liquid + 0x20 + element_index * 0x50;
	}
	return result;
}

// @retail 0xc4250
long function_c4250(long index)
{
	s_record_pool *array = g_4e031c;
	long result = NONE;
	if (array && array->valid && index != NONE)
		result = ((s_c40f0_datum *)array->data)[index & 0xffff].object_index;
	return result;
}

struct s_liquid_object_tree
{
	byte unknown00[0xc];
	long next_sibling;
	long first_child;
};

struct s_liquid_object_header
{
	byte unknown00[8];
	s_liquid_object_tree *object;
};

// @retail 0xc4280
long __stdcall function_c4280(long object_index, long target_index)
{
	long result = NONE;
	if (object_index == target_index)
		return object_index;
	s_liquid_object_tree *object = ((s_liquid_object_header *)g_4e0300->data)[object_index & 0xffff].object;
	if (object->first_child != NONE)
		result = function_c4280(object->first_child, target_index);
	if (result == NONE && object->next_sibling != NONE)
		result = function_c4280(object->next_sibling, target_index);
	return result;
}

// @retail 0xc40f0
void function_c40f0(long tag_index, long object_index, real value)
{
	s_record_pool *array = g_4e031c;

	if (array && array->valid && tag_index != NONE && object_index != NONE)
	{
		s_c40f0_definition *definition = (s_c40f0_definition *)g_4e3b44[tag_index & 0xffff].bytes;
		long datum = data_datum_index(array, function_16bc00(array, 0));

		while (datum != NONE)
		{
			s_c40f0_datum *element = (s_c40f0_datum *)array->data + (datum & 0xffff);

			if (element->object_index == object_index && element->tag_index == tag_index &&
				((s_c40f0_definition *)g_4e3b44[element->tag_index & 0xffff].bytes)->unknown02 == 0 &&
				definition->unknown02 == 0)
			{
				real clamped;

				if (0.0f > value)
					clamped = 0.0f;
				else if (value > 1.0f)
					clamped = 1.0f;
				else
					clamped = value;
				element->value = clamped;
			}
			datum = data_datum_index(array, function_16bc00(array, datum == NONE ? 0 : (datum & 0xffff) + 1));
		}
	}
}


struct s_liquid_definition_ab
{
    byte unknown00[2];
    short kind;
    long enabled;
    byte unknown08[0x68 - 8];
    long count;
};
struct s_liquid_creation_header_ab
{
    short salt;
    byte flags;
    byte type;
    byte unknown04[4];
    byte *object;
};
struct s_object;
s_object *function_badc0(long object_index, dword type_mask);
long function_baf80(long object_index);
point3f *function_b9dd0(long object_index, point3f *result);
struct s_random_draw;
void function_50690(s_random_draw *draw);

static __forceinline long liquid_datum_ab(s_record_pool *pool, long index)
{
    long result = NONE;
    if (index != NONE)
        result = (*(short *)(pool->data + index * pool->size) << 16) | index;
    return result;
}

// @retail 0xc3ad0
long function_c3ad0(long tag_index, long object_index)
{
    long const *local_947334_2 = &tag_index;
    s_record_pool *pool = g_4e031c;
    long result = NONE;
    if (pool && pool->valid && *local_947334_2 != NONE && object_index != NONE)
    {
        real time;
        if (g_510c54 && g_510c54->active)
            time = g_510c54->game_time * g_510c54->rate;
        else
            time = 0.0f;
        s_liquid_definition_ab *definition = (s_liquid_definition_ab *)g_4e3b44[tag_index & 0xffff].bytes;
        if (definition->kind == 2)
        {
            s_liquid_creation_header_ab *header = &((s_liquid_creation_header_ab *)g_4e0300->data)[object_index & 0xffff];
            if (header->type == 5)
            {
                long owner = *(long *)(header->object + 0xc8);
                tag_index = function_baf80(owner);
                if (function_badc0(owner, 0xffffffff))
                {
                    long index = liquid_datum_ab(pool, function_16bc00(pool, 0));
                    while (index != NONE)
                    {
                        s_c40f0_datum *liquid = &((s_c40f0_datum *)pool->data)[index & 0xffff];
                        s_liquid_definition_ab *other = (s_liquid_definition_ab *)g_4e3b44[liquid->tag_index & 0xffff].bytes;
                        if (other->kind == 1 && function_c4280(tag_index, liquid->object_index) != NONE)
                        {
                            function_b9dd0(object_index, &liquid->point);
                            liquid->time = time;
                        }
                        index = record_pool_next_used(pool, index);
                    }
                }
            }
        }
        else if (definition->enabled)
        {
            result = record_pool_allocate(pool);
            if (result != NONE)
            {
                s_c40f0_datum *liquid = &((s_c40f0_datum *)pool->data)[result & 0xffff];
                liquid->flags = (liquid->flags & ~2) | 1;
                liquid->time = definition->kind == 0 ? -1.0f : 0.0f;
                liquid->field_3 = 0;
                liquid->value = definition->kind == 0 ? 0.0f : 1.0f;
                liquid->object_index = object_index;
                liquid->tag_index = tag_index;
                for (long i = 0; i < definition->count; i++)
                    function_50690((s_random_draw *)(liquid->entries + i * 0x50));
            }
        }
    }
    return result;
}
