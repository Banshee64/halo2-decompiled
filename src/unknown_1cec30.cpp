// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_1CEC30.CPP: the havok components (0x1cec30..0x1cffxx): a data
   array of 0x200 components of 0xa0 bytes, one per object with a Havok
   rigid body, and the properties the game keeps on the rigid bodies */

#include "unknown_11c920.h"
#include "globals.h"
#include "data_array.h"
#include "unknown_1cec30.h"
#include <string.h>

inline bool havok_entity_property_exists(hkEntity const *entity, dword key)
{
	long i;

	for (i = 0; i < entity->m_property_count; i++)
	{
		if (entity->m_properties[i].m_key == key)
		{
			return true;
		}
	}
	return false;
}


// @retail 0x1cec30
void havok_components_initialize(void)
{
	g_51e9b8 = data_new_inlined("havok components", 0x200, sizeof(s_havok_component), 4, g_468758);
}

// @retail 0x1cf280
long havok_entity_component_index_get(hkEntity const *entity)
{
	return havok_entity_property_get(entity, HAVOK_PROPERTY_COMPONENT_INDEX);
}

// @retail 0x1cf2b0
void havok_entity_component_index_set(hkEntity *entity, hkPropertyValue component_index)
{
	if (havok_entity_property_exists(entity, HAVOK_PROPERTY_COMPONENT_INDEX))
	{
		entity->removeProperty(HAVOK_PROPERTY_COMPONENT_INDEX);
	}
	entity->addProperty(HAVOK_PROPERTY_COMPONENT_INDEX, component_index);
}

// @retail 0x1cf300
long havok_entity_property_2002_get(hkEntity const *entity)
{
	return havok_entity_property_get(entity, HAVOK_PROPERTY_2002);
}

// @retail 0x1cf330
void havok_entity_property_2002_set(hkEntity *entity, hkPropertyValue value)
{
	if (havok_entity_property_exists(entity, HAVOK_PROPERTY_2002))
	{
		entity->removeProperty(HAVOK_PROPERTY_2002);
	}
	entity->addProperty(HAVOK_PROPERTY_2002, value);
}

// @retail 0x1cf380
long __cdecl havok_entity_property_2003_get(hkEntity const *entity)
{
	return havok_entity_property_get(entity, HAVOK_PROPERTY_2003);
}

// @retail 0x1cf3b0
void havok_entity_property_2003_set(hkEntity *entity, hkPropertyValue value)
{
	if (havok_entity_property_exists(entity, HAVOK_PROPERTY_2003))
	{
		entity->removeProperty(HAVOK_PROPERTY_2003);
	}
	entity->addProperty(HAVOK_PROPERTY_2003, value);
}

// @retail 0x1cf950
void s_havok_component::initialize(long object_index)
{
	s_havok_component *component = this;
	real seconds;
	long ticks;

	component->object_index = object_index;
	component->unknown04 = 0;
	component->unknown0c = NONE;
	seconds = g_510c54->field_2_3 * 0.35f;
	__asm
	{
		fld seconds
		fistp ticks
	}
	component->unknown18 = NONE;
	component->unknown19 = NONE;
	component->unknown20 = NONE;
	component->unknown14 = 0.0f;
	component->unknown1a = false;
	component->unknown1b = false;
	component->unknown1c = false;
	component->unknown10 = -ticks;
	component->rigid_bodies.data = NULL;
	component->rigid_bodies.size = 0;
	component->rigid_bodies.capacity_and_flags = 0x80000000;
	component->unknown7c.data = NULL;
	component->unknown7c.size = 0;
	component->unknown7c.capacity_and_flags = 0x80000000;
	component->unknown88.data = NULL;
	component->unknown88.size = 0;
	component->unknown88.capacity_and_flags = 0x80000000;
	component->unknown94 = NULL;
	component->rigid_body = NULL;
	component->unknown9c = 0;
}

// @retail 0x1cfdf0
bool havok_component_unknown10_recent(s_havok_component const *component)
{
	long game_time = g_510c54->game_time;
	real seconds = g_510c54->field_2_3 * 0.35f;
	long ticks;

	__asm
	{
		fld seconds
		fistp ticks
	}
	return game_time - component->unknown10 < ticks;
}

// @retail 0x1cfe30
void havok_component_unknown10_expire(s_havok_component *component)
{
	long game_time = g_510c54->game_time;
	real seconds = g_510c54->field_2_3 * 0.35f;
	long ticks;

	__asm
	{
		fld seconds
		fistp ticks
	}
	component->unknown10 = game_time - ticks + 1;
}

// @retail 0x1cf0b0
long havok_component_new(long object_index)
{
	s_record_pool *components = g_51e9b8;
	long component_index = record_pool_allocate(components);
	s_havok_component *component = &((s_havok_component *)components->data)[component_index & 0xffff];
	if (component)
	{
		component->initialize(object_index);
	}
	havok_object_count(object_index);
	return component_index;
}
/* the object, its header and definition as 0x1cf8b0 reads them */
struct s_havok_vehicle_definition
{
	byte unknown000[0x1ec];
	dword unknown1ec_0 : 19;
	dword unknown1ec_19 : 1;
	dword unknown1ec_20 : 12;
};

struct s_havok_vehicle
{
	long definition_index;
	byte unknown004[0x134 - 0x4];
	dword unknown134_0 : 1;
	dword unknown134_1 : 1;
	dword unknown134_2 : 30;
	byte unknown138[0x248 - 0x138];
	long unknown248;
};

struct s_havok_component_object_header
{
	short identifier;
	byte flags;
	byte type;
	byte unknown04[4];
	s_havok_vehicle *object;
};

struct s_havok_friction
{
	byte unknown00[8];
	real friction;
	long unknown0c;
};

struct s_havok_material
{
	byte unknown00[0x3c];
	real friction;
	long unknown40;
};

// @retail 0x1cf8b0
void havok_component_friction_get(long component_index, s_havok_friction *result, s_havok_material const *material)
{
	s_havok_component *component = &((s_havok_component *)g_51e9b8->data)[component_index & 0xffff];
	s_havok_component_object_header *header = &((s_havok_component_object_header *)g_4e0300->data)[component->object_index & 0xffff];
	real friction;

	if (((1 << header->type) & 2) && TEST_FIELD_BIT(((s_havok_vehicle_definition *)g_4e3b44[header->object->definition_index & 0xffff].bytes)->unknown1ec_19) &&
		(header->object->unknown248 != NONE || TEST_FIELD_BIT(header->object->unknown134_1)))
	{
		friction = 0.0f;
	}
	else
	{
		friction = material->friction;
	}
	result->friction = friction;
	result->unknown0c = material->unknown40;
}

void function_1d1260(s_havok_component *component);
void __stdcall function_1d01c0(s_havok_component *component);

// @retail 0x1cf9f0
s_havok_component::~s_havok_component()
{
	if (unknown04 & 0x20)
	{
		function_1d1260(this);
	}
	function_1d01c0(this);
	if (unknown94)
	{
		delete unknown94;
		unknown94 = NULL;
	}
}

// @retail 0x1cec80
void havok_components_dispose(void)
{
	s_record_pool_iterator iterator;

	iterator.data = g_51e9b8;
	iterator.index = NONE;
	iterator.datum_index = NONE;
	while (data_iterator_next_inlined(&iterator))
	{
		havok_component_get(iterator.datum_index)->~s_havok_component();
	}
	g_51e9b8->valid = false;
}

// @retail 0x1cf220
void havok_component_delete(long component_index)
{
	s_havok_component *component = havok_component_get(component_index);
	long object_index = component->object_index;
	s_havok_object *object;

	component->~s_havok_component();
	record_pool_release(g_51e9b8, component_index);
	object = havok_object_get(object_index);
	if (TEST_FIELD_BIT(object->havok_flag))
	{
		object->havok_flag = 0;
		(*g_51e9a0)--;
	}
}

// @retail 0x1cfac0
void havok_component_transform_set(s_havok_component *component, transform4x3f const *matrix)
{
	hkRigidBody *rigid_body = component->rigid_body;

	if (rigid_body)
	{
		hkTransform transform;
		hkVector4 translation;

		transform.m_rotation.m_col0.set(matrix->forward.i, matrix->forward.j, matrix->forward.k);
		transform.m_rotation.m_col1.set(matrix->left.i, matrix->left.j, matrix->left.k);
		transform.m_rotation.m_col2.set(matrix->up.i, matrix->up.j, matrix->up.k);
		translation.set(matrix->position.x, matrix->position.y, matrix->position.z);
		transform.m_translation = translation;
		rigid_body->setTransform(transform);
	}
}
/* the object as function_1cf120 reads it: its type, and what it passes to
   function_1c4b00 */
struct s_havok_component_owner
{
	byte unknown000[0x88];
	byte unknown088[0x94 - 0x88];
	byte unknown094[0xaa - 0x94];
	char type;
};

struct s_havok_component_owner_header
{
	short identifier;
	byte flag0 : 1;
	byte flags1 : 7;
	byte type;
	byte unknown04[4];
	s_havok_component_owner *object;
};

void function_1d56a0(s_havok_component *component);
void function_1d56f0(s_havok_component *component);
bool __stdcall function_1d5940(s_havok_component *component, long a, long b, long c);
void function_1d6b80(s_havok_component *component);
void function_1d6ca0(s_havok_component *component);
void function_1c4b00(long object_index, void *a, void *b, long c);

// @retail 0x1cf120
void function_1cf120(long component_index)
{
	s_havok_component *component = havok_component_get(component_index);
	s_havok_component_owner_header *header = &((s_havok_component_owner_header *)g_4e0300->data)[component->object_index & 0xffff];
	s_havok_component_owner *object = header->object;
	bool flag = TEST_FIELD_BIT(header->flag0);

	switch (object->type)
	{
	case 0:
		function_1d6b80(component);
		break;
	case 1:
		function_1d56f0(component);
		break;
	case 7:
		component->unknown04 |= 0x80;
		component->unknown1c = function_1d5940(component, 0, 1, 0);
		break;
	case 11:
		function_1d56a0(component);
		break;
	case 12:
		function_1d6ca0(component);
		break;
	default:
		__assume(0);
	}
	function_1c4b00(component->object_index, object->unknown088, object->unknown094, 0);
	component->unknown04 |= 1;
	if (flag)
	{
		component->unknown04 |= 0x20000;
	}
	else
	{
		component->unknown04 &= ~0x20000;
	}
}

/* the rigid bodies of a component (0x1d0870..0x1d1ca0) */

static inline void make_vector3f(vector3f *vector, real i, real j, real k)
{
	vector->i = i;
	vector->j = j;
	vector->k = k;
}

static inline void vector3d_from_havok(vector3f *vector, hkVector4 const *havok)
{
	make_vector3f(vector, (*havok)(0), (*havok)(1), (*havok)(2));
}

static inline void havok_from_vector3d(hkVector4 *havok, vector3f const *vector)
{
	havok->set(vector->i, vector->j, vector->k);
}

static inline void havok_rigid_body_activate(hkRigidBody *rigid_body)
{
	if (!rigid_body->isActive().m_bool && rigid_body->m_simulation_island)
	{
		rigid_body->activate();
	}
}

// @retail 0x1d08e0
void havok_component_rigid_body_matrix_get(long rigid_body_index, s_havok_component *component, transform4x3f *matrix)
{
	hkTransform transform;

	transform.set(havok_component_rigid_body_get(rigid_body_index, component)->m_motion->m_transform);

	if (TEST_FIELD_BIT(component->transformed))
	{
		transform.setMulEq(component->transform);
	}
	matrix->scale = 1.0f;
	vector3d_from_havok(&matrix->forward, &transform.m_rotation.m_col0);
	vector3d_from_havok(&matrix->left, &transform.m_rotation.m_col1);
	vector3d_from_havok(&matrix->up, &transform.m_rotation.m_col2);
	vector3d_from_havok((vector3f *)&matrix->position, &transform.m_translation);
}

// @retail 0x1d0870
void havok_component_rigid_body_position_get(long rigid_body_index, s_havok_component *component, point3f *position)
{
	if (TEST_FIELD_BIT(component->transformed))
	{
		transform4x3f matrix;

		havok_component_rigid_body_matrix_get(rigid_body_index, component, &matrix);
		*position = matrix.position;
	}
	else
	{
		vector3d_from_havok((vector3f *)position, &havok_component_rigid_body_get(rigid_body_index, component)->m_motion->m_transform.m_translation);
	}
}

/* retail inlines the velocity getters into this file's callers */
static inline void rigid_body_linear_velocity_get(long rigid_body_index, s_havok_component *component, vector3f *velocity)
{
	hkRigidBody *rigid_body = havok_component_rigid_body_get(rigid_body_index, component);

	if (!rigid_body->m_fixed)
	{
		vector3d_from_havok(velocity, &rigid_body->m_motion->m_linear_velocity);
	}
	else
	{
		*velocity = *g_4687a4;
	}
}

static inline void rigid_body_angular_velocity_get(long rigid_body_index, s_havok_component *component, vector3f *velocity)
{
	hkRigidBody *rigid_body = havok_component_rigid_body_get(rigid_body_index, component);

	if (!rigid_body->m_fixed)
	{
		vector3d_from_havok(velocity, &rigid_body->m_motion->m_angular_velocity);
	}
	else
	{
		*velocity = *g_4687a4;
	}
}

// @retail 0x1d09d0
void havok_component_rigid_body_linear_velocity_get(long rigid_body_index, s_havok_component *component, vector3f *velocity)
{
	rigid_body_linear_velocity_get(rigid_body_index, component, velocity);
}

// @retail 0x1d0ad0
void havok_component_rigid_body_angular_velocity_get(long rigid_body_index, s_havok_component *component, vector3f *velocity)
{
	rigid_body_angular_velocity_get(rigid_body_index, component, velocity);
}

// @retail 0x1d0a20
void havok_component_rigid_body_point_velocity_get(long rigid_body_index, s_havok_component *component, point3f const *point, vector3f *velocity)
{
	if (!havok_component_rigid_body_get(rigid_body_index, component)->m_fixed)
	{
		hkVector4 havok_point;
		hkVector4 havok_velocity;

		havok_from_vector3d(&havok_point, (vector3f const *)point);
		havok_component_rigid_body_get(rigid_body_index, component)->m_motion->getPointVelocity(havok_point, havok_velocity);
		vector3d_from_havok(velocity, &havok_velocity);
	}
	else
	{
		*velocity = *g_4687a4;
	}
}

// @retail 0x1d0b20
void havok_component_rigid_body_inertia_get(long rigid_body_index, s_havok_component *component, matrix3x3 *inertia)
{
	hkRotation havok_inertia;

	havok_component_rigid_body_get(rigid_body_index, component)->m_motion->getInertiaWorld(havok_inertia);
	vector3d_from_havok(&inertia->forward, &havok_inertia.m_col0);
	vector3d_from_havok(&inertia->left, &havok_inertia.m_col1);
	vector3d_from_havok(&inertia->up, &havok_inertia.m_col2);
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))

// @retail 0x1d0bb0
real havok_component_rigid_body_mass_get(long rigid_body_index, s_havok_component *component)
{
	return MAX(1.0f, havok_component_rigid_body_get(rigid_body_index, component)->m_motion->getMass());
}

// @retail 0x1d0c00
bool havok_component_rigid_body_keyframed(long rigid_body_index, s_havok_component *component)
{
	return havok_component_rigid_body_get(rigid_body_index, component)->m_motion->getType() == 6;
}

// @retail 0x1d0dd0
void havok_component_rigid_body_linear_velocity_set(long rigid_body_index, s_havok_component *component, vector3f const *velocity)
{
	hkRigidBody *rigid_body = havok_component_rigid_body_get(rigid_body_index, component);

	if (!rigid_body->m_fixed)
	{
		hkVector4 havok_velocity;

		havok_from_vector3d(&havok_velocity, velocity);
		havok_rigid_body_activate(rigid_body);
		rigid_body->m_motion->setLinearVelocity(havok_velocity);
	}
}

// @retail 0x1d0e50
void havok_component_rigid_body_angular_velocity_set(long rigid_body_index, s_havok_component *component, vector3f const *velocity)
{
	if (!TEST_FIELD_BIT(component->flag1) && !havok_component_rigid_body_get(rigid_body_index, component)->m_fixed)
	{
		hkVector4 havok_velocity;
		hkRigidBody *rigid_body;

		havok_from_vector3d(&havok_velocity, velocity);
		rigid_body = havok_component_rigid_body_get(rigid_body_index, component);
		havok_rigid_body_activate(rigid_body);
		rigid_body->m_motion->setAngularVelocity(havok_velocity);
	}
}

// @retail 0x1d10b0
void havok_component_rigid_body_linear_velocity_add(long rigid_body_index, s_havok_component *component, vector3f const *velocity)
{
	hkRigidBody *rigid_body = havok_component_rigid_body_get(rigid_body_index, component);
	hkVector4 impulse;
	real mass;

	havok_from_vector3d(&impulse, velocity);
	mass = rigid_body->m_motion->getMass();
	__m128 mass4 = _mm_set_ss(mass);
	impulse.m_quad = _mm_mul_ps(_mm_shuffle_ps(mass4, mass4, 0), impulse.m_quad);
	havok_rigid_body_activate(rigid_body);
	rigid_body->m_motion->applyLinearImpulse(impulse);
}

// @retail 0x1d1c10
bool havok_component_any_rigid_body_active(s_havok_component *component)
{
	bool result = false;
	long rigid_body_index;

	for (rigid_body_index = 0; rigid_body_index < component->rigid_bodies.size; rigid_body_index++)
	{
		if (havok_component_rigid_body_get(rigid_body_index, component)->isActive().m_bool)
		{
			result = true;
			break;
		}
	}
	return result;
}

// @retail 0x1d1c60
void havok_component_rigid_bodies_activate(s_havok_component *component)
{
	if (TEST_FIELD_BIT(component->flag5))
	{
		long rigid_body_index;

		for (rigid_body_index = 0; rigid_body_index < component->rigid_bodies.size; rigid_body_index++)
		{
			havok_component_rigid_body_get(rigid_body_index, component)->activate();
		}
	}
}

// @retail 0x1d1ca0
bool havok_component_main_rigid_body_movable(s_havok_component *component)
{
	long rigid_body_index = havok_component_main_rigid_body_index_get(component);
	bool result = false;

	if (rigid_body_index != NONE)
	{
		result = havok_component_rigid_body_get(rigid_body_index, component)->m_motion->getType() != 6 &&
			!havok_component_rigid_body_get(rigid_body_index, component)->m_fixed;
	}
	return result;
}

/* the motion's transform, unless the body is fixed in a world (an inline of
   hkRigidBody's, which retail inlines into 0x1d0cf0 once) */
// @retail 0x1d8e80
void hkRigidBody::motion_transform_set(hkTransform const &transform)
{
	havok_rigid_body_activate(this);
	if (!m_fixed || !m_world)
	{
		m_motion->setTransform(transform);
	}
}

void function_1d1260(s_havok_component *component);
void function_1d1540(s_havok_component *component);

// @retail 0x1d0cf0
void havok_component_rigid_body_transform_set(long rigid_body_index, s_havok_component *component, hkTransform const *transform)
{
	hkTransform local;

	local.set(*transform);
	if (TEST_FIELD_BIT(component->transformed))
	{
		hkTransform inverse;

		inverse.setInverse(component->transform);
		local.setMulEq(inverse);
	}
	if (havok_component_rigid_body_get(rigid_body_index, component)->m_fixed && TEST_FIELD_BIT(component->flag5))
	{
		function_1d1260(component);
		havok_component_rigid_body_get(rigid_body_index, component)->motion_transform_set(local);
		function_1d1540(component);
	}
	else
	{
		havok_component_rigid_body_get(rigid_body_index, component)->motion_transform_set(local);
	}
}

// @retail 0x1d0c20
void havok_component_rigid_body_matrix_set(long rigid_body_index, s_havok_component *component, transform4x3f const *matrix)
{
	hkTransform transform;
	hkVector4 translation;

	transform.m_rotation.m_col0.set(matrix->forward.i, matrix->forward.j, matrix->forward.k);
	transform.m_rotation.m_col1.set(matrix->left.i, matrix->left.j, matrix->left.k);
	transform.m_rotation.m_col2.set(matrix->up.i, matrix->up.j, matrix->up.k);
	translation.set(matrix->position.x, matrix->position.y, matrix->position.z);
	transform.m_translation = translation;
	havok_component_rigid_body_transform_set(rigid_body_index, component, &transform);
}

// @retail 0x1d1010
void havok_component_rigid_body_linear_velocity_change(long rigid_body_index, s_havok_component *component, vector3f const *change)
{
	vector3f velocity;

	rigid_body_linear_velocity_get(rigid_body_index, component, &velocity);
	velocity.i += change->i;
	velocity.j += change->j;
	velocity.k += change->k;
	havok_component_rigid_body_linear_velocity_set(rigid_body_index, component, &velocity);
}

// @retail 0x1d1160
void havok_component_rigid_body_point_impulse_apply(long rigid_body_index, s_havok_component *component, point3f const *point, vector3f const *velocity)
{
	if (TEST_FIELD_BIT(component->flag1))
	{
		havok_component_rigid_body_linear_velocity_change(rigid_body_index, component, velocity);
	}
	else
	{
		hkRigidBody *rigid_body = havok_component_rigid_body_get(rigid_body_index, component);
		hkVector4 havok_point;
		hkVector4 impulse;
		real mass;
		__m128 mass4;

		havok_from_vector3d(&havok_point, (vector3f const *)point);
		havok_from_vector3d(&impulse, velocity);
		mass = rigid_body->m_motion->getMass();
		mass4 = _mm_set_ss(mass);
		impulse.m_quad = _mm_mul_ps(_mm_shuffle_ps(mass4, mass4, 0), impulse.m_quad);
		rigid_body->m_motion->applyPointImpulse(impulse, havok_point);
	}
}

/* the state of an object's nodes the rigid bodies drive (0x1d1a20 reads it,
   0x1d1b60 applies it) */
#define MAXIMUM_HAVOK_NODES 64

struct s_havok_node_states
{
	dword valid[MAXIMUM_HAVOK_NODES / 32];
	transform4x3f matrices[MAXIMUM_HAVOK_NODES];
	vector3f linear_velocities[MAXIMUM_HAVOK_NODES];
	vector3f angular_velocities[MAXIMUM_HAVOK_NODES];
};

// @retail 0x1d1a20
void havok_component_node_states_get(s_havok_component *component, s_havok_node_states *states)
{
	long rigid_body_index;

	memset(states->valid, 0, sizeof(states->valid));
	for (rigid_body_index = 0; rigid_body_index < component->rigid_bodies.size; rigid_body_index++)
	{
		s_havok_component_rigid_body *rigid_body = &component->rigid_bodies.data[rigid_body_index];

		if (rigid_body->node_count > 0)
		{
			long node_index = rigid_body->nodes[0];

			if (node_index >= 0 && node_index < MAXIMUM_HAVOK_NODES)
			{
				states->valid[node_index >> 5] |= 1 << (node_index & 0x1f);
				havok_component_rigid_body_matrix_get(rigid_body_index, component, &states->matrices[node_index]);
				rigid_body_linear_velocity_get(rigid_body_index, component, &states->linear_velocities[node_index]);
				rigid_body_angular_velocity_get(rigid_body_index, component, &states->angular_velocities[node_index]);
			}
		}
	}
}

// @retail 0x1d1b60
void havok_component_node_states_set(s_havok_component *component, s_havok_node_states const *states)
{
	long rigid_body_index;

	for (rigid_body_index = 0; rigid_body_index < component->rigid_bodies.size; rigid_body_index++)
	{
		s_havok_component_rigid_body *rigid_body = &component->rigid_bodies.data[rigid_body_index];

		if (rigid_body->node_count > 0)
		{
			long node_index = rigid_body->nodes[0];

			if (node_index >= 0 && node_index < MAXIMUM_HAVOK_NODES &&
				(states->valid[node_index >> 5] & (1 << (node_index & 0x1f))))
			{
				havok_component_rigid_body_matrix_set(rigid_body_index, component, &states->matrices[node_index]);
				havok_component_rigid_body_linear_velocity_set(rigid_body_index, component, &states->linear_velocities[node_index]);
				havok_component_rigid_body_angular_velocity_set(rigid_body_index, component, &states->angular_velocities[node_index]);
			}
		}
	}
}

// @retail 0x1cefb0
void havok_component_rigid_body_state_update(long rigid_body_index, s_havok_component *component)
{
	s_havok_component_rigid_body *rigid_body = &component->rigid_bodies.data[rigid_body_index];
	point3f position;
	vector3f linear_velocity;
	vector3f angular_velocity;

	havok_component_rigid_body_position_get(rigid_body_index, component, &position);
	rigid_body_linear_velocity_get(rigid_body_index, component, &linear_velocity);
	rigid_body_angular_velocity_get(rigid_body_index, component, &angular_velocity);
	rigid_body->position = position;
	rigid_body->linear_velocity = linear_velocity;
	rigid_body->angular_velocity = angular_velocity;
}

void __stdcall function_1c3770(long object_index, dword flags);

/* puts the component's object back in the motion state its flags ask for */
// @retail 0x1d2460
void function_1d2460(s_havok_component *component)
{
	dword flags = component->unknown04;

	if (flags & 0x104000)
	{
		if (!(flags & 0x2000))
		{
			if ((flags & 0x80000) && !(flags & 0x100000))
			{
				function_1c3770(component->object_index, 0);
			}
		}
		else if (flags & 0x100000)
		{
			function_1c3770(component->object_index, 0x2000);
		}
	}
}
