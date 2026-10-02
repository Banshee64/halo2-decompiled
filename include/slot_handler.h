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
typedef short (__stdcall *t_slot_choose)(long actor_index, short level, bool active);
typedef void (__stdcall *t_slot_proc4)(long actor_index, s_slot *slot, long a, long b);
typedef short (__stdcall *t_slot_trigger)(long actor_index, s_slot *slot);

struct s_slot_handler_0
{
	short index;
	short kind;
	long mask;
	long unknown8;
	long unknownc;
	t_slot_trigger trigger;
};

/* the part all kind 1 and 2 handlers share (unknown_1a8080.cpp has its own
   view of it under the same name, for the handler table g_46eeb8) */
struct s_slot_handler
{
	short index;
	short kind;
	long mask;
	long unknown8;
	long unknownc;
	t_slot_priority priority;
	t_slot_evaluate evaluate;
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

/* the handlers by slot type (unknown_1a8080.cpp; 0x83 of them) and what
   enables a type: its handler's unknown8 differs from g_46f348, its mask
   covers g_4ee4ec, and its bit in g_557c40 is set */
extern s_slot_handler *g_46eeb8[];
extern long g_46f348;
extern dword g_4ee4ec;
extern dword g_557c40[];

inline bool slot_type_enabled(short type)
{
	s_slot_handler *handler = g_46eeb8[type];

	return handler->unknown8 != g_46f348 &&
		(handler->mask & g_4ee4ec) == g_4ee4ec &&
		(g_557c40[type >> 5] & (1 << (type & 31))) != 0;
}

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
	t_slot_release release5c;
	t_slot_proc4 proc60;
	short unknown64;
	short unknown66;
	real unknown68;
	long unknown6c;
};

/* a pair of shorts that is unset while negative (g_470fa0 is the unset
   value), and the 6 byte entries of the actor's table at +0x400 */
struct s_reference
{
	short unknown0;
	short unknown2;
};

struct s_reference_entry
{
	short unknown0;
	s_reference reference;
};

/* the results the evaluate callbacks return (0x46fbe4, 0x46fbe8), and the
   value 0x470fa0 (-1) some notify callbacks reset slot fields to */
extern short g_46fbe4;
extern short g_46fbe8;
extern s_reference g_470fa0;

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
	byte unknown26;
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

struct s_object_view
{
	long tag_index;
	byte unknown004[0xb2 - 0x4];
	byte unknownb2;
	byte unknownb3[0x100 - 0xb3];
	real unknown100;
	byte unknown104[0x1fc - 0x104];
	short unknown1fc;
};

inline s_object_view *object_get(long object_index)
{
	return (s_object_view *)((s_object_header_view *)g_4e0300->data)[object_index & 0xffff].object;
}

/* the data arrays of 0xbc byte (g_502424) and 0x50 byte (g_502420) elements
   the handlers keep indices into */
extern s_data_array *g_502424;

struct s_502424_element
{
	byte unknown00[4];
	long unknown04;
	byte unknown08[0x80 - 0x8];
	long unknown80;
	long unknown84;
	long unknown88;
	byte unknown8c[8];
	short unknown94;
	byte unknown96[0xbc - 0x96];
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

/* the actor (0x888 bytes, g_4f55f0) as the slot handlers see it */
struct s_actor_view
{
	// BEGIN s_actor_view
	byte unknown000[0x7 - 0x0];
	bool unknown007;
	byte unknown008[0x24 - 0x8];
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
	short unknown086;
	byte unknown088[0x90 - 0x88];
	s_slot slots[4];
	short current;
	byte unknown192[0x220 - 0x192];
	bool unknown220;
	bool unknown221;
	byte unknown222[0x225 - 0x222];
	bool unknown225;
	byte unknown226[0x227 - 0x226];
	bool unknown227;
	byte unknown228[0x238 - 0x228];
	real_point3d position;
	byte unknown244[0x26c - 0x244];
	long unknown26c;
	short unknown270;
	byte unknown272[0x290 - 0x272];
	real_vector3d unknown290;
	byte unknown29c[0x314 - 0x29c];
	dword unknown314;
	byte unknown318[0x31c - 0x318];
	short unknown31c;
	short unknown31e;
	byte unknown320[0x338 - 0x320];
	long prop_index;
	byte unknown33c[0x344 - 0x33c];
	long unknown344;
	byte unknown348[0x3b0 - 0x348];
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
	real_vector3d unknown424;
	byte unknown430[0x449 - 0x430];
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
	byte unknown464[0x484 - 0x464];
	bool unknown484;
	byte unknown485[0x488 - 0x485];
	bool unknown488;
	byte unknown489[0x4a1 - 0x489];
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
	byte unknown4b8[0x504 - 0x4b8];
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
	byte unknown700[0x85c - 0x700];
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
