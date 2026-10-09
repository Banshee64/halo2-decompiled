// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1E67A0.CPP: character physics update input datum setters */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_0259d0.h"

struct s_shape_contact;
struct s_shape_side;
struct s_contact;
struct s_contact_result;
struct s_tracking_result;
struct s_tracking_source;
struct s_tracking_target;
struct s_unknown_object;
struct s_unknown_output;

struct s_biped_physics_output;
struct s_physics_movement_definition
{
	dword flags;
};

struct s_physics_movement_input
{
	byte field_0[0x2c];
	long index;
	byte field_30[4];
	transform4x3f matrix;
	byte field_68[0xc];
	vector3f vector;
};

struct s_physics_movement_output
{
	long field_0;
	bool enabled;
	byte field_5[3];
	s_physics_movement_definition *definition;
	byte field_c[8];
	dword flags;
	byte field_18[0x54 - 0x18];
	long index;
	transform4x3f matrix;
	vector3f vector;
	real rate;
	vector3f direction;
	bool field_a8;
	byte field_a9[0xdc - 0xa9];
	vector3f control;
	byte field_e8[0xc];
	vector3f default_direction;
	byte field_100[0xc];
	vector3f active_direction;
	byte field_118[0x138 - 0x118];
	vector3f velocity;
	real speed;
	long field_148;
};

struct s_physics_movement_settings
{
	byte field_0[0x24];
	real limited_speed;
	byte field_28[4];
	real forward;
	real backward;
	real sideways;
	real speed;
	real crouched_forward;
	real crouched_backward;
	real crouched_sideways;
	real crouched_speed;
	long field_4c;
};

struct s_physics_movement_globals
{
	byte field_0[0x134];
	s_physics_movement_settings *settings;
};

// @retail 0x1e6120
void function_1e6120(s_biped_physics_output *output, void *physics, real rate,
	bool airborne, bool limited, bool mode, real crouch)
{
	s_physics_movement_output *state = (s_physics_movement_output *)output;
	s_physics_movement_input *input = (s_physics_movement_input *)physics;
	const bool *airborne_reference = &airborne;
	const bool *limited_reference = &limited;
	const bool *mode_reference = &mode;
	const real *crouch_reference = &crouch;
	state->index = input->index;
	state->matrix = input->matrix;
	state->vector = input->vector;
	state->rate = rate;
	state->field_a8 = *mode_reference;
	state->direction = state->default_direction;
	state->enabled = true;
	real blend = 0.0f;
	if (!(state->flags & 4))
		blend = *crouch_reference;
	if ((bool)((state->definition->flags >> 2) & 1) && !*airborne_reference)
	{
		s_physics_movement_settings *settings = ((s_physics_movement_globals *)g_4e034c)->settings;
		vector3f control = state->control;
		real squared = length_sq3f(&control);
		if (squared > 1.0f)
		{
			real scale = 1.0f / (real)sqrt(squared);
			control.i = (real)(control.i * scale);
			control.j = (real)(control.j * scale);
		}
		real remaining = 1.0f - blend;
		real first, second;
		if (control.i > 0.0f)
		{
			first = settings->forward;
			second = settings->crouched_forward;
		}
		else
		{
			first = settings->backward;
			second = settings->crouched_backward;
		}
		state->velocity.i = first * remaining + second * blend;
		state->velocity.j = settings->sideways * remaining + settings->crouched_sideways * blend;
		state->velocity.k = 0.0f;
		state->speed = settings->speed * remaining + settings->crouched_speed * blend;
		state->field_148 = settings->field_4c;
		if (*limited_reference)
		{
			state->velocity.i = control.i > 0.0f ? settings->limited_speed : 0.0f;
			state->velocity.j = 0.0f;
		}
		state->velocity.i *= control.i;
		state->velocity.j *= control.j;
		state->direction = state->active_direction;
	}
	else if (!(state->flags & 8) && !(state->flags & 4))
		state->speed = 3.4028234663852886e+38f;
}

void function_1f1930(const s_shape_contact *contact, s_shape_side *side);
void function_1faeb0(const s_contact *contact, s_contact_result *result);
void function_1fc620(s_tracking_result *result, const s_tracking_source *source, const s_tracking_target *target);
void function_1ec3f0(s_unknown_object *object, s_unknown_output *output);

struct s_contact_dispatch_output
{
	long field_0;
	long direction;
	bool forced;
};

// @retail 0x1e5b50
void function_1e5b50(const byte *contact, s_contact_dispatch_output *output, const byte *state)
{
	if (contact[0x14] & 0x20)
	{
		output->direction = 1;
		output->forced = true;
	}
	else
	{
		switch (*state)
		{
		case 1: function_1f1930((const s_shape_contact *)contact, (s_shape_side *)output); break;
		case 2: function_1faeb0((const s_contact *)contact, (s_contact_result *)output); break;
		case 3: break;
		case 4: function_1fc620((s_tracking_result *)output, (const s_tracking_source *)(state + 0x10), (const s_tracking_target *)contact); break;
		case 5: function_1fc620((s_tracking_result *)output, (const s_tracking_source *)(state + 0x10), (const s_tracking_target *)contact); break;
		case 6: function_1ec3f0((s_unknown_object *)contact, (s_unknown_output *)output); break;
		default: __assume(0);
		}
	}
}


struct s_character_physics_component
{
	byte unknown00[0x10];
	point3f position;
	byte unknown1c;
	byte has_position;
};

struct s_type_94656b
{
	long unknown00;
	byte unknown04;
	byte unknown05[0x27];
	point3f point2c;
	point3f point38;
	point3f point44;
	long unknown50;
	byte unknown54[0x58];
	point3f pointac;
	byte byteb8;
	byte byteb9;
	byte unknownba[2];
	byte bytebc;
	byte unknownbd[3];
	point3f pointc0;
	point3f pointcc;
	real valued8;
};

struct s_character_physics_update_input_datum_a
{
	long unknown00;
	long unknown04;
	long unknown08;
	long unknown0c;
	long unknown10;
	byte unknown14;
	byte unknown15[3];
	unsigned long flags;
	point3f point1c;
	point3f point28;
	point3f point34;
	long unknown40;
};

struct s_source_a
{
	byte unknown00;
	byte unknown01[7];
	long unknown08;
	long unknown0c;
};

struct s_time_entry
{
	long time;
	short a;
	short b;
};

// @retail 0x1e67a0
void function_1e67a0(s_character_physics_component *component, s_type_94656b *datum, byte a, byte b)
{
	point3f *point;
	if (component->has_position)
		point = &component->position;
	else
		point = (point3f *)g_4687b0;
	datum->pointac = *point;
	datum->byteb8 = a;
	datum->byteb9 = b;
	datum->unknown04 = 1;
}

// @retail 0x1e67f0
void function_1e67f0(s_type_94656b *datum, s_character_physics_component *component, long animation_id, point3f *p1, point3f *p2, point3f *p3)
{
	datum->unknown50 = animation_id;
	datum->point2c = *p1;
	datum->point38 = *p2;
	datum->point44 = *p3;
	datum->unknown04 = 1;
}

// @retail 0x1e6850
void function_1e6850(s_type_94656b *datum, byte a, point3f *p1, point3f *p2, real v)
{
	datum->bytebc = a;
	datum->pointc0 = *p1;
	datum->pointcc = *p2;
	datum->valued8 = v;
	datum->unknown04 = 1;
}

// @retail 0x1e68a0
void function_1e68a0(s_character_physics_update_input_datum_a *datum, s_source_a *source, long a1, long a2, long a3, bool b0, bool b1, bool b2, bool b3, bool b4, point3f *p1, point3f *p2, point3f *p3)
{
	datum->unknown04 = source->unknown08;
	datum->unknown08 = source->unknown0c;
	datum->unknown0c = a3;
	datum->unknown10 = source->unknown00;
	datum->unknown00 = a1;
	datum->unknown40 = a2;
	datum->flags = 0;
	datum->unknown14 = 0;
	datum->flags = b0 ? 1 : 0;
	if (b1) datum->flags |= 2; else datum->flags &= ~2;
	if (b2) datum->flags |= 4; else datum->flags &= ~4;
	if (b3) datum->flags |= 8; else datum->flags &= ~8;
	if (b4) datum->flags |= 16; else datum->flags &= ~16;
	datum->point1c = *p1;
	datum->point28 = *p2;
	datum->point34 = *p3;
}

// @retail 0x1e6980
void function_1e6980(s_time_entry *entries, short a, byte b)
{
	long index = 0;
	long oldest = entries[0].time;
	if (entries[1].time < oldest)
	{
		index = 1;
		oldest = entries[1].time;
	}
	if (entries[2].time < oldest)
	{
		index = 2;
		oldest = entries[2].time;
	}
	if (entries[3].time < oldest)
	{
		index = 3;
	}
	entries[index].time = g_510c54->game_time;
	entries[index].a = a;
	entries[index].b = b;
}

#include "unknown_1cafc0.h"
#include <math.h>
#include <xmmintrin.h>

bool g_46fcc4 = true;
bool g_51e9bc;

// @retail 0x1e6360
void function_1e6360(s_biped_physics_output *output, real rate, long object_index, real blend)
{
 s_physics_movement_output *state = (s_physics_movement_output *)output;
 (void)&object_index;
 (void)&blend;
 *(real *)((byte *)state + 0x28) = rate;
 state->enabled = true;
 byte *definition = (byte *)state->definition;
 real input_fraction = 1.0f - ((real)sqrt(state->control.i * state->control.i +
  state->control.j * state->control.j + state->control.k * state->control.k) < 1.0f ?
  (real)sqrt(state->control.i * state->control.i + state->control.j * state->control.j +
   state->control.k * state->control.k) : 1.0f);
 real factor = 1.0f;
 if (*(real *)(definition + 0x90) > 0.0f)
 {
  if (blend == 1.0f) factor = *(real *)(definition + 0x90);
  else if (blend > 0.0f) factor = (*(real *)(definition + 0x90) - 1.0f) * blend + 1.0f;
 }
 factor *= *(real *)((byte *)state + 0x134);
 real scale = *(real *)((byte *)state + 0x20);
 vector3f limits;
 limits.i = (*(real *)(definition + 0x78) * scale) * factor;
 limits.j = (*(real *)(definition + 0x7c) * scale) * factor;
 limits.k = limits.j;
 real acceleration = ((1.0f - input_fraction) * *(real *)(definition + 0x80) +
  *(real *)(definition + 0x84) * input_fraction) * scale * factor;
 state->speed = acceleration;
 *(real *)&state->field_148 = acceleration;
 state->velocity.i = state->control.i * limits.i;
 state->velocity.j = state->control.j * limits.j;
 state->velocity.k = state->control.k * limits.k;
 if (g_46fcc4)
 {
  vector3f movement;
  real angular;
  ((s_animation_state *)*(long *)((byte *)state + 0x1c))->movement_rate_get(object_index, &movement, &angular);
  movement.i *= scale;
  movement.j *= scale;
  movement.k *= scale;
  vector3f direction = movement;
  real length_squared = direction.k * direction.k + direction.j * direction.j + direction.i * direction.i;
  if (length_squared != 0.0f)
  {
   real inverse;
   _mm_store_ss(&inverse, _mm_rsqrt_ss(_mm_load_ss(&length_squared)));
   direction.i *= inverse;
   direction.j *= inverse;
   direction.k *= inverse;
  }
  real projection = 0.0f - (state->velocity.k * direction.k + state->velocity.j * direction.j + state->velocity.i * direction.i);
  state->velocity.i += projection * direction.i;
  state->velocity.j += projection * direction.j;
  state->velocity.k += projection * direction.k;
  state->velocity.i += movement.i;
  state->velocity.j += movement.j;
  state->velocity.k += movement.k;
 }
 if (g_51e9bc)
 {
  long bits = g_510c54->game_time * *(long *)((byte *)state + 0x18);
  real x = ((bits & 8) ? 1.0f : -1.0f) * ((bits & 1) ? 1.0f : 0.0f);
  real y = ((bits & 16) ? 1.0f : -1.0f) * ((bits & 2) ? 1.0f : 0.0f);
  real z = ((bits & 32) ? 1.0f : -1.0f) * ((bits & 4) ? 1.0f : 0.0f);
  x *= (bits & 64) ? 1.0f : 0.5f;
  y *= (bits & 128) ? 1.0f : 0.5f;
  z *= (bits & 256) ? 1.0f : 0.5f;
  state->velocity.i += x * 0.2f * limits.i;
  state->velocity.j += y * 0.2f * limits.j;
  state->velocity.k += z * 0.2f * limits.k;
 }
}

struct s_physics_initial_state
{
 long mode;
 bool enabled;
 byte field_5[3];
 const void *definition;
 long physics8;
 long physicsc;
 dword flags;
 long component_index;
 s_animation_state *animation;
 real scale;
 long value;
 real height;
 byte field_2c[0xdc - 0x2c];
 vector3f control;
 point3f position;
 vector3f forward;
 vector3f up;
 vector3f facing_goal;
 vector3f facing;
 vector3f ground_velocity;
 real gravity;
 real boost;
 vector3f velocity;
 real speed;
 real field_148;
 long material;
};

PRIVATE inline void initial_cross(const vector3f *a, const vector3f *b, vector3f *result)
{
 result->i = a->j * b->k - a->k * b->j;
 result->j = a->k * b->i - a->i * b->k;
 result->k = a->i * b->j - a->j * b->i;
}

PRIVATE inline real initial_dot(const vector3f *a, const vector3f *b)
{
 return a->k * b->k + a->j * b->j + a->i * b->i;
}

// @retail 0x1e5bb0
void function_1e5bb0(s_biped_physics_output *output, void *physics, void *animation_state, real speed_scale,
 long component_index, long object_index, void const *definition, long value, bool b, bool turning,
 bool c, bool landing, bool d, bool grounded, bool e, bool f, real gravity, real boost,
 vector3f const *control, point3f const *position, vector3f const *forward, vector3f const *up,
 vector3f const *facing_goal, vector3f const *facing, vector3f const *ground_velocity, long material)
{
 s_physics_initial_state *state = (s_physics_initial_state *)output;
 s_animation_state *animation = (s_animation_state *)animation_state;
 byte *component = g_51e9b8->data + (component_index & 0xffff) * 0xa0;
 state->mode = *(byte *)physics;
 state->component_index = component_index;
 state->definition = definition;
 state->physics8 = *(long *)((byte *)physics + 8);
 state->physicsc = *(long *)((byte *)physics + 0xc);
 state->value = value;
 state->flags = 0;
 state->animation = animation;
 state->scale = speed_scale;
 if (b) state->flags |= 1; else state->flags &= ~1;
 if (d) state->flags |= 2; else state->flags &= ~2;
 if (grounded) state->flags |= 4; else state->flags &= ~4;
 if (e) state->flags |= 8; else state->flags &= ~8;
 if (f) state->flags |= 16; else state->flags &= ~16;
 if (!((bool)((*(dword *)(component + 4) >> 1) & 1))) state->flags |= 64;
 else state->flags &= ~64;
 state->gravity = gravity;
 state->boost = boost;
 state->control = *control;
 state->position = *position;
 state->forward = *forward;
 state->up = *up;
 state->facing = *facing;
 state->facing_goal = *facing_goal;
 state->ground_velocity = *ground_velocity;
 state->material = material;
 state->velocity = *g_4687a4;
 state->speed = 4.8f;
 state->field_148 = 0.0f;
 state->enabled = false;
 if (animation->graph_tag_index != NONE && *(long *)animation != NONE &&
  *(short *)((byte *)animation + 6) != NONE && !landing)
 {
  vector3f movement;
  real angle;
  animation->movement_rate_get(object_index, &movement, &angle);
  state->velocity.i = movement.i * state->scale;
  state->velocity.j = movement.j * state->scale;
  state->velocity.k = movement.k * state->scale;
  if (fabs(angle) >= 0.0001f)
  {
   vector3f original = state->forward;
   real rotation = angle * g_510c54->rate;
   real sine = (real)sin(rotation);
   real cosine = (real)cos(rotation);
   real projection = (original.j * state->up.j + original.k * state->up.k + original.i * state->up.i) * (1.0f - cosine);
   vector3f cross;
   cross.i = original.j * state->up.k - original.k * state->up.j;
   cross.j = original.k * state->up.i - original.i * state->up.k;
   cross.k = original.i * state->up.j - original.j * state->up.i;
   vector3f rotated;
   rotated.i = original.i * cosine + projection * state->up.i - cross.i * sine;
   rotated.j = projection * state->up.j + original.j * cosine - cross.j * sine;
   rotated.k = projection * state->up.k + original.k * cosine - cross.k * sine;
   if (c && turning)
   {
    real alignment = state->forward.j * facing_goal->j + state->forward.k * facing_goal->k + state->forward.i * facing_goal->i;
    if (alignment > 0.5f)
    {
     vector3f old_cross, new_cross;
     initial_cross(facing_goal, &state->forward, &old_cross);
     initial_cross(facing_goal, &rotated, &new_cross);
     bool moved_away = false;
     if (alignment > 0.99f && alignment > initial_dot(&rotated, facing_goal)) moved_away = true;
     if (initial_dot(&state->up, &old_cross) * initial_dot(&state->up, &new_cross) <= 0.0f || moved_away)
     {
      state->forward = *facing_goal;
      state->flags |= 32;
      return;
     }
    }
   }
   state->forward = rotated;
  }
 }
}

struct s_shape_state;
struct s_direction_rotation_input;
void function_1f1460(byte const *state, s_shape_state const *ground, vector3f *up, vector3f *forward);
void function_1faa20(byte const *state, vector3f *up, vector3f *forward, long *ticks);
void function_1ecf50(vector3f *forward, vector3f *up, s_direction_rotation_input *input);

// @retail 0x1e5af0
void function_1e5af0(void *physics, s_biped_physics_output *output, vector3f const *up, vector3f const *forward)
{
 switch (*(byte *)physics)
 {
 case 1: function_1f1460((byte *)output, (s_shape_state *)((byte *)physics + 0x10), (vector3f *)up, (vector3f *)forward); break;
 case 2: function_1faa20((byte *)output, (vector3f *)up, (vector3f *)forward, (long *)((byte *)physics + 0x10)); break;
 case 3: function_1ecf50((vector3f *)forward, (vector3f *)up, (s_direction_rotation_input *)output); break;
 case 4: break;
 case 5: break;
 case 6: break;
 default: __assume(0);
 }
}
