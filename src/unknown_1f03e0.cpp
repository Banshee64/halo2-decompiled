// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1F03E0.CPP: the state of a moving physics shape (0x74 bytes) and
   the side of a contact it touches */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_0259d0.h"
#include "data_array.h"
#include <math.h>
#include "object_list.h"

#define PIN(x, lower, upper) ((x) < (lower) ? (lower) : ((x) > (upper) ? (upper) : (x)))

struct s_shape_state
{
	point3f point;
	long unknown0c;
	byte unknown10[0x1c - 0x10];
	long unknown1c;
	long unknown20;
	transform4x3f matrix;
	long unknown58;
	long unknown5c;
	short material;
	bool unknown62;
	byte unknown63;
	vector3f normal;
	real unknown70;
};

transform4x3f *g_4687d0;
extern short g_47d8e0;
extern short g_54e898;

// @retail 0x1f03e0
void function_1f03e0(s_shape_state *state)
{
	state->unknown0c = NONE;
	state->point = *(point3f *)g_4687a4;
	state->unknown62 = false;
	state->normal = *g_4687b0;
	state->material = g_47d8e0;
	state->unknown58 = NONE;
	state->unknown5c = NONE;
	state->unknown1c = NONE;
	state->unknown20 = NONE;
	state->unknown70 = 0.0f;
	state->matrix = *g_4687d0;
}

/* the contact at a surface and the side it faces */
struct s_shape_contact
{
	byte unknown00[0xdc];
	vector3f normal;
};

struct s_shape_side
{
	long unknown0;
	long side;
};

// @retail 0x1f1930
void function_1f1930(s_shape_contact const *contact, s_shape_side *side)
{
	if (contact->normal.i != 0.0f || contact->normal.j != 0.0f || contact->normal.k != 0.0f)
	{
		if (fabs(contact->normal.i) < fabs(contact->normal.j))
		{
			if (contact->normal.j < 0.0f)
				side->side = 2;
			else
				side->side = 3;
		}
		else
		{
			if (contact->normal.i < 0.0f)
				side->side = 5;
			else
				side->side = 4;
		}
	}
}

/* the havok components (g_51e9b8, 0xa0 bytes) and their materials */
struct s_component_material
{
	byte unknown00[0x12];
	short material;
	byte unknown14[0x48 - 0x14];
};

struct s_component
{
	byte unknown00[0x88];
	s_component_material *materials;
	byte unknown8c[0xa0 - 0x8c];
};

// @retail 0x1f1df0
void function_1f1df0(long component_index, long material_index, vector3f const *normal, s_shape_state *state)
{
	s_component *component = (s_component *)(g_51e9b8->data + (component_index & 0xffff) * sizeof(s_component));

	state->normal = *normal;
	if (material_index != NONE)
		state->material = component->materials[material_index].material;
	else
		state->material = g_54e898;
}

struct s_shape_carrier_contact
{
	byte unknown00[0x10];
	long object_index;
	byte unknown14[0x58 - 0x14];
	transform4x3f transform;
	byte unknown8c[0xe8 - 0x8c];
	point3f point;
};

struct s_shape_carrier_state
{
	vector3f previous_velocity;
	byte unknown0c[0x24 - 0xc];
	transform4x3f matrix;
};

bool function_182020(long component_index, vector3f *previous_velocity, point3f const *point,
	transform4x3f const *transform, transform4x3f *matrix, vector3f *velocity,
	vector3f *delta_velocity, matrix3x3 *rotation);

// @retail 0x1f1e50
bool function_1f1e50(s_shape_carrier_state *state, s_shape_carrier_contact const *contact,
	vector3f *velocity, vector3f *delta_velocity, matrix3x3 *rotation)
{
	bool result = false;
	long object_index = contact->object_index;
	if (object_index != NONE)
	{
		s_object_list_state *objects = g_5107f0;
		for (long i = 0; i < objects->object_count; i++)
		{
			if (objects->object_indices[i] == object_index)
			{
				struct s_carrier_header { byte unknown00[8]; byte *object; };
				byte *object = ((s_carrier_header *)g_4e0300->data)[object_index & 0xffff].object;
				result = function_182020(*(long *)(object + 0xb4), &state->previous_velocity,
					&contact->point, &contact->transform, &state->matrix, velocity, delta_velocity, rotation);
				break;
			}
		}
	}
	return result;
}

/* the surfaces' minimum and maximum heights: whether the shape stands at a
   height it can step to */
struct s_shape_ground
{
	byte unknown00[0x1c];
	point3f point;
	byte unknown28[0x34 - 0x28];
	real height;
};

// @retail 0x1f2e60
bool function_1f2e60(bool moving, s_shape_ground const *ground, vector3f const *velocity, bool stepping, point3f const *base, real height)
{
	real top = base->z + height;

	if (top - 0.001f > base->z)
	{
		point3f point = ground->point;
		point3f center;
		vector3f v;
		real distance;
		real lower, upper;

		if (moving)
		{
			point.x = velocity->i * g_510c54->rate + point.x;
			point.y = velocity->j * g_510c54->rate + point.y;
			point.z = velocity->k * g_510c54->rate + point.z;
		}
		center.x = base->x;
		center.y = base->y;
		center.z = height + base->z;
		vector3d_from_points3d(&center, &point, &v);
		distance = (real)sqrt(v.k * v.k + v.j * v.j + v.i * v.i) - height;
		lower = moving ? -0.25f : -0.1f;
		upper = stepping ? 0.1f : 0.2f;
		return PIN(distance, lower, upper) == distance;
	}

	return PIN(ground->height, -0.1f, 0.0328f) == ground->height;
}

struct s_havok_component;
void havok_component_rigid_body_matrix_get(long rigid_body_index, s_havok_component *component, transform4x3f *matrix);
void havok_component_rigid_body_point_velocity_get(long rigid_body_index, s_havok_component *component, point3f const *point, vector3f *velocity);
void function_141590(transform4x3f const *in, transform4x3f *out);
int __fastcall function_142a60(transform4x3f const *a, transform4x3f const *b, transform4x3f *result);
real function_f5e00(long vehicle_index);
extern real g_47f05c;

// @retail 0x1f1ee0
bool function_1f1ee0(s_shape_carrier_contact const *contact, s_shape_state *state,
    vector3f *velocity, matrix3x3 *rotation)
{
    byte const *data = (byte const *)contact;
    byte *shape = *(byte **)(data + 8);
    byte *components = g_51e9b8->data;
    byte *component = components + (*(long *)(data + 0x18) & 0xffff) * 0xa0;
    long count = *(long *)(component + 0x8c);
    bool result = false;
    long best = NONE;
    real best_dot = -3.402823466e38f;
    real height = *(real *)(shape + 0xc);
    if (count > 0)
    {
        byte *contacts = *(byte **)(component + 0x88);
        for (long i = 0; i < count; i++)
        {
            byte *entry = contacts + i * 0x48;
            if (contact->point.z + height - 0.001f > *(real *)(entry + 0x24))
            {
                bool stepping = ((*(dword *)(data + 0x14) >> 2) & 1) != 0;
                bool valid;
                if (contact->point.z + height - 0.001f > contact->point.z)
                {
                    point3f point = *(point3f *)(entry + 0x1c);
                    vector3f delta;
                    delta.i = point.x - contact->point.x;
                    delta.j = point.y - contact->point.y;
                    delta.k = point.z - (contact->point.z + height);
                    real distance = (real)sqrt(delta.k * delta.k + delta.j * delta.j + delta.i * delta.i) - height;
                    real upper = stepping ? 0.1f : 0.2f;
                    valid = PIN(distance, -0.1f, upper) == distance;
                }
                else
                {
                    real distance = *(real *)(entry + 0x34);
                    valid = PIN(distance, -0.1f, g_47f05c) == distance;
                }
                if (valid)
                {
                    vector3f *normal = (vector3f *)(entry + 0x28);
                    vector3f const *up = (vector3f const *)(data + 0x8c);
                    real dot = normal->k * up->k + normal->i * up->i + up->j * normal->j;
                    if (dot > 0.8660253882408142f && dot > best_dot)
                    {
                        best_dot = dot;
                        best = i;
                    }
                }
            }
        }
        if (best != NONE)
        {
            byte *entry = contacts + best * 0x48;
            long component_index = *(long *)(entry + 0x14);
            if (component_index != NONE)
            {
                s_havok_component *other = (s_havok_component *)(components + (component_index & 0xffff) * 0xa0);
                transform4x3f matrix;
                long body_index = *(signed char *)(entry + 0x46);
                havok_component_rigid_body_matrix_get(body_index, other, &matrix);
                if (component_index == *(long *)(data + 0x54))
                {
                    long object_index = *(long *)(entry + 0x18);
                    struct s_contact_object_header { byte unknown0[3]; byte type; byte unknown4[8]; };
                    s_contact_object_header *headers = (s_contact_object_header *)g_4e0300->data;
                    if (object_index == NONE || !( (1 << headers[object_index & 0xffff].type) & 2) || function_f5e00(object_index) != 1.0f)
                    {
                        transform4x3f inverse;
                        transform4x3f relative;
                        function_141590(&matrix, &inverse);
                        function_142a60(&contact->transform, &inverse, &relative);
                        havok_component_rigid_body_point_velocity_get(body_index, other, &contact->point, velocity);
                        function_141590(&relative, &relative);
                        *rotation = relative.rotation;
                        result = true;
                    }
                }
                state->matrix = matrix;
                state->unknown1c = component_index;
                state->unknown5c = *(long *)(entry + 0x18);
                state->unknown20 = body_index;
            }
        }
    }
    return result;
}

real function_30bf0(vector3f *v);

PRIVATE inline vector3f contact_cross(vector3f const &a, vector3f const &b)
{
    vector3f result;
    result.i = a.j * b.k - a.k * b.j;
    result.j = a.k * b.i - a.i * b.k;
    result.k = a.i * b.j - a.j * b.i;
    return result;
}

// @retail 0x1f2a80
void function_1f2a80(vector3f const *velocity, byte const *request, bool moving,
    vector3f const *desired, long const *indices, long count, vector3f *out)
{
    byte *component = g_51e9b8->data + (*(long *)(request + 0x40) & 0xffff) * 0xa0;
    long selected = NONE;
    real lowest = 0.0f;
    for (long i = 0; i < count; i++)
    {
        byte *entry = *(byte **)(component + 0x88) + indices[i] * 0x48;
        real z = *(real *)(entry + 0x30);
        if (lowest > z && z > -0.999f && function_1f2e60(moving, (s_shape_ground *)entry,
            velocity, ((*(dword *)(request + 0x18) >> 1) & 1) != 0,
            (point3f const *)(request + 0x1c), *(real *)(*(byte *const *)request + 0xc)))
        {
            lowest = z;
            selected = i;
        }
    }
    *out = *g_4687b0;
    byte *contacts = *(byte **)(component + 0x88);
    switch (count)
    {
    case 1:
        *out = *(vector3f *)(contacts + indices[0] * 0x48 + 0x28);
        break;
    case 2:
        {
            vector3f const *a = (vector3f *)(contacts + indices[0] * 0x48 + 0x28);
            vector3f const *b = (vector3f *)(contacts + indices[1] * 0x48 + 0x28);
            if (selected != NONE)
            {
                vector3f axis = contact_cross(*a, *b);
                vector3f const *normal = (vector3f *)(contacts + indices[selected] * 0x48 + 0x28);
                if (selected == 0)
                    *out = contact_cross(axis, *normal);
                else
                    *out = contact_cross(*normal, axis);
            }
            else
            {
                vector3f average;
                average.i = b->i + a->i;
                average.j = b->j + a->j;
                average.k = b->k + a->k;
                function_30bf0(&average);
                real dot_a = a->i * desired->i + desired->k * a->k + a->j * desired->j;
                real dot_average = desired->i * average.i + desired->k * average.k + desired->j * average.j;
                *out = dot_average > dot_a ? *a : average;
            }
        }
        break;
    case 3:
        if (selected != NONE)
        {
            vector3f const *normal = (vector3f *)(contacts + indices[selected] * 0x48 + 0x28);
            real scale = -normal->k;
            out->i = normal->i * scale + g_4687b0->i;
            out->j = normal->j * scale + g_4687b0->j;
            out->k = scale * normal->k + g_4687b0->k;
        }
        else
            *out = *g_4687b0;
        break;
    }
    real length = (real)sqrt(out->i * out->i + out->j * out->j + out->k * out->k);
    if (!(fabs(length) < 0.0001f))
    {
        real inverse = 1.0f / length;
        out->i *= inverse;
        out->j = inverse * out->j;
        out->k *= inverse;
    }
    else
        *out = *g_4687b0;
}

// @retail 0x1f1460
void function_1f1460(byte const *state, s_shape_state const *ground, vector3f *arg_9650d9, vector3f *arg_3a7661)
{
    byte *settings = *(byte **)(state + 8);
    vector3f const *old_forward = (vector3f const *)(state + 0xf4);
    vector3f const *old_up = (vector3f const *)(state + 0x100);
    if ((*(dword *)settings >> 3) & 1)
    {
        vector3f up = ground->normal;
        vector3f axis = contact_cross(*old_up, up);
        bool rotate = true;
        if (function_30bf0(&axis) == 0.0f)
        {
            real dot = old_up->k * up.k + old_up->j * up.j + old_up->i * up.i;
            if (dot > 0.0f)
                rotate = false;
            else
                axis = *old_up;
        }
        if (rotate)
        {
            vector3f limited = *old_up;
            double angle = (double)g_510c54->rate * 5.235987663269043f;
            real sine = (real)sin(angle);
            real cosine = (real)cos(angle);
            real dot = (limited.k * axis.k + limited.j * axis.j + limited.i * axis.i) * (1.0f - cosine);
            vector3f cross = contact_cross(limited, axis);
            limited.i = dot * axis.i + limited.i * cosine - cross.i * sine;
            limited.j = axis.j * dot + limited.j * cosine - cross.j * sine;
            limited.k = axis.k * dot + limited.k * cosine - cross.k * sine;
            cross = contact_cross(limited, up);
            if (cross.k * axis.k + cross.j * axis.j + cross.i * axis.i > 0.0f)
                up = limited;
        }
        vector3f cross = contact_cross(*old_forward, up);
        vector3f forward = contact_cross(up, cross);
        if (function_30bf0(&forward) == 0.0f)
        {
            cross = contact_cross(up, *old_up);
            forward = contact_cross(up, cross);
            if (function_30bf0(&forward) == 0.0f)
            {
                up = *g_4687b0;
                forward = *g_4687a8;
            }
        }
        *arg_9650d9 = forward;
        *arg_3a7661 = up;
    }
    else
    {
        *arg_9650d9 = *old_forward;
        *arg_3a7661 = *g_4687b0;
        arg_9650d9->k = 0.0f;
        if (function_30bf0(arg_9650d9) == 0.0f)
            *arg_9650d9 = *g_4687a8;
    }
}

struct s_unknown_1eb550;
extern s_unknown_1eb550 *g_51e9c4;
real g_47ffb0 = 0.5f;
real normalize2d(point2f *v);
bool __stdcall function_a75d0(vector3f *vector, real maximum);

PRIVATE inline vector3f shape_add(vector3f const &a, vector3f const &b)
{
    vector3f r; r.i = a.i + b.i; r.j = a.j + b.j; r.k = a.k + b.k; return r;
}
PRIVATE inline vector3f shape_subtract(vector3f const &a, vector3f const &b)
{
    vector3f r; r.i = a.i - b.i; r.j = a.j - b.j; r.k = a.k - b.k; return r;
}
PRIVATE inline vector3f shape_scale(vector3f const &a, real scale)
{
    vector3f r; r.i = a.i * scale; r.j = a.j * scale; r.k = a.k * scale; return r;
}
PRIVATE inline real shape_dot(vector3f const &a, vector3f const &b)
{
    return a.k * b.k + a.j * b.j + a.i * b.i;
}

// @retail 0x1f0870
void function_1f0870(byte const *state, byte *out, byte *history, vector3f const *current)
{
    byte *settings = *(byte **)(state + 8);
    dword flags = *(dword *)(state + 0x14);
    bool flying = ((*(dword *)settings >> 3) & 1) != 0;
    bool gravity = ((flags >> 3) & 1) != 0;
    bool damping = ((flags >> 4) & 1) != 0;
    vector3f const &input = *(vector3f const *)(state + 0x138);
    vector3f const &facing = *(vector3f const *)(state + 0x9c);
    vector3f const &up = *(vector3f const *)(state + 0x100);
    vector3f const &normal = *(vector3f const *)(state + 0x8c);
    vector3f const &basis = *(vector3f const *)(state + 0x118);
    vector3f *velocity = (vector3f *)(out + 0xc);
    real scale = *(real *)(state + 0x20);
    if (!(flags & 1))
    {
        vector3f left = contact_cross(up, facing);
        *velocity = shape_scale(facing, input.i * scale);
        *velocity = shape_add(*velocity, shape_scale(left, input.j * scale));
        *velocity = shape_add(*velocity, shape_scale(up, input.k * scale));
        gravity = false;
    }
    else if (flags & 4)
    {
        vector3f delta;
        delta.i = scale * (facing.i * input.i - facing.j * input.j) - current->i;
        delta.j = scale * (facing.j * input.i + facing.i * input.j) - current->j;
        point2f limited = { delta.i, delta.j };
        real step = *(real *)(state + 0x148) * g_510c54->rate;
        if (normalize2d(&limited) > step)
        {
            limited.x *= step;
            limited.y *= step;
        }
        else
        {
            limited.x = delta.i;
            limited.y = delta.j;
        }
        velocity->i = current->i + limited.x;
        velocity->j = current->j + limited.y;
        velocity->k = current->k - *(real *)(state + 0x130) * g_510c54->rate;
    }
    else
    {
        real magnitude = (real)sqrt(input.i * input.i + input.j * input.j + input.k * input.k);
        vector3f desired;
        if (flying)
        {
            vector3f left = contact_cross(normal, basis);
            if (function_30bf0(&left) == 0.0f)
            {
                left = contact_cross(normal, *g_4687b0);
                if (function_30bf0(&left) == 0.0f)
                {
                    left = contact_cross(normal, *g_4687a8);
                    function_30bf0(&left);
                }
            }
            vector3f forward = contact_cross(left, normal);
            function_30bf0(&forward);
            desired = shape_add(shape_scale(left, input.j), shape_scale(forward, input.i));
            desired.k += input.k;
            function_30bf0(&desired);
        }
        else
        {
            if (normal.k > 0.001f)
            {
                desired.i = facing.i * input.i - facing.j * input.j;
                desired.j = facing.j * input.i + facing.i * input.j;
                desired.k = input.k - (normal.j * desired.j + normal.i * desired.i) / normal.k;
            }
            else
            {
                vector3f left = contact_cross(*g_4687b0, basis);
                function_30bf0(&left);
                vector3f forward = shape_add(basis, shape_scale(normal, -shape_dot(normal, basis)));
                left = shape_add(left, shape_scale(normal, -shape_dot(normal, left)));
                desired = shape_add(shape_scale(left, input.j), shape_scale(forward, input.i));
                desired.k = (desired.k + input.k) * 5.0f;
            }
            function_30bf0(&desired);
            if (*(real *)(settings + 0x58) >= desired.k)
                magnitude *= *(real *)(settings + 0x4c);
            else if (*(real *)(settings + 0x58) > desired.k)
                magnitude *= 1.0f + (*(real *)(settings + 0x4c) - 1.0f) * (desired.k - *(real *)(settings + 0x58)) /
                    (*(real *)(settings + 0x5c) - *(real *)(settings + 0x58));
            else if (desired.k >= *(real *)(settings + 0x64))
                magnitude *= *(real *)(settings + 0x50);
            else if (desired.k > *(real *)(settings + 0x60))
                magnitude *= 1.0f + (desired.k - *(real *)(settings + 0x60)) * (*(real *)(settings + 0x50) - 1.0f) /
                    (*(real *)(settings + 0x64) - *(real *)(settings + 0x60));
        }
        vector3f delta = shape_subtract(shape_scale(desired, scale * magnitude), *current);
        vector3f limited = delta;
        real step = *(real *)(state + 0x144) * g_510c54->rate;
        if (function_30bf0(&limited) > step)
        {
            limited = shape_scale(limited, step);
            if (flying)
                gravity = false;
        }
        else
        {
            limited = delta;
            gravity = false;
        }
        if (!state[0xa8] && *(long *)((byte *)g_51e9c4 + 0x18) - g_510c54->game_time <= 0)
            limited = shape_subtract(limited, shape_scale(normal, 0.234f));
        if (damping)
        {
            real fraction = 1.0f - *(real *)(state + 0x98);
            limited.i *= fraction;
            limited.j *= fraction;
        }
        if (gravity)
            limited.k -= *(real *)(state + 0x130) * g_510c54->rate;
        *velocity = shape_add(*current, limited);
    }
    if (!((*(dword *)settings >> 2) & 1) && !(flags & 4))
    {
        if (*(long *)(history + 0xc) != NONE && *(long *)(history + 0xc) == g_510c54->game_time - 1)
        {
            vector3f change = shape_add(shape_subtract(input, *(vector3f *)(history + 0x10)), *(vector3f *)(out + 0x64));
            real maximum = (real)sqrt(change.k * change.k + change.j * change.j + change.i * change.i) + g_47ffb0;
            vector3f offset = shape_subtract(*(vector3f const *)(state + 0x124), *(vector3f *)(out + 0x64));
            vector3f relative = shape_subtract(*velocity, offset);
            function_a75d0(&relative, maximum);
            *velocity = shape_add(relative, offset);
        }
        *(vector3f *)(history + 0x10) = input;
        *(long *)(history + 0xc) = g_510c54->game_time;
    }
    if (gravity)
        *(dword *)out |= 1;
    else
        *(dword *)out &= ~1;
}
