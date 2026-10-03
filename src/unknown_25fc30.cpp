// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_25FC30.CPP: firing position evaluators (actor_firing_position) */

#include "cseries.h"
#include "unknown_25fc30.h"
#include "globals.h"
#include "data_array.h"
#include "slot_handler.h"
#include "unknown_2626b0.h"

typedef bool (__stdcall *firing_position_evaluate_proc)(long, firing_position_evaluation_context *, firing_position *);
typedef void (__stdcall *firing_position_pre_evaluate_proc)(long, firing_position_evaluation_context *, short, firing_position *);

struct firing_position_pre_evaluator
{
	short flags;
	firing_position_pre_evaluate_proc proc;
};

struct firing_position_post_evaluator
{
	short flags;
	firing_position_evaluate_proc proc;
};

firing_position *g_51eca0;
extern firing_position_pre_evaluator g_44ad90[12];
extern firing_position_post_evaluator g_44adf0[12];

void __stdcall function_25dd50(long actor_index, firing_position_evaluation_context *context, short count, firing_position *positions);
void __stdcall function_25eea0(long actor_index, firing_position_evaluation_context *context, short count, firing_position *positions);
void __stdcall function_25f290(long actor_index, firing_position_evaluation_context *context, short count, firing_position *positions);
void __stdcall function_25e430(long actor_index, firing_position_evaluation_context *context, short count, firing_position *positions);
bool __stdcall function_25fb60(long actor_index, firing_position_evaluation_context *context, firing_position *position);

bool function_1b6070(long index, short unknown0, short unknown2);
bool function_29e050(byte *unknown, long target_index, firing_position_definition *definition, s_reference reference, long *unknown6a0);

// @retail 0x25dd20
long __stdcall function_25dd20(long key)
{
	return (key & 0xff) << 2;
}

// @retail 0x25dd30
bool __stdcall function_25dd30(long a, long b)
{
	return a == b;
}

// @retail 0x25e780
void __stdcall function_25e780(
	long actor_index,
	firing_position_evaluation_context *context,
	short count,
	firing_position *positions)
{
	for (short i = 0; i < count; i++)
	{
		firing_position *position = &positions[i];

		if (position->unknown4c &&
			!((real)context->unknown684 > position->unknown28) &&
			!((real)context->unknown684 > position->unknown2c))
		{
			real value = 0.f;
			real time = (real)context->unknown688;

			if (time <= 0.f || time > position->unknown28)
			{
				value = 20.f;
			}
			else
			{
				real difference = time + 20.f - position->unknown28;
				if (difference > 0.f)
				{
					value = difference;
				}
			}

			position->score += value;
		}
	}
}

// @retail 0x25e800
void __stdcall function_25e800(
	long actor_index,
	firing_position_evaluation_context *context,
	short count,
	firing_position *positions)
{
	long prop_index = NONE;
	short cached_sector = NONE;
	short cached_area = NONE;
	bool cached_result = false;
	bool use_facing = false;
	bool use_normal = false;

	if (context->unknown668 && magnitude_squared3d(&context->unknown66c) > 0.f)
	{
		use_normal = context->unknown55;
		use_facing = context->unknown54;
	}

	if (context->unknown04)
	{
		prop_index = context->unknown08;
	}

	for (short i = 0; i < count; i++)
	{
		firing_position *position = &positions[i];

		if (prop_index != NONE)
		{
			short sector = position->reference.unknown2;
			bool blocked = true;

			if (!(sector & 0x8000))
			{
				short area = NONE;
				s_262b40_result *result = function_262b40(position->reference);
				if (result)
				{
					area = result->unknown10;
				}

				if (sector != cached_sector || area != cached_area)
				{
					cached_result = function_1b6070(prop_node_get(prop_index)->unknown08, sector, area);
					cached_sector = sector;
					cached_area = area;
				}
				blocked = cached_result;
			}

			if (blocked)
			{
				position->unknown4d = true;
				if (!context->unknown14)
				{
					position->unknown4c = false;
					continue;
				}
			}
		}

		if (position->unknown4c)
		{
			real value = context->unknown11 ? 1.f : 5.f;

			if (!(context->unknown18 * 0.5f > position->unknown18))
			{
				if (context->unknown18 > position->unknown18)
				{
					value = (context->unknown18 - position->unknown18) * (1.f / (context->unknown18 * 0.5f)) * value;
				}
				else
				{
					value = 0.f;
				}
			}
			position->score += value;

			if (context->unknown618)
			{
				if (20.f > position->unknown28)
				{
					position->score = (20.f - position->unknown28) * 0.25f + position->score;
				}
				else if (!context->unknown11)
				{
					position->unknown4d = true;
					if (!context->unknown14)
					{
						position->unknown4c = false;
						continue;
					}
				}

				if (use_facing)
				{
					real dot = dot_product3d(&position->unknown34, &context->unknown66c);
					real facing = 0.f;

					if (0.f > dot)
					{
						position->unknown4d = true;
						if (!context->unknown14)
						{
							position->unknown4c = false;
							continue;
						}
					}
					else
					{
						dot *= 1.4142135f;
						if (0.f > dot)
						{
							dot = 0.f;
						}
						facing = dot * 15.f;
					}
					position->score += facing;
				}

				if (use_normal)
				{
					real dot = dot_product3d(&position->unknown40, &context->unknown66c);

					if (0.f > dot)
					{
						position->unknown4d = true;
						if (!context->unknown14)
						{
							position->unknown4c = false;
						}
					}
					else
					{
						dot *= 1.4142135f;
						if (0.f > dot)
						{
							dot = 0.f;
						}
						position->score += dot * 15.f;
					}
				}
			}
		}
	}
}

// @retail 0x25eb00
void __stdcall function_25eb00(
	long actor_index,
	firing_position_evaluation_context *context,
	short count,
	firing_position *positions)
{
	if (context->unknown618 && context->unknown54 && context->unknown668)
	{
		for (short i = 0; i < count; i++)
		{
			firing_position *position = &positions[i];

			if (position->unknown4c)
			{
				position->score *= (real)fabs(dot_product3d(&context->unknown66c, &position->unknown34));
			}
		}
	}
}

// @retail 0x25eb70
void __stdcall function_25eb70(
	long actor_index,
	firing_position_evaluation_context *context,
	short count,
	firing_position *positions)
{
	for (short i = 0; i < count; i++)
	{
		firing_position *position = &positions[i];

		if (position->unknown4c)
		{
			real value;

			if (context->range08 > position->unknown18)
			{
				value = 0.f;
			}
			else if (context->range08 * 2.f > position->unknown18)
			{
				value = (position->unknown18 - context->range08) / context->range08 * 8.f;
			}
			else if (context->range18 > position->unknown18)
			{
				value = (context->range18 - position->unknown18) * 8.f / (context->range18 - context->range08 * 2.f);
			}
			else
			{
				value = 0.f;
			}
			position->score += value;

			if (context->unknown618)
			{
				real distance_value;

				if (context->range0c * context->range0c > position->unknown30)
				{
					distance_value = 0.f;
				}
				else if (context->range10 * context->range10 > position->unknown30)
				{
					distance_value = (real)((sqrt(position->unknown30) - context->range0c) * 10.f / (context->range10 - context->range0c));
				}
				else
				{
					distance_value = 10.f;
				}
				position->score += distance_value;
			}
		}
	}
}

// @retail 0x25ec90
void __stdcall function_25ec90(
	long actor_index,
	firing_position_evaluation_context *context,
	short count,
	firing_position *positions)
{
	short i;

	if (context->unknown18 > 0.f)
	{
		for (i = 0; i < count; i++)
		{
			firing_position *position = &positions[i];

			if (position->unknown4c)
			{
				real value = (1.f - position->unknown18 / context->unknown18) * 8.f;
				if (0.f > value)
				{
					value = 0.f;
				}
				position->score += value;
			}
		}
	}

	if (context->unknown68c)
	{
		for (i = 0; i < count; i++)
		{
			firing_position *position = &positions[i];

			short unknown2 = position->reference.unknown2;

			if (position->unknown4c)
			{
				if ((unknown2 & 0x8000) ||
					unknown2 != context->unknown68e ||
					position->definition->unknown10 != context->unknown690)
				{
					position->unknown4d = true;
					if (!context->unknown14)
					{
						position->unknown4c = false;
					}
				}
			}
		}
	}
}

// @retail 0x25ed60
void __stdcall function_25ed60(
	long actor_index,
	firing_position_evaluation_context *context,
	short count,
	firing_position *positions)
{
	for (short i = 0; i < count; i++)
	{
		firing_position *position = &positions[i];

		if (position->unknown4c)
		{
			real value = (1.f - position->unknown28 * 0.05f) * 8.f;
			if (0.f > value)
			{
				value = 0.f;
			}
			position->score += value;
		}
	}
}

// @retail 0x25edd0
void __stdcall function_25edd0(
	long actor_index,
	firing_position_evaluation_context *context,
	short count,
	firing_position *positions)
{
	if (context->unknown618 && context->unknown61c > 0.f)
	{
		for (short i = 0; i < count; i++)
		{
			firing_position *position = &positions[i];

			if (position->unknown4c)
			{
				real value;

				if (position->unknown2c < 3.4028235e38f)
				{
					value = position->unknown2c / (context->unknown61c * 0.8f);
					if (0.5f > value)
					{
						position->unknown4d = true;
						if (!context->unknown14)
						{
							position->unknown4c = false;
							continue;
						}
					}

					if (0.f > value)
					{
						value = 0.f;
					}
					else if (value > 1.f)
					{
						value = 1.f;
					}
				}
				else
				{
					value = 1.f;
				}
				position->score += value * 8.f;
			}
		}
	}
}

// @retail 0x25fb60
bool __stdcall function_25fb60(long actor_index, firing_position_evaluation_context *context, firing_position *position)
{
	s_actor_view *actor = actor_get(actor_index);

	if (context->unknown56 ||
		(context->unknown5a && (!position || position->unknown58)) ||
		(context->unknown58 && (!position || position->unknown59)))
	{
		if (!position)
		{
			context->unknown680 += 15.f;
		}
		else if (position->unknown4c)
		{
			if (function_29e050(context->unknown60c, actor->unknown26c, position->definition, position->reference, &context->unknown6a0))
			{
				position->score += 15.f;
			}
			else
			{
				position->unknown4d = true;
				if (!context->unknown14)
				{
					position->unknown4c = false;
				}
			}
		}
	}
	return position ? position->unknown4c : true;
}

// @retail 0x25fc30
bool __stdcall function_25fc30(
	long unused,
	firing_position_evaluation_context *context,
	firing_position *position)
{
	if (position)
	{
		if (!context->unknown11)
		{
			s_game_time_globals *globals = g_510c54;
			long time = globals->game_time;
			long start = NONE;
			short x = 0;
			bool flag = true;

			if (position->type == 0 && position->unknown18 < 6.f)
			{
				start = time;
				x = 7;
				flag = false;
			}

			if (context->unknown10)
			{
				if (flag)
				{
					position->score += 15.f;
				}
			}
			else if (!flag)
			{
				position->unknown4d = true;
				if (!context->unknown14)
				{
					position->unknown4c = false;
				}
			}

			if (position->unknown4c)
			{
				real a = 0.f;

				if (start == NONE)
				{
					a = 10.f;
				}
				else
				{
					real scaled = globals->ticks_per_second * 10.f;
					long rounded;

					__asm
					{
						fld scaled
						fistp rounded
					}

					if (start + rounded < time)
					{
						a = 10.f;
					}
					else if (start < time)
					{
						a = (time - start) * globals->rate;
					}
				}
				position->score += a;

				real b = 0.f;
				if (x < 4)
				{
					b = (4 - x) * 5.f;
				}
				position->score += b;
			}
		}

		return position->unknown4c;
	}

	return false;
}

// @retail 0x25fd50
bool __stdcall function_25fd50(
	long unused,
	firing_position_evaluation_context *context,
	firing_position *position)
{
	if (context->unknown618)
	{
		if (!position)
		{
			context->unknown680 += 15.f;
		}
		else
		{
			real value = 0.f;

			if (position->unknown58)
			{
				switch (position->type)
				{
				case 2:
					value = 15.f;
					break;
				case 3:
					value = 8.f;
					break;
				case 0:
				case 1:
					value = 6.f;
					break;
				case 4:
					value = 12.f;
					break;
				}
			}
			else
			{
				switch (position->type)
				{
				case 2:
					value = 12.f;
					break;
				case 4:
					value = 10.f;
					break;
				case 3:
					value = 4.f;
					break;
				case 1:
					if (position->definition->flags & 0x10)
					{
						break;
					}
				case 0:
					position->unknown4d = true;
					if (!context->unknown14)
					{
						position->unknown4c = false;
					}
					break;
				}
			}

			position->score += value;
		}
	}

	if (!position)
	{
		return true;
	}

	return position->unknown4c;
}

// @retail 0x25fe50
bool __stdcall function_25fe50(
	long unused,
	firing_position_evaluation_context *context,
	firing_position *position)
{
	if (context->unknown618)
	{
		if (!position)
		{
			context->unknown680 += 10.f;
		}
		else
		{
			real value = 0.f;

			switch (position->type)
			{
			case 1:
			case 4:
				value = 2.f;
				break;
			case 0:
				value = 10.f;
				break;
			case 2:
			case 3:
				value = 0.f;
				break;
			}

			position->score += value;
		}
	}

	if (!position)
	{
		return true;
	}

	return position->unknown4c;
}

// @retail 0x25fee0
bool __stdcall function_25fee0(
	long unused,
	firing_position_evaluation_context *context,
	firing_position *position)
{
	if (context->unknown618)
	{
		if (!position)
		{
			context->unknown680 += 20.f;
		}
		else
		{
			real value = 0.f;

			switch (position->type)
			{
			case 0:
				value = 20.f;
				break;
			case 1:
				value = 10.f;
				break;
			default:
				{
					real d = context->unknown61c - 7.5f;
					if (d < 0.f || position->unknown30 > d * d)
					{
						position->unknown4d = true;
						if (!context->unknown14)
						{
							position->unknown4c = false;
						}
					}
				}
				break;
			}

			position->score += value;
		}
	}

	if (!position)
	{
		return true;
	}

	return position->unknown4c;
}

// @retail 0x25ff90
bool __stdcall function_25ff90(
	long unused,
	firing_position_evaluation_context *context,
	firing_position *position)
{
	if (context->unknown618)
	{
		if (!position)
		{
			context->unknown680 += context->unknown644 ? 6.f : 15.f;
		}
		else
		{
			real value = 0.f;

			switch (position->type)
			{
			default:
				position->unknown4d = true;
				if (!context->unknown14)
				{
					position->unknown4c = false;
				}
				break;
			case 1:
				value = context->unknown644 ? 2.5f : 5.f;
				break;
			case 0:
				value = context->unknown644 ? 6.f : 15.f;
				break;
			}

			position->score += value;
		}
	}

	if (!position)
	{
		return true;
	}

	return position->unknown4c;
}
// @retail 0x260060
void function_260060(
	long a,
	long b,
	firing_position_evaluation_context *context,
	firing_position *position)
{
	for (firing_position_pre_evaluator *e = g_44ad90; e->proc; e++)
	{
		if ((1 << context->type) & e->flags)
		{
			e->proc(a, context, b, position);
		}
	}
}

// @retail 0x2600a0
bool firing_position_post_evaluate(
	long a,
	firing_position_evaluation_context *context,
	firing_position *position)
{
	bool result = true;

	for (firing_position_post_evaluator *e = g_44adf0; result && e->proc; e++)
	{
		if ((1 << context->type) & e->flags)
		{
			result = e->proc(a, context, position);
		}
	}

	return result;
}

// @retail 0x2600e0
bool __stdcall function_2600e0(
	long a,
	long b,
	void *unused)
{
	firing_position *pa = &g_51eca0[a];
	firing_position *pb = &g_51eca0[b];

	if (pa->unknown4c != pb->unknown4c)
	{
		return (pa->unknown4c ? -1 : 1) > 0;
	}
	if (pa->unknown4d != pb->unknown4d)
	{
		return (pa->unknown4d ? 1 : -1) > 0;
	}
	if (pa->score > pb->score)
	{
		return false;
	}
	if (pb->score > pa->score)
	{
		return true;
	}
	return false;
}
firing_position_pre_evaluator g_44ad90[12] =
{
	{-1, function_25dd50},
	{0x1, function_25eea0},
	{0x4d, function_25f290},
	{0x1, function_25eb00},
	{0x10, function_25ec90},
	{0x100, function_25ed60},
	{0x82, function_25eb70},
	{0x20, function_25e800},
	{0x8, function_25e780},
	{0x6, function_25edd0},
	{0xd, function_25e430},
};

firing_position_post_evaluator g_44adf0[12] =
{
	{0x41, function_25ff90},
	{0x8, function_25fee0},
	{0x6, function_25fd50},
	{0x20, function_25fc30},
	{0x80, function_25fe50},
	{-1, function_25fb60},
};

bool (__stdcall *g_comparator)(long, long, void *) = function_2600e0;
