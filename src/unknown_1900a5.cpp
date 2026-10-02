// @flags /O1 /Gr
/* UNKNOWN_1900A5.CPP: player slot query */

#include "cseries.h"
#include "unknown_19b516.h"

struct s_player_slot_head
{
	byte unknown00[0x470];
};

struct s_player_slot_flags
{
	byte unknown00[8];
	byte flags;
};

struct s_player_slot : s_player_slot_head, s_player_slot_flags
{
	byte unknown479[0xc70 - 0x479];
};

// the 16 player slots (the same array as g_54e8e0 in unknown_058cb0.cpp)
s_player_slot g_54e8e0_slots[16];

// @retail 0x1900a5
bool function_1900a5(long player)
{
	s_player_slot_flags *slot = &g_54e8e0_slots[player];
	return (slot->flags & 3) != 0;
}
