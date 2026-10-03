#include "cseries.h"
#include <string.h>
#include "unknown_19b516.h"

// @flags /O1 /Oi /Ob1 /Gr

/* the fields of s_message written by the constructor-like setup below
   (unknown_19b516.h declares only the callback and field_c) */
struct s_message_view
{
	word type;
	word flags;
	long value;
	long data;
	dword field_c;
	s_id_triplet id;
	long callback;
};

// @retail 0x149f49
void function_149f49(s_message *message, word a, dword *id, word b, long c, long d, long e)
{
	s_message_view *view = (s_message_view *)message;

	view->type = a;
	view->flags = b;
	view->value = c;
	view->data = d;

	if (id)
	{
		memcpy(id, &view->id, sizeof(view->id));
	}
	else
	{
		memset(&view->id, 0xff, sizeof(view->id));
	}

	view->callback = e;
}
