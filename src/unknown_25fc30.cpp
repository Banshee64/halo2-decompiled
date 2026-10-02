// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_25FC30.CPP: firing position evaluators (actor_firing_position) */

#include "cseries.h"
#include "unknown_25fc30.h"

struct firing_position_time_globals
{
	byte unknown00[2];
	short unknown02;
	real unknown04;
	long time;
};

typedef bool (__stdcall *firing_position_evaluate_proc)(long, firing_position_evaluation_context *, firing_position *);
typedef void (__stdcall *firing_position_pre_evaluate_proc)(long, firing_position_evaluation_context *, long, firing_position *);

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

firing_position_time_globals *g_510c54;
firing_position *g_51eca0;
firing_position_pre_evaluator g_44ad90[12];
extern firing_position_post_evaluator g_44adf0[12];

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
			firing_position_time_globals *globals = g_510c54;
			long time = globals->time;
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
					real scaled = globals->unknown02 * 10.f;
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
						a = (time - start) * globals->unknown04;
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
firing_position_post_evaluator g_44adf0[12] =
{
	{1, function_25fc30},
	{1, function_25fd50},
	{1, function_25fe50},
	{1, function_25fee0},
	{1, function_25ff90},
};

bool (__stdcall *g_comparator)(long, long, void *) = function_2600e0;
