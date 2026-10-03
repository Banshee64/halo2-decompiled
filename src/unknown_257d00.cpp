// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_257D00.CPP: the ai's command scripts: the "command scripts"
   (g_502408) an actor runs, chained from the actor's +0x858, and the "joint
   command scripts" (g_502404) that run one script on several actors */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"
#include "slot_handler.h"
#include "unknown_2551c0.h"
#include <string.h>

/* a command script (0xd4 bytes) */
struct s_cs_datum
{
	short salt;
	byte unknown02[2];
	short type;
	byte unknown06[0x28 - 0x6];
	long unknown28;
	long unknown2c;
	short state;
	byte unknown32[2];
	long script_index;
	long thread_index;
	long next_index;
	long joint_index;
	bool unknown44;
	bool unknown45;
	bool unknown46;
	byte unknown47;
	short unknown48;
	byte unknown4a[2];
	long unknown4c;
	byte unknown50;
	bool unknown51;
	bool unknown52;
	byte unknown53;
	short unknown54;
	byte unknown56[2];
	long unknown58;
	bool unknown5c;
	byte unknown5d[3];
	real unknown60;
	bool unknown64;
	byte unknown65[7];
	bool unknown6c;
	byte unknown6d[7];
	bool unknown74;
	bool unknown75;
	byte unknown76;
	bool unknown77;
	bool unknown78;
	bool unknown79;
	bool unknown7a;
	byte unknown7b;
	short unknown7c;
	bool unknown7e;
	bool unknown7f;
	bool unknown80;
	bool unknown81;
	bool unknown82;
	bool unknown83;
	short unknown84;
	bool unknown86;
	byte unknown87[5];
	bool unknown8c;
	byte unknown8d[3];
	long unknown90;
	long unknown94;
	byte unknown98;
	bool unknown99;
	short unknown9a;
	byte unknown9c[0xac - 0x9c];
	bool unknownac;
	byte unknownad[3];
	long unknownb0;
	byte unknownb4[0xd1 - 0xb4];
	bool unknownd1;
	byte unknownd2[2];
};

/* a participant of a joint command script */
struct s_joint_cs_participant
{
	long actor_index;
	long cs_index;
	short unknown8;
	byte unknowna[2];
};

/* a joint command script (0x8c bytes) */
struct s_joint_cs_datum
{
	short salt;
	byte unknown02[2];
	long script_index;
	short unknown08;
	short participant_count;
	long thread_index;
	s_joint_cs_participant participants[10];
	short leader;
	bool unknown8a;
	byte unknown8b;
};

/* the actor fields the command scripts keep */
struct s_actor_cs_view
{
	byte unknown000[0x1c4];
	struct
	{
		short type;
		byte unknown2[2];
		short unknown4;
		short unknown6;
		short unknown8;
		short timer;
	} entries[3];
	byte unknown1e8[0x3b8 - 0x1e8];
	bool unknown3b8;
	byte unknown3b9[0x858 - 0x3b9];
	long first_cs_index;
	long current_cs_index;
	byte unknown860[0x888 - 0x860];
};

/* the scenario's command script point sets (g_4e0350 +0x1d8) */
struct s_cs_point_set
{
	byte unknown00[0x20];
	long point_count;
	byte unknown24[0x30 - 0x24];
};

struct s_cs_scenario_data
{
	long point_set_count;
	s_cs_point_set *point_sets;
};

struct s_cs_scenario_view
{
	byte unknown000[0x1d8];
	long script_data_count;
	s_cs_scenario_data *script_data;
};

/* the state a command script keeps per participant (0x24 bytes) */
struct s_cs_state
{
	byte unknown0[3];
	byte flags3;
	short unknown4;
	byte unknown6[2];
	short unknown8;
	byte unknowna[0x24 - 0xa];
};

typedef short (__stdcall *cs_iterate_proc)(long actor_index, long object_index, s_cs_state *state, long cs_index);

extern s_data_array *g_502404;
extern s_data_array *g_4f9384;

void __stdcall function_1f4280(long actor_index);
long function_209520(short script_index);
void function_267770(long prop_index, long actor_index);

short function_258b60(long actor_index, cs_iterate_proc proc, long cs_index);
short __stdcall function_258cc0(long actor_index, long object_index, s_cs_state *state, long cs_index);
short __stdcall function_259d90(long actor_index, long object_index, s_cs_state *state, long cs_index);
void function_259e70(long cs_index);
void function_258540(long actor_index, long cs_index);
void function_258480(long joint_index, long actor_index);
void function_2583e0(long joint_index);

inline s_cs_datum *cs_get(long cs_index)
{
	return (s_cs_datum *)(g_502408->data + (cs_index & 0xffff) * sizeof(s_cs_datum));
}

inline s_joint_cs_datum *joint_cs_get(long joint_index)
{
	return (s_joint_cs_datum *)(g_502404->data + (joint_index & 0xffff) * sizeof(s_joint_cs_datum));
}

inline s_actor_cs_view *actor_cs_get(long actor_index)
{
	return (s_actor_cs_view *)(g_4f55f0->data + (actor_index & 0xffff) * sizeof(s_actor_cs_view));
}

// @retail 0x257d00
void function_257d00(void)
{
	g_502408 = data_new_inlined("command scripts", 0x28, sizeof(s_cs_datum), 0, g_510c2c);
	g_502404 = data_new_inlined("joint command scripts", 10, sizeof(s_joint_cs_datum), 0, g_510c2c);
}

// @retail 0x257d80
long function_257d80(long thread_index, short script_index)
{
	long cs_index = datum_new(g_502408);

	if (cs_index != NONE)
	{
		s_cs_datum *cs = cs_get(cs_index);

		cs->script_index = script_index;
		if (thread_index != NONE)
		{
			cs->thread_index = thread_index;
		}
		else
		{
			cs->thread_index = function_209520(script_index);
		}

		cs->unknown45 = false;
		cs->unknown46 = false;
		cs->unknown51 = false;
		cs->unknown52 = false;
		cs->unknown5c = false;
		cs->unknown64 = false;
		cs->unknown6c = false;
		cs->unknown74 = false;
		cs->unknown75 = false;
		cs->unknown7e = false;
		cs->unknown79 = false;
		cs->unknown7a = false;
		cs->unknown7f = false;
		cs->unknown80 = false;
		cs->unknown81 = false;
		cs->unknown82 = false;
		cs->unknown83 = false;
		cs->unknown84 = 0;
		cs->unknown86 = false;
		cs->unknown8c = false;
		cs->unknown99 = false;
		cs->unknown77 = false;
		cs->unknown78 = false;
		cs->state = 1;
		cs->joint_index = NONE;
		cs->next_index = NONE;
		cs->unknown60 = 0.f;
		cs->unknown7c = 10;
		cs->unknown90 = NONE;
		cs->unknown94 = NONE;
		cs->unknown9a = NONE;
		function_259e70(cs_index);
	}

	return cs_index;
}

// @retail 0x257e70
long function_257e70(short script_index)
{
	long joint_index = datum_new(g_502404);

	if (joint_index != NONE)
	{
		s_joint_cs_datum *joint = joint_cs_get(joint_index);

		joint->participant_count = 0;
		joint->script_index = script_index;
		joint->thread_index = function_209520(script_index);
		if (joint->thread_index == NONE)
		{
			datum_delete(g_502404, joint_index);
			return NONE;
		}
	}

	return joint_index;
}

// @retail 0x257fa0
long function_257fa0(long actor_index, short script_index, long thread_index)
{
	s_actor_cs_view *actor = actor_cs_get(actor_index);
	long cs_index = function_257d80(thread_index, script_index);

	if (cs_index != NONE)
	{
		s_cs_datum *cs = cs_get(cs_index);

		if (actor->first_cs_index != NONE)
		{
			s_cs_datum *first = cs_get(actor->first_cs_index);
			if (first->state == 0)
			{
				first->state = 3;
			}
		}

		cs->next_index = actor->first_cs_index;
		actor->first_cs_index = cs_index;
		actor->current_cs_index = cs_index;
		function_258b60(actor_index, function_258cc0, cs_index);
	}

	return cs_index;
}

// @retail 0x258040
long function_258040(long actor_index, short script_index, long thread_index)
{
	s_actor_cs_view *actor = actor_cs_get(actor_index);
	long cs_index = function_257d80(thread_index, script_index);

	if (cs_index != NONE)
	{
		long *link = &actor->first_cs_index;

		while (*link != NONE)
		{
			link = &cs_get(*link)->next_index;
		}
		*link = cs_index;

		if (actor->first_cs_index == cs_index)
		{
			actor->current_cs_index = cs_index;
		}
	}

	return cs_index;
}

// @retail 0x258340
bool function_258340(short participant_index, long joint_index)
{
	s_joint_cs_datum *joint = joint_cs_get(joint_index);

	if (participant_index < joint->participant_count)
	{
		long cs_index = joint->participants[joint->leader].cs_index;
		if (cs_index != NONE)
		{
			cs_get(cs_index)->unknown44 = true;
		}

		cs_index = joint->participants[participant_index].cs_index;
		if (cs_index != NONE)
		{
			cs_get(cs_index)->unknown44 = false;
			joint->leader = participant_index;
			return true;
		}

		function_2583e0(joint_index);
		return false;
	}

	return true;
}

// @retail 0x2583e0
void function_2583e0(long joint_index)
{
	s_joint_cs_datum *joint = joint_cs_get(joint_index);

	for (short i = 0; i < joint->participant_count; i++)
	{
		s_joint_cs_participant *participant = &joint->participants[i];

		if (participant->actor_index != NONE)
		{
			long cs_index = participant->cs_index;
			s_cs_datum *cs = cs_get(cs_index);

			cs->thread_index = NONE;
			cs->joint_index = NONE;
			function_258540(participant->actor_index, cs_index);
		}
	}

	if (joint->thread_index != NONE)
	{
		datum_delete(g_4f9384, joint->thread_index);
	}
	datum_delete(g_502404, joint_index);
}

// @retail 0x258480
void function_258480(long joint_index, long actor_index)
{
	s_joint_cs_datum *joint = joint_cs_get(joint_index);
	bool remaining = false;

	for (short i = 0; i < joint->participant_count; i++)
	{
		s_joint_cs_participant *participant = &joint->participants[i];

		if (participant->cs_index != NONE)
		{
			s_cs_datum *cs = cs_get(participant->cs_index);

			if (participant->actor_index == actor_index)
			{
				cs->joint_index = NONE;
				participant->actor_index = NONE;
				participant->cs_index = NONE;
			}
			else if (participant->actor_index != NONE)
			{
				remaining = true;
				cs->unknown7e = true;
			}
		}
	}

	if (!remaining)
	{
		if (joint->thread_index != NONE)
		{
			datum_delete(g_4f9384, joint->thread_index);
		}
		datum_delete(g_502404, joint_index);
	}
}

// @retail 0x258540
void function_258540(long actor_index, long cs_index)
{
	s_cs_datum *cs = cs_get(cs_index);
	s_actor_cs_view *actor = actor_cs_get(actor_index);

	if (cs->joint_index != NONE)
	{
		function_258480(cs->joint_index, actor_index);
	}
	else if (cs->thread_index != NONE)
	{
		datum_delete(g_4f9384, cs->thread_index);
	}

	long *link = &actor->first_cs_index;
	while (*link != NONE)
	{
		if (*link == cs_index)
		{
			*link = cs->next_index;
			break;
		}
		link = &cs_get(*link)->next_index;
	}

	if (actor->current_cs_index == cs_index)
	{
		actor->current_cs_index = NONE;
	}
	datum_delete(g_502408, cs_index);
}

// @retail 0x258600
void function_258600(long actor_index)
{
	long cs_index = actor_cs_get(actor_index)->first_cs_index;

	while (cs_index != NONE)
	{
		long next_index = cs_get(cs_index)->next_index;
		function_258540(actor_index, cs_index);
		cs_index = next_index;
	}
}

// @retail 0x258b20
void function_258b20(long index, long actor_index)
{
	s_cs_datum *cs = cs_get(index);

	function_258b60(actor_index, function_259d90, index);
	cs->state = 3;
}

// @retail 0x258cc0
short __stdcall function_258cc0(long actor_index, long object_index, s_cs_state *state, long cs_index)
{
	memset(state, 0, sizeof(s_cs_state));
	state->unknown4 = 0;
	return 0;
}

// @retail 0x259e70
void function_259e70(long cs_index)
{
	s_cs_datum *cs = cs_get(cs_index);

	if (cs->unknownac || cs->unknownb0 != NONE)
	{
		cs->unknownac = false;
		cs->unknownb0 = NONE;
		cs->unknownd1 = false;
		cs->unknown52 = false;
		cs->unknown46 = false;
	}
}

// @retail 0x25aa10
void function_25aa10(long actor_index, long object_index)
{
	long cs_index = actor_cs_get(actor_index)->first_cs_index;

	while (cs_index != NONE)
	{
		s_cs_datum *cs = cs_get(cs_index);

		if (cs->unknown46 && cs->unknown48 == 1 && cs->unknown4c == object_index)
		{
			cs->unknown46 = false;
			cs->unknown4c = NONE;
		}
		if (cs->unknown52 && cs->unknown54 == 1 && cs->unknown58 == object_index)
		{
			cs->unknown52 = false;
			cs->unknown58 = NONE;
		}
		if (cs->unknownb0 == object_index)
		{
			function_259e70(cs_index);
		}
		cs_index = cs->next_index;
	}
}

// @retail 0x25aad0
bool function_25aad0(long actor_index, real *value)
{
	s_actor_view *actor = actor_get(actor_index);
	long cs_index = ((s_actor_cs_view *)actor)->first_cs_index;
	bool result = false;

	if (cs_index != NONE && !actor->unknown007)
	{
		s_cs_datum *cs = cs_get(cs_index);

		if ((cs->state == 0 || cs->state == 1) && cs->unknown60 > 0.f)
		{
			*value = cs->unknown60;
			result = true;
		}
	}
	return result;
}

// @retail 0x25ab50
bool function_25ab50(long reference)
{
	bool result = false;

	if (reference != NONE)
	{
		s_cs_scenario_view *scenario = (s_cs_scenario_view *)g_4e0350;

		if (scenario->script_data_count > 0)
		{
			long set_index = (reference >> 16) & 0xffff;

			if (set_index >= 0)
			{
				s_cs_scenario_data *data = scenario->script_data;

				if (set_index < data->point_set_count)
				{
					long point_index = reference & 0xffff;

					if (point_index >= 0 && point_index < data->point_sets[set_index].point_count)
					{
						result = true;
					}
				}
			}
		}
	}
	return result;
}

// @retail 0x25aba0
void function_25aba0(long actor_index)
{
	s_actor_cs_view *actor = actor_cs_get(actor_index);

	actor->unknown3b8 = false;
	long i = 0;
	do
	{
		if (actor->entries[i].type != NONE)
		{
			actor->entries[i].timer--;
			if (actor->entries[i].timer <= 0)
			{
				actor->entries[i].type = NONE;
				actor->entries[i].unknown4 = NONE;
				actor->entries[i].unknown6 = NONE;
				actor->entries[i].timer = 0;
			}
		}
		i++;
	}
	while (i < 3);
}

// @retail 0x258b60
short function_258b60(long actor_index, cs_iterate_proc proc, long cs_index)
{
	s_actor_view *actor = actor_get(actor_index);

	if (!actor->unknown007)
	{
		short result = proc(actor_index, actor->unknown018, (s_cs_state *)((byte *)actor + 0x864), cs_index);
		*(short *)((byte *)actor + 0x868) = result;
		return result;
	}

	long perception_index = ((s_handler_actor_view *)actor)->perception_index;
	if (perception_index == NONE)
	{
		return 2;
	}

	short succeeded = 0;
	short failed = 0;
	short count = 0;
	s_ai_object_iterator iterator;
	s_handler_object_view *object;

	iterator.next_index = perception_get(perception_index)->object_index;
	iterator.index = NONE;
	while ((object = function_290c80(&iterator)) != NULL)
	{
		byte *data;
		if (!object->flags134 && (data = (byte *)object + object->ai_offset) != NULL)
		{
			short result = proc(actor_index, iterator.index, (s_cs_state *)(data + 0x28), cs_index);
			*(short *)(data + 0x2c) = result;
			if (result == 2)
			{
				succeeded++;
			}
			else if (result == 1)
			{
				failed++;
			}
			count++;
		}
	}

	if ((real)(failed + succeeded) >= (real)count * 0.8f)
	{
		return (succeeded > failed) + 1;
	}
	return 0;
}

// @retail 0x259d90
short __stdcall function_259d90(long actor_index, long object_index, s_cs_state *state, long cs_index)
{
	s_actor_view *actor = actor_get(actor_index);

	switch (cs_get(cs_index)->type)
	{
	case 1:
	case 2:
	case 3:
	case 15:
	case 16:
	case 17:
		if (actor->unknown018 != NONE)
		{
			function_1f4280(actor_index);
			return 1;
		}
		state->flags3 &= ~0x40;
		break;
	case 4:
		state->flags3 &= ~1;
		state->unknown8 = NONE;
		break;
	case 7:
	case 8:
		state->flags3 &= ~4;
		state->unknown8 = 0;
		break;
	case 6:
	case 14:
	case 18:
		state->flags3 &= ~0x20;
		break;
	}
	return 1;
}