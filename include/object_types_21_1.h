#pragma once

/* OBJECT_TYPES_21_1.H: the unit object type, the simulation entity
   definitions "game-engine-player" and "breakable-surface-group", and the
   simulation event definitions whose vtables sit at 0x4514e8, 0x4517a8,
   0x4519f8 and 0x451ae0.

   The entity definitions have 27 slots (a45d0, 2bcc80 and a5750 at slots
   6..8); the event definitions have 12 (9bed0, a5750 and a size method at
   slots 4..6). The two kinds are separate hierarchies from the object types
   of object_type_definitions.h. Slots that no decompiled function owns keep
   empty placeholder bodies. */

#include "cseries.h"
#include "object_type_definitions.h"

/* an entity as the entity definitions see it: the identifier, and the slot
   the entity holds in the shared tables */
struct s_entity_slot
{
	long id;
	byte unknown04[4];
	long slot;
};

/* something with a real at +0x14 */
struct s_float_holder
{
	byte unknown00[0x14];
	real value;
};

/* the entity of the unit object type: the object index, and at +0x1c the
   unit data whose identifier at +0x94 carries a 10-bit index and a salt */
struct s_unit_entity_data
{
	byte unknown00[0x94];
	long identifier;
};

struct s_unit_entity
{
	long field0;
	byte unknown04[4];
	long object_index;
	byte unknown0c[0x10];
	s_unit_entity_data *data;
};

struct s_long4
{
	long a;
	long b;
	long c;
	long d;
};

/* the state of a unit (0x24 bytes) */
struct s_unit_entity_state
{
	long field0;
	long field4;
	byte field8;
	byte unknown09[3];
	long fieldc;
	s_long4 field10;
	short field20;
};

/* the unit object type (vtable 0x451ae0 + 14*4) */
class c_unit_type : public c_object_type_definition
{
public:
	virtual long v0();
	virtual const char *v1();
	virtual long v2();
	virtual long v3();
	virtual long v4();
	virtual long v5();
	virtual void v9(long a, long b, long *size);
	virtual void v21(s_entity *entity);
	virtual void v26(long index, long b, s_entity_state *state);
	virtual bool v28(long index);
	virtual bool v32(long a);
};

/* the "game-engine-player" entity definition (vtable 0x4514e8) */
class c_game_engine_player_entity_definition
{
public:
	virtual long v0();
	virtual const char *v1();
	virtual long v2();
	virtual long v3();
	virtual long v4();
	virtual long v5() { return 0; }
	virtual bool v6(long a) { return false; }
	virtual bool v7(long a) { return false; }
	virtual bool v8(long a, long b) { return false; }
	virtual void v9(long a, long b, long *size);
	virtual void v10(long a, long b, long c, long d) {}
	virtual void v11(long a, long b, long *size);
	virtual void v12(long a) {}
	virtual void v13(long a) {}
	virtual void v14(long a) {}
	virtual void v15(long a) {}
	virtual bool v16(s_float_holder *a, s_float_holder *b, long c);
	virtual bool v17(long a, long b, long c) { return false; }
	virtual void v18(s_entity_slot *entity, long b, short *slot);
	virtual bool v19(long a, short *b, long c, long d);
	virtual bool v20(s_entity_slot *entity, long b, long c, long d);
	virtual void v21(s_entity_slot *entity);
	virtual bool v22(s_entity_slot *entity, long b, short *slot, long d, long e, long f);
	virtual bool v23(s_entity_slot *entity, long b, long c, long d);
	virtual bool v24(s_entity_slot *entity);
	virtual bool v25(long a) { return false; }
	virtual bool v26(long a) { return false; }
};

/* the "breakable-surface-group" entity definition (vtable 0x4517a8) */
class c_breakable_surface_group_entity_definition
{
public:
	virtual long v0();
	virtual const char *v1();
	virtual long v2();
	virtual long v3() { return 0; }
	virtual long v4() { return 0; }
	virtual long v5() { return 0; }
	virtual bool v6(long a) { return false; }
	virtual bool v7(long a) { return false; }
	virtual bool v8(long a, long b) { return false; }
	virtual void v9(long a, long b, long *size);
	virtual void v10(long a, long b, long c, long d) {}
	virtual void v11(long a, long b, long *size);
	virtual void v12(long a) {}
	virtual void v13(long a) {}
	virtual void v14(long a) {}
	virtual void v15(long a) {}
	virtual bool v16(long a, long b, long c) { return false; }
	virtual bool v17(long a, long b, long c) { return false; }
	virtual void v18(s_entity_slot *entity, long b, short *slot);
	virtual bool v19(long a, long b, long c, long *d);
	virtual bool v20(long a, long b, long c, long d) { return false; }
	virtual void v21(s_entity_slot *entity);
	virtual bool v22(s_entity_slot *entity, long b, short *slot, long d, long e, long f);
	virtual bool v23(s_entity_slot *entity, long b, long c, long d) { return false; }
	virtual bool v24(s_entity_slot *entity);
	virtual bool v25(s_entity_slot *entity);
	virtual bool v26(long a) { return false; }
};

/* the size and shape of a "damage section response" event's data */
struct s_event_section_data
{
	byte unknown00[0x18];
	word *kind;
};

/* the event definition base: 12 slots */
class c_event_definition
{
public:
	virtual long v0() { return 0; }
	virtual const char *v1() { return 0; }
	virtual long v2() { return 0; }
	virtual long v3() { return 0; }
	virtual bool v4() { return true; }
	virtual bool v5(long a, long b) { return false; }
	virtual void v6(long a, long b, long *size) {}
	virtual real v7(long a, long b, long c) { return 1.0f; }
	virtual void v8() {}
	virtual void v9() {}
	virtual void v10() {}
	virtual void v11() {}
};

class c_unit_melee_initiate_event_definition : public c_event_definition
{
public:
	virtual const char *v1();
	virtual void v6(long a, long b, long *size);
};

class c_unit_pickup_event_definition : public c_event_definition
{
public:
	virtual const char *v1();
	virtual void v6(s_event_section_data *a, long b, long *size);
};

class c_unit_grenade_release_event_definition : public c_event_definition
{
public:
	virtual long v0();
	virtual const char *v1();
	virtual long v2();
	virtual void v6(long a, long b, long *size);
};

class c_vehicle_trick_event_definition : public c_event_definition
{
public:
	virtual const char *v1();
	virtual void v6(long a, long b, long *size);
};

class c_vehicle_flip_event_definition : public c_event_definition
{
public:
	virtual long v0();
	virtual const char *v1();
};

class c_unit_grenade_initiate_event_definition : public c_event_definition
{
public:
	virtual long v0();
	virtual const char *v1();
	virtual void v6(long a, long b, long *size);
};

class c_unit_board_vehicle_event_definition : public c_event_definition
{
public:
	virtual const char *v1();
	virtual void v6(long a, long b, long *size);
};

class c_unit_exit_vehicle_event_definition : public c_event_definition
{
public:
	virtual const char *v1();
};

class c_unit_melee_damage_event_definition : public c_event_definition
{
public:
	virtual long v0();
	virtual const char *v1();
	virtual void v6(long a, long b, long *size);
};

class c_unit_enter_vehicle_event_definition : public c_event_definition
{
public:
	virtual const char *v1();
};

class c_breakable_surface_damage_event_definition : public c_event_definition
{
public:
	virtual const char *v1();
	virtual long v2();
	virtual void v6(long a, long b, long *size);
	virtual real v7(long a, long b, long c);
};

class c_damage_section_response_event_definition : public c_event_definition
{
public:
	virtual const char *v1();
	virtual void v6(long a, long b, long *size);
};

class c_damage_aftermath_event_definition : public c_event_definition
{
public:
	virtual const char *v1();
	virtual void v6(long a, long b, long *size);
};
