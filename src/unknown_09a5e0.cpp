// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_09A5E0.CPP: the unit object type, the "game-engine-player" and
   "breakable-surface-group" entity definitions, and the simulation event
   definitions of vtables 0x4514e8, 0x4517a8, 0x4519f8 and 0x451ae0 */

#include "cseries.h"
#include "globals.h"
#include "object_type_definitions.h"
#include "object_types_21_1.h"
#include <math.h>

#define OBJECT_HEADER(index) (&((s_object_header *)g_4e0300->headers)[(index) & 0xFFFF])
#define OBJECT(index) (OBJECT_HEADER(index)->object)
#define UNIT_OBJECT(index) ((s_unit_object_view *)OBJECT(index))

/* the unit object, as this code sees it */
struct s_unit_object_view
{
	long definition_index;
	byte unknown04[0x16];
	short field1a;
	byte unknown1c[8];
	long field24;
	byte unknown28[0xaf - 0x28];
	byte field_af;
	byte unknownb0[0x138 - 0xb0];
	short field138;
	byte unknown13a[2];
	long field13c;
	byte unknown140[0x2a0 - 0x140];
	long field2a0;
	short field2a4;
	byte unknown2a6[2];
	long field2a8;
	long field2ac;
};

/* g_4e8c24: a table whose elements (0x21c bytes) are at +0x44 */
struct s_element_table
{
	byte unknown00[0x44];
	byte *elements;
};

/* the manager the entity definitions call: 50 virtual methods, the ones used
   here are 47, 48 and 49 */
class c_slot_manager
{
public:
	virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
	virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
	virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11();
	virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
	virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
	virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
	virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
	virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
	virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
	virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
	virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
	virtual void v44(); virtual void v45(); virtual void v46();
	virtual void v47(long a, long b, long c);
	virtual void v48(long a, long b, long c, long d);
	virtual bool v49(long a, long b, long c, long d);
};

/* g_4e9ae8: the slot table (16 identifiers at +0x2c) and, at +0xc14, which
   manager is current */
struct s_slot_globals
{
	byte unknown00[0x2c];
	long slots[16];
	byte unknown6c[0xc14 - 0x6c];
	long manager_index;
};

s_slot_globals *g_4e9ae8;
c_slot_manager *g_55e4d0[1];
extern long g_4eca60[8];
s_element_table *g_4e8c24;

/* the identifier in a slot, or NONE when there is no manager */
static long slot_identifier(short index)
{
	long id = NONE;

	if (g_55e4d0[g_4e9ae8->manager_index])
		id = g_4e9ae8->slots[index];
	return id;
}

/* the identifier in a slot, or NONE when there is no manager */
static long slot_of(c_slot_manager *manager, short index)
{
	long id = NONE;

	if (manager)
		id = g_4e9ae8->slots[index];
	return id;
}

static bool slot_matches(c_slot_manager *manager, short index, s_entity_slot *entity)
{
	bool result = false;

	if (manager)
	{
		long id = g_4e9ae8->slots[index];

		if (id != NONE && ((entity->id ^ id) & 0x3ff) == 0)
			result = true;
	}
	return result;
}

static void long4_clear(s_long4 *value)
{
	value->a = 0;
	value->b = 0;
	value->c = 0;
	value->d = 0;
}

static void unit_state_clear(s_unit_entity_state *state)
{
	state->field0 = 0;
	state->field4 = 0;
	*(long *)&state->field8 = 0;
	state->fieldc = 0;
	long4_clear(&state->field10);
	*(long *)&state->field20 = 0;
}

// ---- the unit object type ----

// @retail 0x9d200
long c_unit_type::v0()
{
	return 9;
}

// @retail 0x9d210
const char *c_unit_type::v1()
{
	return "unit";
}

// @retail 0x9d220
long c_unit_type::v2()
{
	return 0xf8;
}

// @retail 0x9d230
long c_unit_type::v3()
{
	return 0x24;
}

// @retail 0x9b940
long c_unit_type::v4()
{
	return 0x18;
}

// @retail 0x9d240
long c_unit_type::v5()
{
	return 0x5dcc3f;
}

// @retail 0x9dcf0
void c_unit_type::v9(long a, long b, long *size)
{
	*size = 0x91;
}

// @retail 0x9dea0
void c_unit_type::v21(s_entity *entity)
{
	s_unit_entity *unit = (s_unit_entity *)entity;
	s_unit_entity_data *data = unit->data;

	if (unit->object_index != NONE)
		*(long *)((byte *)OBJECT(unit->object_index) + 0xd4) = unit->field0;
	if (data->identifier != NONE)
	{
		long salt = (unsigned long)data->identifier >> 28;
		salt = (salt + 1) % 16;
		data->identifier = ((byte)salt << 28) | (data->identifier & 0x3ff);
	}
}

// @retail 0x9d280
void c_unit_type::v26(long index, long b, s_entity_state *state)
{
	s_unit_entity_state *unit_state = (s_unit_entity_state *)state;
	s_unit_object_view *first = UNIT_OBJECT(index);
	s_unit_object_view *object;

	unit_state_clear(unit_state);

	object = UNIT_OBJECT(index);
	unit_state->field4 = object->definition_index;
	unit_state->field0 = object->field1a;
	unit_state->field8 = object->field_af;
	unit_state->fieldc = object->field24;

	long4_clear(&unit_state->field10);
	if (first->field13c != NONE)
	{
		s_long4 *element = (s_long4 *)(g_4e8c24->elements + (first->field13c & 0xffff) * 0x21c + 0x84);

		unit_state->field10 = *element;
	}
	unit_state->field20 = first->field138;
}

// @retail 0x9d250
bool c_unit_type::v28(long index)
{
	return !TEST_FIELD_BIT(OBJECT(index)->flag2);
}

// @retail 0x9dc90
bool c_unit_type::v32(long index)
{
	s_unit_object_view *object = UNIT_OBJECT(index);

	object->field2a0 = NONE;
	object->field2a4 = -1;
	object->field2a8 = NONE;
	object->field2ac = NONE;
	return true;
}

// ---- the "game-engine-player" entity definition ----

// @retail 0x9ac70
long c_game_engine_player_entity_definition::v0()
{
	return 6;
}

// @retail 0x9ac80
const char *c_game_engine_player_entity_definition::v1()
{
	return "game-engine-player";
}

// @retail 0x9beb0
long c_game_engine_player_entity_definition::v2()
{
	return 0x38;
}

// @retail 0x9bec0
long c_game_engine_player_entity_definition::v3()
{
	return 2;
}

// @retail 0x9ac90
long c_game_engine_player_entity_definition::v4()
{
	return 0xb;
}

// @retail 0x9edd0
void c_game_engine_player_entity_definition::v9(long a, long b, long *size)
{
	*size = 5;
}

// @retail 0x9ad10
void c_game_engine_player_entity_definition::v11(long a, long b, long *size)
{
	*size = 0xb;
}

// @retail 0x9b8e0
bool c_game_engine_player_entity_definition::v16(s_float_holder *a, s_float_holder *b, long c)
{
	bool result = false;

	if (fabs(b->value - a->value) < 3.0518044e-5f)
		result = true;
	b->value = 0.0f;
	a->value = 0.0f;
	return result;
}

// @retail 0x9ad80
void c_game_engine_player_entity_definition::v18(s_entity_slot *entity, long b, short *slot)
{
	long index;

	for (index = 0; index < 16; index++)
	{
		if (slot_identifier(index) == entity->id)
			break;
	}
	*slot = (short)index;
}

// @retail 0x9afb0
bool c_game_engine_player_entity_definition::v19(long a, short *b, long c, long d)
{
	g_55e4d0[g_4e9ae8->manager_index]->v47(*b, c, d);
	return true;
}

// @retail 0x9aff0
bool c_game_engine_player_entity_definition::v20(s_entity_slot *entity, long b, long c, long d)
{
	bool result = false;
	long id = entity->id;
	c_slot_manager *manager = g_55e4d0[g_4e9ae8->manager_index];
	long index;

	for (index = 0; index < 16; index++)
	{
		if (slot_of(manager, index) == id)
			break;
	}
	if (index != 16)
	{
		manager->v48(index, b, c, d);
		result = true;
	}
	return result;
}

// @retail 0x9b0a0
void c_game_engine_player_entity_definition::v21(s_entity_slot *entity)
{
	c_slot_manager *manager = g_55e4d0[g_4e9ae8->manager_index];
	long index;

	for (index = 0; index < 16; index++)
	{
		long id = slot_of(manager, index);

		if (id != NONE && ((entity->id ^ id) & 0x3ff) == 0)
			break;
	}
	if (index != 16 && manager)
	{
		long *slot = &g_4e9ae8->slots[(short)index];

		if (*slot != NONE)
		{
			long id = entity->id;

			if (((id ^ *slot) & 0x3ff) == 0)
				*slot = id;
		}
	}
}

// @retail 0x9aeb0
bool c_game_engine_player_entity_definition::v22(s_entity_slot *entity, long b, short *slot, long d, long e, long f)
{
	short index = *slot;
	c_slot_manager *manager = g_55e4d0[g_4e9ae8->manager_index];

	if (manager)
	{
		if (g_4e9ae8->slots[index] != NONE)
			g_4e9ae8->slots[index] = NONE;
	}
	g_4e9ae8->slots[index] = entity->id;
	return true;
}

// @retail 0x9b180
bool c_game_engine_player_entity_definition::v23(s_entity_slot *entity, long b, long c, long d)
{
	bool result = false;
	long id = entity->id;
	c_slot_manager *manager = g_55e4d0[g_4e9ae8->manager_index];
	long index;

	for (index = 0; index < 16; index++)
	{
		if (slot_of(manager, index) == id)
			break;
	}
	if (index >= 0 && index < 16)
	{
		if (manager->v49(index, b, c, d))
			result = true;
	}
	return result;
}

// @retail 0x9af00
bool c_game_engine_player_entity_definition::v24(s_entity_slot *entity)
{
	bool result = false;
	c_slot_manager *manager = g_55e4d0[g_4e9ae8->manager_index];
	long index;

	for (index = 0; index < 16; index++)
	{
		if (slot_of(manager, index) == entity->id)
			break;
	}
	if (index != 16 && entity->id == slot_of(manager, index))
	{
		g_4e9ae8->slots[(short)index] = NONE;
		result = true;
	}
	return result;
}

// ---- the "breakable-surface-group" entity definition ----

// @retail 0x9cd70
long c_breakable_surface_group_entity_definition::v0()
{
	return 8;
}

// @retail 0x9cd80
const char *c_breakable_surface_group_entity_definition::v1()
{
	return "breakable-surface-group";
}

// @retail 0x9cd90
long c_breakable_surface_group_entity_definition::v2()
{
	return 4;
}

// @retail 0x9cda0
void c_breakable_surface_group_entity_definition::v9(long a, long b, long *size)
{
	*size = 0x10;
}

// @retail 0x9ce20
void c_breakable_surface_group_entity_definition::v11(long a, long b, long *size)
{
	*size = 0x21;
}

// @retail 0x9cfd0
void c_breakable_surface_group_entity_definition::v18(s_entity_slot *entity, long b, short *slot)
{
	*slot = (short)entity->slot;
}

// @retail 0x9cff0
bool c_breakable_surface_group_entity_definition::v19(long a, long b, long c, long *d)
{
	*d = 0;
	return true;
}

// @retail 0x9d0a0
void c_breakable_surface_group_entity_definition::v21(s_entity_slot *entity)
{
	long slot = entity->slot;

	if (slot >= 0 && slot < 8)
	{
		long id = g_4eca60[slot];

		if (id != NONE && ((entity->id ^ id) & 0x3ff) == 0)
			g_4eca60[slot] = entity->id;
	}
}

// @retail 0x9d0e0
bool c_breakable_surface_group_entity_definition::v22(s_entity_slot *entity, long b, short *slot, long d, long e, long f)
{
	entity->slot = *slot;
	if (d)
		v23(entity, d, e, f);
	return true;
}

// @retail 0x9d1b0
bool c_breakable_surface_group_entity_definition::v24(s_entity_slot *entity)
{
	entity->slot = NONE;
	return true;
}

// @retail 0x9d1c0
bool c_breakable_surface_group_entity_definition::v25(s_entity_slot *entity)
{
	long slot = entity->slot;

	if (slot >= 0 && slot < 8)
	{
		if (g_4eca60[slot] != NONE)
		{
			g_4eca60[slot] = NONE;
		}
		long id = entity->id;
		g_4eca60[slot] = id;
	}
	return true;
}

// ---- the event definitions ----

// @retail 0x9f9e0
const char *c_unit_melee_initiate_event_definition::v1()
{
	return "unit-melee-initiate";
}

// @retail 0x9f9f0
void c_unit_melee_initiate_event_definition::v6(long a, long b, long *size)
{
	*size = 2;
}

// @retail 0x9fe20
const char *c_unit_pickup_event_definition::v1()
{
	return "unit-pickup";
}

// @retail 0x9fe40
void c_unit_pickup_event_definition::v6(s_event_section_data *a, long b, long *size)
{
	long result = 12;

	if (*a->kind == 1)
		result = 0x1c;
	*size = result;
}

// @retail 0x9a5e0
long c_unit_grenade_release_event_definition::v0()
{
	return 0x16;
}

// @retail 0x9f330
const char *c_unit_grenade_release_event_definition::v1()
{
	return "unit-grenade-release";
}

// @retail 0x9f340
long c_unit_grenade_release_event_definition::v2()
{
	return 0x1c;
}

// @retail 0x9f350
void c_unit_grenade_release_event_definition::v6(long a, long b, long *size)
{
	*size = 0x42;
}

// @retail 0x9fcf0
const char *c_vehicle_trick_event_definition::v1()
{
	return "vehicle-trick";
}

// @retail 0x9b950
void c_vehicle_trick_event_definition::v6(long a, long b, long *size)
{
	*size = 0;
}

// @retail 0x9fb90
long c_vehicle_flip_event_definition::v0()
{
	return 0x13;
}

// @retail 0x9fba0
const char *c_vehicle_flip_event_definition::v1()
{
	return "vehicle-flip";
}

// @retail 0x9f180
long c_unit_grenade_initiate_event_definition::v0()
{
	return 0xe;
}

// @retail 0x9f190
const char *c_unit_grenade_initiate_event_definition::v1()
{
	return "unit-grenade-initiate";
}

// @retail 0x9f1a0
void c_unit_grenade_initiate_event_definition::v6(long a, long b, long *size)
{
	*size = 1;
}

// @retail 0x9eef0
const char *c_unit_board_vehicle_event_definition::v1()
{
	return "unit-board-vehicle";
}

// @retail 0x9ef00
void c_unit_board_vehicle_event_definition::v6(long a, long b, long *size)
{
	*size = 0x20;
}

// @retail 0x9edc0
const char *c_unit_exit_vehicle_event_definition::v1()
{
	return "unit-exit-vehicle";
}

// @retail 0x9f550
long c_unit_melee_damage_event_definition::v0()
{
	return 0x17;
}

// @retail 0x9f560
const char *c_unit_melee_damage_event_definition::v1()
{
	return "unit-melee-damage";
}

// @retail 0x9f570
void c_unit_melee_damage_event_definition::v6(long a, long b, long *size)
{
	*size = 0x2a;
}

// @retail 0x9ebe0
const char *c_unit_enter_vehicle_event_definition::v1()
{
	return "unit-enter-vehicle";
}

// @retail 0x9ca40
const char *c_breakable_surface_damage_event_definition::v1()
{
	return "breakable-surface-damage";
}

// @retail 0x9ca50
long c_breakable_surface_damage_event_definition::v2()
{
	return 0x34;
}

// @retail 0x9ca60
void c_breakable_surface_damage_event_definition::v6(long a, long b, long *size)
{
	*size = 0xa0;
}

// @retail 0xa51e0
real c_breakable_surface_damage_event_definition::v7(long a, long b, long c)
{
	return g_45dbd8;
}

// @retail 0x9c7d0
const char *c_damage_section_response_event_definition::v1()
{
	return "damage-section-response";
}

// @retail 0x9c7e0
void c_damage_section_response_event_definition::v6(long a, long b, long *size)
{
	*size = 9;
}

// @retail 0x9bea0
const char *c_damage_aftermath_event_definition::v1()
{
	return "damage-aftermath";
}

// @retail 0x9bee0
void c_damage_aftermath_event_definition::v6(long a, long b, long *size)
{
	*size = 0x49;
}
