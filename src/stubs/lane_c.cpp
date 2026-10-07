// stubs for lane C (0x1c0000..0x1cffff): callees outside the region that are
// not decompiled yet, and the library (Havok) functions the region calls
#include "unknown_11c920.h"
#include "unknown_1cec30.h"

class c_library_30c470
{
public:
	void detach(void *callback);
};

// @stub 0x30c470
void c_library_30c470::detach(void *callback) { }

class c_278da0_kind;
class c_library_311690
{
public:
	void update(long value);
	byte unknown00[0x54];
	c_278da0_kind *kind;
};

// @stub 0x311690
void c_library_311690::update(long value) { }

class c_278b60_filter;
struct s_278b60_dispatch;
struct s_278b60_filter;
class c_library_30c190
{
public:
	c_278b60_filter *filter();
	byte unknown00[0xcc];
	s_278b60_dispatch *dispatch;
	s_278b60_filter *filter_object;
};

// @stub 0x30c190
c_278b60_filter *c_library_30c190::filter() { return NULL; }
#include "slot_owner.h"
#include "unknown_1c62f0.h"
#include "unknown_0259a0.h"
#include "unknown_1765e0.h"

// @stub 0x2d91d0
void __cdecl function_2d91d0(void *array, long element_size) { }

// @stub 0x30c170
byte *__fastcall function_30c170(hkWorld *world) { return 0; }

// @stub 0x3123a0
hkPropertyValue hkEntity::removeProperty(dword key) { return hkPropertyValue(0); }

// @stub 0x312530
void hkEntity::addProperty(dword key, hkPropertyValue value) { }

/* slot handler callbacks of the region not decompiled yet (their handler
   structs hold their addresses) */

/* game functions outside the region called by the ai lifecycle callbacks */



// @stub 0x292130
void function_292130(void) { }


// @stub 0x292e00
void function_292e00(void) { }

// @stub 0x292f60
void function_292f60(void) { }
/* the animation graph lookups (0x1d9000..0x1de000) */


/* the physics callees of the havok components */
struct s_havok_component;

// @stub 0x1d1260
void function_1d1260(s_havok_component *component) { }

// @stub 0x3126f0
void hkRigidBody::setTransform(hkTransform const &transform) { }
/* callees of the slot handler callbacks (unknown_0259a0.h) */


// @stub 0x1697c0
bool __stdcall function_1697c0(long flags, point3f const *point, vector3f const *vector,
	long ignore_object_index, long ignore_unit_index, s_collision_result_1697c0 *result) { return false; }


// @stub 0x26d100
long function_26d100(vector3f const *up, s_collision_result_1697c0 *collision, long *unknown, point3f const *point) { return 0; }

/* callees of the physics code (unknown_1c25a0.cpp, unknown_1cec30.cpp) */

// @stub 0x30bd50
void hkEntityApi::removeEntityListener(hkEntityListener *listener) { }

// @stub 0x30f800
hkBool hkWorld::removeEntity(hkEntity *entity) { return hkBool(); }

// @stub 0x278f00
void function_278f00(void) { }

// @stub 0x1d1540
void function_1d1540(s_havok_component *component) { }

// @stub 0x1d56a0
void function_1d56a0(s_havok_component *component) { }

// @stub 0x1d56f0
void function_1d56f0(s_havok_component *component) { }

// @stub 0x1d5940
bool __stdcall function_1d5940(s_havok_component *component, long a, long b, long c) { return false; }

// @stub 0x1d6b80
void function_1d6b80(s_havok_component *component) { }

// @stub 0x1d6ca0
void function_1d6ca0(s_havok_component *component) { }

/* in the region, not decompiled yet */


// @stub 0x30f2d0
void hkWorld::addEntity(hkEntity *entity) { }

// @stub 0x30cc60
void hkWorld::removeSimulationIsland(hkSimulationIsland *island) { }

// @stub 0x30bc90
void hkEntityApi::activate(void) { }



/* the rigid body accessors of the havok components (unknown_1cec30.cpp) */

// @stub 0x30bc40
hkBool hkRigidBody::isActive(void) const { return hkBool(); }

// @stub 0x30bc60
void hkRigidBody::activate(void) { }

// @stub 0x2da5d0
void hkTransform::setMulEq(hkTransform const &b) { }

/* callees of ai.cpp's 0x1caa40 */

// @stub 0x2da3f0
void hkTransform::setInverse(hkTransform const &t) { }


struct real_quaternion_transform;





// @stub 0x260160
bool function_260160(long actor_index, s_261d20_entry *entry, s_prop_search *search) { return false; }

/* the animation channels (unknown_1c62f0.cpp: real code there, under #if 0) */

// @stub 0x1c66a0
void __stdcall c_animation_channel_advance(c_animation_channel *channel, real frame, s_animation_state *state,
	animation_event_callback callback, long user) { }

// @stub 0x290250
void __stdcall function_290250(long tag_index, long ticks, long object_index, long node_index, real lower, real upper,
	transform4x3f const *matrix)
{
}


// @stub 0x2d9160
void __cdecl function_2d9160(void *array, long capacity, long element_size) { }


class c_component_rotation
{
public:
	hkVector4 value;
	void set(hkRotation const &rotation);
};

// @stub 0x2da2d0
void c_component_rotation::set(hkRotation const &rotation) { }

// @stub 0x2db880
void __cdecl function_2db880(hkVector4 const *position, c_component_rotation const *rotation, real frequency, hkRigidBody *body) { }


class c_contact_rule_fallback
{
public:
 hkBool accepts(long a, long b);
};

// @stub 0x2df8a0
hkBool c_contact_rule_fallback::accepts(long a, long b)
{
 hkBool result;
 result.m_bool = 0;
 return result;
}

class c_component_joint_strength
{
public:
    void set_strength(real strength);
};

// @stub 0x312b80
void c_component_joint_strength::set_strength(real strength) {}

#include <xmmintrin.h>
struct c_component_joint_snapshot_0
{
    __m128 data[6];
    c_component_joint_snapshot_0();
};
struct c_component_joint_reader_0
{
    void read(c_component_joint_snapshot_0 *output);
};

struct c_component_joint_snapshot_1
{
    __m128 data[9];
    c_component_joint_snapshot_1();
};
struct c_component_joint_reader_1
{
    void read(c_component_joint_snapshot_1 *output);
};

struct c_component_joint_snapshot_2
{
    __m128 data[9];
    c_component_joint_snapshot_2();
};
struct c_component_joint_reader_2
{
    void read(c_component_joint_snapshot_2 *output);
};

struct c_component_joint_snapshot_4
{
    __m128 data[3];
    c_component_joint_snapshot_4();
};
struct c_component_joint_reader_4
{
    void read(c_component_joint_snapshot_4 *output);
};

// @stub 0x313b30
c_component_joint_snapshot_0::c_component_joint_snapshot_0() {}

// @stub 0x313c50
void c_component_joint_reader_0::read(c_component_joint_snapshot_0 *output) {}

// @stub 0x313320
c_component_joint_snapshot_1::c_component_joint_snapshot_1() {}

// @stub 0x3134a0
void c_component_joint_reader_1::read(c_component_joint_snapshot_1 *output) {}

// @stub 0x312920
c_component_joint_snapshot_2::c_component_joint_snapshot_2() {}

// @stub 0x312ab0
void c_component_joint_reader_2::read(c_component_joint_snapshot_2 *output) {}

// @stub 0x3140c0
c_component_joint_snapshot_4::c_component_joint_snapshot_4() {}

// @stub 0x3141c0
void c_component_joint_reader_4::read(c_component_joint_snapshot_4 *output) {}


class c_world_callback_registration
{
public:
    void remove(void *callback);
};

// @stub 0x30c3e0
void c_world_callback_registration::remove(void *callback) {}

struct c_contact_query_bounds_info { c_contact_query_bounds_info(); };
struct c_contact_query_transform_info { c_contact_query_transform_info(); };
class c_contact_query_bounds_volume
{
public:
    c_contact_query_bounds_volume(c_contact_query_bounds_info const *info);
};
class c_contact_query_transform_volume
{
public:
    c_contact_query_transform_volume(c_contact_query_transform_info const *info);
};
class c_contact_query_world
{
public:
    void add(void *volume);
    void remove(void *volume);
};

// @stub 0x312310
c_contact_query_bounds_info::c_contact_query_bounds_info() {}

// @stub 0x30b6a0
c_contact_query_transform_info::c_contact_query_transform_info() {}

// @stub 0x30b480
c_contact_query_bounds_volume::c_contact_query_bounds_volume(c_contact_query_bounds_info const *info) {}

// @stub 0x30bb50
c_contact_query_transform_volume::c_contact_query_transform_volume(c_contact_query_transform_info const *info) {}

// @stub 0x30e4b0
void c_contact_query_world::add(void *volume) {}

// @stub 0x30e5e0
void c_contact_query_world::remove(void *volume) {}

class hkMemory;
struct s_fixed_memory_statistics_16;
struct s_physics_pool_statistics;

// @stub 0x22c390
void function_22c390(hkMemory *memory, s_fixed_memory_statistics_16 *statistics) {}

// @stub 0x22cb00
void function_22cb00(hkMemory *memory, s_physics_pool_statistics *statistics) {}


struct s_physics_mass_array;
struct s_physics_mass;
// @stub 0x2dba80
void __cdecl function_2dba80(s_physics_mass_array const *array, s_physics_mass *result) {}

class c_havok_reference_counted;
class c_physics_shape_list
{
public:
    c_physics_shape_list(c_havok_reference_counted **shapes, long count);
};
// @stub 0x2fc320
c_physics_shape_list::c_physics_shape_list(c_havok_reference_counted **shapes, long count) {}

class c_2de7b0
{
public:
    virtual ~c_2de7b0();
    byte field_4[0x4c];
};

// @stub 0x2de7b0
c_2de7b0::~c_2de7b0() {}

struct s_2dc030
{
    byte field_0[8];
    long field_8;
    real field_c;
    real field_10;
    s_2dc030();
};

class c_2dbfc0
{
public:
    byte field_0[4];
    word field_4;
    byte field_6[0x3a];
    c_2dbfc0(long arg_0, void *arg_1, real arg_2);
    void function_2dbcf0(s_2dc030 const *arg_0);
};

// @stub 0x2dbfc0
c_2dbfc0::c_2dbfc0(long arg_0, void *arg_1, real arg_2) {}

// @stub 0x2dc030
s_2dc030::s_2dc030() {}

// @stub 0x2dbcf0
void c_2dbfc0::function_2dbcf0(s_2dc030 const *arg_0) {}

struct s_1d1870
{
 char field_0;
 s_1d1870() {}
 s_1d1870(bool arg_0) : field_0(arg_0) {}
};

class c_314710
{
public:
    byte field_0[0x60];
    c_314710(hkEntity *arg_0);
    s_1d1870 function_3144b0(void *arg_0);
    s_1d1870 function_314580(void *arg_0);
};

// @stub 0x314710
c_314710::c_314710(hkEntity *arg_0) {}

struct c_shape_library_base_a
{
 c_shape_library_base_a(c_havok_reference_counted *arg_0, long arg_1);
};
// @stub 0x2df760
c_shape_library_base_a::c_shape_library_base_a(c_havok_reference_counted *arg_0, long arg_1) {}

struct s_2e5a10 { byte field_0; s_2e5a10(const s_2e5a10 &arg_0) : field_0(arg_0.field_0) {} };

struct s_1c3a40
{
 void function_2e5e70(long arg_0, long arg_1);
 void function_2e5a10(s_2e5a10 arg_0);
};

// @stub 0x2e5e70
void s_1c3a40::function_2e5e70(long arg_0, long arg_1) {}

// @stub 0x2e5a10
void s_1c3a40::function_2e5a10(s_2e5a10 arg_0) {}

// @stub 0x2e6190
void __cdecl function_2e6190(s_1c3a40 *arg_0) {}

// @stub 0x2e6cd0
void __cdecl function_2e6cd0(s_1c3a40 *arg_0) {}

// @stub 0x2e7d10
void __cdecl function_2e7d10(s_1c3a40 *arg_0) {}

// @stub 0x2e9030
void __cdecl function_2e9030(s_1c3a40 *arg_0) {}

// @stub 0x2eb540
void __cdecl function_2eb540(s_1c3a40 *arg_0) {}

// @stub 0x2ebbb0
void __cdecl function_2ebbb0(s_1c3a40 *arg_0) {}

// @stub 0x2ecbf0
void __cdecl function_2ecbf0(s_1c3a40 *arg_0) {}

// @stub 0x2ed700
void __cdecl function_2ed700(s_1c3a40 *arg_0) {}

// @stub 0x2ed9b0
void __cdecl function_2ed9b0(s_1c3a40 *arg_0) {}

// @stub 0x2ee240
void __cdecl function_2ee240(s_1c3a40 *arg_0) {}

// @stub 0x2eee90
void __cdecl function_2eee90(s_1c3a40 *arg_0) {}

// @stub 0x2ef900
void __cdecl function_2ef900(s_1c3a40 *arg_0) {}

// @stub 0x2efe20
void __cdecl function_2efe20(s_1c3a40 *arg_0) {}

// @stub 0x2f0e00
void __cdecl function_2f0e00(s_1c3a40 *arg_0) {}

// @stub 0x2f1ab0
void __cdecl function_2f1ab0(s_1c3a40 *arg_0) {}

// @stub 0x2f2800
void __cdecl function_2f2800(s_1c3a40 *arg_0) {}

// @stub 0x2f42b0
void __cdecl function_2f42b0(s_1c3a40 *arg_0) {}

// @stub 0x2f7a70
void __cdecl function_2f7a70(s_1c3a40 *arg_0) {}

// @stub 0x2f8900
void __cdecl function_2f8900(s_1c3a40 *arg_0) {}

// @stub 0x2f96d0
void __cdecl function_2f96d0(s_1c3a40 *arg_0) {}

// @stub 0x2faac0
void __cdecl function_2faac0(s_1c3a40 *arg_0) {}

// @stub 0x2fb9d0
void __cdecl function_2fb9d0(s_1c3a40 *arg_0) {}

// @stub 0x2daf00
void __cdecl function_2daf00(s_1c3a40 *arg_0) {}

// @stub 0x3144b0
s_1d1870 c_314710::function_3144b0(void *arg_0) { return s_1d1870(); }

// @stub 0x314580
s_1d1870 c_314710::function_314580(void *arg_0) { return s_1d1870(); }

struct s_311340 { s_311340(); };
class c_311ba0 { public: c_311ba0(const s_311340 *arg_0); };
// @stub 0x311340
s_311340::s_311340() {}
// @stub 0x311ba0
c_311ba0::c_311ba0(const s_311340 *arg_0) {}
