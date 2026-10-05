#include "unknown_11c920.h"
#include "data_array.h"
#include "physical_memory.h"
#include <xmmintrin.h>
// @flags /O2 /arch:SSE /Gr

struct s_13d370
{
	long field_0;
	dword field_4;
	long field_8;
	long field_c;
};

struct s_physical_allocator_view;
struct s_physical_block_view;
bool function_13d320(s_physical_allocator_view const *arg_1, s_physical_block_view const *arg_2, s_physical_block_view const *arg_3);

// @retail 0x13d370
long __stdcall function_13d370(s_physical_object *arg_1, long arg_2, long arg_3)
{
	// Retail keeps the manager parameter on the stack.
	s_physical_object *const *local_29 = &arg_1;
	long local_1 = arg_2 >> (*local_29)->page_shift;
	if (arg_2 & ((1 << (*local_29)->page_shift) - 1))
		local_1++;
	long local_2 = NONE;
	long local_3 = arg_3 < 0 ? 0 : (arg_3 > 8 ? 8 : arg_3);
	if (local_1 < (*local_29)->limits[local_3] || local_3 == 0)
	{
		s_13d370 local_4[256];
		long local_5 = 0;
		long local_6 = 0;
		long local_7 = (*local_29)->first;
		long local_8 = NONE;
		long local_9 = 0;
		long local_10 = NONE;
		dword local_11;
		s_13d370 local_12;
		bool local_13 = false;
		while (local_9 < (*local_29)->page_count)
		{
			long local_14 = (local_6 + 1) % 256;
			if (local_14 != local_5)
			{
				local_4[local_6].field_0 = local_8;
				local_4[local_6].field_4 = 0;
				local_4[local_6].field_8 = local_9;
				local_4[local_6].field_c = 0;
				local_6 = local_14;
			}
			long local_15;
			dword local_16;
			if (local_7 == NONE)
			{
				local_15 = (*local_29)->page_count - local_9;
				local_16 = 0;
				local_9 = (*local_29)->page_count;
			}
			else
			{
				s_physical_block *local_17 = &((s_physical_block *)(*local_29)->blocks->data)[local_7 & 0xffff];
				if (local_9 == local_17->offset)
				{
					local_15 = local_17->pages;
					local_16 = local_17->time;
					bool local_18 = (*local_29)->busy_proc && (*local_29)->busy_proc(local_7);
					if ((dword)(local_17->time + arg_3) >= (dword)(*local_29)->time)
						local_18 = true;
					else if (!local_18 && (local_10 == NONE || (dword)local_17->time < local_11))
					{
						local_10 = local_7;
						local_11 = local_17->time;
					}
					local_8 = local_7;
					local_7 = local_17->next;
					_mm_prefetch((char const *)&((s_physical_block *)(*local_29)->blocks->data)[local_7 & 0xffff], _MM_HINT_T0);
					local_9 = local_17->offset + local_17->pages;
					if (local_18)
					{
						local_5 = local_6;
						continue;
					}
				}
				else
				{
					local_15 = local_17->offset - local_9;
					local_16 = 0;
					local_9 = local_17->offset;
				}
			}
			for (long local_19 = local_5; local_19 != local_6; local_19 = (local_19 + 1) % 256)
			{
				s_13d370 *local_20 = &local_4[local_19];
				_mm_prefetch((char const *)&local_4[(local_19 + 1) % 256], _MM_HINT_T0);
				if (local_16 > local_20->field_4)
					local_20->field_4 = local_16;
				local_20->field_c += local_15;
				if (local_20->field_c >= local_1)
				{
					bool local_21 = true;
					if (local_13)
					{
						switch ((*local_29)->state)
						{
						case 0:
							local_21 = local_20->field_4 < local_12.field_4 ||
								(local_20->field_4 == local_12.field_4 && local_20->field_c < local_12.field_c);
							break;
						case 1:
							local_21 = function_13d320((s_physical_allocator_view const *)*local_29,
								(s_physical_block_view const *)local_20, (s_physical_block_view const *)&local_12);
							break;
						default:
							{
								dword local_22 = (*local_29)->time - local_20->field_4;
								dword local_23 = (*local_29)->time - local_12.field_4;
								local_22 = local_22 > 0 ? local_22 : 0;
								local_23 = local_23 > 0 ? local_23 : 0;
								local_21 = (long)(local_12.field_c * local_22) > (long)(local_20->field_c * local_23);
								break;
							}
						}
					}
					if (local_21)
					{
						local_12 = *local_20;
						local_13 = true;
					}
					local_5 = (local_5 + 1) % 256;
				}
			}
		}
		if (local_13)
		{
			s_record_pool_iterator local_24;
			local_24.data = (*local_29)->blocks;
			local_24.index = NONE;
			local_24.datum_index = NONE;
			s_physical_block *local_25;
			while ((local_25 = (s_physical_block *)data_iterator_next_inlined(&local_24)) != NULL)
			{
				if (local_25->offset < local_12.field_8 + local_1 && local_25->offset + local_25->pages > local_12.field_8)
					(*local_29)->block_delete(local_24.datum_index);
			}
			if ((*local_29)->blocks->actual_count == (*local_29)->blocks->maximum_count && local_10 != NONE)
			{
				if (local_12.field_0 == local_10)
					local_12.field_0 = ((s_physical_block *)(*local_29)->blocks->data)[local_10 & 0xffff].previous;
				(*local_29)->block_delete(local_10);
			}
			local_2 = record_pool_allocate((*local_29)->blocks);
			if (local_2 != NONE)
			{
				s_physical_block *local_26 = &((s_physical_block *)(*local_29)->blocks->data)[local_2 & 0xffff];
				if (local_12.field_0 == NONE)
				{
					local_26->previous = NONE;
					if ((*local_29)->first == NONE)
						(*local_29)->last = local_2;
					else
						((s_physical_block *)(*local_29)->blocks->data)[(*local_29)->first & 0xffff].previous = local_2;
					local_26->next = (*local_29)->first;
					(*local_29)->first = local_2;
				}
				else
				{
					s_physical_block *local_27 = &((s_physical_block *)(*local_29)->blocks->data)[local_12.field_0 & 0xffff];
					if (local_27->next == NONE)
					{
						local_26->previous = (*local_29)->last;
						(*local_29)->last = local_2;
					}
					else
					{
						s_physical_block *local_28 = &((s_physical_block *)(*local_29)->blocks->data)[local_27->next & 0xffff];
						local_26->previous = local_28->previous;
						local_28->previous = local_2;
					}
					local_26->next = local_27->next;
					local_27->next = local_2;
				}
				local_26->offset = local_12.field_8;
				local_26->pages = local_1;
				local_26->time = (*local_29)->time;
				return local_2;
			}
		}
		if (arg_3 == local_3)
			(*local_29)->limits[local_3] = (*local_29)->limits[local_3] < local_1 ? (*local_29)->limits[local_3] : local_1;
	}
	return local_2;
}
