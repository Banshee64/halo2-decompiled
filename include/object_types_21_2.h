#pragma once

/* OBJECT_TYPES_21_2.H: the item, projectile, weapon and device object types
   (vtables 0x451f68, 0x4521b4, 0x4524d8 and 0x452848), and the small event
   definition classes (projectile, weapon and game engine events) that share
   those retail tables. The object types derive from the shared base in
   object_type_definitions.h; an event definition has its own 12-slot vtable.
   Slots this batch does not decompile are placeholders. */

#include "cseries.h"
#include "object_type_definitions.h"

/* the item type (vtable 0x451f68) */
class c_item_type : public c_object_type_definition
{
public:
	virtual const char *v1();
	virtual long v2();
	virtual long v3();
	virtual long v5();
	virtual void v9(long a, long b, long *size);
	virtual void v26(long index, long b, s_entity_state *state);
};

/* the projectile type (vtable part of 0x4521b4, slots 23..58) */
class c_projectile_type : public c_object_type_definition
{
public:
	virtual long v0();
	virtual const char *v1();
	virtual long v3();
	virtual void v9(long a, long b, long *size);
	virtual void v21(s_entity *entity);
	virtual void v26(long index, long b, s_entity_state *state);
	virtual bool v30(long index);
};

/* the weapon type (vtable part of 0x4524d8, slots 36..71) */
class c_weapon_type : public c_object_type_definition
{
public:
	virtual const char *v1();
	virtual long v2();
	virtual long v5();
	virtual void v26(long index, long b, s_entity_state *state);
};

/* the device type (vtable 0x452848) */
class c_device_type : public c_object_type_definition
{
public:
	virtual const char *v1();
	virtual long v2();
	virtual long v5();
	virtual bool v34(long a, s_entity_data *source, long *block);
};

/* an event definition: 12 slots, the type id, the name, a number, three
   shared slots, the size of the event's data, and others */
class c_event_definition
{
public:
	virtual long v0() { return 0; }
	virtual const char *v1() { return 0; }
	virtual long v2() { return 0; }
	virtual void v3() {}
	virtual void v4() {}
	virtual void v5() {}
	virtual void v6(void *a, long b, long *size) {}
	virtual real v7(long a, long b, long c) { return 0.0f; }
	virtual void v8() {}
	virtual void v9() {}
	virtual void v10() {}
	virtual void v11() {}
};

class c_projectile_impact_effect_event : public c_event_definition
{
public:
	virtual const char *v1();
	virtual void v6(void *a, long b, long *size);
};

class c_projectile_effect_event : public c_event_definition
{
public:
	virtual const char *v1();
	virtual long v2();
	virtual void v6(void *a, long b, long *size);
};

class c_projectile_object_impact_effect_event : public c_event_definition
{
public:
	virtual const char *v1();
	virtual long v2();
	virtual void v6(void *a, long b, long *size);
};

class c_projectile_attached_event : public c_event_definition
{
public:
	virtual const char *v1();
	virtual void v6(void *a, long b, long *size);
};

class c_weapon_put_away_event : public c_event_definition
{
public:
	virtual const char *v1();
	virtual void v6(void *a, long b, long *size);
};

class c_weapon_fire_event : public c_event_definition
{
public:
	virtual const char *v1();
	virtual long v2();
	virtual void v6(void *a, long b, long *size);
};

class c_weapon_pickup_event : public c_event_definition
{
public:
	virtual const char *v1();
	virtual void v6(void *a, long b, long *size);
};

class c_weapon_effect_event : public c_event_definition
{
public:
	virtual const char *v1();
	virtual void v6(void *a, long b, long *size);
};

class c_weapon_drop_event : public c_event_definition
{
public:
	virtual long v0();
	virtual const char *v1();
};

class c_weapon_reload_event : public c_event_definition
{
public:
	virtual const char *v1();
};

class c_game_engine_request_boot_player_event : public c_event_definition
{
public:
	virtual long v0();
	virtual const char *v1();
	virtual void v6(void *a, long b, long *size);
	virtual real v7(long a, long b, long c);
};

/* the game engine event: its own slot 5 and 6 differ from the base event */
class c_game_engine_event
{
public:
	virtual long v0();
	virtual const char *v1();
	virtual long v2() { return 0; }
	virtual void v3() {}
	virtual void v4() {}
	virtual bool v5(struct s_event_holder *a, struct s_event_mask *b);
	virtual void v6(void *a, long b, long *size);
	virtual void v7() {}
	virtual void v8() {}
	virtual void v9() {}
	virtual void v10() {}
	virtual void v11() {}
};
