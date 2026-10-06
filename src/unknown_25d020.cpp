// @flags /O2 /Ob1 /arch:SSE /Gr
// LTCG drops unused arg_4 and breaks the matched caller at 0x25bcb0.
// Retain this body until the six-argument convention can be reproduced.
#if 0
#include "unknown_25d020.h"

long function_25d810(long object_index, long actor_index, bool create);
bool __stdcall function_25c230(long arg_0, long arg_1, short arg_2);
bool function_25da40(s_prop_datum *datum);
void function_25c860(long prop_ref_index);
void function_25c4e0(long prop_ref_index);
void function_25b620(long prop_ref_index, long actor_index, bool unknown);
real function_30bf0(vector3f *v);

inline s_type_f95cd3 *prop_ref_view(s_prop_datum *datum)
{
	s_type_f95cd3 *result = NULL;
	if (datum->tracking_index != NONE)
	{
		s_type_e5ff81 *tracking = tracking_get(datum->tracking_index);
		if (tracking)
		{
			result = &tracking->view;
		}
	}
	return result;
}

static __forceinline long props_ticks_round(real ticks_real)
{
	long ticks;

	__asm
	{
		fld ticks_real
		fistp ticks
	}
	return ticks;
}

// pending 0x25d020
bool __stdcall function_25d020(long arg_0, long arg_1, long arg_2, long arg_3, long arg_4, long arg_5)
{
	bool local_0 = false;
	bool local_1 = false;
	s_prop_datum *local_2 = NULL;
	short local_3 = 2;
	if (arg_5 != NONE)
	{
		local_2 = prop_ref_get(arg_5);
		local_3 = local_2->unknown1a;
	}
	if (arg_3 == NONE)
	{
		arg_3 = function_25d810(arg_1, arg_0, true);
		if (arg_3 == NONE)
			return local_0;
	}
	s_prop_datum *local_4 = prop_ref_get(arg_3);
	s_type_5cfb45 *local_5 = function_25d690(local_4);
	s_type_f95cd3 *local_6 = prop_ref_view(local_4);
	if (!(local_4->state >= 1 && local_4->state <= 2))
	{
		if (local_4->state < 1)
		{
			function_25c230(arg_0, arg_3, local_3);
			local_6 = prop_ref_view(local_4);
			local_5 = function_25d690(local_4);
			local_1 = true;
		}
		if (function_25da40(local_4))
		{
			if (local_1)
				function_25c860(arg_3);
			if (arg_5 != NONE)
			{
				s_type_5cfb45 *local_7 = function_25d690(local_2);
				if (local_5 != local_7 && local_7->unknown00 > local_5->unknown00)
				{
					*local_5 = *local_7;
					if (local_6)
					{
						s_type_f95cd3 *local_8 = function_25d740((s_prop_node *)local_2);
						if (local_8)
							local_6->unknown94 = local_8->unknown94;
					}
					s_2641c0 local_9;
					if (function_2641c0(arg_0, &local_9, &local_5->position))
						local_4->unknown28 = distance3d(&local_5->position, &local_9.field_c);
				}
			}
			else if (local_5->unknown00 < g_510c54->game_time)
			{
				s_2640c0 local_10;
				s_2641c0 local_11;
				function_2640c0(local_4->object_index, (s_object_motion_view *)&local_10);
				if (function_2641c0(arg_0, &local_11, &function_25d690(local_4)->position))
					function_264330(arg_0, arg_3, &local_11, &local_10, true);
			}
			if (local_6)
			{
				local_6->unknown8e = (short)props_ticks_round(g_510c54->field_2_3 * 30.f);
				real local_12;
				if (local_4->unknown28 < 10.f)
					local_12 = 1.5f;
				else
					local_12 = (local_4->unknown28 - 10.f) * 0.5 + 1.5;
				if (!local_1)
				{
					vector3f local_13;
					vector3d_from_points3d(&local_5->position, &local_6->unknowna4, &local_13);
					if (!local_6->unknowna2 || length_sq3f(&local_13) > local_12 * local_12)
					{
						function_25c4e0(arg_3);
						if (local_6->unknowna2)
						{
							vector3d_from_points3d(&local_6->unknowna4, &local_5->position, &local_6->unknown94);
							function_30bf0(&local_6->unknown94);
						}
						else
							local_6->unknown94 = *g_4687a4;
						local_6->unknowna4 = local_5->position;
						local_6->unknowna2 = true;
					}
				}
			}
		}
		switch ((short)arg_2)
		{
		case 2:
			function_25b620(arg_3, arg_0, local_1);
			break;
		}
	}
	if (local_6)
		*(real *)((byte *)local_6 + 0x3c) = function_265d30(arg_0, arg_3);
	return true;
}
#endif
