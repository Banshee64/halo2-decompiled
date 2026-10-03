/* SLOT_HANDLER.H: the actor slot handlers of 0x1b0000..0x1bffff (lane B).

   g_46eeb8 (unknown_1a8080.cpp) points at one handler per slot type. Each
   handler is a static struct in the data of its own source file, next to
   its callbacks; all callbacks take the actor index first and are called
   through these structs, so they keep the standard __stdcall convention.

   - kind 0 handlers are 0x14 bytes: the header and one test.
   - kind 1 handlers are 0x4c bytes: a choice callback, then a count and an
     array of child entries.
   - kind 2 handlers are 0x4c bytes, three updates at the end; some (those
     built on the shared callbacks at 0x26e4b0..0x26e710) are 0x70 bytes. */

#ifndef SLOT_HANDLER_H
#define SLOT_HANDLER_H

#include "cseries.h"
#include "real_math.h"
#include "globals.h"
#include "slot_owner.h"

/* the slot header every handler state starts with (s_slot of slot_owner.h
   is the whole 0x40 bytes) */
struct s_slot_header
{
	short type;
	short state;
	short unknown4;
	byte unknown6[2];
	long time;
};

typedef short (__stdcall *t_slot_priority)(long actor_index);
typedef short (__stdcall *t_slot_evaluate)(long actor_index, s_slot *slot, bool active);
typedef bool (__stdcall *t_slot_start)(long actor_index, s_slot *slot);
typedef void (__stdcall *t_slot_proc)(long actor_index, s_slot *slot);
typedef void (__stdcall *t_slot_notify)(long actor_index, s_slot *slot, bool active);
typedef void (__stdcall *t_slot_release)(long actor_index, s_slot *slot, long index);
struct s_slot_target_list;
typedef void (__stdcall *t_slot_list)(long actor_index, s_slot *slot, s_slot_target_list *list);
typedef short (__stdcall *t_slot_choose)(long actor_index, short level, bool active);
typedef void (__stdcall *t_slot_proc4)(long actor_index, s_slot *slot, long a, long b);
typedef short (__stdcall *t_slot_trigger)(long actor_index, s_slot *slot);
/* the same callbacks as the query and evaluate procedures take them from the
   action lists (unknown_1a58b0.cpp): a plain argument in place of the flag */
typedef short (__stdcall *t_slot_query)(long actor_index, long argument);
typedef short (__stdcall *t_slot_evaluate_argument)(long actor_index, s_slot *slot, long argument);

struct s_slot_handler_0
{
	short index;
	short kind;
	long mask;
	long unknown8;
	long unknownc;
	union
	{
		t_slot_trigger trigger;
		t_slot_query query;
	};
};

/* the part all kind 1 and 2 handlers share; the handler table g_46eeb8
   points at it */
struct s_slot_handler
{
	short index;
	short kind;
	long mask;
	long unknown8;
	long unknownc;
	t_slot_priority priority;
	union
	{
		t_slot_evaluate evaluate;
		t_slot_evaluate_argument evaluate_argument;
	};
	t_slot_start start;
	t_slot_proc stop;
	short wanted_type;
	byte unknown22[2];
	t_slot_release release24;
	t_slot_release release28;
	t_slot_release release2c;
	t_slot_proc proc30;
	t_slot_notify notify34;
	long unknown38;
	long unknown3c;
};

/* the handlers by slot type (unknown_1a8080.cpp) and what enables a type:
   its handler's unknown8 differs from g_46f348, its mask covers g_4ee4ec,
   and its bit in g_557c40 is set (globals.h) */
enum
{
	k_slot_type_count = 0x83
};

extern s_slot_handler *g_46eeb8[k_slot_type_count];

inline bool slot_type_enabled(short type)
{
	s_slot_handler *handler = g_46eeb8[type];

	return handler->unknown8 != g_46f348 &&
		(handler->mask & g_4ee4ec) == g_4ee4ec &&
		(g_557c40[type >> 5] & (1 << (type & 31))) != 0;
}

/* an element (0xb4 bytes) of the block of the actor's tag, found by
   function_1e5450 (unknown_1e5450.cpp) */
struct s_tag_element
{
	byte unknown00[4];
	long key;
	byte unknown08[4];
	long tag_index;
	byte unknown10[0x94 - 0x10];
	real unknown94;
	byte unknown98[0xb4 - 0x98];
};

s_tag_element *function_1e5450(long owner_index, long key);

/* a child entry of a kind 1 handler */
struct s_slot_child
{
	short type;
	short flags;
	short unknown4;
	byte unknown6[2];
	real unknown8;
	long unknownc;
	long unknown10;
};

struct s_slot_handler_1
{
	s_slot_handler head;
	t_slot_choose choose;
	long child_count;
	s_slot_child *children;
};

/* a kind 1 handler built on the shared callbacks at 0x26e750..0x26e8a0 */
struct s_slot_handler_1x
{
	s_slot_handler_1 base;
	t_slot_proc update4c;
	t_slot_release release50;
	t_slot_proc4 proc54;
	t_slot_proc4 proc58;
	short unknown5c;
	short unknown5e;
	real unknown60;
};

struct s_slot_handler_2
{
	s_slot_handler head;
	t_slot_proc update40;
	t_slot_proc update44;
	t_slot_proc update48;
};

struct s_slot_handler_2x
{
	s_slot_handler_2 base;
	t_slot_proc update4c;
	t_slot_release release50;
	t_slot_release release54;
	t_slot_release release58;
	t_slot_list list5c;
	t_slot_proc4 proc60;
	short unknown64;
	short unknown66;
	real unknown68;
	long unknown6c;
};

/* the 6 byte entries of the actor's table at +0x400 (s_reference and its
   unset value g_470fa0 are in globals.h) */
struct s_reference_entry
{
	short unknown0;
	s_reference reference;
};

/* the list some kind 2x callbacks get: entries of actor indices */
struct s_slot_target_entry
{
	long actor_index;
	long unknown4;
	long unknown8;
};

struct s_slot_target_list
{
	short unknown0;
	short count;
	s_slot_target_entry entries[10];
};

/* trivial callbacks; retail folds each with identical functions elsewhere */
static bool __stdcall slot_start_true(long actor_index, s_slot *slot)
{
	return true;
}

static void __stdcall slot_proc_nothing(long actor_index, s_slot *slot)
{
}

static bool __stdcall slot_release_true(long actor_index, s_slot *slot, long index)
{
	return true;
}

static void __stdcall slot_release_nothing(long actor_index, s_slot *slot, long index)
{
}

/* callbacks shared by several handlers (identical functions folded) */
short __stdcall function_1a8370(long actor_index);
short __stdcall function_1bced0(long actor_index, s_slot *slot, bool active);
short __stdcall function_1b2d90(long actor_index, s_slot *slot, bool active);
short __stdcall function_1a79e0(long actor_index, short level, bool active);
short __stdcall function_1adcd0(long actor_index);

/* the prop data (unknown_25d690.cpp): g_502418 holds 0x3c byte nodes, whose
   view (0x124 bytes in g_502414, from +0x70) function_25d740 returns */
struct prop_view;
struct s_prop_node;
prop_view *function_25d740(s_prop_node *node);

struct s_prop_datum;
struct prop_state;
prop_state *prop_state_get(s_prop_datum *datum);

struct s_prop_node_view
{
	byte unknown00[8];
	long unknown08;
	byte unknown0c[8];
	long view_index;
	short type;
	byte unknown1a[6];
	long object_index;
	short unknown24;
	char unknown26;
	char unknown27;
	real unknown28;
	long next_index;
	byte unknown30[0x3c - 0x30];
};

struct s_prop_view_fields
{
	short unknown00;
	byte unknown02[0x4a];
	bool unknown4c;
	byte unknown4d[0x69 - 0x4d];
	bool unknown69;
	byte unknown6a[6];
	short unknown70;
	byte unknown72[0x88 - 0x72];
	bool unknown88;
	byte unknown89[3];
	short unknown8c;
};

struct s_prop_state_view
{
	long unknown00;
};

inline s_prop_state_view *prop_node_state(s_prop_node_view *node)
{
	return (s_prop_state_view *)prop_state_get((s_prop_datum *)node);
}

/* the objects: g_4e0300 holds 12 byte headers with the object at +8 */
struct s_object_header_view
{
	byte unknown00[8];
	byte *object;
};

struct s_slot_object_view
{
	long tag_index;
	byte unknown004[0x30 - 0x4];
	real_point3d unknown030;
	byte unknown03c[0xb2 - 0x3c];
	byte unknownb2;
	byte unknownb3[0xec - 0xb3];
	real unknownec;
	byte unknownf0[0x100 - 0xf0];
	real unknown100;
	byte unknown104[0x1fc - 0x104];
	short unknown1fc;
};

inline s_slot_object_view *object_get(long object_index)
{
	return (s_slot_object_view *)((s_object_header_view *)g_4e0300->data)[object_index & 0xffff].object;
}

/* the data arrays of 0xbc byte (g_502424) and 0x50 byte (g_502420) elements
   the handlers keep indices into (globals.h) */
struct s_502424_target
{
	long unknown0;
	long unknown4;
	union
	{
		long unknown8;
		real_point3d point;
	};
	short unknown14;
	byte unknown16[2];
};

struct s_502424_element
{
	byte unknown00[4];
	long unknown04;
	byte unknown08[0x80 - 0x8];
	s_502424_target target;
	byte unknown98[0xbc - 0x98];
};

struct s_502420_element
{
	byte unknown00[0x18];
	long first_actor_index;
	byte unknown1c[0xa];
	short unknown26;
	byte unknown28[0x50 - 0x28];
};

inline s_502424_element *element_502424_get(long index)
{
	return (s_502424_element *)(g_502424->data + (index & 0xffff) * sizeof(s_502424_element));
}

inline s_502420_element *element_502420_get(long index)
{
	return (s_502420_element *)(g_502420->data + (index & 0xffff) * sizeof(s_502420_element));
}

inline s_prop_node_view *prop_node_get(long index)
{
	return (s_prop_node_view *)(g_502418->data + (index & 0xffff) * sizeof(s_prop_node_view));
}

inline s_prop_view_fields *prop_node_view(s_prop_node_view *node)
{
	return (s_prop_view_fields *)function_25d740((s_prop_node *)node);
}

/* a target of the actor: a point or an object */
union u_actor_target
{
	real_point3d point;
	real_vector3d vector;
	long object_index;
};

/* the flags at +0x314 of the actor */
struct s_actor_flags314
{
	dword bit0 : 1;
	dword bit1 : 1;
	dword unknown : 30;
};

/* the actor (0x888 bytes, g_4f55f0) as the slot handlers see it */
struct s_actor_view
{
	// BEGIN s_actor_view
	byte unknown000[0x7 - 0x0];
	bool unknown007;
	byte unknown008[0x18 - 0x8];
	long unknown018;
	byte unknown01c[0x24 - 0x1c];
	short unknown024;
	byte unknown026[0x30 - 0x26];
	long unknown030;
	byte unknown034[0x54 - 0x34];
	long unknown054;
	long first_prop_index;
	byte unknown05c[0x7c - 0x5c];
	long unknown07c;
	long next_index;
	short unknown084;
	union
	{
		short unknown086;
		byte unknown086_byte;
	};
	byte unknown088[0x90 - 0x88];
	s_slot slots[4];
	short current;
	byte unknown192[0x1e8 - 0x192];
	long times[14];
	bool unknown220;
	bool unknown221;
	byte unknown222;
	bool unknown223;
	byte unknown224;
	bool unknown225;
	byte unknown226[0x227 - 0x226];
	bool unknown227;
	bool unknown228;
	byte unknown229;
	bool unknown22a;
	byte unknown22b[0x238 - 0x22b];
	real_point3d position;
	byte unknown244[0x266 - 0x244];
	byte unknown266;
	byte unknown267;
	byte unknown268;
	byte unknown269;
	byte unknown26a[2];
	long unknown26c;
	short unknown270;
	byte unknown272[0x290 - 0x272];
	real_vector3d unknown290;
	byte unknown29c[0x314 - 0x29c];
	s_actor_flags314 unknown314;
	byte unknown318[0x31c - 0x318];
	short unknown31c;
	short unknown31e;
	byte unknown320[0x338 - 0x320];
	long prop_index;
	byte unknown33c[0x344 - 0x33c];
	long unknown344;
	long unknown348;
	byte unknown34c[0x354 - 0x34c];
	bool unknown354;
	byte unknown355[0x3b0 - 0x355];
	short unknown3b0;
	byte unknown3b2[0x3b4 - 0x3b2];
	long unknown3b4;
	bool unknown3b8;
	byte unknown3b9[0x3bc - 0x3b9];
	real unknown3bc;
	real_vector3d unknown3c0;
	long unknown3cc;
	short unknown3d0;
	short unknown3d2;
	byte unknown3d4[0x3f2 - 0x3d4];
	bool unknown3f2;
	byte unknown3f3[0x3fe - 0x3f3];
	short unknown3fe;
	s_reference_entry unknown400[4];
	s_reference unknown418;
	short unknown41c;
	byte unknown41e[0x420 - 0x41e];
	short unknown420;
	byte unknown422[0x424 - 0x422];
	u_actor_target unknown424;
	short unknown430;
	byte unknown432[0x434 - 0x432];
	short unknown434;
	byte unknown436[0x438 - 0x436];
	u_actor_target unknown438;
	short unknown444;
	byte unknown446[0x449 - 0x446];
	bool unknown449;
	bool unknown44a;
	byte unknown44b[0x44d - 0x44b];
	bool unknown44d;
	byte unknown44e[0x450 - 0x44e];
	dword unknown450;
	byte unknown454[0x456 - 0x454];
	bool unknown456;
	byte unknown457[0x458 - 0x457];
	real_vector3d unknown458;
	byte unknown464[0x480 - 0x464];
	bool unknown480;
	byte unknown481;
	bool unknown482;
	bool unknown483;
	bool unknown484;
	bool unknown485;
	byte unknown486[0x488 - 0x486];
	bool unknown488;
	byte unknown489[0x48c - 0x489];
	bool unknown48c;
	byte unknown48d[0x490 - 0x48d];
	real_point3d unknown490;
	short unknown49c;
	byte unknown49e[0x4a0 - 0x49e];
	bool unknown4a0;
	bool unknown4a1;
	byte unknown4a2[0x4a4 - 0x4a2];
	byte unknown4a4;
	byte unknown4a5;
	byte unknown4a6[0x4a8 - 0x4a6];
	long unknown4a8;
	short unknown4ac;
	bool unknown4ae;
	byte unknown4af[0x4b4 - 0x4af];
	real unknown4b4;
	byte unknown4b8[0x4cc - 0x4b8];
	real unknown4cc;
	byte unknown4d0[0x504 - 0x4d0];
	short unknown504;
	byte unknown506[0x50c - 0x506];
	bool unknown50c;
	byte unknown50d[0x5ac - 0x50d];
	long unknown5ac;
	short unknown5b0;
	byte unknown5b2[0x5b4 - 0x5b2];
	short unknown5b4;
	short unknown5b6;
	byte unknown5b8[0x6fc - 0x5b8];
	dword unknown6fc;
	byte unknown700[0x7c0 - 0x700];
	real unknown7c0;
	byte unknown7c4[0x810 - 0x7c4];
	struct
	{
		dword unknown0 : 13;
		dword bit13 : 1;
		dword unknown14 : 18;
	} unknown810;
	byte unknown814[0x858 - 0x814];
	long unknown858;
	long unknown85c;
	byte unknown860[0x888 - 0x860];
	// END s_actor_view
};

inline s_actor_view *actor_get(long actor_index)
{
	return (s_actor_view *)(g_4f55f0->data + (actor_index & 0xffff) * sizeof(s_actor_view));
}

/* clears the actor's state at 0x4ac..0x5b6 (inlined by several start
   callbacks) */
inline void actor_reset_state(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);

	actor->unknown50c = false;
	actor->unknown5ac = NONE;
	actor->unknown5b0 = NONE;
	actor->unknown5b4 = 0;
	actor->unknown5b6 = 0;
	actor->unknown4ac = 0;
	actor->unknown504 = 0;
}

#endif
