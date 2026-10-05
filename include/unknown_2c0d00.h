/* UNKNOWN_2C0D00.H: the list of circular obstacles on the ground plane that
   a path is steered around (unknown_2c0d00.cpp) */

#ifndef UNKNOWN_2C0D00_H
#define UNKNOWN_2C0D00_H

#include "unknown_11c920.h"
#include "unknown_0259d0.h"

/* an obstacle (0x14 bytes) */
struct s_obstacle
{
	word flags;
	/* the group of overlapping obstacles it is in */
	short group;
	long object_index;
	point2f center;
	real radius;
};

/* the obstacles (0x50c bytes) */
struct s_obstacle_list
{
	short group_count;
	short count;
	short flag0_count;
	short flag3_count;
	byte unknown08[4];
	s_obstacle obstacles[64];
};

short obstacle_list_find_containing(s_obstacle_list const *list, short ignore_index, point2f const *point, real radius);

#endif
