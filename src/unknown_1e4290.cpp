// @flags /O2 /Ob1 /Gr
/* UNKNOWN_1E4290.CPP: an actor flag setter (lane M; called by the behaviors of
   0x1a8000..0x1affff) */

#include "unknown_11c920.h"
#include "ai_actor.h"

struct s_actor_timers_x
{
	byte field_000[0x2e8];
	long reference;
	short field_2ec;
	short field_2ee;
	bool field_2f0;
	byte field_2f1;
	short field_2f2;
	short field_2f4;
	byte field_2f6[0xa];
	long field_300;
	short field_304;
	short field_306;
};

// @retail 0x1e36d0
void function_1e36d0(long actor_index)
{
	s_actor_timers_x *actor = (s_actor_timers_x *)actor_get(actor_index);
	if (actor->reference != NONE)
	{
		actor->field_2ee--;
		if (actor->field_2ee <= 0)
		{
			actor->reference = NONE;
			actor->field_2ec = NONE;
			actor->field_2ee = NONE;
			actor->field_2f0 = false;
		}
	}
	if (actor->field_2f2 > 0)
		actor->field_2f2--;
	if (actor->field_2f4 > 0)
		actor->field_2f4--;
	if (actor->field_304 > 0)
	{
		actor->field_304--;
		if (actor->field_304 == 0)
			actor->field_300 = NONE;
	}
	if (actor->field_306 >= 0 && actor->field_306 < 32767)
		actor->field_306++;
}

// @retail 0x1e4220
void function_1e4220(long actor_index, bool enabled)
{
	s_actor_view *actor = actor_get(actor_index);
	*((bool *)actor + 0x6d0) = enabled;
	if (enabled)
		actor->flags810 |= 0x08000000;
	else
		actor->flags810 &= ~0x08000000;
}

// @retail 0x1e4260
void function_1e4260(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	dword flags = actor->flags810;
	actor->flags810 = flags | 2;
}

// @retail 0x1e4290
void function_1e4290(long actor_index, bool value)
{
	s_actor_view *actor = actor_get(actor_index);
	bool *enable = &value;

	if (*enable)
	{
		if (!actor->unknown229 && actor->unknown26c != NONE)
			*enable = !ai_object_get(actor->unknown26c)->unknown34c;
		if (*enable)
			actor->flags810 |= 0x800;
	}
	else
	{
		actor->flags810 &= ~0x800;
	}
}
