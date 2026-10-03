// @flags /O2 /Gr
/* UNKNOWN_058DD0.CPP: the game-session state machine states */

#include "cseries.h"
#include "globals.h"
#include "unknown_058dd0.h"
#include <xtl.h>
#include <string.h>


#define SESSION_STATE_IS_LIVE(state) ((state) > 2 && (state) <= 8)

/* ---- names ---- */

// @retail 0x58dd0
const char *c_session_state_none::get_name()
{
	return "none";
}

// @retail 0x58de0
const char *c_session_state_pre_game::get_name()
{
	return "pre-game";
}

// @retail 0x58df0
const char *c_session_state_start_match::get_name()
{
	return "start-match";
}

// @retail 0x58e00
const char *c_session_state_start_game::get_name()
{
	return "start-game";
}

// @retail 0x58e10
const char *c_session_state_in_match::get_name()
{
	return "in-match";
}

// @retail 0x58e20
const char *c_session_state_in_game::get_name()
{
	return "in-game";
}

// @retail 0x58e30
const char *c_session_state_post_game::get_name()
{
	return "post-game";
}

// @retail 0x58e40
const char *c_session_state_joining::get_name()
{
	return "joining";
}

// @retail 0x58e50
const char *c_session_state_matchmaking::get_name()
{
	return "matchmaking";
}

// @retail 0x58e60
const char *c_session_state_post_match::get_name()
{
	return "post-match";
}

/* ---- the default enter, shared by post-match, none and post-game ---- */

// @retail 0x6e1b0
void c_session_state::enter(long a, long b, long c)
{
	if (!skip_cleanup)
	{
		c_network_session *s = owner->session_b;
		if (s->state != 0 && !function_058d90(s))
		{
			network_session_leave(s, false);
		}
	}
}

/* ---- none ---- */

// @retail 0x6e130
bool c_session_state_none::update()
{
	s_session_owner *o = owner;
	c_network_session *s = o->session_a;
	if (s->state != 0 && !function_058d90(s))
	{
		o->data_size = 0;
		o->failed = true;
		o->error_code = 1;
		long *d = o->data;
		*d = 0;
		if (o->data_size > 0)
		{
			memcpy(d, 0, o->data_size);
		}
		return false;
	}
	s = o->session_b;
	if (s->state != 0 && !function_058d90(s))
	{
		network_session_leave(s, false);
	}
	return false;
}

/* ---- pre-game ---- */

// @retail 0x6e1e0
bool c_session_state_pre_game::update()
{
	s_session_owner *o = owner;
	c_network_session *a = o->session_a;
	bool result = function_06dfa0();
	if (!result && !o->failed)
	{
		switch (a->type)
		{
		case 0:
			if (a->state != 1)
			{
				function_06df60(o, 0, 0, 0);
			}
			break;
		case 1:
			if (function_058d50(a))
			{
				result = function_06e360();
			}
			if (a->function_058d20())
			{
				result = function_06e410();
			}
			break;
		case 2:
		case 3:
		case 4:
			if (function_058d70(a))
			{
				long kind = a->members[a->current_member].unknown88;
				if (kind == 3 || kind == 4)
				{
					function_06df60(o, 2, 0, 0);
				}
			}
			break;
		case 5:
			function_06df60(o, 4, 0, 0);
			break;
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
			function_06df60(o, 6, 0, 0);
			break;
		case 15:
			function_06df60(o, 5, 0, 0);
			break;
		default:
			__assume(0);
		}
	}
	return result;
}

// @retail 0x6e310
void c_session_state_pre_game::enter(long a, long b, long c)
{
	if (!skip_cleanup)
	{
		c_network_session *s = owner->session_b;
		if (s->state != 0 && !function_058d90(s))
		{
			network_session_leave(s, false);
		}
	}
	unknown18 = NONE;
	unknown14 = 0;
	unknown1c = 0;
}

/* ---- start-game ---- */

// @retail 0x6e500
bool c_session_state_start_game::update()
{
	s_session_owner *o = owner;
	c_network_session *a = o->session_a;
	bool result = function_06dfa0();
	if (!result && !o->failed && SESSION_STATE_IS_LIVE(a->state))
	{
		if (a->type == 4)
		{
			function_06df60(o, 3, 0, 0);
			return result;
		}
		if (a->type == 2)
		{
			if (a->function_058d20() && function_06e720(a) && function_06e6b0(a, &flag10))
			{
				network_session_stop_countdown(a);
				network_session_set_mode(a, 4);
				result = true;
			}
		}
		else
		{
			function_06df60(o, 1, 0, 0);
		}
	}
	return result;
}

// @retail 0x6e5d0
void c_session_state_start_game::enter(long a, long b, long c)
{
	c_network_session *s = owner->session_a;
	if (!skip_cleanup)
	{
		c_network_session *t = owner->session_b;
		if (t->state != 0 && !function_058d90(t))
		{
			network_session_leave(t, false);
		}
	}
	function_06e620(s);
	flag10 = 0;
}

/* ---- in-game ---- */

// @retail 0x6ea20
bool c_session_state_in_game::update()
{
	s_session_owner *o = owner;
	c_network_session *a = o->session_a;
	bool result = function_06dfa0();
	if (!result && !o->failed && SESSION_STATE_IS_LIVE(a->state))
	{
		if (a->type == 4)
		{
			if (function_0682c0())
			{
				if (a->function_058d20())
				{
					network_session_set_mode(a, 1);
					result = true;
				}
				else
				{
					network_session_leave(a, false);
					result = true;
				}
				return result;
			}
			if (a->function_058d20())
			{
				if (function_138a10())
				{
					network_session_set_mode(a, 5);
					result = true;
				}
				else if (a->get_value_49c4())
				{
					function_1388e0();
				}
			}
			if (function_138800())
			{
				s_game_options_view *options = g_4e6948;
				if (options->id_a == id_a && options->id_b == id_b)
				{
					long x = NONE;
					long y = NONE;
					byte *z;
					if (a->get_values_4d08(&x, &y, &z))
					{
						if (options->position_a != x || options->position_b != y)
						{
							function_06ec10(a);
						}
					}
				}
			}
		}
		else
		{
			function_06df60(o, a->type == 5 ? 4 : 1, 0, 0);
		}
	}
	return result;
}

// @retail 0x6eb90
void c_session_state_in_game::enter(long a, long b, long c)
{
	c_network_session *s = owner->session_a;
	if (!skip_cleanup)
	{
		c_network_session *t = owner->session_b;
		if (t->state != 0 && !function_058d90(t))
		{
			network_session_leave(t, false);
		}
	}
	function_06ec10(s);
}

// @retail 0x6ebe0
void c_session_state_in_game::leave(long a)
{
	s_game_options_view *options = g_4e6948;
	if (options && options->flag1120 && options->id_a == id_a && options->id_b == id_b)
	{
		function_068750();
	}
}

/* ---- in-match ---- */

// @retail 0x729a0
bool c_session_state_in_match::update()
{
	s_session_owner *o = owner;
	c_network_session *b = o->session_b;
	c_network_session *a = o->session_a;
	bool result = function_06dfa0();
	if (!result && !o->failed && SESSION_STATE_IS_LIVE(b->state))
	{
		if (b->type == 4)
		{
			if (function_0682c0())
			{
				network_session_leave(b, false);
				if (a->function_058d20())
				{
					network_session_set_mode(a, 1);
				}
				else
				{
					network_session_leave(a, false);
				}
				return true;
			}
			if (function_138a10() && b->function_058d20())
			{
				network_session_set_mode(b, 5);
				return true;
			}
		}
		else if (b->type == 5)
		{
			function_06df60(o, 9, 0, 0);
			return true;
		}
		else
		{
			network_session_leave(b, false);
			result = true;
		}
	}
	return result;
}

// @retail 0x72a90
void c_session_state_in_match::enter(long a, long b, long c)
{
	c_network_session *s = owner->session_b;
	if (!skip_cleanup && s->state != 0 && !function_058d90(s))
	{
		network_session_leave(s, false);
	}
	id_a = NONE;
	id_b = NONE;
	if (function_06ec80(s, true))
	{
		s_game_options_view *options = g_4e6948;
		id_a = options->id_a;
		id_b = options->id_b;
	}
	else
	{
		network_session_leave(s, false);
	}
}

// @retail 0x72b00
void c_session_state_in_match::leave(long a)
{
	s_game_options_view *options = g_4e6948;
	if (options && options->flag1120 && options->id_a == id_a && options->id_b == id_b)
	{
		function_068750();
	}
	if (g_510548)
	{
		time = g_51054c;
	}
	else
	{
		time = GetTickCount();
	}
}

/* ---- start-match ---- */

// @retail 0x728b0
void c_session_state_start_match::enter(long a, long b, long c)
{
	c_network_session *s = owner->session_b;
	if (!skip_cleanup && s->state != 0 && !function_058d90(s))
	{
		network_session_leave(s, false);
	}
	function_072950();
	mode = 3;
	unknown14 = 0;
	unknown18 = 0;
}

// @retail 0x72900
void c_session_state_start_match::leave(long a)
{
	c_network_session *s = owner->session_a;
	if (mode == 3)
	{
		mode = 1;
	}
	long state = s->state;
	if (state == 5 || state == 6 || state == 7 || state == 8)
	{
		network_session_host_set_value49f8(s, mode);
	}
}

/* ---- matchmaking ---- */

// @retail 0x70a20
void c_session_state_matchmaking::enter(long a, long b, long c)
{
	c_network_session *s = owner->session_a;
	if (!skip_cleanup)
	{
		c_network_session *t = owner->session_b;
		if (t->state != 0 && !function_058d90(t))
		{
			network_session_leave(t, false);
		}
	}
	unknown968 = 0;
	unknowna7c = 0;
	unknown970 = 0;
	unknowna68 = 0;
	if (g_510548)
	{
		time = g_51054c;
	}
	else
	{
		time = GetTickCount();
	}
	mode = 3;
	long state = s->state;
	bool live = false;
	if (state == 5 || state == 6 || state == 7 || state == 8)
	{
		live = true;
	}
	flag10 = live;
	memset(unknown14, 0, sizeof(unknown14));
}

// @retail 0x70ad0
void c_session_state_matchmaking::leave(long a)
{
	c_network_session *s = owner->session_a;
	if (mode == 3)
	{
		mode = 1;
	}
	if (flag97c)
	{
		function_090c80(&flag97c);
	}
	if (flaga78)
	{
		function_070c50(mode == 2);
	}
	if (flaga64)
	{
		function_070d20(mode == 2);
	}
	long state = s->state;
	if (state == 5 || state == 6 || state == 7 || state == 8)
	{
		network_session_host_set_value49f8(s, mode);
	}
}

/* ---- post-match ---- */

// @retail 0x72b50
bool c_session_state_post_match::update()
{
	s_session_owner *o = owner;
	c_network_session *a = o->session_a;
	c_network_session *b = o->session_b;
	bool result = function_06dfa0();
	if (!result && !o->failed)
	{
		if (b->type == 5 && (a->type == 13 || a->type == 12))
		{
			if (!SESSION_STATE_IS_LIVE(b->state) || !a->function_058d20())
			{
				return result;
			}
			long index = b->member_index;
			s_session_snapshot snapshot;
			s_session_snapshot compare;
			memset(&snapshot, 0, sizeof(snapshot));
			snapshot.unknown40 = 2;
			network_session_get_key(b, (s_session_id *)snapshot.unknown04, snapshot.unknown0c, NULL, &snapshot.unknown00);
			memcpy(snapshot.words, b->members[index].words, sizeof(snapshot.words));
			if (!network_session_get_data5ddc(a, (s_parameters_part *)&compare) || memcmp(&compare, &snapshot, sizeof(snapshot)) != 0)
			{
				network_session_host_set_data5ddc(a, (const s_parameters_part *)&snapshot);
			}
			return result;
		}
		network_session_leave(b, false);
		return true;
	}
	return result;
}

/* ---- joining ---- */

// @retail 0x6fd10
bool c_session_state_joining::update()
{
	s_session_owner *o = owner;
	c_network_session *c = o->session_c;
	c_network_session *a = o->session_a;
	if (unknown104 == 0)
	{
		if (flag10)
		{
			function_06f4b0(this);
		}
		if (unknown104 == 0)
		{
			if (!flag10)
			{
				if (a->state != 0)
				{
					function_06f700(this);
				}
				else
				{
					function_06fcc0(this);
				}
			}
		}
	}
	if (unknown104 != 0)
	{
		long state = a->state;
		if (state == 5 || state == 6 || state == 7 || state == 8)
		{
			network_session_set_mode(a, 1);
		}
	}
	if (flagf8)
	{
		c_network_session *swap = o->session_a;
		o->session_a = o->session_c;
		o->session_c = swap;
		c = swap;
		unknown104 = 2;
	}
	if (unknown104 != 0)
	{
		network_session_leave(c, false);
		o->failed = true;
		o->error_code = 1;
		o->data_size = 0;
		long *d = o->data;
		*d = 0;
		if (o->data_size > 0)
		{
			memcpy(d, 0, o->data_size);
		}
	}
	return false;
}

// @retail 0x6fe20
void c_session_state_joining::enter(long a, long b, long c)
{
	if (!skip_cleanup)
	{
		c_network_session *s = owner->session_b;
		if (s->state != 0 && !function_058d90(s))
		{
			network_session_leave(s, false);
		}
	}
	unknownfc = 0;
	unknown100 = 0;
	unknown104 = 0;
}

// @retail 0x6fe80
void c_session_state_joining::leave(long a)
{
	function_06f0f0();
}

/* ---- post-game ---- */

// @retail 0x6f050
bool c_session_state_post_game::update()
{
	s_session_owner *o = owner;
	c_network_session *a = o->session_a;
	bool result = function_06dfa0();
	if (!result && !o->failed && a->type != 5)
	{
		function_06df60(o, 1, 0, 0);
		result = true;
	}
	return result;
}

/* ---- the session client ---- */

// @retail 0x6daa0
bool c_session_client::function_06daa0(long a)
{
	c_network_session *s = session;
	bool result = false;
	if (SESSION_STATE_IS_LIVE(s->state) && s->flag49fd)
	{
		void *address = s->data4a00;
		if (address)
		{
			result = function_063190(address, a) != NONE;
		}
	}
	return result;
}

// @retail 0x6dae0
void c_session_client::function_06dae0(long a, s_session_remote *remote)
{
	c_network_session *s = session;
	if (SESSION_STATE_IS_LIVE(s->state) && s->flag49fd)
	{
		void *address = s->data4a00;
		if (address && function_06dcc0(remote))
		{
			if (function_06de10(remote))
			{
				return;
			}
			byte has_address = remote->has_address;
			if (has_address)
			{
				if (function_0632e0(address, remote->address144) != NONE)
				{
					return;
				}
			}
			if (has_address)
			{
				if (function_063510(remote->address04, address, remote->id))
				{
					goto fallback;
				}
			}
			if (function_06dd00(a, remote))
			{
				return;
			}
		}
	}
fallback:
	s = session;
	long local[3];
	local[0] = 0;
	local[1] = 0;
	local[0] = s->unknown1c;
	local[2] = 0;
	local[1] = s->unknown20;
	local[2] = 3;
	function_07b140(s->unknown04, a, 10, 12, local);
}

// @retail 0x6dbc0
void c_session_client::function_06dbc0()
{
	function_06dc60(this, 3);
}

// @retail 0x6dbd0
void c_session_client::function_06dbd0(const s_session_id *id)
{
	c_network_session *s = session;
	switch (s->state)
	{
	case 2:
		return;
	case 3:
		break;
	case 4:
		return;
	case 5:
		break;
	case 6:
		return;
	case 7:
		if (s->flag7420)
		{
			return;
		}
		break;
	case 8:
		if (s->flag7420)
		{
			return;
		}
		break;
	}
	if (mode == 2 && s->type == 1)
	{
		if (memcmp(&s->members[s->current_member].id, id, sizeof(*id)) != 0)
		{
			function_06d380(this, id);
		}
	}
}
