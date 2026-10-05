#include "unknown_11c920.h"
#include "globals.h"
#include "object_list.h"
#include "unknown_1428b0.h"

// @flags /O2 /arch:SSE /Gr

struct s_carried_object
{
	long definition_index;
	byte unknown004[0x14 - 4];
	long parent_index;
	byte unknown018[0x88 - 0x18];
	vector3f velocity;
	byte unknown094[0xb4 - 0x94];
	long component_index;
	long platform_index;
	byte unknown0bc[4];
	word flag0 : 1;
	word flag1 : 1;
	word flag2 : 1;
	word : 13;
};

struct s_carried_object_header
{
	byte unknown00[8];
	s_carried_object *object;
};

struct s_carrier_definition
{
	word unknown00;
	word : 7;
	word flag7 : 1;
	word : 8;
};

struct s_carrier_motion
{
	point3f previous_center;
	point3f center;
	vector3f previous_velocity;
	vector3f velocity;
	vector3f previous_angular_velocity;
	vector3f angular_velocity;
	byte unknown048[0xb0 - 0x48];
	transform4x3f transform;
	bool valid;
	bool active;
	byte unknown0e6[2];
};

struct s_small_index;
struct s_location;
short function_0b67a0(s_small_index const *data);
transform4x3f *function_ba160(long object_index, transform4x3f *matrix);
int __fastcall function_142a60(transform4x3f const *a, transform4x3f const *b, transform4x3f *result);
void function_b75a0(long object_index, point3f const *point, vector3f const *forward,
	vector3f const *up, s_location const *location, bool unknown);

PRIVATE inline void carrier_point_velocity(point3f const *center, vector3f const *linear,
	vector3f const *angular, point3f const *point, vector3f *result)
{
	vector3f offset;
	offset.i = point->x - center->x;
	offset.j = point->y - center->y;
	offset.k = point->z - center->z;
	result->i = angular->j * offset.k - angular->k * offset.j;
	result->j = angular->k * offset.i - angular->i * offset.k;
	result->k = angular->i * offset.j - angular->j * offset.i;
	result->i = linear->i + result->i;
	result->j = linear->j + result->j;
	result->k = linear->k + result->k;
}

// @retail 0x109a00
bool function_109a00(long object_index, vector3f *delta, bool rotate, vector3f *velocity)
{
	s_carried_object_header *headers = (s_carried_object_header *)g_4e0300->data;
	s_carried_object *object = headers[object_index & 0xffff].object;
	volatile bool result = false;

	if (TEST_FIELD_BIT(object->flag2))
	{
		long platform_index = object->platform_index;
		for (long i = 0; i < g_5107f0->object_count; i++)
		{
			if (g_5107f0->object_indices[i] == platform_index)
			{
				s_carrier_motion *motion = (s_carrier_motion *)&g_5107f0->entries[i];
				if (motion->valid && motion->active && object->parent_index == NONE)
				{
					s_carried_object *platform = headers[platform_index & 0xffff].object;
					s_carrier_definition *definition = (s_carrier_definition *)g_4e3b44[platform->definition_index & 0xffff].bytes;
					long component_index = platform->component_index;
					if (component_index != NONE &&
						function_0b67a0((s_small_index *)(g_51e9b8->data + (component_index & 0xffff) * 0xa0)) != NONE)
					{
						transform4x3f matrix;
						function_ba160(object_index, &matrix);
						if (rotate && TEST_FIELD_BIT(definition->flag7))
						{
							transform4x3f moved;
							vector3f previous_velocity;
							function_142a60(&motion->transform, &matrix, &moved);
							carrier_point_velocity(&motion->previous_center, &motion->previous_velocity,
								&motion->previous_angular_velocity, &matrix.position, &previous_velocity);
							object->velocity.i -= previous_velocity.i;
							object->velocity.j -= previous_velocity.j;
							object->velocity.k -= previous_velocity.k;
							function_142640(&motion->transform, &object->velocity, &object->velocity);
							carrier_point_velocity(&motion->center, &motion->velocity,
								&motion->angular_velocity, &moved.position, delta);
							function_b75a0(object_index, &moved.position, &moved.forward, &moved.up, NULL, false);
							*velocity = object->velocity;
						}
						else
						{
							point3f moved;
							vector3f change;
							transform4x3f_apply_point(&motion->transform, &matrix.position, &moved);
							real ticks = (real)g_510c54->field_2_3;
							change.i = ticks * (matrix.position.x - moved.x);
							change.j = ticks * (matrix.position.y - moved.y);
							change.k = ticks * (matrix.position.z - moved.z);
							function_b75a0(object_index, &moved, NULL, NULL, NULL, false);
							*velocity = object->velocity;
							object->velocity.i += change.i;
							object->velocity.j += change.j;
							object->velocity.k += change.k;
							delta->i = -change.i;
							delta->j = -change.j;
							delta->k = -change.k;
						}
						result = true;
					}
				}
				break;
			}
		}
	}
	return result;
}
