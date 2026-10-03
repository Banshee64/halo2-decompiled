// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_209520.CPP: the script runtime's threads (hs_runtime): starting a
   script's thread, evaluating an expression node into a frame, and reading
   and casting global values (outside functions the command scripts of lane I
   call) */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"
#include "hs.h"

/* a frame of a thread's stack: the expression it evaluates and where its
   value goes; the frame's own data follows it */
struct s_hs_frame
{
	s_hs_frame *next;
	long expression_index;
	long *result;
	short size;
	byte unknown0e[2];
};

/* a script thread (g_4f9384, 0x418 bytes) */
struct s_hs_thread
{
	short salt;
	byte type;
	byte flags;
	long script_index;
	long sleep_until;
	byte unknown0c[4];
	s_hs_frame *frame;
	long result;
	s_hs_frame stack;
	byte unknown28[0x418 - 0x28];
};

/* an expression node (g_4f9394, 20 bytes) */
struct s_hs_expression
{
	short salt;
	short value_type;
	short type;
	byte flags;
	byte unknown07[0x10 - 0x7];
	long value;
};

/* a global's runtime value (g_4f9380, 8 bytes) */
struct s_hs_global_value
{
	byte unknown0[4];
	script_value value;
};

/* an external global's definition (g_473468 points at one per global) */
struct s_hs_external_global
{
	short type;
	byte unknown2[2];
	void *address;
};

/* the scenario's scripts and globals (0x28 bytes each) */
struct s_hs_script
{
	byte unknown00[0x20];
	short type;
	short return_type;
	long root_expression_index;
};

struct s_hs_global
{
	byte unknown00[0x20];
	short type;
	byte unknown22[0x28 - 0x22];
};

struct s_hs_scenario_view
{
	byte unknown000[0x1bc];
	s_hs_script *scripts;
	byte unknown1c0[4];
	s_hs_global *globals;
};

typedef long (__stdcall *t_hs_cast_proc)(long value);

extern s_data_array *g_4f9384;
extern s_data_array *g_4f9394;
extern t_hs_cast_proc g_4f5770[0x3e * 0x3e];
extern s_data_array *g_4f9380;
s_hs_external_global *g_473468[1];
char const *g_470010 = "";

long function_bb760(short index);

long function_2097c0(long script_index, byte type);
void function_2099f0(long thread_index, long *result, long expression_index);
long function_209bc0(short global_index);
long function_20a290(short type, short value_type, long value);
void function_20a2e0(short global_index);

inline s_hs_thread *hs_thread_get(long thread_index)
{
	return (s_hs_thread *)(g_4f9384->data + (thread_index & 0xffff) * sizeof(s_hs_thread));
}

inline s_hs_expression *hs_expression_get(long expression_index)
{
	return (s_hs_expression *)(g_4f9394->data + (expression_index & 0xffff) * sizeof(s_hs_expression));
}

inline s_hs_script *hs_script_get(long script_index)
{
	return &((s_hs_scenario_view *)g_4e0350)->scripts[script_index];
}

// @retail 0x209520
long function_209520(short script_index)
{
	s_hs_script *script = hs_script_get(script_index);
	long thread_index = NONE;

	if (script->type == 5)
	{
		thread_index = function_2097c0(script_index, 4);
		if (thread_index != NONE)
		{
			function_2099f0(thread_index, &hs_thread_get(thread_index)->result, script->root_expression_index);
		}
	}
	return thread_index;
}

// @retail 0x2097c0
long function_2097c0(long script_index, byte type)
{
	long thread_index = datum_new(g_4f9384);

	if (thread_index != NONE)
	{
		s_hs_thread *thread = hs_thread_get(thread_index);

		thread->frame = &thread->stack;
		thread->frame->next = NULL;
		thread->frame->size = 0;
		thread->frame->expression_index = NONE;
		thread->type = type;
		thread->script_index = script_index;
		thread->flags = 0;
		if (script_index != NONE && hs_script_get(script_index)->type == 1)
		{
			thread->sleep_until = -2;
		}
		else
		{
			thread->sleep_until = 0;
		}
	}
	return thread_index;
}

// @retail 0x2099f0
void function_2099f0(long thread_index, long *result, long expression_index)
{
	s_hs_thread *thread = hs_thread_get(thread_index);
	s_hs_expression *expression = hs_expression_get(expression_index);

	if (expression->flags & 1)
	{
		if (expression->flags & 4)
		{
			word global_index = (word)expression->value;
			short type;

			if (global_index & 0x8000)
			{
				type = g_473468[global_index & 0x7fff]->type;
			}
			else
			{
				type = ((s_hs_scenario_view *)g_4e0350)->globals[global_index & 0x7fff].type;
			}
			*result = function_20a290(expression->type, type, function_209bc0(global_index));
		}
		else
		{
			*result = function_20a290(expression->type, expression->value_type, expression->value);
		}
	}
	else
	{
		s_hs_frame *frame;

		thread->frame->result = result;
		frame = hs_thread_get(thread_index)->frame;
		s_hs_frame *pushed = (s_hs_frame *)((byte *)frame + sizeof(s_hs_frame) + frame->size);
		pushed->next = frame;
		hs_thread_get(thread_index)->frame = pushed;
		pushed->size = 0;
		thread->flags |= 1;
		thread->frame->expression_index = expression_index;
	}
}

// @retail 0x209bc0
long function_209bc0(short global_index)
{
	function_20a2e0(global_index);

	long index = global_index;

	return ((s_hs_global_value *)g_4f9380->data)[(index & 0x8000) ? (index & 0x7fff) : (index & 0x7fff) + 0x41d].value.d;
}

// @retail 0x20a290
long function_20a290(short type, short value_type, long value)
{
	if (value_type != type && value_type != _hs_type_passthrough)
	{
		if (type < _hs_type_object_name || type > _hs_type_scenery_name)
		{
			if (type >= _hs_type_object && type <= _hs_type_scenery)
			{
				if (value_type >= _hs_type_object_name && value_type <= _hs_type_scenery_name)
				{
					return function_bb760((short)value);
				}
			}
			else
			{
				return g_4f5770[type * 0x3e + value_type](value);
			}
		}
	}
	return value;
}

// @retail 0x20a2e0
void function_20a2e0(short global_index)
{
	if (global_index & 0x8000)
	{
		long index = global_index;
		long slot = index & 0x7fff;

		if (!(index & 0x8000))
		{
			slot += 0x41d;
		}

		s_hs_global_value *value = &((s_hs_global_value *)g_4f9380->data)[slot];
		s_hs_external_global *global = g_473468[global_index & 0x7fff];

		switch (global->type)
		{
		case _hs_type_boolean:
			value->value.b = global->address ? *(byte *)global->address : (byte)0;
			break;
		case _hs_type_real:
			value->value.r = global->address ? *(real *)global->address : 0.f;
			break;
		case _hs_type_short_integer:
			value->value.w = global->address ? *(word *)global->address : (word)0;
			break;
		case _hs_type_long_integer:
			value->value.d = global->address ? *(dword *)global->address : 0;
			break;
		case _hs_type_string:
			value->value.d = global->address ? *(dword *)global->address : (dword)g_470010;
			break;
		case _hs_type_script:
			value->value.w = global->address ? *(word *)global->address : (word)NONE;
			break;
		case _hs_type_string_id:
			value->value.d = global->address ? *(dword *)global->address : 0;
			break;
		case _hs_type_unit_seat_mapping:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_trigger_volume:
			value->value.w = global->address ? *(word *)global->address : (word)NONE;
			break;
		case _hs_type_cutscene_flag:
			value->value.w = global->address ? *(word *)global->address : (word)NONE;
			break;
		case _hs_type_cutscene_camera_point:
			value->value.w = global->address ? *(word *)global->address : (word)NONE;
			break;
		case _hs_type_cutscene_title:
			value->value.w = global->address ? *(word *)global->address : (word)NONE;
			break;
		case _hs_type_cutscene_recording:
			value->value.w = global->address ? *(word *)global->address : (word)NONE;
			break;
		case _hs_type_device_group:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_ai:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_ai_command_list:
			value->value.w = global->address ? *(word *)global->address : (word)NONE;
			break;
		case _hs_type_ai_command_script:
			value->value.w = global->address ? *(word *)global->address : (word)NONE;
			break;
		case _hs_type_ai_behavior:
			value->value.w = global->address ? *(word *)global->address : (word)NONE;
			break;
		case _hs_type_ai_orders:
			value->value.w = global->address ? *(word *)global->address : (word)NONE;
			break;
		case _hs_type_starting_profile:
			value->value.w = global->address ? *(word *)global->address : (word)NONE;
			break;
		case _hs_type_conversation:
			value->value.w = global->address ? *(word *)global->address : (word)NONE;
			break;
		case _hs_type_structure_bsp:
			value->value.w = global->address ? *(word *)global->address : (word)NONE;
			break;
		case _hs_type_navpoint:
			value->value.w = global->address ? *(word *)global->address : (word)NONE;
			break;
		case _hs_type_point_reference:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_style:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_hud_message:
			value->value.w = global->address ? *(word *)global->address : (word)NONE;
			break;
		case _hs_type_object_list:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_sound:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_effect:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_damage:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_looping_sound:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_animation_graph:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_damage_effect:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_object_definition:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_bitmap:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_shader:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_render_model:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_structure_definition:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_lightmap_definition:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_game_difficulty:
			value->value.w = global->address ? *(word *)global->address : (word)NONE;
			break;
		case _hs_type_team:
			value->value.w = global->address ? *(word *)global->address : (word)NONE;
			break;
		case _hs_type_actor_type:
			value->value.w = global->address ? *(word *)global->address : (word)NONE;
			break;
		case _hs_type_hud_corner:
			value->value.w = global->address ? *(word *)global->address : (word)NONE;
			break;
		case _hs_type_model_state:
			value->value.w = global->address ? *(word *)global->address : (word)NONE;
			break;
		case _hs_type_network_event:
			value->value.w = global->address ? *(word *)global->address : (word)NONE;
			break;
		case _hs_type_object:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_unit:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_vehicle:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_weapon:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_device:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_scenery:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_object_name:
			value->value.w = global->address ? *(word *)global->address : (word)NONE;
			break;
		}
	}
}
