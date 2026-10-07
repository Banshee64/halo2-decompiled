/* UNKNOWN_2BA3E0.CPP: filling a placement (a position, a unit direction and
   a second vector) */

#include "unknown_11c920.h"
#include "unknown_0259d0.h"

// @flags /O2 /arch:SSE /Gr

/* a placement (0x28 bytes) */
struct s_placement
{
	point3f position;
	vector3f direction;
	vector3f up;
	short value24;
};

/* the direction used when the given one cannot be normalized */
extern vector3f *g_4687bc;

real function_1201a0(vector3f *v, vector3f const *fallback);

// @retail 0x2ba3e0
void placement_set(s_placement *placement, vector3f const *direction, point3f const *position, vector3f const *up, short value)
{
	vector3f normal = *direction;

	function_1201a0(&normal, g_4687bc);
	placement->value24 = value;
	placement->position = *position;
	placement->direction = normal;
	placement->up = *up;
}

#include "data_array.h"
extern s_record_pool *g_51ec84;

struct s_particle_2b96
{
    long id;
    long next;
    byte unknown08[0x1c - 8];
    point3f position;
    vector3f velocity;
    byte unknown34[12];
};

PRIVATE __forceinline real inverse_sqrt_2b96(real squared)
{
    real inverse;
    __asm
    {
        rsqrtss xmm0, squared
        movss inverse, xmm0
    }
    return inverse;
}

// @retail 0x2b9670
s_particle_2b96 *function_2b9670(s_particle_2b96 *particle, long index)
{
    s_particle_2b96 *result = 0;
    vector3f velocity = particle->velocity;
    real squared = velocity.i * velocity.i + velocity.j * velocity.j + velocity.k * velocity.k;
    if (squared != 0.0f)
    {
        real inverse = inverse_sqrt_2b96(squared);
        velocity.i *= inverse;
        velocity.j *= inverse;
        velocity.k *= inverse;
    }
    real closest = 0.0f;
    while (index != NONE)
    {
        s_particle_2b96 *other = &((s_particle_2b96 *)g_51ec84->data)[index & 0xffff];
        index = other->next;
        if (other != particle)
        {
            vector3f delta;
            delta.i = particle->position.x - other->position.x;
            delta.j = particle->position.y - other->position.y;
            delta.k = particle->position.z - other->position.z;
            real distance = delta.i * delta.i + delta.j * delta.j + delta.k * delta.k;
            if (distance < closest || !result)
            {
                result = other;
                closest = distance;
            }
        }
    }
    return result;
}

PRIVATE __forceinline real particle_vector_squared_2b(vector3f const *v)
{
    real squared = v->k * v->k;
    squared += v->i * v->i;
    squared += v->j * v->j;
    return squared;
}

PRIVATE __forceinline void scale_particle_vector_2b(real scale, vector3f *v)
{
    v->i = scale * v->i;
    v->j = v->j * scale;
    v->k = v->k * scale;
}

PRIVATE __forceinline void normalize_particle_vector_2b(vector3f *v)
{
    real squared = particle_vector_squared_2b(v);
    if (squared != 0.0f)
    {
        real inverse = inverse_sqrt_2b96(squared);
        scale_particle_vector_2b(inverse, v);
    }
}

// @retail 0x2b98b0
void function_2b98b0(s_particle_2b96 const *particle, s_particle_2b96 const *other, real scale, vector3f *result)
{
    vector3f a = particle->velocity;
    vector3f b = other->velocity;
    normalize_particle_vector_2b(&a);
    normalize_particle_vector_2b(&b);
    vector3f delta;
    delta.i = (b.i - a.i) * scale;
    delta.j = (b.j - a.j) * scale;
    delta.k = (b.k - a.k) * scale;
    result->i += delta.i;
    result->j += delta.j;
    result->k += delta.k;
}

// @retail 0x2b9750
void function_2b9750(s_particle_2b96 const *other, s_particle_2b96 const *particle, real radius, real scale, vector3f *result)
{
    if (fabs(scale) < 0.0001f)
        return;
    vector3f delta;
    delta.i = other->position.x - particle->position.x;
    delta.j = other->position.y - particle->position.y;
    delta.k = other->position.z - particle->position.z;
    union { real value; long bits; } distance;
    distance.value = delta.i * delta.i + delta.j * delta.j + delta.k * delta.k;
    distance.bits = (distance.bits >> 1) + 0x1fc00000;
    real ratio = distance.value / (radius > 0.1f ? radius : 0.1f);
    delta.i = particle->position.x - other->position.x;
    delta.j = particle->position.y - other->position.y;
    delta.k = particle->position.z - other->position.z;
    normalize_particle_vector_2b(&delta);
    if (ratio < 1.0f)
        scale = 0.0f - ratio * scale;
    delta.i *= scale;
    delta.j *= scale;
    delta.k *= scale;
    result->i += delta.i;
    result->j += delta.j;
    result->k += delta.k;
}

#include "globals.h"
#include "unknown_246cd0.h"
void __stdcall function_173ba0(dword mask, void *a, void *b, void const *c, real *values);
void function_211b70(short index, real const *position, byte flags, vector3f *out);
real function_17c900(short function_type, real input);

struct s_particle_property_entry_2ba
{
    long unused;
    s_particle_property property;
};
struct s_particle_properties_2ba
{
    byte unknown00[8];
    s_particle_property_entry_2ba *properties;
    dword constant_mask;
    dword input_mask;
};

// @retail 0x2ba100
void function_2ba100(s_particle_properties_2ba const *definition, void *system, long first, real scale)
{
    dword remaining = definition->constant_mask;
    s_particle_property_entry_2ba *properties = definition->properties;
    long next = first;
    dword valid = 0;
    void *current_system = 0;
    void *current_emitter = 0;
    s_particle_2b96 *current_particle = 0;
    real properties_values[3] = { 0.0f, 0.0f, 0.0f };
    real values[17];
    if (system)
    {
        current_system = system;
        valid = 0;
    }
    dword requested = definition->input_mask & 0x107f0;
    function_173ba0(requested, current_system, current_emitter, current_particle, values);
    valid |= requested;
    for (dword i = 0; i < 3 && remaining; ++i)
    {
        dword bit = 1 << i;
        if (remaining & bit)
        {
            properties_values[i] = function_246cd0(&properties[i].property, values);
            remaining &= ~bit;
        }
    }
    while (next != NONE)
    {
        s_particle_2b96 *particle = &((s_particle_2b96 *)g_51ec84->data)[next & 0xffff];
        next = particle->next;
        if (!(*((byte *)particle + 2) & 9) && *(real *)((byte *)particle + 8) <= 1.0f)
        {
            if (particle != current_particle)
            {
                current_particle = particle;
                valid &= 0xffff07f0;
            }
            requested = definition->input_mask & 0xf80f;
            function_173ba0(requested & ~valid, current_system, current_emitter, current_particle, values);
            valid |= requested;
            for (dword j = 0; j < 3; ++j)
            {
                if (!(definition->constant_mask & (1 << j)))
                    properties_values[j] = function_246cd0(&properties[j].property, values);
            }
            short cluster = *(short *)((byte *)system + 0x20);
            short wind_index = NONE;
            if (cluster != NONE)
            {
                byte *clusters = *(byte **)((byte *)g_4e0348 + 0xa0);
                wind_index = *(short *)(clusters + cluster * 0xb0 + 0x76);
            }
            vector3f wind;
            function_211b70(wind_index, particle->position.n, 8, &wind);
            real time = g_510c54->game_time * g_510c54->rate;
            real phase = function_17c900(10, time + properties_values[1] * scale);
            real amount = (phase - 0.5f) * scale;
            *(real *)((byte *)particle + 0x34) += amount * properties_values[2];
            amount *= properties_values[0];
            vector3f delta;
            delta.i = amount * wind.i;
            delta.j = wind.j * amount;
            delta.k = wind.k * amount;
            particle->velocity.i += delta.i;
            particle->velocity.j += delta.j;
            particle->velocity.k += delta.k;
        }
    }
}

#include "unknown_0259a0.h"

// @retail 0x2b9a00
void function_2b9a00(s_particle_2b96 const *particle, real distance, vector3f *result)
{
    vector3f direction = particle->velocity;
    union
    {
        s_collision_result_1697c0 value;
        byte storage[0x5c];
    } collision;
    real squared = direction.i * direction.i + direction.j * direction.j + direction.k * direction.k;
    collision.value.unknown24 = NONE;
    if (squared != 0.0f)
    {
        real inverse = inverse_sqrt_2b96(squared);
        scale_particle_vector_2b(inverse, &direction);
    }
    direction.i *= distance;
    direction.j *= distance;
    direction.k *= distance;
    if (function_1697c0(0x800005, &particle->position, &direction, NONE, NONE, &collision.value))
    {
        vector3f const *normal = (vector3f const *)(collision.storage + 0x28);
        result->i += normal->i;
        result->j += normal->j;
        result->k += normal->k;
    }
}
