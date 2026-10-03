// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1C25A0.CPP: the physics (Havok) system's lifecycle callbacks
   (0x441624..0x441638 in the lifecycle table) */

#include "cseries.h"
#include "game_state.h"
#include "data_array.h"
#include "globals.h"
#include "unknown_1cec30.h"
#include <xtl.h>
#include <stdio.h>
#include <stdarg.h>

/* the havok components (unknown_1cec30.cpp) */
struct s_manager_globals;
extern s_manager_globals *g_51e9b8;
void havok_components_initialize(void);

void game_state_initialize_1edbc0(void);
void function_2263c0(void);
void function_146b30(void);
void function_146b80(void);
void function_146de0(void);
void function_226440(void);

/* an aligned block of physical memory: the offset back to the allocation
   sits before it */
void *g_479888;

// @retail 0x1c25a0
void havok_initialize(void)
{
	g_51e9a0 = (long *)game_state_malloc("havok", "havok", sizeof(long));
	*g_51e9a0 = 0;
	game_state_initialize_1edbc0();
	function_2263c0();
	havok_components_initialize();
	function_146b30();
	function_146b80();
}

// @retail 0x1c2600
void havok_dispose(void)
{
	byte *block;

	function_146de0();
	block = (byte *)g_479888;
	if (!VirtualFree(block - ((long *)block)[-1], 0, MEM_RELEASE))
	{
		GetLastError();
	}
	g_479888 = NULL;
	data_dispose((s_data_array *)g_51e9b8);
	g_51e9b8 = NULL;
	function_226440();
}
/* the havok components (0xa0 bytes; unknown_1cec30.cpp) as the physics
   update sees them: a list of contacts at +0x70, each 0x60 bytes */
struct s_havok_contact_state
{
	byte unknown00[0xa8];
	long time;
	word flags;
};

struct s_havok_contact_owner
{
	byte unknown00[0x44];
	s_havok_contact_state *state;
};

struct s_havok_contact
{
	byte unknown00[0x40];
	s_havok_contact_owner *owner;
	byte unknown44[0x60 - 0x44];
};

/* an hkArray of contacts */
struct s_havok_contact_array
{
	s_havok_contact *data;
	long size;
	dword capacity_and_flags;

	s_havok_contact &operator[](long index)
	{
		return data[index];
	}
};

struct s_havok_component_contacts
{
	byte unknown00[0x70];
	s_havok_contact_array contacts;
	byte unknown7c[0xa0 - 0x7c];
};

inline s_havok_component_contacts *havok_component_contacts_get(long component_index)
{
	return &((s_havok_component_contacts *)((s_data_array *)g_51e9b8)->data)[component_index & 0xffff];
}

/* the components pool's counts (g_47f058 selects which limit applies) */
bool g_47f058;

// @retail 0x1c3930
bool havok_object_type_can_have_component(long definition_index)
{
	byte type = *g_4e3b44[definition_index & 0xffff].bytes;
	bool result = true;

	if ((1 << type) & 0x1883)
	{
		if (!g_47f058)
		{
			result = *g_51e9a0 < 0x200;
		}
		else
		{
			result = ((s_data_array *)g_51e9b8)->actual_count < ((s_data_array *)g_51e9b8)->maximum_count;
		}
	}
	return result;
}

// @retail 0x1c3980
void havok_object_count(long object_index)
{
	s_havok_object *object = havok_object_get(object_index);

	if (!TEST_FIELD_BIT(object->havok_flag))
	{
		object->havok_flag = 1;
		(*g_51e9a0)++;
	}
}

// @retail 0x1c4fc0
void havok_component_contacts_mark1(long component_index)
{
	if (component_index != NONE)
	{
		s_havok_component_contacts *component = havok_component_contacts_get(component_index);
		long i;

		for (i = 0; i < component->contacts.size; i++)
		{
			s_havok_contact_state *state = component->contacts[i].owner->state;

			if (state)
			{
				if (state->time != g_510c54->game_time)
				{
					state->flags = 0;
				}
				state->flags |= 1;
				state->time = g_510c54->game_time;
			}
		}
	}
}

// @retail 0x1c5040
void havok_component_contacts_mark2(long component_index)
{
	if (component_index != NONE)
	{
		s_havok_component_contacts *component = havok_component_contacts_get(component_index);
		long i;

		for (i = 0; i < component->contacts.size; i++)
		{
			s_havok_contact_state *state = component->contacts[i].owner->state;

			if (state)
			{
				if (state->time != g_510c54->game_time)
				{
					state->flags = 0;
				}
				state->flags |= 2;
				state->time = g_510c54->game_time;
			}
		}
	}
}
#define PIN(value, lower, upper) ((value) < (lower) ? (lower) : (value) > (upper) ? (upper) : (value))

/* the game time of the last ... (NONE when unset) */
long g_47f054 = NONE;

// @retail 0x1c58a0
bool function_1c58a0(void)
{
	long time = g_47f054;
	bool result = false;

	if (time != NONE)
	{
		long game_time = g_510c54->game_time;

		long lower = game_time - 3;

		result = (time < lower ? lower : game_time < time ? game_time : time) == time;
	}
	return result;
}

/* a Havok collision body: its shape, the shape key in its parent, and the
   parent body */
struct s_havok_shape_view
{
	byte unknown00[8];
	dword user_data;
};

struct s_havok_cd_body
{
	s_havok_shape_view *shape;
	long shape_key;
	byte unknown08[4];
	s_havok_cd_body *parent;
};

// @retail 0x1c55b0
long havok_cd_body_shape_key_get(s_havok_cd_body const *body)
{
	long result = NONE;

	while (body->parent)
	{
		if (body->parent->shape->user_data == 0xcabcabb0)
		{
			break;
		}
		body = body->parent;
	}
	if (body->parent)
	{
		result = body->shape_key;
	}
	return result;
}

// @retail 0x1c4560
void havok_printf(char const *format, ...)
{
	char buffer[0x104];
	va_list arguments;

	va_start(arguments, format);
	_vsnprintf(buffer, 0xfe, format, arguments);
}