/* PROPS.H: the ai's props (src/unknown_25d690.cpp): what an actor knows of
   the objects around it.

   - "prop" (g_50241c): 0x100 elements of 0xc4 bytes, one per object an
     actor team knows of; its state starts at +0x58.
   - "prop_ref" (g_502418): 0x400 elements of 0x3c bytes, one per actor and
     prop, chained from the actor's first_prop_index.
   - "tracking" (g_502414): 0x64 elements of 0x124 bytes for the props an
     actor follows closely: a state at +4 and a view at +0x70. */

#ifndef PROPS_H
#define PROPS_H

#include "cseries.h"
#include "globals.h"
#include "data_array.h"
#include "slot_handler.h"

/* the state of a prop as an actor perceives it (0x6c bytes) */
struct prop_state
{
	long unknown00;
	real_point3d position;
	byte unknown10[0x3c - 0x10];
	long unknown3c;
	long unknown40;
	long unknown44;
	real_point3d unknown48;
	short unknown54;
	byte unknown56[2];
	bool unknown58;
	byte unknown59[0x5e - 0x59];
	bool unknown5e;
	bool unknown5f;
	bool unknown60;
	bool unknown61;
	bool unknown62;
	bool unknown63;
	bool unknown64;
	bool unknown65;
	bool unknown66;
	bool unknown67;
	byte unknown68;
	bool unknown69;
	byte unknown6a[2];
};

/* the view of a tracked prop (0xb4 bytes) */
struct prop_view
{
	byte unknown00[6];
	short unknown06;
	byte unknown08[8];
	long unknown10;
	long unknown14;
	byte unknown18[0x2a - 0x18];
	bool unknown2a;
	byte unknown2b[0x39 - 0x2b];
	char unknown39;
	byte unknown3a[0x4c - 0x3a];
	bool unknown4c;
	byte unknown4d[3];
	long unknown50;
	real unknown54;
	real unknown58;
	real unknown5c;
	real unknown60;
	bool unknown64;
	byte unknown65;
	short unknown66;
	bool unknown68;
	bool unknown69;
	byte unknown6a[2];
	bool unknown6c;
	bool unknown6d;
	byte unknown6e[2];
	short unknown70;
	byte unknown72[0x88 - 0x72];
	bool unknown88;
	byte unknown89;
	short unknown8a;
	short unknown8c;
	short unknown8e;
	short unknown90;
	byte unknown92[2];
	real_vector3d unknown94;
	byte unknowna0[2];
	bool unknowna2;
	byte unknowna3;
	real_point3d unknowna4;
	long unknownb0;
};

/* a "prop" element (0xc4 bytes) */
struct prop_datum
{
	short salt;
	byte unknown02[2];
	short unknown04;
	byte unknown06[0x10 - 0x6];
	long unknown10;
	byte unknown14[0x1c - 0x14];
	long actor_index;
	byte unknown20[2];
	bool unknown22;
	bool unknown23;
	bool unknown24;
	bool unknown25;
	byte unknown26[0x32 - 0x26];
	bool unknown32;
	bool unknown33;
	bool unknown34;
	byte unknown35;
	bool unknown36;
	byte unknown37[0x3c - 0x37];
	bool unknown3c;
	byte unknown3d[0x58 - 0x3d];
	prop_state state;
};

/* a "prop_ref" element (0x3c bytes): an actor's reference to a prop */
struct s_prop_datum
{
	short salt;
	byte unknown02[2];
	long actor_index;
	long prop_index;
	byte unknown0c[4];
	real unknown10;
	long tracking_index;
	short type;
	short unknown1a;
	short unknown1c;
	byte unknown1e[2];
	long object_index;
	short state;
	char unknown26;
	char unknown27;
	real unknown28;
	long next_index;
	byte unknown30[0x3c - 0x30];
};

/* the same element as function_25d740 takes it */
struct s_prop_node : s_prop_datum
{
};

/* a "tracking" element (0x124 bytes) */
struct tracking_datum
{
	short salt;
	byte unknown02[2];
	prop_state state;
	prop_view view;
};

/* the prop types (g_470f10), indexed by s_prop_datum::type */
struct s_prop_type_entry
{
	short id;
	short unknown2;
	short unknown4;
	short kind;
	short unknown8;
	short unknowna[3];
};

extern s_prop_type_entry g_470f10[9];

/* the actor fields the prop code reads (the actor of g_4f55f0, 0x888 bytes) */
struct s_actor_prop_view
{
	byte unknown000[9];
	bool unknown009;
	byte unknown00a[0x58 - 0xa];
	long first_prop_index;
	long tracked_prop_indices[8];
	long unknown07c;
	byte unknown080[0x684 - 0x80];
	short unknown684;
	short unknown686;
	short unknown688;
	byte unknown68a[2];
	long unknown68c;
	byte unknown690[0x888 - 0x690];
};

inline prop_datum *prop_get(long prop_index)
{
	return (prop_datum *)(g_50241c->data + (prop_index & 0xffff) * sizeof(prop_datum));
}

inline s_prop_datum *prop_ref_get(long prop_ref_index)
{
	return (s_prop_datum *)(g_502418->data + (prop_ref_index & 0xffff) * sizeof(s_prop_datum));
}

inline tracking_datum *tracking_get(long tracking_index)
{
	return (tracking_datum *)(g_502414->data + (tracking_index & 0xffff) * sizeof(tracking_datum));
}

inline s_actor_prop_view *actor_prop_view_get(long actor_index)
{
	return (s_actor_prop_view *)(g_4f55f0->data + (actor_index & 0xffff) * sizeof(s_actor_prop_view));
}

prop_state *prop_state_get(s_prop_datum *datum);
prop_view *prop_view_get(long index);
prop_view *function_25d740(s_prop_node *node);

#endif
