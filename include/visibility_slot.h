/* VISIBILITY_SLOT.H: the table of visibility test slots (g_51f40c; 020560,
   020670) */

#ifndef VISIBILITY_SLOT_H
#define VISIBILITY_SLOT_H

#include "cseries.h"

/* a slot of the visibility table (g_51f40c, 32 bytes each; the data begins at
   +8, the release is 020e50) */
struct s_slot
{
	dword a : 1;
	dword b : 2;
	dword c : 3;
	dword d : 12;
	dword e : 6;
	dword f : 8;
	dword used : 1;
	dword valid : 1;
	dword size : 5;
	dword j : 9;
	dword k : 16;
	byte data[24];
};

/* 511 slots: the table ends where g_5233ec begins */
extern s_slot g_51f40c[511];
extern dword g_5233f0[3][16];

#endif
