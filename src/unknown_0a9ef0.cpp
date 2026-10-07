#include "unknown_11c920.h"
#include "globals.h"

// @flags /O2 /arch:SSE /Gr

bool function_aa970(long index);
transform4x3f *function_ba160(long index, transform4x3f *matrix);
void function_ba1d0(long index, vector3f *linear, vector3f *angular);
void function_aaca0(long index, point3f const *position, vector3f const *forward, vector3f const *up,
	vector3f const *linear, vector3f const *angular, vector3f *linear_result, vector3f *angular_result);
void function_aa9e0(long index, point3f const *previous_position, vector3f const *velocity);
void __stdcall function_b77d0(long index, vector3f const *linear, vector3f const *angular);
void function_aab40(long index, point3f const *position, vector3f const *forward,
	vector3f const *up, vector3f const *linear, vector3f const *angular);

// The selection result must be supplied by the caller in addition to mode.
// Keep this candidate disabled until the caller's interface supplies it.
#if 0
void function_aa260(long selection, vector3f const *linear, long object_index, long mode,
	point3f const *position, vector3f const *forward, vector3f const *up, vector3f const *angular)
{
	switch (selection)
	{
	case 1:
		function_aab40(object_index, position, forward, up, linear, angular);
		break;
	case 2:
		{
			bool change_linear = linear != NULL;
			bool change_angular = angular != NULL;
			if (!function_aa970(object_index) && (change_linear || change_angular))
			{
				transform4x3f current;
				function_ba160(object_index, &current);
				point3f desired_position = position ? *position : current.position;
				vector3f desired_forward = forward ? *forward : current.forward;
				vector3f desired_up = up ? *up : current.up;
				vector3f desired_linear, desired_angular;
				function_ba1d0(object_index, &desired_linear, &desired_angular);
				if (change_linear)
				{
					vector3f difference;
					difference.i = linear->i - desired_linear.i;
					difference.j = linear->j - desired_linear.j;
					difference.k = linear->k - desired_linear.k;
					if (mode == 2 && difference.k * difference.k + difference.i * difference.i +
						difference.j * difference.j <= 0.06f)
						change_linear = false;
					else desired_linear = *linear;
				}
				if (change_angular)
				{
					vector3f difference;
					difference.i = angular->i - desired_angular.i;
					difference.j = angular->j - desired_angular.j;
					difference.k = angular->k - desired_angular.k;
					if (mode == 2 && difference.k * difference.k + difference.j * difference.j +
						difference.i * difference.i <= 0.01f)
						change_angular = false;
					else desired_angular = *angular;
				}
				vector3f adjusted_linear, adjusted_angular;
				function_aaca0(object_index, &desired_position, &desired_forward, &desired_up,
					&desired_linear, &desired_angular, &adjusted_linear, &adjusted_angular);
				if (change_linear)
					function_aa9e0(object_index, NULL, &adjusted_linear);
				if (change_linear || change_angular)
					function_b77d0(object_index, change_linear ? &adjusted_linear : NULL,
						change_angular ? &adjusted_angular : NULL);
			}
		}
		break;
	}
}
#endif
