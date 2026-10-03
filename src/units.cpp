// @flags /O2 /Gr
/* UNITS.CPP: the seats of units (units.obj) */

#include "cseries.h"
#include "globals.h"
#include "units.h"

struct s_unit_object
{
	long definition_index;
	dword unknown04 : 26;
	dword flag26 : 1;
	dword unknown04b : 5;
	byte unknown08[4];
	long next_sibling;
	long first_child;
	long parent_index;
	byte unknown18[0xaa - 0x18];
	byte type;
	byte unknownab[0x10a - 0xab];
	word unknown10a_0 : 2;
	word flag10a_2 : 1;
	word unknown10a_3 : 13;
	byte unknown10c[0x12a - 0x10c];
	short offset12a;
	byte unknown12c[0x1fc - 0x12c];
	short parent_seat_index;
};

struct s_unit_object_header
{
	short identifier;
	byte flags;
	byte type;
	byte unknown04[4];
	s_unit_object *object;
};

struct s_unit_definition
{
	byte unknown000[0x1c8];
	long seat_count;
	s_unit_seat_definition *seats;
};

#define UNIT_OBJECT_HEADER(index) (&((s_unit_object_header *)g_4e0300->data)[(index) & 0xffff])
#define UNIT_OBJECT(index) (UNIT_OBJECT_HEADER(index)->object)
#define UNIT_DEFINITION(object) ((s_unit_definition *)g_4e3b44[(object)->definition_index & 0xffff].bytes)

bool function_1cb920(void *data, long label);

// @retail 0xc8200
bool function_c8200(long object_index, long unit_index, short seat_index)
{
	s_unit_definition *definition = UNIT_DEFINITION(UNIT_OBJECT(object_index));
	bool result = false;

	if (seat_index >= 0 && seat_index < definition->seat_count)
	{
		s_unit_object *unit = UNIT_OBJECT(unit_index);

		if (unit->type == 1)
			return true;

		if (function_1cb920((byte *)unit + unit->offset12a, definition->seats[seat_index].label))
			return true;
	}

	return result;
}

// @retail 0xc8a40
void __stdcall function_c8a40(long object_index, s_object_seat *seats, short *count, short maximum_count)
{
	s_unit_object_header *header = UNIT_OBJECT_HEADER(object_index);
	s_unit_object *object = header->object;

	if (*count < maximum_count)
	{
		if (((1 << header->type) & 3) && !TEST_FIELD_BIT(object->flag10a_2))
		{
			s_unit_definition *definition = UNIT_DEFINITION(object);
			long seat_index;

			for (seat_index = 0; seat_index < definition->seat_count; seat_index++)
			{
				if (*count >= maximum_count)
					return;

				seats[*count].seat_index = (short)seat_index;
				seats[*count].object_index = object_index;
				seats[*count].definition = &definition->seats[seat_index];
				(*count)++;
			}
		}

		if (*count < maximum_count)
		{
			long child_index;

			for (child_index = object->first_child; child_index != NONE; )
			{
				s_unit_object *child = UNIT_OBJECT(child_index);

				if (((1 << child->type) & 3) && TEST_FIELD_BIT(child->flag26))
					function_c8a40(child_index, seats, count, maximum_count);

				child_index = child->next_sibling;
			}
		}
	}
}

// @retail 0xc8f60
long unit_seat_get_occupant(long unit_index, short seat_index)
{
	long child_index = UNIT_OBJECT(unit_index)->first_child;

	while (child_index != NONE)
	{
		s_unit_object *child = UNIT_OBJECT(child_index);

		if (((1 << child->type) & 3) && child->parent_seat_index == seat_index)
			break;

		child_index = child->next_sibling;
	}

	return child_index;
}
