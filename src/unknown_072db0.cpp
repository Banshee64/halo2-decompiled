// @flags /O2 /Gr
#include <string.h>
#include "unknown_11c920.h"
#include "unknown_0662e0.h"
#include "unknown_059ad0.h"
#include "globals.h"
#include <xtl.h>
#include <xonline.h>

/* The two vtables at 0x450b44 (slots 0..8 and 9..16) belong to two small
   helper classes sharing one layout; only the slots decompiled so far have
   bodies, the others are placeholders. */

struct s_helper_game
{
	byte unknown00[0x18];
	long l18;
	byte unknown1c[0x1118 - 0x1c];
	long l1118;
	byte unknown111c[0x4994 - 0x111c];
	long l4994;
	byte unknown4998[0x741c - 0x4998];
	long mode;
};

struct s_helper_source
{
	long type;
	byte unknown04[0x2c];
	s_helper_game *game;
	long unknown34;
	c_class_58d20 *other_game;
};

struct s_helper_output
{
	long l0;
	long l4;
	long l8;
	long lc;
	long l10;
	long l14;
	long l18;
	long l1c;
	long l20;
	long l24;
	long l28;
};


struct s_online_match_session_info
{
	XNKEY key;
	XNKID session_id;
	XNADDR address;
	s_helper_output output;
};

class c_helper_a
{
public:
	virtual void v0(long a, long b);
	virtual void v1();
	virtual void v2() {}
	virtual void v3(s_helper_output *out);
	virtual long v4();
	virtual long v5(long size, void *data) { return 0; }
	virtual long v6();
	virtual bool v7();
	virtual void v8(long a);

	s_helper_source *source;
	bool b8;
	bool b9;
	bool ba;
	byte unknownb;
	long lc;
	long l10;
	long l14;
	long l18;
	long l1c;
	long l20;
	bool listening;
	byte unknown25[3];
	long listen_time;
	long l2c;
	bool b30;
	byte unknown31[3];
	s_online_match_session_info info;
};

class c_helper_b
{
public:
	virtual void v0(long a, long b) {}
	virtual void v1() {}
	virtual void v2() {}
	virtual void v3() {}
	virtual long v4();
	virtual void v5() {}
	virtual long v6();
	virtual bool v7();
	s_helper_source *source;
	bool b8;
	bool b9;
};

// @retail 0x72db0
void c_helper_a::v0(long a, long b)
{
	source = (s_helper_source *)a;
	lc = NONE;
	l14 = NONE;
	l1c = NONE;
	ba = 0;
	l10 = 0;
	l18 = 0;
	l20 = 0;
	l2c = b;
	b30 = 0;
	b9 = 0;
	b8 = 1;
}

// @retail 0x73750
void c_helper_a::v8(long a)
{
	source = (s_helper_source *)a;
	ba = 0;
	lc = NONE;
	l10 = 0;
	l14 = NONE;
	l18 = 0;
	l1c = NONE;
	l20 = 0;
	l2c = 3;
	b30 = 0;
	b9 = 0;
	b8 = 1;
}

// @retail 0x73790
long c_helper_a::v4()
{
	return 60000;
}

// @retail 0x73820
bool c_helper_a::v7()
{
	bool result = false;

	if (b9)
	{
		if (source->type != 0)
		{
			s_helper_game *game = source->game;

			long mode = game->mode;

			if (mode == 5 || mode == 6 || mode == 7 || mode == 8)
			{
				if (game->l18 != 0)
					result = true;
			}
		}
	}
	return result;
}

// @retail 0x73860
void c_helper_a::v3(s_helper_output *out)
{
	s_helper_game *game = source->game;

	memset(out, 0, sizeof(s_helper_output));
	out->l0 = out->l4 = out->l8 = out->lc = 0;
	out->l10 = NONE;
	out->l14 = NONE;
	long bits = 16;
	if (game->mode > 2 && game->mode <= 8)
		bits = game->l4994;
	bits -= game->l1118;
	out->l24 = 0;
	out->l1c = NONE;
	out->l20 = NONE;
	out->l18 = bits;
	out->l28 = (1 << bits) - 1;
}

// @retail 0x738d0
long c_helper_a::v6()
{
	long result = g_network_configuration.valueca0;
	long type = source->type;

	if (type == 3 || type == 8)
		result = g_network_configuration.valueca4;
	return result;
}

// @retail 0x73350
long c_helper_b::v4()
{
	return g_network_configuration.valuec94;
}

// @retail 0x73470
long c_helper_b::v6()
{
	return g_network_configuration.valuec9c;
}

void function_6b640(long task_index);

// @retail 0x73310
void c_helper_a::v1()
{
	if (lc != NONE)
	{
		function_6b640(lc);
		lc = NONE;
	}
	if (l14 != NONE)
	{
		function_6b640(l14);
		l14 = NONE;
	}
	b8 = false;
}

bool online_match_session_get_info(long task_index, s_online_match_session_info *info);
bool transport_security_register_key(long index, long local, bool host, const XNKID *kid, const XNKEY *key);

// @retail 0x72f00
void function_72f00(c_helper_a *helper)
{
	if (helper->lc != NONE && online_match_session_get_info(helper->lc, &helper->info))
	{
		const s_long_pair *id = (const s_long_pair *)&helper->info.session_id;
		if ((id->a | id->b) != 0 && *(long *)&helper->info.key != 0)
		{
			helper->ba = true;
			helper->b30 = true;
			if (!transport_security_register_key(helper->l2c, 0, false, &helper->info.session_id, &helper->info.key))
				helper->ba = false;
		}
		if (helper->lc != NONE)
		{
			function_6b640(helper->lc);
			helper->lc = NONE;
		}
	}
}

// @retail 0x737b0
void function_737b0(c_helper_a *helper)
{
	if (helper->v7())
	{
		c_class_58d20 *game = (c_class_58d20 *)helper->source->game;
		const s_long_pair *previous = NULL;
		if (game->state > 2 && game->state <= 8 && game->flag4998)
			previous = &game->data4999;
		const s_long_pair *current = NULL;
		if (helper->ba)
			current = (const s_long_pair *)&helper->info.session_id;
		if (current != previous && (!current || !previous || memcmp(current, previous, sizeof(*current)) != 0))
			game->set_data_4999(current);
	}
}

// @retail 0x746b0
long function_746b0(long previous, long current, long first, long second)
{
	long result = 0;
	long difference = current - previous + 127;
	short *scale = NULL;
	short *offset;
	byte *weight;
	if (first != NONE && (second == NONE || first < second))
	{
		scale = g_network_configuration.value378;
		offset = (short *)g_network_configuration.valueb74;
		weight = (byte *)g_network_configuration.value9f4;
	}
	else if (second != NONE && (first == NONE || second < first))
	{
		scale = g_network_configuration.value576;
		offset = (short *)g_network_configuration.valuea74;
		weight = (byte *)g_network_configuration.value974;
	}
	if (scale)
		result = scale[difference] * weight[current] / 100 + offset[current];
	return result;
}

struct s_surface_description
{
	long type;
	long field_4;
	dword field_8;
	char names[9][0x10];
	char descriptions[9][0x80];
	char field_51c[128];
	byte unknown59c[0x5a4 - 0x59c];
	long points[16];
	long field_5e4;
	bool flag_5e8;
	byte unknown5e9[3];
	long width;
	long height;
	long depth;
	long field_5f8;
	long field_5fc;
	long field_600;
	bool flag_604;
	byte unknown605[3];
	long field_608;
	long field_60c;
	byte unknown610[4];
};
struct s_game_variant_globals
{
	dword unknown0;
	dword flags;
	word count;
	word state;
};
extern s_surface_description g_551ae8[16];
extern s_game_variant_globals g_47d8f4;
bool function_1934f0(s_surface_description *variant);

// @retail 0x75790
bool __stdcall function_75790(dword identifier, long *index)
{
	bool result = false;
	for (long i = 0; i < 16 && !result; i++)
	{
		s_surface_description *variant = NULL;
		if (i >= 0 && i < 16 && g_47d8f4.count && (g_47d8f4.flags & 2) &&
			function_1934f0(&g_551ae8[i]))
			variant = &g_551ae8[i];
		if (variant && variant->field_8 == identifier)
		{
			*index = i;
			result = true;
		}
	}
	return result;
}

struct s_surface_description;
s_surface_description *function_192e60(long index);
long function_193300(s_surface_description *variant);
long network_session_get_maximum_players(c_class_58d20 *session);

// @retail 0x73360
bool c_helper_b::v7()
{
	bool result = false;
	if (b9 && source->type == 6)
	{
		c_class_58d20 *game = (c_class_58d20 *)source->game;
		if (game->function_058d20() && game->type == 11)
		{
			c_class_58d20 *other = source->other_game;
			if (other->function_058d20())
			{
				long index = other->get_value_49c8();
				s_surface_description *variant = function_192e60(index);
				if (index != NONE && variant && network_session_get_maximum_players(other) <= function_193300(variant))
					result = true;
			}
		}
	}
	return result;
}

struct s_match_rating_entry
{
	bool active;
	signed char rank;
	short unknown02;
	short skill;
	short unknown06;
	long value;
	byte unknown0c[12];
};
struct s_match_rating_collection
{
	byte unknown00[0xdc4];
	s_match_rating_entry entries[16];
};

// @retail 0x74720
long __stdcall function_74720(const s_match_rating_collection *collection, long selected)
{
	const s_match_rating_entry *current = &collection->entries[selected];
	long rank;
	long skill;
	long value;
	long count = 0;
	long total = 0;
	skill = current->skill;
	rank = current->rank;
	value = current->value;
	for (long i = 0; i < 16; i++)
	{
		const s_match_rating_entry *entry = &collection->entries[i];
		if (i != selected)
		{
			long other_skill = entry->skill;
			if (entry->active && other_skill != NONE)
			{
				total += function_746b0(other_skill, skill, rank, entry->rank);
				count++;
			}
		}
	}
	if (count > 0)
		total /= count;
	if (total + value < 0)
		total = -value;
	if (total + value > 0x3fffffff)
		total = 0x3fffffff - value;
	return total < g_network_configuration.valuec7c ? g_network_configuration.valuec7c :
		(total > g_network_configuration.valuec78 ? g_network_configuration.valuec78 : total);
}

struct s_match_player_rating_entry
{
	bool active;
	byte unknown01[9];
	byte flags;
	byte unknown0b[0x8c - 0x0b];
	signed char team;
	byte unknown8d[7];
	long key;
	short skill;
	short unknown9a;
	long value;
	signed char rank;
	byte unknowna1[3];
};
struct s_match_player_rating_collection
{
	byte unknown00[4];
	long key;
	byte unknown08[0x12b - 8];
	bool team_mode;
	byte unknown12c[0x384 - 0x12c];
	s_match_player_rating_entry players[16];
	s_match_rating_entry teams[16];
};

// @retail 0x74800
long __stdcall function_74800(const s_match_player_rating_collection *collection, long selected)
{
	long key = collection->key;
	bool team_mode = collection->team_mode;
	const s_match_player_rating_entry *current = &collection->players[selected];
	long skill = current->skill;
	long team = current->team;
	long value = current->value;
	long count = 0;
	long total = 0;
	long rank;
	if (team_mode)
		rank = collection->teams[team].rank;
	else
		rank = current->rank;
	for (long i = 0; i < 16; i++)
	{
		const s_match_player_rating_entry *entry = &collection->players[i];
		if (entry->key == key)
		{
			long other_skill = entry->skill;
			if (entry->active && entry->team != NONE && !(entry->flags & 3) && other_skill != NONE)
			{
				long other_team = entry->team;
				long other_rank;
				if (team_mode)
					other_rank = collection->teams[other_team].rank;
				else
					other_rank = entry->rank;
				if (i != selected && other_team != team)
				{
					total += function_746b0(other_skill, skill, rank, other_rank);
					count++;
				}
			}
		}
	}
	if (count > 0)
		total /= count;
	if (value + total < 0)
		total = -value;
	if (value + total > 0x3fffffff)
		total = 0x3fffffff - value;
	return total < g_network_configuration.valuec7c ? g_network_configuration.valuec7c :
		(total > g_network_configuration.valuec78 ? g_network_configuration.valuec78 : total);
}

extern long g_510518;
long g_510514;
long g_510530;

// @retail 0x73dc0
void function_73dc0(long value)
{
	if (g_510518)
	{
		if (g_510530 != NONE)
		{
			function_6b640(g_510530);
			g_510530 = NONE;
		}
		g_510518 = 0;
		g_510514 = value;
	}
}

extern s_session_id g_510540;

extern long g_467214;
long online_task_get_logon_status(long task_index);

// @retail 0x73ae0
bool function_73ae0(unsigned __int64 *created_id)
{
	bool result = false;
	if (g_467214 != NONE)
	{
		switch (online_task_get_logon_status(g_467214))
		{
		case 1:
			if (SUCCEEDED(XOnlineArbitrationCreateRoundID(created_id)))
				result = true;
			break;
		}
	}
	return result;
}

// @retail 0x73b10
long __stdcall function_73b10(long a, long b)
{
	const long *a_reference = &a;
	long result = 0;
	if (g_510540.a == *a_reference && g_510540.b == b)
	{
		switch (g_510518)
		{
		case 1: result = 1; break;
		case 2: result = 2; break;
		case 3: result = 3; break;
		case 0:
			switch (g_510514)
			{
			case 0: result = 4; break;
			case 1: result = 5; break;
			case 2: result = 6; break;
			case 3: result = 7; break;
			case 4: result = 8; break;
			}
			break;
		}
	}
	return result;
}

extern bool g_51051c;
extern bool g_51051d;
extern s_session_id g_510520;
extern s_session_id g_510528;
long g_510534;
long g_510538;
long online_round_register(bool free_for_all, XNKID const *session_id, ULONGLONG const *round_key, word seconds);

// @retail 0x73bc0
void function_73bc0(const s_session_id *session_id, const s_session_id *round_key,
	long first, long second, bool free_for_all)
{
	if (g_510518)
	{
		if (g_510530 != NONE)
		{
			function_6b640(g_510530);
			g_510530 = NONE;
		}
		g_510518 = 0;
		g_510514 = 4;
	}
	long task = online_round_register(free_for_all, (const XNKID *)session_id,
		(const ULONGLONG *)round_key, (word)g_network_configuration.value374);
	g_510528 = *session_id;
	g_510520 = *round_key;
	g_510540.a = first;
	g_510540.b = second;
	g_510514 = 4;
	g_510534 = 0;
	g_510538 = NONE;
	g_51051c = false;
	g_51051d = false;
	g_510530 = task;
	if (task != NONE)
		g_510518 = 1;
	else
	{
		g_510518 = 0;
		g_510514 = 1;
	}
}

struct s_online_match_session;
long online_match_session_update(const s_online_match_session *session);

// @retail 0x73100
void function_73100(c_helper_a *helper)
{
	s_helper_output output;
	bool changed = false;
	helper->v3(&output);
	long last = helper->l18;
	if (!last || output.l28 != helper->info.output.l28 || output.l1c != helper->info.output.l1c || output.l20 != helper->info.output.l20)
		changed = true;
	long now = g_510548 ? g_51054c : GetTickCount();
	if ((now - last >= helper->v4() && memcmp(&helper->info.output, &output, sizeof(output)) != 0) || changed)
	{
		helper->info.output = output;
		helper->l14 = online_match_session_update((const s_online_match_session *)&helper->info);
		helper->l18 = g_510548 ? g_51054c : GetTickCount();
	}
}

long online_task_poll(long task_index);
bool online_task_continue_failed(long task_index);
struct s_search_session;
long online_match_session_delete(const s_search_session *session, bool *unavailable);
long function_75870(void);
long function_75890(long time);

// @retail 0x72f60
void function_72f60(c_helper_a *helper)
{
	if (helper->l1c != NONE)
	{
		bool clear = false;
		switch (online_task_poll(helper->l1c))
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			function_6b640(helper->l1c);
			helper->l1c = NONE;
			clear = true;
			break;
		case 3:
			if (!online_task_continue_failed(helper->l1c))
				clear = true;
			function_6b640(helper->l1c);
			helper->l1c = NONE;
			break;
		case 4:
			function_6b640(helper->l1c);
			helper->l1c = NONE;
			break;
		case 5:
			clear = false;
			helper->l1c = NONE;
			break;
		default:
			helper->l1c = NONE;
			break;
		}
		if (clear)
		{
			memset(&helper->info, 0, sizeof(helper->info));
			helper->b30 = false;
		}
	}
	if (!helper->ba && helper->b30 && helper->l1c == NONE && helper->b9 &&
		(!helper->l20 || function_75890(helper->l20) >= 3000))
	{
		bool unavailable;
		helper->l1c = online_match_session_delete((const s_search_session *)&helper->info, &unavailable);
		if (unavailable)
		{
			memset(&helper->info, 0, sizeof(helper->info));
			helper->b30 = false;
		}
		helper->l20 = function_75870();
	}
}

struct s_search_value
{
	bool valid;
	byte unknown01[3];
	long value;
};

struct s_search_player_values
{
	long state;
	long time;
	s_search_value values[6];
	byte unknown38[0x98 - 0x38];
};

struct s_search_value_group
{
	byte unknown00[4];
	XUID owner;
	byte unknown10[0x1c - 0x10];
	s_search_player_values players[16];
};

s_search_value_group g_509454[5];
#pragma pack(push, 1)
struct s_session_interface_user
{
	bool valid;
	XUID xuid;
	byte unknown0d[3];
	long unknown10;
	byte properties[0x90];
	long unknowna4;
	long unknowna8[3];
	long unknownb4[3];
	long unknownc0[3];
	byte unknowncc[4];
};
#pragma pack(pop)

struct s_session_interface_globals
{
	bool initialized;
	byte unknown01;
	wchar_t machine_name[16];
	wchar_t session_name[32];
	bool unknown62;
	byte unknown63;
	union
	{
		byte unknown64[32];
		struct
		{
			byte unknown64_00[0x14];
			long unknown78;
		};
	};
	long unknown84;
	long unknown88;
	long unknown8c;
	long unknown90;
	long unknown94;
	long unknown98;
	s_session_interface_user users[4];
	long update3dc[3];
	long unknown3e8[3];
	long value3f4[3];
	byte data400[3][0x130];
	byte unknown790[0x7d4 - 0x790];
	void *session_manager;
};

extern s_session_interface_globals g_4cd868;

// @retail 0x73a10
bool function_73a10(long player, long group_index, long field, long *value)
{
	bool result = false;
	s_search_value_group *group = &g_509454[group_index];
	if (player >= 0 && player < 16 && g_47d8f4.count && (g_47d8f4.flags & 2) &&
		function_1934f0(&g_551ae8[player]))
	{
		s_search_player_values *values = &group->players[player];
		if (values->state == 3)
		{
			XUID xuid;
			if (g_4cd868.users[group_index].valid)
			{
				xuid = g_4cd868.users[group_index].xuid;
				if (memcmp(&group->owner, &xuid, sizeof(xuid)) == 0)
				{
					result = true;
					*value = values->values[field].value;
				}
			}
		}
	}
	return result;
}

const long g_46722c[6][2] =
{
	{0, 0x3fffffff}, {0, 127}, {0, 127},
	{0, 0x7fffffff}, {0, 0x7fffffff}, {0, 0x7fffffff}
};

// @retail 0x74970
long function_74970(long value, long previous)
{
	long level = 0;
	for (long i = 1; i < 128; i++)
	{
		if (value < g_network_configuration.value774[i])
		{
			if (level == previous - 1 && value > (g_network_configuration.value774[level] + g_network_configuration.value774[level + 1]) / 2)
				level = previous;
			break;
		}
		level++;
	}
	return level;
}

// @retail 0x749b0
void function_749b0(long group, long player, long index, long value)
{
	if (value < g_46722c[index][0])
		value = g_46722c[index][0];
	else if (value > g_46722c[index][1])
		value = g_46722c[index][1];
	if (!g_509454[group].players[player].values[index].valid || value != g_509454[group].players[player].values[index].value)
	{
		g_509454[group].players[player].values[index].value = value;
		g_509454[group].players[player].values[index].valid = true;
	}
}

// @retail 0x74a00
void function_74a00(long group, long player, long index, long amount)
{
	if (g_509454[group].players[player].values[index].valid && amount)
	{
		long value = g_509454[group].players[player].values[index].value + amount;
		if (value < g_46722c[index][0])
			value = g_46722c[index][0];
		else if (value > g_46722c[index][1])
			value = g_46722c[index][1];
		g_509454[group].players[player].values[index].value = value;
	}
}

// @retail 0x74a60
void function_74a60(long group, long player, long index, long value)
{
	if (g_509454[group].players[player].values[index].valid)
	{
		long bounded = g_46722c[index][0];
		if (value >= bounded)
		{
			bounded = g_46722c[index][1];
			if (value <= bounded)
				bounded = value;
		}
		if (bounded > g_509454[group].players[player].values[index].value)
			g_509454[group].players[player].values[index].value = bounded;
	}
}

// @retail 0x739a0
void function_739a0(long group, long player)
{
	for (long i = 0; i < 5; i++)
	{
		if ((group == NONE || group == i) && g_509454[i].players[player].state == 3)
		{
			g_509454[i].players[player].state = 4;
			g_509454[i].players[player].time = g_510548 ? g_51054c : GetTickCount();
		}
	}
}

// @retail 0x731d0
void function_731d0(c_helper_a *helper)
{
	byte data[0x3fc];
	if (helper->listening && !helper->ba)
	{
		if (g_4cf8d4)
			XNetQosListen(&helper->info.session_id, NULL, 0, 0, 16);
		helper->listening = false;
	}
	if (helper->ba)
	{
		if (!helper->listening)
		{
			helper->listen_time = 0;
			if (g_4cf8d4 && XNetQosListen(&helper->info.session_id, NULL, 0, 0, 1) == 0)
			{
				if (g_4cf8d4)
					XNetQosListen(&helper->info.session_id, NULL, 0, 0, 5);
				helper->listening = true;
			}
		}
		if (!helper->listening)
			return;
		long last = helper->listen_time;
		if (!last || (long)(g_510548 ? g_51054c : GetTickCount()) - last >= g_network_configuration.valuec98)
		{
			long size = helper->v5(sizeof(data), data);
			if (g_4cf8d4)
				XNetQosListen(&helper->info.session_id, size > 0 ? data : NULL, size, 0, 5);
			long rate = helper->v6();
			if (g_4cf8d4)
				XNetQosListen(&helper->info.session_id, NULL, 0, rate, 9);
			helper->listen_time = g_510548 ? g_51054c : GetTickCount();
		}
	}
}
