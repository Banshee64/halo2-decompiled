#include "unknown_11c920.h"
#include "data_array.h"

// @flags /O2 /Gr

struct s_script_stack_link
{
	s_script_stack_link *next;
	long expression_index;
};

struct s_script_stack_view
{
	byte unknown00[0x10];
	s_script_stack_link *frame;
	byte unknown14[0x418 - 0x14];
};

extern s_record_pool *g_4f9384;

// @retail 0x209c60
void function_209c60(long thread_index)
{
	s_script_stack_view *thread = &((s_script_stack_view *)g_4f9384->data)[thread_index & 0xffff];
	thread->frame = thread->frame->next;
}

struct s_script_wake_thread
{
	byte unknown00[3];
	byte flags;
	long script_index;
	long sleep_until;
	long saved_sleep;
	s_script_stack_link *frame;
	byte unknown14[0x418 - 0x14];
};

struct s_script_wake_expression
{
	short salt;
	short type;
	byte unknown04[0x14 - 4];
};

extern s_record_pool *g_4f9394;

// @retail 0x209c80
void __stdcall function_209c80(long thread_index)
{
	long const *index_reference = &thread_index;
	s_script_wake_thread *thread = &((s_script_wake_thread *)g_4f9384->data)[*index_reference & 0xffff];
	if (thread->sleep_until != NONE)
	{
		thread->sleep_until = 0;
		if (thread->flags & 2)
		{
			thread->sleep_until = thread->saved_sleep;
			thread->flags &= ~2;
		}
		else
		{
			s_script_stack_link *frame = thread->frame;
			if (frame->expression_index != NONE &&
				((s_script_wake_expression *)g_4f9394->data)[frame->expression_index & 0xffff].type == 21)
			{
				function_209c60(*index_reference);
			}
			else
			{
				frame = frame->next;
				if (frame && frame->expression_index != NONE &&
					((s_script_wake_expression *)g_4f9394->data)[frame->expression_index & 0xffff].type == 21)
				{
					function_209c60(*index_reference);
					function_209c60(*index_reference);
					thread->flags &= ~1;
				}
			}
		}
	}
}
