/* AI_ACTOR.H: the actor (0x888 bytes, g_4f55f0) and its unit object as lane
   M's behavior code (0x1a8000..0x1affff) and the actor helpers it calls
   (0x1e1f20..0x1e9700) see them. Only the fields they touch are named; the
   layout agrees with s_actor_view of slot_handler.h. */

#ifndef AI_ACTOR_H
#define AI_ACTOR_H

#include "cseries.h"
#include "real_math.h"
#include "globals.h"
#include "unknown_20fe20.h"

struct s_ai_actor
{
	byte unknown000[0x4 - 0x0];
	short unknown004;
	byte unknown006[0x7 - 0x6];
	bool unknown007;
	byte unknown008[0x18 - 0x8];
	long unit_index;
	byte unknown01c[0x40 - 0x1c];
	bool unknown040;
	byte unknown041[0x54 - 0x41];
	long character_index;
	byte unknown058[0x7c - 0x58];
	long unknown07c;
	long next_index;
	byte unknown084[0x1f4 - 0x84];
	long unknown1f4;
	long unknown1f8;
	byte unknown1fc[0x225 - 0x1fc];
	bool unknown225;
	bool unknown226;
	byte unknown227[0x229 - 0x227];
	bool unknown229;
	byte unknown22a[0x238 - 0x22a];
	real_point3d position;
	byte unknown244[0x258 - 0x244];
	real_vector3d unknown258;
	bool unknown264;
	byte unknown265[0x268 - 0x265];
	bool unknown268;
	byte unknown269[0x26c - 0x269];
	long unknown26c;
	short unknown270;
	byte unknown272[0x274 - 0x272];
	long unknown274;
	byte unknown278[0x290 - 0x278];
	real_vector3d unknown290;
	byte unknown29c[0x338 - 0x29c];
	long prop_index;
	byte unknown33c[0x358 - 0x33c];
	short unknown358;
	byte unknown35a[0x35e - 0x35a];
	bool unknown35e;
	byte unknown35f[0x360 - 0x35f];
	long unknown360;
	byte unknown364[0x368 - 0x364];
	long unknown368;
	real unknown36c;
	real_point3d unknown370;
	real_vector3d unknown37c;
	byte unknown388[0x398 - 0x388];
	real unknown398;
	real unknown39c;
	byte unknown3a0[0x3e0 - 0x3a0];
	short unknown3e0;
	byte unknown3e2[0x418 - 0x3e2];
	s_reference unknown418;
	short unknown41c;
	byte unknown41e[0x420 - 0x41e];
	short unknown420;
	byte unknown422[0x424 - 0x422];
	real_vector3d unknown424;
	byte unknown430[0x44d - 0x430];
	bool unknown44d;
	byte unknown44e[0x456 - 0x44e];
	bool unknown456;
	byte unknown457[0x458 - 0x457];
	real_vector3d unknown458;
	bool unknown464;
	bool unknown465;
	bool unknown466;
	byte unknown467[0x468 - 0x467];
	s_node_point unknown468;
	byte unknown478[0x488 - 0x478];
	bool unknown488;
	byte unknown489[0x504 - 0x489];
	short unknown504;
	byte unknown506[0x50c - 0x506];
	bool unknown50c;
	byte unknown50d[0x510 - 0x50d];
	s_node_point unknown510;
	byte unknown520[0x524 - 0x520];
	real unknown524;
	byte unknown528[0x5d0 - 0x528];
	bool unknown5d0;
	byte unknown5d1[0x5ec - 0x5d1];
	real_vector3d unknown5ec;
	byte unknown5f8[0x810 - 0x5f8];
	dword flags810;
	byte unknown814[0x888 - 0x814];
};

struct s_ai_object
{
	long definition_index;
	byte unknown004[0x19 - 0x4];
	byte flags19;
	byte unknown01a[0x30 - 0x1a];
	real_point3d position;
	byte unknown03c[0x88 - 0x3c];
	real_vector3d velocity;
	byte unknown094[0x134 - 0x94];
	dword flags134;
	byte unknown138[0x212 - 0x138];
	char current_weapon;
	byte unknown213[0x218 - 0x213];
	long weapons[4];
	byte unknown228[0x23c - 0x228];
	char unknown23c;
	byte unknown23d[0x34c - 0x23d];
	byte unknown34c;
	byte unknown34d[0x350 - 0x34d];
};

inline s_ai_actor *ai_actor_get(long actor_index)
{
	return (s_ai_actor *)(g_4f55f0->data + (actor_index & 0xffff) * sizeof(s_ai_actor));
}

struct s_ai_object_header
{
	byte unknown00[8];
	s_ai_object *object;
};

inline s_ai_object *ai_object_get(long object_index)
{
	return ((s_ai_object_header *)g_4e0300->data)[object_index & 0xffff].object;
}

/* the actor helpers (unknown_1e1f20.cpp, unknown_1e4290.cpp,
   unknown_1e5240.cpp, unknown_1e9700.cpp) */
long actor_get_weapon(long actor_index);
bool function_1e2030(long actor_index);
void function_1e4290(long actor_index, bool value);
void function_1e4650(long actor_index, bool value);
void *function_1e5280(long actor_index, long key);
void *function_1e5240(long actor_index);
void *function_1e5380(long actor_index);
real function_1e9700(short row);

#endif