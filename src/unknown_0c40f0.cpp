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
	byte unknown02[6];
	long tag_index;
	long object_index;
	real value;
	byte unknown14[0x110 - 0x14];
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
