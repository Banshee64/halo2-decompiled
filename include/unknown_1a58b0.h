/* UNKNOWN_1A58B0.H: the actor action handlers, candidate lists and slot-owner
   views of unknown_1a58b0.cpp (guessed layouts; only the fields used are
   named). The handler table g_46eeb8 itself is defined in unknown_1a8080.cpp. */

#ifndef UNKNOWN_1A58B0_H
#define UNKNOWN_1A58B0_H

#include "cseries.h"

struct s_actor_slot
{
	short type;
	short state;
	short unknown4;
	byte unknown6[2];
	long time;
	byte unknownc[0x34];
};

/* an element of the actor slot-owner data array g_4f55f0 (0x888 bytes) */
struct s_actor_owner
{
	byte unknown00[0x30];
	long unknown30;
	byte unknown34[0x20];
	long tag_index;
	byte unknown58[0x2e];
	byte unknown86;
	byte unknown87[9];
	s_actor_slot slots[4];
	short current;
	byte unknown192[0x56];
	long times[14];
	byte unknown220[0x266 - 0x220];
	byte unknown266;
	byte unknown267;
	byte unknown268;
	byte unknown269;
	byte unknown26a[2];
	long unknown26c;
	byte unknown270[0x858 - 0x270];
	long unknown858;
	byte unknown85c[0x888 - 0x85c];
};

/* a registered action: the mask of the build features it needs, the
   mismatching build tag, and the query procedures (a handler of kind 0 takes
   the extra argument) */
struct s_slot_handler
{
	short unknown0;
	short kind;
	dword unknown4;
	long unknown8;
	long unknownc;
	union
	{
		short (__stdcall *query1)(long);
		short (__stdcall *query2)(long, long);
	};
	short (__stdcall *proc14)(long, s_actor_slot *, long);
};

/* a node of the list that candidate actions are linked into */
struct s_action_node
{
	short key;
	byte unknown02[10];
	s_action_node *next;
	short order;
	byte unknown12[2];
};

struct s_candidate_entry
{
	short kind;
	short key;
	s_action_node node;
};

struct s_candidate_list
{
	byte unknown00[4];
	s_candidate_entry *entries;
	short count;
	byte unknown0a[2];
};

/* three rows of one candidate entry per handler */
struct s_candidate_table
{
	s_candidate_entry entries[0x83];
};

/* five dwords: a set of 0x83 bits */
struct s_flag_bits
{
	dword d[5];
};

/* the sort settings (g_51e99c): flags at +0x14 */
struct s_sort_globals
{
	byte unknown00[0x14];
	dword flags;
};

#endif
