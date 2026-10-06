// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_209520.CPP: the script runtime's threads (script_thread_runner): starting a
   script's thread, evaluating an expression node into a frame, and reading
   and casting global values (outside functions the command scripts of lane I
   call) */

#include "unknown_11c920.h"
#include "globals.h"
#include "data_array.h"
#include "hs.h"
#include <stddef.h>
#include <string.h>

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

extern s_record_pool *g_4f9384;
extern s_record_pool *g_4f9394;
extern t_hs_cast_proc g_4f5770[0x3e * 0x3e];
extern s_record_pool *g_4f9380;
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

long function_209e70(short script_index);
void __stdcall function_209c80(long thread_index);
void function_209ae0(long thread_index, long value);

// @retail 0x208c40
void __stdcall function_208c40(short function_index, long thread_index, bool initialize)
{
	s_hs_expression *expression = hs_expression_get(hs_thread_get(thread_index)->frame->expression_index);
	s_hs_expression *name = hs_expression_get(expression->value);
	s_hs_expression *argument = hs_expression_get(*(long *)((byte *)name + 8));
	long target = function_209e70((short)argument->value);
	if (target != NONE)
		function_209c80(target);
	function_209ae0(thread_index, 0);
}

s_type_f4462a const g_44b1f8 = { _hs_type_void, 2, function_208c40, "<script name>", 0, { 0 } };

// @retail 0x208750
void __stdcall function_208750(short function_index, long thread_index, bool initialize)
{
	s_hs_expression *expression = hs_expression_get(hs_thread_get(thread_index)->frame->expression_index);
	s_hs_expression *name = hs_expression_get(expression->value);
	long argument_index = *(long *)((byte *)name + 8);
	long target = thread_index;
	if (argument_index != NONE)
	{
		long script_index;
		function_2099f0(thread_index, &script_index, argument_index);
		target = function_209e70((short)script_index);
	}
	if (target != NONE)
		hs_thread_get(target)->sleep_until = -2;
	function_209ae0(thread_index, 0);
}

s_type_f4462a const g_44b1d8 = { _hs_type_void, 2, function_208750, "[<script>]", 0, { 0 } };

long *__stdcall function_209d50(long thread_index, short parameter_count, short const *parameter_types, bool initialize);

extern short const g_445710[62] =
{
	0, 0, 0, 0, 0, 1, 4, 2, 4, 4, 2, 4, 4, 2, 2, 2,
	2, 2, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 2, 4,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2,
	2, 2, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2
};

// @retail 0x208570
void __stdcall function_208570(short function_index, long thread_index, bool initialize)
{
	s_hs_expression *expression = hs_expression_get(hs_thread_get(thread_index)->frame->expression_index);
	s_hs_expression *name = hs_expression_get(expression->value);
	s_hs_expression *argument = hs_expression_get(*(long *)((byte *)name + 8));
	short type = argument->type;
	short types[2];
	types[1] = type;
	types[0] = type;
	long *values = function_209d50(thread_index, 2, types, initialize);
	if (values)
	{
		script_value result;
		bool comparison;
		switch (type)
		{
		case _hs_type_real:
		{
			real left = (real)*(real *)&values[0];
			real right = (real)*(real *)&values[1];
			switch (function_index)
			{
			case 15: comparison = left > right; break;
			case 16: comparison = left < right; break;
			case 17: comparison = left >= right; break;
			case 18: comparison = left <= right; break;
			default: comparison = false; break;
			}
			break;
		}
		case _hs_type_long_integer:
		{
			real left = (real)*(long *)&values[0];
			real right = (real)*(long *)&values[1];
			switch (function_index)
			{
			case 15: comparison = left > right; break;
			case 16: comparison = left < right; break;
			case 17: comparison = left >= right; break;
			case 18: comparison = left <= right; break;
			default: comparison = false; break;
			}
			break;
		}
		default:
		{
			real left = (real)*(short *)&values[0];
			real right = (real)*(short *)&values[1];
			switch (function_index)
			{
			case 15: comparison = left > right; break;
			case 16: comparison = left < right; break;
			case 17: comparison = left >= right; break;
			case 18: comparison = left <= right; break;
			default: comparison = false; break;
			}
			break;
		}
		}
		result.b = comparison;
		function_209ae0(thread_index, result.d);
	}
}

s_type_f4462a const g_44b188 = { _hs_type_boolean, 2, function_208570, "<number> <number>", 0, { 0 } };
s_type_f4462a const g_44b198 = { _hs_type_boolean, 2, function_208570, "<number> <number>", 0, { 0 } };
s_type_f4462a const g_44b1a8 = { _hs_type_boolean, 2, function_208570, "<number> <number>", 0, { 0 } };
s_type_f4462a const g_44b1b8 = { _hs_type_boolean, 2, function_208570, "<number> <number>", 0, { 0 } };

// @retail 0x2084c0
void __stdcall function_2084c0(short function_index, long thread_index, bool initialize)
{
	script_value result;
	s_hs_expression *expression = hs_expression_get(hs_thread_get(thread_index)->frame->expression_index);
	s_hs_expression *name = hs_expression_get(expression->value);
	s_hs_expression *argument = hs_expression_get(*(long *)((byte *)name + 8));
	short type = argument->type;
	short types[2];
	types[1] = type;
	types[0] = type;
	long *arguments = function_209d50(thread_index, 2, types, initialize);
	if (arguments)
	{
		bool equal = memcmp(&arguments[0], &arguments[1], g_445710[type]) == 0;
		if (function_index == 14)
			equal = !equal;
		result.b = equal;
		function_209ae0(thread_index, result.d);
	}
}

s_type_f4462a const g_44b168 = { _hs_type_boolean, 2, function_2084c0, "<expression> <expression>", 0, { 0 } };
s_type_f4462a const g_44b178 = { _hs_type_boolean, 2, function_2084c0, "<expression> <expression>", 0, { 0 } };

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
	long thread_index = record_pool_allocate(g_4f9384);

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

/* the thread being run (NONE between runs), and whether scripts run */
long g_4f938c = NONE;
bool g_4f9388;

void function_209ae0(long thread_index, long value);

/* room for a value in the thread's current frame */
static inline long *hs_frame_allocate_value(long thread_index)
{
	s_hs_frame *frame = hs_thread_get(thread_index)->frame;
	long *value = (long *)((byte *)frame + offsetof(s_hs_frame, unknown0e) + frame->size);

	frame->size += sizeof(long);
	return value;
}

// @retail 0x207ef0
void __stdcall function_207ef0(short function_index, long thread_index, bool initialize)
{
	long *condition = hs_frame_allocate_value(thread_index);
	long *branch = hs_frame_allocate_value(thread_index);
	long *value = hs_frame_allocate_value(thread_index);
	if (initialize)
	{
		*condition = 0;
		*branch = NONE;
		s_hs_expression *expression = hs_expression_get(hs_thread_get(thread_index)->frame->expression_index);
		s_hs_expression *name = hs_expression_get(expression->value);
		function_2099f0(thread_index, condition, *(long *)((byte *)name + 8));
	}
	else if (*branch == NONE)
	{
		s_hs_expression *expression = hs_expression_get(hs_thread_get(thread_index)->frame->expression_index);
		s_hs_expression *name = hs_expression_get(expression->value);
		s_hs_expression *test = hs_expression_get(*(long *)((byte *)name + 8));
		if (*(bool *)condition)
			*branch = *(long *)((byte *)test + 8);
		else
		{
			s_hs_expression *then_expression = hs_expression_get(*(long *)((byte *)test + 8));
			*branch = *(long *)((byte *)then_expression + 8);
			if (*branch == NONE)
			{
				function_209ae0(thread_index, 0);
				return;
			}
		}
		function_2099f0(thread_index, value, *branch);
	}
	else
		function_209ae0(thread_index, *value);
}

s_type_f4462a const g_44b0b8 = { _hs_type_passthrough, 2, function_207ef0, "<boolean> <then> [<else>]", 0, { 0 } };

// @retail 0x208310
void __stdcall function_208310(short function_index, long thread_index, bool initialize)
{
	s_hs_frame *frame = hs_thread_get(thread_index)->frame;
	short *count = (short *)((byte *)frame + offsetof(s_hs_frame, unknown0e) + frame->size);
	frame->size += sizeof(short);
	long *next = hs_frame_allocate_value(thread_index);
	real *value = (real *)hs_frame_allocate_value(thread_index);
	real *result = (real *)hs_frame_allocate_value(thread_index);
	if (initialize)
	{
		*count = 0;
		s_hs_expression *expression = hs_expression_get(hs_thread_get(thread_index)->frame->expression_index);
		s_hs_expression *name = hs_expression_get(expression->value);
		*next = *(long *)((byte *)name + 8);
	}
	else
	{
		if (*count == 0)
			*result = *value;
		else switch (function_index)
		{
		case 7: *result += *value; break;
		case 8: *result -= *value; break;
		case 9: *result *= *value; break;
		case 10: *result /= *value; break;
		case 11: *result = *result > *value ? *value : *result; break;
		case 12: *result = *result > *value ? *result : *value; break;
		}
		(*count)++;
	}
	if (*next != NONE)
	{
		function_2099f0(thread_index, (long *)value, *next);
		*next = *(long *)((byte *)hs_expression_get(*next) + 8);
	}
	else
	{
		script_value returned;
		returned.r = *result;
		function_209ae0(thread_index, returned.d);
	}
}

s_type_f4462a const g_44b108 = { _hs_type_real, 2, function_208310, "<number(s)>", 0, { 0 } };
s_type_f4462a const g_44b118 = { _hs_type_real, 2, function_208310, "<number> <number>", 0, { 0 } };
s_type_f4462a const g_44b128 = { _hs_type_real, 2, function_208310, "<number(s)>", 0, { 0 } };
s_type_f4462a const g_44b138 = { _hs_type_real, 2, function_208310, "<number> <number>", 0, { 0 } };
s_type_f4462a const g_44b148 = { _hs_type_real, 2, function_208310, "<number(s)>", 0, { 0 } };
s_type_f4462a const g_44b158 = { _hs_type_real, 2, function_208310, "<number(s)>", 0, { 0 } };

// @retail 0x2081e0
void __stdcall function_2081e0(short function_index, long thread_index, bool initialize)
{
	long *next = hs_frame_allocate_value(thread_index);
	long *value = hs_frame_allocate_value(thread_index);
	s_hs_frame *frame = hs_thread_get(thread_index)->frame;
	bool *result = (bool *)((byte *)frame + offsetof(s_hs_frame, unknown0e) + frame->size);
	frame->size++;
	bool conjunction = function_index == 5;
	if (initialize)
	{
		s_hs_expression *expression = hs_expression_get(hs_thread_get(thread_index)->frame->expression_index);
		s_hs_expression *name = hs_expression_get(expression->value);
		*next = *(long *)((byte *)name + 8);
		*result = conjunction;
	}
	else if (conjunction)
		*result = *result && *(bool *)value;
	else
		*result = *result || *(bool *)value;
	if (*next != NONE && *result == conjunction)
	{
		function_2099f0(thread_index, value, *next);
		*next = *(long *)((byte *)hs_expression_get(*next) + 8);
	}
	else
	{
		script_value returned;
		returned.b = *result;
		function_209ae0(thread_index, returned.d);
	}
}

s_type_f4462a const g_44b0e8 = { _hs_type_boolean, 2, function_2081e0, "<boolean(s)>", 0, { 0 } };
s_type_f4462a const g_44b0f8 = { _hs_type_boolean, 2, function_2081e0, "<boolean(s)>", 0, { 0 } };

// @retail 0x2087e0
void __stdcall function_2087e0(short function_index, long thread_index, bool initialize)
{
	long *delay = hs_frame_allocate_value(thread_index);
	long *script = hs_frame_allocate_value(thread_index);
	s_hs_frame *frame = hs_thread_get(thread_index)->frame;
	short *state = (short *)((byte *)frame + offsetof(s_hs_frame, unknown0e) + frame->size);
	frame->size += sizeof(short);
	if (initialize)
	{
		s_hs_expression *expression = hs_expression_get(hs_thread_get(thread_index)->frame->expression_index);
		s_hs_expression *name = hs_expression_get(expression->value);
		function_2099f0(thread_index, delay, *(long *)((byte *)name + 8));
		*state = 0;
	}
	else
	{
		if (*state == 0)
		{
			s_hs_expression *expression = hs_expression_get(hs_thread_get(thread_index)->frame->expression_index);
			s_hs_expression *name = hs_expression_get(expression->value);
			s_hs_expression *argument = hs_expression_get(*(long *)((byte *)name + 8));
			long script_expression = *(long *)((byte *)argument + 8);
			*state = 1;
			if (script_expression != NONE)
			{
				function_2099f0(thread_index, script, script_expression);
				return;
			}
			*script = NONE;
		}
		if (*state != 0)
		{
			long target = thread_index;
			if (*(short *)script != NONE)
				target = function_209e70(*(short *)script);
			if (target != NONE && *(short *)delay >= 0)
			{
				real seconds = (real)*(short *)delay * 0.03333333507180214f * (real)g_510c54->field_2_3;
				long ticks;
				__asm
				{
					fld seconds
					fistp ticks
				}
				if (ticks > 0)
				{
					s_hs_thread *thread = hs_thread_get(target);
					long until = g_510c54->game_time + ticks;
					if (thread->sleep_until != NONE)
					{
						if (target != thread_index && !(thread->flags & 2))
						{
							thread->flags |= 2;
							*(long *)thread->unknown0c = thread->sleep_until;
						}
						hs_thread_get(target)->sleep_until = until;
					}
				}
			}
			function_209ae0(thread_index, 0);
		}
	}
}

s_type_f4462a const g_44b1c8 = { _hs_type_void, 2, function_2087e0, "<short> [<script>]", 0, { 0 } };

PRIVATE inline long hs_delay_ticks(long delay)
{
	real seconds = (real)delay * 0.03333333507180214f * (real)g_510c54->field_2_3;
	long ticks;
	__asm
	{
		fld seconds
		fistp ticks
	}
	return ticks;
}

// @retail 0x2089b0
void __stdcall function_2089b0(short function_index, long thread_index, bool initialize)
{
	s_hs_thread *thread = hs_thread_get(thread_index);
	long *condition = hs_frame_allocate_value(thread_index);
	long *interval = hs_frame_allocate_value(thread_index);
	long *timeout = hs_frame_allocate_value(thread_index);
	long *start = hs_frame_allocate_value(thread_index);
	s_hs_frame *frame = hs_thread_get(thread_index)->frame;
	short *state = (short *)((byte *)frame + offsetof(s_hs_frame, unknown0e) + frame->size);
	frame->size += sizeof(short);
	s_hs_expression *expression = hs_expression_get(thread->frame->expression_index);
	s_hs_expression *name = hs_expression_get(expression->value);
	s_hs_expression *test = hs_expression_get(*(long *)((byte *)name + 8));
	long optional = *(long *)((byte *)test + 8);
	if (initialize)
	{
		*(bool *)condition = false;
		*start = g_510c54->game_time;
		*state = 0;
		*(short *)interval = 30;
		*timeout = NONE;
		if (optional != NONE)
		{
			function_2099f0(thread_index, interval, optional);
			return;
		}
	}
	if (*state == 0)
	{
		*state = 1;
		if (optional != NONE)
		{
			long next = *(long *)((byte *)hs_expression_get(optional) + 8);
			if (next != NONE)
			{
				function_2099f0(thread_index, timeout, next);
				return;
			}
		}
	}
	else if (*state != 1)
		return;
	long duration = *timeout == NONE ? NONE : hs_delay_ticks(*timeout);
	if (*(bool *)condition || (duration != NONE && g_510c54->game_time >= *start + duration))
		function_209ae0(thread_index, 0);
	else
	{
		expression = hs_expression_get(thread->frame->expression_index);
		name = hs_expression_get(expression->value);
		function_2099f0(thread_index, condition, *(long *)((byte *)name + 8));
		long ticks = hs_delay_ticks(*(short *)interval);
		if (ticks < 1)
			ticks = 1;
		thread->sleep_until = g_510c54->game_time + ticks;
		if (duration != NONE)
			thread->sleep_until = thread->sleep_until < *start + duration ? thread->sleep_until : *start + duration;
	}
}

s_type_f4462a const g_44b1e8 = { _hs_type_void, 2, function_2089b0, "<boolean> [<short>]", 0, { 0 } };

// @retail 0x207cf0
void __stdcall function_207cf0(short function_index, long thread_index, bool initialize)
{
	s_hs_frame *frame = hs_thread_get(thread_index)->frame;
	short *count = (short *)((byte *)frame + offsetof(s_hs_frame, unknown0e) + frame->size);
	frame->size += sizeof(short);
	dword *used = (dword *)hs_frame_allocate_value(thread_index);
	long *result = hs_frame_allocate_value(thread_index);
	if (initialize)
	{
		s_hs_expression *expression = hs_expression_get(hs_thread_get(thread_index)->frame->expression_index);
		s_hs_expression *name = hs_expression_get(expression->value);
		long next = *(long *)((byte *)name + 8);
		*count = 0;
		while (next != NONE)
		{
			next = *(long *)((byte *)hs_expression_get(next) + 8);
			(*count)++;
		}
		memset(used, 0, ((*count + 31) >> 5) * sizeof(dword));
	}
	g_4e7408->unknown0 = g_4e7408->unknown0 * 0x19660d + 0x3c6ef35f;
	short first = (short)(((g_4e7408->unknown0 >> 16) * *count) >> 16);
	short offset = 0;
	for (; offset < *count; offset++)
	{
		short selected = (first + offset) % *count;
		if (!(used[selected >> 5] & (1 << (selected & 31))))
		{
			s_hs_expression *expression = hs_expression_get(hs_thread_get(thread_index)->frame->expression_index);
			s_hs_expression *name = hs_expression_get(expression->value);
			long next = *(long *)((byte *)name + 8);
			for (short i = 0; i < selected; i++)
				next = *(long *)((byte *)hs_expression_get(next) + 8);
			function_2099f0(thread_index, result, next);
			used[selected >> 5] |= 1 << (selected & 31);
			break;
		}
	}
	if (offset == *count)
		function_209ae0(thread_index, *result);
}

s_type_f4462a const g_44b0a8 = { _hs_type_passthrough, 2, function_207cf0, "<expression(s)>", 0, { 0 } };

// @retail 0x207c20
void __stdcall function_207c20(short function_index, long thread_index, bool initialize)
{
	long *next = hs_frame_allocate_value(thread_index);
	long *result = hs_frame_allocate_value(thread_index);
	if (initialize)
	{
		s_hs_expression *expression = hs_expression_get(hs_thread_get(thread_index)->frame->expression_index);
		s_hs_expression *name = hs_expression_get(expression->value);
		*next = *(long *)((byte *)name + 8);
		*result = 0;
	}
	if (*next != NONE)
	{
		function_2099f0(thread_index, result, *next);
		*next = *(long *)((byte *)hs_expression_get(*next) + 8);
	}
	else
		function_209ae0(thread_index, *result);
}

s_type_f4462a const g_44b098 = { _hs_type_passthrough, 2, function_207c20, "<expression(s)>", 0, { 0 } };

extern short const g_4456d4[6] = { -1, 3, 2, 4, 0x380, 0x40 };

// @retail 0x208f10
void __stdcall function_208f10(short function_index, long thread_index, bool initialize)
{
	s_hs_thread *thread = hs_thread_get(thread_index);
	s_hs_frame *frame = thread->frame;
	long *value = (long *)((byte *)frame + offsetof(s_hs_frame, unknown0e) + frame->size);
	frame->size += sizeof(long);
	if (initialize)
	{
		s_hs_expression *expression = hs_expression_get(thread->frame->expression_index);
		s_hs_expression *name = hs_expression_get(expression->value);
		function_2099f0(thread_index, value, *(long *)((byte *)name + 8));
	}
	else
	{
		long object_index = *value;
		if (object_index != NONE)
		{
			byte *object = *(byte **)(g_4e0300->data + (object_index & 0xffff) * 12 + 8);
			long type_mask = 1 << object[0xaa];
			if (!(g_4456d4[(short)(function_index - 23)] & type_mask))
				object_index = NONE;
		}
		function_209ae0(thread_index, object_index);
	}
}

s_type_f4462a const g_44b218 = { _hs_type_unit, 2, function_208f10, "<object>", 0, { 0 } };

/* calls a script from an expression: evaluates its root into the frame, or
   returns the value computed */
// @retail 0x209bf0
void function_209bf0(long thread_index, short script_index, bool initialize)
{
	s_hs_script *script = hs_script_get(script_index);
	long *value = hs_frame_allocate_value(thread_index);

	if (initialize)
	{
		function_2099f0(thread_index, value, script->root_expression_index);
	}
	else
	{
		function_209ae0(thread_index, *value);
	}
}

/* runs the thread until it sleeps, waits or finishes */
// @retail 0x209850
void function_209850(long thread_index)
{
	s_hs_thread *thread = hs_thread_get(thread_index);
	s_hs_script *script = NULL;

	g_4f938c = thread_index;
	if (thread->type == 0 || thread->type == 4)
	{
		script = hs_script_get(thread->script_index);
	}
	thread->sleep_until = 0;
	if (thread->frame == &thread->stack)
	{
		thread->frame->size = 0;
		function_2099f0(thread_index, hs_frame_allocate_value(thread_index), script->root_expression_index);
	}
	while (thread->frame != &thread->stack && thread->sleep_until >= 0 &&
		(!g_4e6948 || !g_4e6948->flag1120 || thread->sleep_until <= g_510c54->game_time) && g_4f9388)
	{
		s_hs_frame *frame = thread->frame;
		s_hs_expression *expression = hs_expression_get(frame->expression_index);
		bool initialize = (thread->flags & 1) != 0;

		frame->size = 0;
		thread->flags &= ~1;
		if (!(expression->flags & 2))
		{
			short function_index = expression->value_type;

			g_4744e0[function_index]->evaluate(function_index, thread_index, initialize);
		}
		else
		{
			function_209bf0(thread_index, expression->value_type, initialize);
		}
	}
	if (thread->frame == &thread->stack)
	{
		if (thread->type == 0)
		{
			if (script->type == 0 || script->type == 1)
			{
				thread->sleep_until = NONE;
				g_4f938c = NONE;
				return;
			}
		}
		else if (thread->type == 4)
		{
			thread->sleep_until = NONE;
			g_4f938c = NONE;
			return;
		}
		else if (thread->type == 2)
		{
			record_pool_release(g_4f9384, thread_index);
		}
	}
	g_4f938c = NONE;
}

// @retail 0x209490
long function_209490(long expression_index)
{
	long const *expression_reference = &expression_index;
	long result = NONE;
	if (g_4f9388 && *expression_reference != NONE)
	{
		long thread_index = function_2097c0(NONE, 3);
		if (thread_index != NONE)
		{
			s_hs_thread *thread = hs_thread_get(thread_index);
			long *value = &thread->result;
			function_2099f0(thread_index, value, *expression_reference);
			if (thread->flags & 1)
				function_209850(thread_index);
			result = *value;
			record_pool_release(g_4f9384, thread_index);
		}
	}
	return result;
}
