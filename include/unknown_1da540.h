#ifndef UNKNOWN_1DA540_H
#define UNKNOWN_1DA540_H
#include "unknown_11c920.h"
#include "unknown_0259d0.h"

struct s_collision_damage_entry
{
	long definition_index;
	dword flags;
	byte unknown08[0x1c - 8];
	long object_value28;
	long object_value2c;
	point3f position;
	point3f origin;
	vector3f direction;
	byte unknown48[0x7c - 0x48];
	short unknown7c;
	byte unknown7e[0x84 - 0x7e];
	byte kind;
	byte unknown85[3];
	long instance_index;
	long material_index;
	long surface_index;

	s_collision_damage_entry();
};
#endif
