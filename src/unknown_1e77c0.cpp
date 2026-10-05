// @flags /O2 /Gr
/* UNKNOWN_1E77C0.CPP: recording a unit request in its player's local state
   (the e6900 callee) */

#include "unknown_11c920.h"
#include "globals.h"

/* a player (g_4e8c24, 0x21c bytes): +0x28 is its local player slot */
struct s_player_request_view
{
	byte unknown00[0x28];
	short local_player_index;
	byte unknown2a[0xc0 - 0x2a];
	char team;
	byte unknownc1[0x21c - 0xc1];
};

struct s_time_entry
{
	long time;
	short a;
	short b;
};

/* a local player's state in g_51e9c0 (src/unknown_1e6a40.cpp), 0x1b0 bytes:
   the four last unit requests at +0x150 */
struct s_local_player_state_view
{
	byte unknown000[0x150];
	s_time_entry requests[4];
	byte unknown170[0x198 - 0x170];
	byte timer198;
	byte timer199;
	byte timer19a;
	byte unknown19b[0x1b0 - 0x19b];
};

struct s_unknown_1e6a40;
extern s_unknown_1e6a40 *g_51e9c0;

void function_1e6980(s_time_entry *entries, short a, byte b);

// @retail 0x1e77c0
void function_1e77c0(long player_index, long type, byte result)
{
	s_player_request_view *player = (s_player_request_view *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c);

	if (player->local_player_index != NONE)
		function_1e6980(((s_local_player_state_view *)g_51e9c0)[player->local_player_index].requests, (short)type, result);
}

struct s_request_object
{
	byte field_0[0xaa];
	byte type;
	byte field_ab[0x138 - 0xab];
	short team;
};

struct s_request_object_header
{
	byte field_0[8];
	s_request_object *object;
};

bool function_1df560(short team_a, short team_b);

// @retail 0x1e8fa0
void function_1e8fa0(long player_index, long object_index, byte kind)
{
	s_player_request_view *player = (s_player_request_view *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c);
	s_request_object *object = ((s_request_object_header *)g_4e0300->data)[object_index & 0xffff].object;
	long local_index = player->local_player_index;
	if (local_index != NONE && ((1 << object->type) & 3) && function_1df560(player->team, object->team))
	{
		s_local_player_state_view *state = &((s_local_player_state_view *)g_51e9c0)[local_index];
		byte type = kind & 0x3f;
		if (type == 0x15 || type == 0x16)
			state->timer198 = (byte)g_510c54->field_2_3;
		else if (type == 3)
			state->timer199 = (byte)g_510c54->field_2_3;
		else if ((type >= 5 && type <= 0x13) || (type >= 0x27 && type <= 0x28))
			state->timer19a = (byte)g_510c54->field_2_3;
	}
}

struct s_parent_seat_object
{
	long definition_index;
	byte field_4[0x14 - 4];
	long parent_index;
	byte field_18[0x1fc - 0x18];
	short seat_index;
};

struct s_parent_seat_header
{
	short salt;
	byte flags;
	byte type;
	byte field_4[4];
	s_parent_seat_object *object;
};

struct s_parent_seat
{
	dword : 2;
	dword enabled : 1;
	dword : 29;
	byte field_4[0xb0 - 4];
};

struct s_parent_seat_definition
{
	byte field_0[0x1cc];
	s_parent_seat *seats;
	byte field_1d0[0x21a - 0x1d0];
	short type;
};

// @retail 0x1e8eb0
bool function_1e8eb0(long object_index, long type, bool any_nonzero)
{
	s_parent_seat_header *headers = (s_parent_seat_header *)g_4e0300->data;
	s_parent_seat_object *object = headers[object_index & 0xffff].object;
	bool result = false;
	const long *type_reference = &type;
	const bool *any_reference = &any_nonzero;
	if (object->parent_index != NONE && object->seat_index != NONE)
	{
		short seat_index = object->seat_index;
		s_parent_seat_header *parent = &headers[object->parent_index & 0xffff];
		if ((1 << parent->type) & 2)
		{
			s_parent_seat_definition *definition = (s_parent_seat_definition *)g_4e3b44[parent->object->definition_index & 0xffff].bytes;
			if (TEST_FIELD_BIT(definition->seats[seat_index].enabled))
			{
				if (definition->type == *type_reference || (*any_reference && definition->type != 0))
					result = true;
			}
		}
	}
	return result;
}
