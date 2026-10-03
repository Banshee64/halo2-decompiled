// @flags /O2 /arch:SSE /Gr
/* IMPACTS.CPP: the havok contact impacts (sounds and effects played where
   two havok components touch): a data array of 0x20 impacts of 0xa0 bytes
   and one of 0x20 impact arrays (the impacts of one component) */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"
#include "real_math.h"

/* a global material type (a 16 bit index) */
class c_global_material_type
{
public:
	bool operator==(c_global_material_type other) const
	{
		return m_index == other.m_index;
	}

	short m_index;
};

class c_impact
{
public:
	static bool included_in_impact_material(long impact_definition_index, c_global_material_type material_a,
		c_global_material_type material_b, c_global_material_type impact_material_a,
		c_global_material_type impact_material_b);
};

struct s_impact
{
	short salt;
	byte flags;
	char sort_index;
	real priority;
	short reference_count;
	char age;
	byte material_flags;
	char type;
	char unknownd;
	bool unknowne;
	byte unknownf;
	char unknown10;
	bool unknown11;
	byte unknown12[2];
	real unknown14;
	real unknown18;
	char unknown1c;
	char unknown1d;
	char unknown1e;
	char unknown1f;
	char unknown20;
	byte unknown21[3];
	long component_a;
	long component_b;
	c_global_material_type material_a;
	c_global_material_type material_b;
	long sound_a;
	long sound_b;
	long effect_a;
	long effect_b;
	real unknown40;
	real unknown44;
	real unknown48;
	real unknown4c;
	real_vector3d unknown50;
	real_vector3d normal;
	real_point3d position;
	byte unknown74[0x8c - 0x74];
	real unknown8c;
	byte unknown90;
	byte unknown91;
	byte unknown92[2];
	long time;
	real unknown98;
	short unknown9c;
	short unknown9e;
};

/* what creates an impact: the two havok components in contact, their
   materials, the contact point and normal */
struct s_impact_data
{
	bool unknown00;
	byte unknown01[3];
	long component_a;
	long unknown08;
	c_global_material_type material_a;
	byte unknown0e[2];
	long component_b;
	long unknown14;
	c_global_material_type material_b;
	byte unknown1a[2];
	real_point3d position;
	real_vector3d normal;
	long type;
	bool unknown38;
	byte unknown39;
	short unknown3a;
	short unknown3c;
};

/* the havok components (g_51e9b8) as the impacts see them */
struct s_impact_component
{
	short salt;
	byte unknown02[2];
	dword flag0 : 1;
	dword flag1 : 1;
	dword flag2 : 1;
	dword flag3 : 1;
	dword flag4 : 1;
	dword flag5 : 1;
	dword flag6 : 1;
	dword flag7 : 1;
	dword flag8 : 1;
	dword flag9 : 1;
	dword flag10 : 1;
	dword flag11 : 1;
	dword unknown_flags : 20;
	long object_index;
	byte unknown0c[0x18 - 0xc];
	char rigid_body_index;
	byte unknown19[3];
	char unknown1c;
	byte unknown1d[3];
	long impact_array_index;
	byte unknown24[0x70 - 0x24];
	byte *rigid_bodies;
	long rigid_body_count;
	byte unknown78[4];
	byte *contacts;
	long contact_count;
	byte unknown84[4];
	byte *constraints;
	long constraint_count;
	byte unknown90[0xa0 - 0x90];
};

/* the objects as the impacts see them */
struct s_impact_object
{
	long definition_index;
	byte unknown04[0x28 - 0x4];
	long unknown28;
	long unknown2c;
	byte unknown30[0x70 - 0x30];
	real_vector3d unknown70;
	real_vector3d unknown7c;
	real_vector3d velocity;
};

struct s_impact_object_header
{
	short salt;
	byte flags;
	byte type;
	byte unknown04[4];
	s_impact_object *object;
};

PRIVATE inline s_impact_component *impact_component_get(long component_index)
{
	return (s_impact_component *)g_51e9b8->data + (component_index & 0xffff);
}

PRIVATE inline s_impact_object_header *impact_object_header_get(long object_index)
{
	return (s_impact_object_header *)g_4e0300->data + (object_index & 0xffff);
}

/* the impacts of one havok component */
struct s_impact_array
{
	short salt;
	short count;
	long impacts[15];
};

/* what a player is to the impacts: its local player index */
struct s_impact_player
{
	byte unknown00[0x28];
	short local_index;
};

/* the two data arrays (defined in unknown_03d380.cpp, whose game state
   callbacks clear them) */
extern s_data_array *g_51ebfc;
extern s_data_array *g_51ec00;
long g_502138;
long g_50213c[32];
real g_55c2d0;

struct s_small_index;
short function_0b67a0(const s_small_index *data);
real_point3d *function_b9dd0(long object_index, real_point3d *result);
real function_30bf0(real_vector3d *v);

#define MIN(a, b) ((a) > (b) ? (b) : (a))

PRIVATE inline long game_seconds_to_ticks_round(real seconds)
{
	real ticks = g_510c54->ticks_per_second * seconds;
	long result;

	__asm
	{
		fld ticks
		fistp result
	}
	return result;
}

PRIVATE inline s_player_state *local_player_state(long local_index)
{
	s_player_state *result = NULL;

	if (local_index != NONE && g_4686c4 != NONE)
	{
		s_player_4e9bd4 *player = &g_4e9bd4[local_index];
		result = &player->state;
	}
	return result;
}

// @retail 0x2263c0
void function_2263c0(void)
{
	g_51ebfc = data_new_inlined("impacts", 0x20, sizeof(s_impact), 0, g_468758);
	g_51ec00 = data_new_inlined("impact arrarys", 0x20, sizeof(s_impact_array), 0, g_468758);
}

// @retail 0x226440
void function_226440(void)
{
	data_dispose(g_51ebfc);
	data_dispose(g_51ec00);
	g_51ebfc = NULL;
	g_51ec00 = NULL;
}

// @retail 0x227600
bool c_impact::included_in_impact_material(
	long impact_definition_index,
	c_global_material_type material_a,
	c_global_material_type material_b,
	c_global_material_type impact_material_a,
	c_global_material_type impact_material_b)
{
	if (material_a == impact_material_a && material_b == impact_material_b ||
		material_b == impact_material_a && material_a == impact_material_b)
	{
		return true;
	}
	return false;
}

// @retail 0x227350
long __stdcall impact_sort_compare(
	long impact_index_a,
	long impact_index_b,
	void *context)
{
	s_impact *impacts = (s_impact *)g_51ebfc->data;

	return impacts[impact_index_a & 0xffff].priority > impacts[impact_index_b & 0xffff].priority;
}

// @retail 0x227390
long impacts_last_sorted(void)
{
	long index = g_502138;
	long result = NONE;

	if (index != NONE)
	{
		result = g_50213c[index];
		while (index >= 0 && g_50213c[index] == NONE)
		{
			g_502138 = --index;
		}
	}
	return result;
}

// @retail 0x22a0e0
bool impact_has_sounds_or_effects(
	s_impact *impact)
{
	return impact->sound_a != NONE || impact->sound_b != NONE || impact->effect_a != NONE || impact->effect_b != NONE;
}

// @retail 0x229430
void impact_set_peak(
	s_impact *impact,
	real value)
{
	if (!impact->unknown11 || value > impact->unknown18)
	{
		long game_time = g_510c54->game_time;

		if (impact->time == NONE ||
			game_time - impact->time > game_seconds_to_ticks_round(0.2f) ||
			value > impact->unknown98 * 1.3f)
		{
			impact->unknown11 = true;
			impact->unknown18 = value;
		}
	}
}

// @retail 0x22a560
real impact_distance_squared_to_nearest_player(
	real_point3d const *point,
	long type)
{
	real result = 25000000.0f;
	s_data_iterator iterator;
	s_impact_player *player;

	iterator.data = g_4e8c24;
	iterator.index = NONE;
	while ((player = (s_impact_player *)data_iterator_next_inlined(&iterator)) != NULL)
	{
		if (player->local_index != NONE)
		{
			s_player_state *state = local_player_state(player->local_index);
			real_vector3d delta;
			real distance;

			vector3d_from_points3d(point, &state->position, &delta);
			switch (type)
			{
			case 0:
				distance = delta.j * delta.j + delta.i * delta.i + delta.k * delta.k;
				break;
			case 1:
				distance = delta.j * delta.j + delta.i * delta.i + delta.k * delta.k;
				break;
			case 2:
				distance = delta.j * delta.j + delta.i * delta.i + delta.k * delta.k;
				break;
			case 3:
				distance = delta.j * delta.j + delta.i * delta.i + delta.k * delta.k;
				break;
			default:
				__assume(0);
			}
			result = MIN(distance, result);
		}
	}
	return result;
}

// @retail 0x2283e0
bool impact_component_b_is_faster(
	long component_a,
	long component_b)
{
	bool result = false;

	if (component_b != NONE)
	{
		s_impact_object *object_b = impact_object_header_get(impact_component_get(component_b)->object_index)->object;
		s_impact_object *object_a = impact_object_header_get(impact_component_get(component_a)->object_index)->object;

		result = magnitude_squared3d(&object_b->velocity) > magnitude_squared3d(&object_a->velocity);
	}
	return result;
}

PRIVATE inline bool impact_component_is_biped(long component_index)
{
	return component_index != NONE &&
		((1 << impact_object_header_get(impact_component_get(component_index)->object_index)->type) & 1);
}

// @retail 0x2276d0
bool impact_components_valid(
	long component_a,
	long component_b)
{
	bool a_is_biped = impact_component_is_biped(component_a);
	bool b_is_biped = impact_component_is_biped(component_b);
	bool a_valid = true;
	bool b_valid = true;

	if (a_is_biped)
		a_valid = TEST_FIELD_BIT(impact_component_get(component_a)->flag11);
	if (b_is_biped)
		b_valid = TEST_FIELD_BIT(impact_component_get(component_b)->flag11);
	return a_valid && b_valid;
}

// @retail 0x22a440
void impact_set_contact(
	s_impact *impact,
	s_impact_data const *data,
	real_vector3d const *vector,
	real unknown44,
	bool unknownf)
{
	impact->normal = data->normal;
	impact->position = data->position;
	impact->unknown40 = 0.0f;
	impact->unknown44 = unknown44;
	impact->unknown50 = *vector;
	impact->unknownf = unknownf;
}

// @retail 0x22a4b0
void impact_set_contact_from_component(
	s_impact *impact)
{
	s_impact_component *component = impact_component_get(impact->component_a);
	s_impact_object *object = impact_object_header_get(component->object_index)->object;

	impact->normal = object->unknown7c;
	function_b9dd0(component->object_index, &impact->position);
	impact->unknown40 = 0.0f;
	impact->unknown44 = 0.0f;
	impact->unknown50 = object->velocity;
	if (function_30bf0(&impact->unknown50) == g_45dbd8)
		impact->unknown50 = object->unknown70;
}

// @retail 0x227640
bool impacts_match(
	long impact_component_a,
	long impact_component_b,
	long component_a,
	long component_b,
	c_global_material_type impact_material_a,
	c_global_material_type impact_material_b,
	c_global_material_type material_a,
	c_global_material_type material_b,
	long impact_type,
	long type,
	bool impact_flag,
	bool flag,
	long impact_unknown,
	long unknown)
{
	if (impact_flag)
	{
		if (flag)
		{
			return impact_material_a == material_a && impact_material_b == material_b ||
				impact_material_b == material_a && impact_material_a == material_b;
		}
	}
	else if (!flag)
	{
		return (impact_component_a == component_a && impact_component_b == component_b ||
			impact_component_a == component_b && impact_component_b == component_a) &&
			c_impact::included_in_impact_material(NONE, impact_material_a, impact_material_b, material_a, material_b) &&
			impact_unknown == unknown &&
			impact_type == type;
	}
	return false;
}

// @retail 0x2274f0
bool impact_matches_data(
	s_impact *impact,
	s_impact_data const *data,
	bool check_position)
{
	long component_a = data->component_a;

	if (impacts_match(impact->component_a, impact->component_b, component_a, data->component_b,
		impact->material_a, impact->material_b, data->material_a, data->material_b,
		impact->unknownd, data->type, impact->unknowne, data->unknown38, impact->unknown9c, data->unknown3a))
	{
		if (check_position)
		{
			real_vector3d delta;
			real distance_squared;

			vector3d_from_points3d(&impact->position, &data->position, &delta);
			distance_squared = magnitude_squared3d(&delta);
			if (!(distance_squared < 0.25f) &&
				(!(distance_squared < 16.0f) || !(dot_product3d(&impact->normal, &data->normal) > g_55c2d0)))
			{
				return false;
			}
		}
		if (impact_components_valid(component_a, data->component_b))
			return true;
	}
	return false;
}

PRIVATE inline void cross_product3d(real_vector3d const *a, real_vector3d const *b, real_vector3d *result)
{
	result->i = a->j * b->k - a->k * b->j;
	result->j = a->k * b->i - a->i * b->k;
	result->k = a->i * b->j - a->j * b->i;
}

// @retail 0x227810
void impact_build_matrix(
	long component_index,
	s_impact const *impact,
	matrix3x3 *matrix)
{
	s_impact_object *object = impact_object_header_get(impact_component_get(component_index)->object_index)->object;
	real_vector3d const *forward = &object->unknown70;
	real dot;

	matrix->up = impact->normal;
	dot = dot_product3d(forward, &impact->normal);
	if (dot < 0.0f)
		dot = -dot;
	if (!(dot < 0.9f))
		forward = &object->unknown7c;
	matrix->forward = *forward;
	dot = dot_product3d(&matrix->up, &matrix->forward);
	matrix->forward.i -= matrix->up.i * dot;
	matrix->forward.j -= matrix->up.j * dot;
	matrix->forward.k -= matrix->up.k * dot;
	function_30bf0(&matrix->forward);
	cross_product3d(&matrix->up, &matrix->forward, &matrix->left);
	function_30bf0(&matrix->left);
}
