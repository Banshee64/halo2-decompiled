/* OBJECT_DEFAULT_PLACEMENT.H: the sequence the scripted animation starters
   (0x10a660, 0x11b520) inline to put an object back at the default placement
   when it is attached to another object */

#ifndef OBJECT_DEFAULT_PLACEMENT_H
#define OBJECT_DEFAULT_PLACEMENT_H

#include "unknown_11c920.h"
#include "globals.h"

void __stdcall function_b9a50(long unit_index);
void function_b8840(long unit_index);
struct s_location;
void function_b75a0(long object_index, point3f const *point, vector3f const *forward, vector3f const *up,
	s_location const *location, bool unknown);
void __stdcall function_b77d0(long object_index, vector3f const *linear_velocity, vector3f const *angular_velocity);
void function_b7360(long object_index);
void __stdcall function_b8600(long object_index, long unknown);
void function_b7290(long object_index);
void function_b9b90(long object_index, bool disable);
void __stdcall function_bd020(long object_index);

struct s_object_list;
extern s_object_list *g_4de2f4;

/* the object fields this sequence reads (a local view) */
struct s_object_default_placement_view
{
	byte unknown000[4];
	dword : 7;
	dword hidden : 1;
	dword flag8 : 1;
	dword : 23;
	byte unknown008[0xc0 - 0x8];
	byte : 6;
	byte flag_c0_6 : 1;
	byte : 1;
};

struct s_object_default_placement_header
{
	byte unknown00[8];
	s_object_default_placement_view *object;
};

inline s_object_default_placement_view *object_default_placement_get(long object_index)
{
	return ((s_object_default_placement_header *)g_4e0300->data)[object_index & 0xffff].object;
}

/* moves an object to the default placement and back onto the map */
__forceinline void object_reset_default_placement(long object_index, s_object_default_placement_view *object,
	bool reset_velocity)
{
	function_b9a50(object_index);
	if (TEST_FIELD_BIT(object->flag_c0_6))
		function_b8840(object_index);
	function_b75a0(object_index, g_468788, g_4687a8, g_4687b0, NULL, false);
	if (reset_velocity)
		function_b77d0(object_index, g_4687a4, g_4687a4);
	function_b7360(object_index);

	s_object_default_placement_view *placed = object_default_placement_get(object_index);
	placed->hidden = false;
	if (!TEST_FIELD_BIT(placed->flag8) && g_4de2f4 && *(byte *)g_4de2f4)
		function_b8600(object_index, 0);
	function_b7290(object_index);
}

#endif
