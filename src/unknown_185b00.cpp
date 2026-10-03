// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_185B00.CPP: the per player control state (g_4ed284, allocated by
   185ab0): flags, then four entries of 0x94 bytes, one per local player,
   holding the unit, its facing and the pitch bounds */

#include "cseries.h"
#include "globals.h"
#include <math.h>
#include <string.h>

#define k_maximum_local_players 4

struct s_player_control_target
{
	long a;
	long b;
	long c;
};

struct s_player_control_entry
{
	long unit_index;
	byte unknown04[8];
	short value0c;
	short value0e;
	real yaw;
	real pitch;
	byte unknown18[0x10];
	short value28;
	char value2a;
	char value2b;
	short value2c;
	short value2e;
	byte value30;
	byte unknown31[0x13];
	long value44;
	long value48;
	long player_index;
	byte unknown50[0xc];
	short value5c;
	byte unknown5e[2];
	real value60;
	real value64;
	byte unknown68[4];
	s_player_control_target target;
	byte unknown78[8];
	real minimum_pitch;
	real maximum_pitch;
	byte value88;
	byte value89;
	byte unknown8a[0xa];
};

struct s_player_control_globals
{
	bool initialized;
	byte unknown01[3];
	dword flags4;
	dword flags8;
	dword flagsc;
	dword flags10;
	s_player_control_entry entries[k_maximum_local_players];
};

struct s_unknown_185ab0;
extern s_unknown_185ab0 *g_4ed284;

struct s_player_datum
{
	short salt;
	byte unknown02[0x2a];
	long unit_index;
};

struct s_control_object
{
	byte unknown00[0x150];
	real_vector3d forward;
	byte unknown15c[0x214 - 0x15c];
	byte value214[4];
	byte unknown218[0x23d - 0x218];
	char value23d;
	byte unknown23e[3];
	char value241;
};

struct s_object_header
{
	byte unknown00[8];
	s_control_object *object;
};

static inline s_player_control_globals *player_control_globals(void)
{
	return (s_player_control_globals *)g_4ed284;
}

static inline byte *datum_try_and_get(s_data_array *data, long datum_index)
{
	byte *result = 0;

	if (datum_index != NONE)
	{
		long index = datum_index & 0xffff;

		if (index < data->high_water_index)
		{
			byte *datum = data->data + data->size * index;
			short salt = *(short *)datum;

			if (salt != 0 && salt == (datum_index >> 16))
			{
				result = datum;
			}
		}
	}

	return result;
}

static inline void vector_to_angles(real_vector3d const *vector, real *yaw, real *pitch)
{
	*yaw = (real)atan2(vector->j, vector->i);
	*pitch = (real)atan2(vector->k, sqrt(vector->i * vector->i + vector->j * vector->j));
}

static __forceinline void player_control_entry_set_facing(s_player_control_entry *entry, real_vector3d const *forward)
{
	vector_to_angles(forward, &entry->yaw, &entry->pitch);
	if (entry->yaw < 0.0f)
	{
		entry->yaw += 6.2831855f;
	}
}

static inline void player_control_target_clear(s_player_control_target *target)
{
	target->a = 0;
	target->b = 0;
	target->c = 0;
}

static inline void player_control_target_reset(s_player_control_target *target)
{
	target->a = 0;
	target->b = 0;
}

// @retail 0x1874b0
void player_control_set_facing(long player_index, real_vector3d const *forward)
{
	player_control_entry_set_facing(&player_control_globals()->entries[player_index], forward);
}

// @retail 0x1872d0
void player_control_set_unit(long player_index, long unit_index)
{
	if (player_index != NONE)
	{
		s_player_control_globals *globals = player_control_globals();
		s_player_control_entry *entry = &globals->entries[player_index];

		if (!globals->initialized || entry->unit_index != unit_index)
		{
			memset(entry, 0, sizeof(*entry));
			entry->maximum_pitch = 1.4922565f;
			entry->value89 = 0;
			entry->value88 = 0;
			entry->value30 = 0;
			entry->minimum_pitch = -1.4922565f;
			entry->value0c = 0;
			entry->value0e = 0;
			entry->unit_index = unit_index;
			entry->value28 = NONE;
			entry->value2a = NONE;
			entry->value2b = NONE;
			entry->value2c = NONE;
			entry->value2e = NONE;
			entry->value5c = 0;
			entry->value60 = 0.0f;
			entry->value64 = 0.0f;
			entry->value44 = NONE;
			entry->value48 = NONE;
			entry->player_index = NONE;
			player_control_target_clear(&entry->target);
			player_control_target_reset(&entry->target);
			entry->target.b = NONE;
			entry->target.c = NONE;

			if (unit_index != NONE)
			{
				s_control_object *object = ((s_object_header *)g_4e0300->data)[unit_index & 0xffff].object;

				player_control_entry_set_facing(entry, &object->forward);
				*(long *)&entry->value28 = *(long *)object->value214;
				entry->value2c = object->value23d;
				entry->value2e = object->value241;
			}
		}
	}
}

// @retail 0x185b00
void function_185b00(void)
{
	s_player_control_globals *globals = player_control_globals();

	globals->flags8 = 0;
	globals->flags4 = 0;
	globals->flagsc = 0;
	globals->flags10 = 0;
	for (long i = 0; i < k_maximum_local_players; i++)
	{
		player_control_set_unit(i, NONE);
	}
	globals->initialized = true;
}

// @retail 0x185b30
real function_185b30(short count, real const *values, real t)
{
	bool negative = t < 0.0f;
	long last = count - 1;
	real position = (real)(fabs(t) * last);

	if (0.0f > position)
	{
		position = 0.0f;
	}
	else if (position > (real)count - 1.0f)
	{
		position = (real)count - 1.0f;
	}

	short index = (short)position;
	index = (short)(index < 0 ? 0 : (index > last ? last : index));

	short next = (short)(index + 1 <= last ? index + 1 : last);
	real result = (values[next] - values[index]) * (position - index) + values[index];

	if (negative)
	{
		result = 0.0f - result;
	}
	return result;
}

// @retail 0x187420
real function_187420(long player_index)
{
	s_player_control_globals *globals = player_control_globals();

	return globals->entries[player_index].value60 > globals->entries[player_index].value64 ? globals->entries[player_index].value60 : globals->entries[player_index].value64;
}

// @retail 0x187450
long function_187450(long player_index)
{
	s_player_control_entry *entry = &player_control_globals()->entries[player_index];
	long datum_index = entry->player_index;
	long result = NONE;

	if (datum_index != NONE)
	{
		s_player_datum *player = (s_player_datum *)datum_try_and_get(g_4e8c24, datum_index);
		if (player && player->unit_index != NONE)
		{
			result = datum_index;
		}
	}
	return result;
}

// @retail 0x187a40
short function_187a40(long player_index)
{
	short result = NONE;

	if (player_index != NONE)
	{
		result = player_control_globals()->entries[player_index].value2e;
	}
	return result;
}

// @retail 0x187a60
void function_187a60(long datum_index)
{
	for (long i = 0; i < k_maximum_local_players; i++)
	{
		s_player_control_entry *entry = &player_control_globals()->entries[i];

		if (entry->value44 == datum_index)
		{
			entry->value60 = 0.0f;
			entry->value64 = 0.0f;
			entry->value44 = NONE;
			entry->value48 = NONE;
			entry->player_index = NONE;
			entry->value5c = 0;
		}
	}
}

/* the actions a player's controls ask for in a tick */
struct s_player_action_flags1c
{
	dword bit0 : 1;
	dword bit1 : 1;
	dword bit2 : 1;
	dword bit3 : 1;
	dword bit4 : 1;
	dword bit5 : 1;
	dword unknown : 26;
};

struct s_player_action_flags20
{
	word bit0 : 1;
	word bit1 : 1;
	word bit2 : 1;
	word bit3 : 1;
	word unknown : 12;
};

struct s_player_action
{
	real throttle_i;
	real throttle_j;
	real trigger;
	byte unknown0c[4];
	real pitch;
	real yaw;
	dword flags18;
	union
	{
		dword flags1c;
		s_player_action_flags1c bits1c;
	};
	union
	{
		word flags20;
		s_player_action_flags20 bits20;
	};
	byte unknown22[2];
	real zoom;
};

struct s_player_action_triggers
{
	byte unknown00[8];
	byte left;
	byte right;
};

#define FLAG(bit) (1 << (bit))
#define SET_FLAG(flags, bit, value) ((value) ? ((flags) |= FLAG(bit)) : ((flags) &= ~FLAG(bit)))

// @retail 0x187b30
void player_control_update_action_flags(s_player_action *action, s_player_action_triggers const *triggers)
{
	if (action->flags20 & FLAG(2))
	{
		g_4ed284->flags4 |= FLAG(0);
	}
	if (action->flags18 & FLAG(1))
	{
		g_4ed284->flags4 |= FLAG(1);
	}
	if (action->flags18 & FLAG(26))
	{
		g_4ed284->flags4 |= FLAG(5);
	}
	if (action->flags18 & FLAG(2))
	{
		g_4ed284->flags4 |= FLAG(20);
	}
	if (action->flags18 & FLAG(5))
	{
		g_4ed284->flags4 |= FLAG(6);
	}
	if (action->flags1c & FLAG(4))
	{
		g_4ed284->flags4 |= FLAG(2);
	}
	if (action->flags1c & FLAG(5))
	{
		g_4ed284->flags4 |= FLAG(3);
	}
	if (action->flags1c & FLAG(2))
	{
		g_4ed284->flags4 |= FLAG(9);
	}
	if (action->flags1c & FLAG(0))
	{
		g_4ed284->flags4 |= FLAG(7);
	}
	if (action->flags1c & FLAG(1))
	{
		g_4ed284->flags4 |= FLAG(8);
	}
	if (action->trigger > 0.0f)
	{
		g_4ed284->flags4 |= FLAG(4);
	}
	if (triggers->left > 0)
	{
		g_4ed284->flags4 |= FLAG(18);
	}
	if (triggers->right > 0)
	{
		g_4ed284->flags4 |= FLAG(19);
	}
	if (action->yaw > 0.0f)
	{
		g_4ed284->flags4 |= FLAG(10);
	}
	else if (action->yaw < 0.0f)
	{
		g_4ed284->flags4 |= FLAG(11);
	}
	if (action->pitch > 0.0f)
	{
		g_4ed284->flags4 |= FLAG(12);
	}
	else if (action->pitch < 0.0f)
	{
		g_4ed284->flags4 |= FLAG(13);
	}
	if (action->throttle_i > 0.0f)
	{
		g_4ed284->flags4 |= FLAG(14);
	}
	else if (action->throttle_i < 0.0f)
	{
		g_4ed284->flags4 |= FLAG(15);
	}
	if (action->throttle_j > 0.0f)
	{
		g_4ed284->flags4 |= FLAG(16);
	}
	else if (action->throttle_j < 0.0f)
	{
		g_4ed284->flags4 |= FLAG(17);
	}
	if (action->zoom > 0.0f)
	{
		g_4ed284->flags4 |= FLAG(23);
	}
	else if (action->zoom < 0.0f)
	{
		g_4ed284->flags4 |= FLAG(24);
	}

	s_unknown_185ab0 *globals = g_4ed284;
	if (TEST_FIELD_BIT(globals->bits8.bit0))
	{
		action->flags20 = (action->flags20 & ~(FLAG(1) | FLAG(2))) | FLAG(3);
	}
	else if (TEST_FIELD_BIT(g_4ed284->bitsc.bit0))
	{
		SET_FLAG(globals->flagsc, 0, TEST_FIELD_BIT(action->bits20.bit2));
		action->flags20 = (action->flags20 & ~(FLAG(1) | FLAG(2))) | FLAG(3);
	}

	if (TEST_FIELD_BIT(globals->bits8.bit2))
	{
		action->flags1c &= ~FLAG(4);
	}
	else if (TEST_FIELD_BIT(g_4ed284->bitsc.bit2))
	{
		SET_FLAG(globals->flagsc, 2, TEST_FIELD_BIT(action->bits1c.bit4));
		action->flags1c &= ~FLAG(4);
	}

	if (TEST_FIELD_BIT(globals->bits8.bit3))
	{
		action->flags1c &= ~FLAG(5);
	}
	else if (TEST_FIELD_BIT(g_4ed284->bitsc.bit3))
	{
		SET_FLAG(globals->flagsc, 3, TEST_FIELD_BIT(action->bits1c.bit5));
		action->flags1c &= ~FLAG(5);
	}

	s_185ab0_flags bits = globals->bits4;
	if (TEST_FIELD_BIT(bits.bit21))
	{
		action->yaw = (real)fabs(action->yaw);
	}
	else if (TEST_FIELD_BIT(bits.bit22))
	{
		action->yaw = -(real)fabs(action->yaw);
	}

	if (TEST_FIELD_BIT(globals->bitsc.bit12))
	{
		action->pitch = action->pitch > 0.0f ? 0.0f : action->pitch;
	}
	if (TEST_FIELD_BIT(globals->bitsc.bit13))
	{
		action->pitch = action->pitch > 0.0f ? action->pitch : 0.0f;
	}
	if (TEST_FIELD_BIT(globals->bitsc.bit10))
	{
		action->yaw = action->yaw > 0.0f ? 0.0f : action->yaw;
	}
	if (TEST_FIELD_BIT(globals->bitsc.bit11))
	{
		action->yaw = action->yaw > 0.0f ? action->yaw : 0.0f;
	}
}
