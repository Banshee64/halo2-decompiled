// @flags /O2 /Ob1 /arch:SSE /Gr
/* OBJECT_PLACEMENT.CPP: the scenario's object placements

The functions follow object_placement.obj in Bungie's May 2003 builds
(halo-symbol-atlas): the objects the scenario places, made from its datums
and palettes when a map or structure bsp loads, and kept in step with the
game. object_placement_initialize and object_placement_initialize_for_new_map
(0xd4de0, 0xd4e20) are in unknown_0d4de0.cpp. */

#include "cseries.h"
#include "globals.h"
