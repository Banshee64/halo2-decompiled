// @flags /O2 /Ob1 /Gr
/* OBJECT_LISTS.CPP: the script engine's object lists. A list (g_4f55d8, 12
   bytes each) heads a chain of references (g_4f55d4), each naming one object. */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"

struct s_object_list
{
	word salt;
	short reference_count;
	short unknown04;
	short count;
	long first_reference_index;
};

struct s_object_list_reference
{
	word salt;
	word unknown02;
	long object_index;
	long next_reference_index;
};

extern s_record_pool *g_4f55d8;
extern s_record_pool *g_4f55d4;

void object_lists_garbage_collect(void);

// @retail 0x1ded60
long function_1ded60(void)
{
	s_record_pool *lists = g_4f55d8;
	long list_index = record_pool_allocate(lists);

	if (list_index == NONE)
	{
		object_lists_garbage_collect();
		list_index = record_pool_allocate(lists);
	}

	if (list_index != NONE)
	{
		s_object_list *list = &((s_object_list *)lists->data)[list_index & 0xffff];
		list->count = 0;
		list->first_reference_index = NONE;
	}

	return list_index;
}

// @retail 0x1dedb0
void function_1dedb0(long list_index, long object_index)
{
	s_object_list *list = &((s_object_list *)g_4f55d8->data)[list_index & 0xffff];
	long reference_index = record_pool_allocate(g_4f55d4);

	if (reference_index != NONE)
	{
		s_object_list_reference *reference = (s_object_list_reference *)(g_4f55d4->data + (reference_index & 0xffff) * g_4f55d4->size);
		reference->object_index = object_index;
		reference->next_reference_index = list->first_reference_index;
		list->first_reference_index = reference_index;
	}

	list->count++;
}
