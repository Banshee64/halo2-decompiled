/* UNKNOWN_0B68C0.H: the list of object type definitions the initialize runs
   through (g_4e0330, defined in unknown_0b68c0.cpp) */

#ifndef UNKNOWN_0B68C0_H
#define UNKNOWN_0B68C0_H

#include "cseries.h"

struct s_callback_node
{
	byte unknown00[0x14];
	void (*callback)(void);
	byte unknown18[0xac];
	s_callback_node *next;
};

extern s_callback_node *g_4e0330;

#endif
