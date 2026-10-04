// @flags /O1 /Gr
/* UNKNOWN_250155.CPP: the screens and lists of 0x250155..0x2541b2 (the
   matchmaking screens) */

#include "cseries.h"
#include <stdlib.h>
#include <string.h>
#include "data_array.h"
#include "screen_widgets.h"
#include "unknown_19b510.h"
#include "unknown_19b516.h"
#include "unknown_234c64.h"
#include "user_interface_controller_sign_in.h"

#pragma intrinsic(memset, memcpy)

bool function_1999b3(void);
long function_19a161(void);

/* the screen of 0x24fd74 (not written yet) */
class c_screen_24fd74 : public c_screen_widget
{
public:
	void function_250155();

	byte unknown610[0x142c - 0x610];
	short mode;
};

/* shows the network connection's state in the mode's text */
// @retail 0x250155
void c_screen_24fd74::function_250155()
{
	long index;

	switch (mode)
	{
	case 0:
		index = 12;
		break;
	case 1:
		index = 7;
		break;
	case 2:
		index = 12;
		break;
	default:
		index = NONE;
		break;
	}

	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)find_text((short)index);
	if (text)
	{
		if (function_1999b3())
		{
			text->value6e = false;
		}
		else
		{
			text->value6e = true;
			switch (function_19a161())
			{
			case 0:
				text->set_string(0x4000201);
				break;
			case 1:
				text->set_string(0xf000202);
				break;
			case 2:
				text->set_string(0x6000203);
				break;
			}
		}
	}
}

/* "matchmaking list" (vtable 0x45a340): sixteen items, sorted by slot 3 */
class c_matchmaking_list : public c_list_widget
{
public:
	c_matchmaking_list(word user_flags);

	virtual void v1();
	/* fills the list with the session's players */
	virtual void v3();
	virtual void v20(c_user_interface_widget *item, long unused);

	void show_player(c_user_interface_widget *item, long player_index);
	void show_open_slot(c_user_interface_widget *item);
	void show_empty_slot(c_user_interface_widget *item);

	c_list_item_widget items[16];
};

// @retail 0x251469
c_matchmaking_list::c_matchmaking_list(word user_flags) :
	c_list_widget(user_flags)
{
}

// @retail 0x2514d7
void c_matchmaking_list::v1()
{
	data = user_interface_data_new("matchmaking list", 16, 4);
	if (data)
	{
		data_make_valid(data);
	}
	((c_widget *)this)->c_widget::v9();
}

// @retail 0x251497 deleting c_matchmaking_list
// @retail 0x2514b5 destructor c_matchmaking_list

c_screen_widget *__stdcall function_2519bb(s_screen_parameters *parameters);
long network_time_get(void); /* 0x75870, network_observer.cpp */

bool g_51ec99;

/* the matchmaking screen (vtable 0x45a398) */
class c_matchmaking_screen : public c_screen_widget
{
public:
	c_matchmaking_screen(long a, long b, word user_flags);

	virtual bool v10(s_widget_event *event);
	virtual void v18(void *parameters);
	virtual screen_load_proc get_load_proc();

	long get_title();

	long value610;
	long value614;
	long value618;
	long value61c;
	bool value620;
	s_text_256 texts[6];
	short value1222;
	byte unknown1224[0x1424 - 0x1224];
	c_matchmaking_list list;
};

c_screen_widget *__stdcall function_233395(s_screen_parameters *parameters);
/* the count of the postgame statistics' players (screen_postgame_statistics.cpp) */
extern long g_51ec08;

/* opens the saved film's postgame statistics, when there are players */
// @retail 0x2510aa
void function_2510aa(long controller_index)
{
	if (g_51ec08 > 0)
	{
		s_screen_parameters parameters;

		parameters.field_c = 0;
		function_149f49((s_message *)&parameters, 0, 0, 1 << controller_index, 3, 4, (long)function_233395);
		parameters.load(&parameters);
	}
}

// @retail 0x25137e
screen_load_proc c_matchmaking_screen::get_load_proc()
{
	return function_2519bb;
}

struct s_widget_view_2b0a;
void function_2b0a14(s_widget_view_2b0a *widget, short index);
bool voice_port_flag0_only(long port);
bool function_53750(long player_index);

/* the item's voice icon hides */
// @retail 0x251963
void function_251963(c_user_interface_widget *item)
{
	c_user_interface_widget *bitmap = item->find_child(8, 0, false);

	if (bitmap)
	{
		bitmap->value6e = false;
	}
}

/* the item's voice icon shows the player's voice state */
// @retail 0x251977
void function_251977(long player, c_user_interface_widget *item)
{
	c_user_interface_widget *bitmap = item->find_child(8, 0, false);

	if (bitmap && player != NONE)
	{
		short index;

		if (voice_port_flag0_only(player))
		{
			index = !function_53750(player);
		}
		else
		{
			index = 2;
		}
		function_2b0a14((s_widget_view_2b0a *)bitmap, index);
		bitmap->value6e = true;
	}
}

// @retail 0x2519bb
c_screen_widget *__stdcall function_2519bb(s_screen_parameters *parameters)
{
	c_matchmaking_screen *screen = new c_matchmaking_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2519f7
c_matchmaking_screen::c_matchmaking_screen(long a, long b, word user_flags) :
	c_screen_widget(0xd2, a, b, user_flags),
	value610(0),
	value618(0),
	value61c(network_time_get()),
	value620(false),
	value1222(0),
	list(user_flags)
{
	g_51ec99 = false;
}

// @retail 0x251a73 deleting c_matchmaking_screen
// @retail 0x251a91 destructor c_matchmaking_screen

/* a message box callback that accepts */
// @retail 0x2523b7
bool __stdcall function_2523b7(long controller)
{
	return true;
}

/* ---- the network squad browser ---- */

/* "network squad list" (vtable 0x45a500): the squads found on the system
   link */
class c_network_squad_list : public c_list_widget
{
public:
	c_network_squad_list(word user_flags, bool alternate);

	/* forgets the focused squad */
	virtual void v2();
	/* folded with c_widget's v2 */
	virtual void *get_item_data() { return items; }

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[5];
	long value308;
	c_list_item_handler handler;
	bool searching;
	bool alternate;
};

/* the network squad browser screen (vtable 0x45a558) */
class c_network_squad_browser_screen : public c_screen_with_menu
{
public:
	c_network_squad_browser_screen(long a, long b, word user_flags, bool alternate);

	virtual screen_load_proc get_load_proc();

	c_network_squad_list list;
	bool alternate;
	bool value93d;
	bool value93e;
};

void __stdcall function_252ed8(void *list);
bool function_199df9(bool offline, bool system_link);
void function_199a03(long mode);
void function_199c47(long index);
bool function_199ba5(long index);
byte *function_199bbf(long index);
word function_1901fc(void);
void function_236299(long sound);

/* a datum of the list: a squad, or the item that creates one */
struct s_network_squad_datum
{
	short salt;
	short unknown02;
	long index;
	bool create;
	byte unknown09[3];
};

/* a squad found on the system link */
struct s_network_squad
{
	byte unknown00[0xbc];
	short player_count;
};

/* the squad focused last */
long g_470b18 = NONE;

// @retail 0x252b72
c_network_squad_list::c_network_squad_list(word user_flags, bool alternate) :
	c_list_widget(user_flags),
	value308(0),
	handler(this, (list_item_method)&c_network_squad_list::handle_item)
{
	g_470b18 = NONE;
	searching = false;
	this->alternate = alternate;
	data = user_interface_data_new("network squad list", 0x21, 0xc);
	data_make_valid(data);
	function_252ed8(this);
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2530ec
void c_network_squad_list::handle_item(s_controller_reference **controller, long *item)
{
	if (data && *item != NONE)
	{
		s_network_squad_datum *datum = &((s_network_squad_datum *)data->data)[*item & 0xffff];

		if (datum->create)
		{
			if (function_199df9(0, alternate))
			{
				function_199a03(2);
			}
		}
		else
		{
			long index = datum->index;
			s_network_squad *squad;

			if (index != NONE && function_199ba5(index) && (squad = (s_network_squad *)function_199bbf(index)) != 0)
			{
				if (squad->player_count >= 16)
				{
					dialog_ok_show(1, 0x41, 4, function_1901fc(), 0, 0);
				}
				else
				{
					function_199c47(index);
					return;
				}
			}
			function_236299(2);
		}
	}
}

// @retail 0x252c0a deleting c_network_squad_list
// @retail 0x252c28 destructor c_network_squad_list

// @retail 0x252c5e
void c_network_squad_list::v2()
{
	g_470b18 = NONE;
	((c_widget *)this)->c_widget::v10();
}

c_screen_widget *__stdcall function_253185(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2531c9(s_screen_parameters *parameters);

// @retail 0x253185
c_screen_widget *__stdcall function_253185(s_screen_parameters *parameters)
{
	c_network_squad_browser_screen *screen = new c_network_squad_browser_screen(parameters->a, parameters->b, parameters->user_flags, false);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2531c9
c_screen_widget *__stdcall function_2531c9(s_screen_parameters *parameters)
{
	c_network_squad_browser_screen *screen = new c_network_squad_browser_screen(parameters->a, parameters->b, parameters->user_flags, true);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x253221
c_network_squad_browser_screen::c_network_squad_browser_screen(long a, long b, word user_flags, bool alternate) :
	c_screen_with_menu(0xd, a, b, user_flags, &list),
	list(user_flags, alternate)
{
	this->alternate = alternate;
	value93d = false;
	value93e = false;
}

// @retail 0x253268 deleting c_network_squad_browser_screen
// @retail 0x253286 destructor c_network_squad_browser_screen

// @retail 0x25320d
screen_load_proc c_network_squad_browser_screen::get_load_proc()
{
	return alternate ? function_2531c9 : function_253185;
}


long function_1480ff(long screen_id);

// @retail 0x251aa6
void c_matchmaking_screen::v18(void *parameters)
{
	volatile long definition_index = function_1480ff(screen_id);
	s_screen_layout layout =
	{
		0,
		1,
		{
			{ 0, 0, &list, 0 }
		}
	};

	build(&layout);
	c_user_interface_widget::v1();
}

bool function_592f0(void);
byte function_199eaa(void);
bool function_199e7e(byte value);
/* (screen_multiplayer_pregame_lobby.cpp): retail inlined the window test of
   channel 3, index 4 */
// @retail 0x2507dc
void function_2507dc(void)
{
	if (function_592f0() && function_199eaa() && !function_1473b6(&g_54d598.windows_3[4]))
	{
		function_199e7e(0);
	}
}

/* ---- the pregame lobby's player slots (0x2510e1..0x25142f) ---- */

byte *network_session_interface_get_data_4db0(void);
bool function_19a84e(long *a, long *b);
long function_19989d(void);
void function_19a864(void);
bool function_199967(void);
long function_199d7c(void);
long network_session_manager_get_value49ac(void);
short function_18f93a(void);
short player_slot_count_active(void);
bool function_1900a5(long player);
void function_199e3c(long controller);
short function_1900ff(long controller);
long function_199f34(void);
bool function_19a179(long player_index);
void function_19a7e9(long controller, long value);
void function_149ef3(word user_flags, long load);
c_screen_widget *__stdcall function_2b8536(s_screen_parameters *parameters);

/* a session entry as the lobby sees it */
struct s_session_entry_view
{
	byte unknown00[0xc54];
	byte counts[16];
};

struct s_entry_c;
s_entry_c *function_19c5f0(long key);

/* whether the session's settings are valid */
// @retail 0x2510e1
bool function_2510e1(void)
{
	bool result = false;
	byte *data = network_session_interface_get_data_4db0();
	long a;
	long b;

	if (function_19a84e(&a, &b) && data)
	{
		if (a != NONE)
		{
			result = true;
		}
		else
		{
			long index = *(long *)(data + 0x44);

			if (index >= 1 && index <= 9)
			{
				result = true;
			}
		}
	}
	return result;
}

// @retail 0x251121
bool function_251121(void)
{
	switch (function_19989d())
	{
	case 1:
	case 3:
	case 5:
		function_19a864();
		break;
	}
	return true;
}

/* the count of the session's current entry, at least one */
// @retail 0x251139
long function_251139(void)
{
	if (function_2510e1())
	{
		byte *data = network_session_interface_get_data_4db0();
		long a;
		long b;
		s_session_entry_view *entry;

		function_19a84e(&b, &a);
		entry = (s_session_entry_view *)function_19c5f0(a);
		if (entry)
		{
			long index = *(long *)(data + 0x44);

			if (index >= 1 && index <= 9)
			{
				long count = entry->counts[index];

				if (count > 1)
				{
					return count;
				}
			}
		}
	}
	return 1;
}

// @retail 0x251188
bool function_251188(long value)
{
	bool result = false;

	switch (value)
	{
	case 2:
	case 5:
	case 6:
	case 7:
	case 11:
	case 12:
	case 13:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 25:
		result = true;
		break;
	}
	return result;
}

/* asks the player to sign in */
// @retail 0x2512d3
void function_2512d3(long controller)
{
	dialog_ok_show(3, 0x88, 4, (word)(1 << controller), 0, 0);
}

// @retail 0x25122f
void function_25122f(long controller)
{
	function_199f34();
	function_19a7e9(controller, 10);
}

// @retail 0x2512ec
void __stdcall function_2512ec(long player, bool signed_in)
{
	function_25122f(player);
}

// @retail 0x25132e
bool __stdcall function_25132e(long controller)
{
	function_25122f(controller);
	function_199e3c(controller);
	return true;
}

// @retail 0x251345
bool __stdcall function_251345(long controller)
{
	function_25122f(controller);
	function_19a179(NONE);
	function_199e3c(controller);
	return true;
}

// @retail 0x251242
void function_251242(long controller)
{
	if (function_18f93a() == 1 && !function_1900a5(controller))
	{
		function_199e3c(controller);
	}
	else
	{
		s_player_slot_profile *profile = player_slot_profile_get(controller);

		profile->callback = function_2512ec;
		profile->sign_out();
	}
}

// @retail 0x251284
void function_251284(long controller)
{
	if (player_slot_count_active() == 1)
	{
		dialog_choice_show(3, 0x8b, 4, (word)(1 << controller), function_25132e, 0, 0);
	}
	else
	{
		player_slot_profile_get(controller)->show_dialog(function_2512ec, 0x8b);
	}
}

// @retail 0x2512f8
void function_2512f8(long controller, bool choice)
{
	if (choice)
	{
		dialog_choice_show(3, 0x8b, 4, (word)(1 << controller), function_251345, 0, 0);
	}
	else
	{
		function_149ef3((word)(1 << controller), (long)function_2b8536);
	}
}

// @retail 0x2511b6
void function_2511b6(long controller)
{
	long count = function_18f93a();
	long players = function_199f34();

	if (function_1900a5(controller) || count == 1 && players <= count)
	{
		function_251242(controller);
	}
	else if (count == 1)
	{
			if (function_592f0())
			{
				function_2512f8(controller, players == 2);
			}
			else
			{
				function_25122f(controller);
				if (function_1900ff(controller) > 0)
				{
						function_2512d3(controller);
				}
				else
				{
						function_251284(controller);
				}
			}
	}
	else if (function_1900ff(controller) > 0)
	{
		function_2512d3(controller);
	}
	else
	{
		function_251242(controller);
	}
}

struct s_session_player_view
{
	byte unknown00[0x48];
	byte flags;
};

// @retail 0x251364
long function_251364(s_session_player_view *player)
{
	if (player && (player->flags & 1) && function_199967())
	{
		return 1;
	}
	return 0;
}

// @retail 0x25142f
long function_25142f(void)
{
	long result = 0;

	switch (function_199d7c())
	{
	case 4:
		result = 1;
		break;
	case 6:
		result = 2;
		break;
	case 7:
		result = (network_session_manager_get_value49ac() != NONE) + 2;
		break;
	case 8:
		result = 3;
		break;
	}
	return result;
}

/* ---- the matchmaking list's items (0x251384..0x2517e2) ---- */

bool function_199dc9(long a, long b, long c);
long function_199ebc(void);
long function_199ef8(void);
bool function_19a0c5(void);
bool function_19a127(void);
bool function_19a951(long player_index);
bool function_19a9b4(long player_index);
byte *function_19aaa5(long player_index);
byte *function_19ab0e(long player_index);
void function_24c0c4(c_widget *widget);
void function_2b01a2(long value, s_widget_item *item);
void function_2b01b5(s_widget_item *item, short value);
long function_149ead(long value);
void function_22f042(s_widget_item *items, c_user_interface_widget *widget, long count);

/* a player as the matchmaking list shows it */
struct s_matchmaking_player
{
	byte unknown00[0x40];
	dword value40[4];
	byte unknown50[0x7c - 0x50];
	char team;
	byte unknown7d;
	char value7e;
	byte unknown7f[0x81 - 0x7f];
	char value81;
};

/* a datum of the list: a player, 16 for an empty slot or NONE */
struct s_matchmaking_datum
{
	short salt;
	short player;
};

/* by team, then by value7e (the larger first) */
// @retail 0x251384
int __cdecl matchmaking_compare_team_and_value(void const *a, void const *b)
{
	s_matchmaking_player *player_a = (s_matchmaking_player *)function_19ab0e(*(long const *)a);
	s_matchmaking_player *player_b = (s_matchmaking_player *)function_19ab0e(*(long const *)b);

	if (player_a->team > player_b->team)
	{
		return 1;
	}
	if (player_a->team < player_b->team)
	{
		return -1;
	}
	if (player_a->value7e < player_b->value7e)
	{
		return 1;
	}
	return player_a->value7e > player_b->value7e ? -1 : 0;
}

/* by team */
// @retail 0x2513c9
int __cdecl matchmaking_compare_team(void const *a, void const *b)
{
	s_matchmaking_player *player_a = (s_matchmaking_player *)function_19ab0e(*(long const *)a);
	s_matchmaking_player *player_b = (s_matchmaking_player *)function_19ab0e(*(long const *)b);

	if (player_a->team > player_b->team)
	{
		return 1;
	}
	return player_a->team < player_b->team ? -1 : 0;
}

/* by value7e (the larger first) */
// @retail 0x2513fc
int __cdecl matchmaking_compare_value(void const *a, void const *b)
{
	s_matchmaking_player *player_a = (s_matchmaking_player *)function_19ab0e(*(long const *)a);
	s_matchmaking_player *player_b = (s_matchmaking_player *)function_19ab0e(*(long const *)b);

	if (player_a->value7e < player_b->value7e)
	{
		return 1;
	}
	return player_a->value7e > player_b->value7e ? -1 : 0;
}

/* fills the list with the session's players, then the empty slots */
// @retail 0x2514fd
void c_matchmaking_list::v3()
{
	long player_count = 0;
	long index_count = 0;
	long indices[16];
	long item_count;
	long mode = function_25142f();
	bool teams = function_19a0c5();
	bool values = function_19a127();
	long i;

	if (mode != 3 && (mode < 1 || teams))
	{
		long count = function_199ebc();

		item_count = 0;
		item_count = function_199dc9((long)&player_count, (long)&item_count, 0) ? item_count : 0;
		item_count = count > item_count ? count : item_count;
		for (i = 0; i < 16; i++)
		{
			if (function_19a951(i))
			{
				indices[index_count++] = i;
			}
		}
		for (i = index_count; i < 16; i++)
		{
			indices[i] = NONE;
		}
	}
	else
	{
		player_count = function_199ef8();
		for (i = 0; i < 16; i++)
		{
			if (function_19a9b4(i))
			{
				indices[index_count++] = i;
			}
		}
		for (i = index_count; i < 16; i++)
		{
			indices[i] = NONE;
		}
		if (mode == 1)
		{
			long count = 0;

			count = function_199dc9(0, (long)&count, 0) ? count : 0;
			item_count = player_count > count ? player_count : count;
		}
		else
		{
			item_count = player_count;
			if (mode == 3)
			{
				if (teams)
				{
					if (values)
					{
						qsort(indices, index_count, sizeof(long), matchmaking_compare_team_and_value);
					}
					else
					{
						qsort(indices, index_count, sizeof(long), matchmaking_compare_value);
					}
				}
				else if (values)
				{
					qsort(indices, index_count, sizeof(long), matchmaking_compare_team);
				}
			}
		}
	}
	data_delete_all(data);
	for (i = 0; i < item_count; i++)
	{
		long datum_index = datum_new(data);

		if (datum_index != NONE)
		{
			s_matchmaking_datum *datum = &((s_matchmaking_datum *)data->data)[datum_index & 0xffff];

			if (i < index_count)
			{
				datum->player = (short)indices[i];
			}
			else if (i >= player_count)
			{
				datum->player = NONE;
			}
			else
			{
				datum->player = 16;
			}
		}
	}
	function_24c0c4((c_widget *)this);
	((c_widget *)this)->c_widget::v11();
}

/* an empty slot that a player may still fill */
// @retail 0x251703
void c_matchmaking_list::show_open_slot(c_user_interface_widget *item)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)item->find_child(6, 0, false);
	c_user_interface_widget *bitmap = item->find_child(10, 0, false)->find_child(8, 1, false);
	s_widget_item definition;

	function_2b0a14((s_widget_view_2b0a *)bitmap, 1);
	text->value6e = true;
	text->set_string(0xe00075e);
	function_251963(item);
	definition.value5e = true;
	definition.flags = 0x20;
	function_2b01a2(NONE, &definition);
	function_22f042(&definition, item, 1);
}

/* an empty slot */
// @retail 0x251778
void c_matchmaking_list::show_empty_slot(c_user_interface_widget *item)
{
	c_user_interface_widget *text = item->find_child(6, 0, false);
	c_user_interface_widget *bitmap = item->find_child(10, 0, false)->find_child(8, 1, false);
	s_widget_item definition;

	function_2b0a14((s_widget_view_2b0a *)bitmap, 0);
	text->value6e = false;
	function_251963(item);
	definition.value5e = true;
	definition.flags = 0x20;
	function_2b01a2(NONE, &definition);
	function_22f042(&definition, item, 1);
}

/* a player's slot: name, emblem, team and voice */
// @retail 0x2517e2
void c_matchmaking_list::show_player(c_user_interface_widget *item, long player_index)
{
	c_user_interface_widget *text = item->find_child(6, 0, false);
	c_user_interface_widget *models = item->find_child(10, 0, false);
	c_user_interface_widget *bitmap1 = models->find_child(8, 1, false);
	c_user_interface_widget *bitmap2 = models->find_child(8, 2, false);
	c_user_interface_widget *bitmap3 = models->find_child(8, 3, false);
	long mode = function_25142f();
	s_matchmaking_player *player;

	text->value6e = false;
	bitmap1->value6e = false;
	bitmap3->value6e = false;
	bitmap2->value6e = true;
	if (mode != 3 && (mode < 1 || function_19a0c5()))
	{
		if (!function_19a951(player_index))
		{
			goto empty;
		}
		player = (s_matchmaking_player *)function_19aaa5(player_index);
	}
	else
	{
		if (!function_19a9b4(player_index))
		{
			goto empty;
		}
		player = (s_matchmaking_player *)function_19ab0e(player_index);
	}
	if (player)
	{
		s_widget_item definition;

		definition.flags = 0;
		if (function_19a0c5())
		{
			function_2b01a2(player->value7e, &definition);
		}
		else
		{
			function_2b01b5(&definition, (short)function_149ead(NONE));
		}
		memcpy(definition.value48, player->value40, sizeof(definition.value48));
		definition.flags |= 2;
		definition.value4 = (long)player;
		definition.flags |= 1;
		definition.value64 = player->value81;
		definition.flags |= 0x100;
		if (network_session_manager_get_value49ac() != NONE && function_19a127())
		{
			char team = player->team;

			if (team >= 0 && team < 8)
			{
				definition.value5c = team;
				definition.flags |= 4;
				definition.value5f = false;
				definition.flags |= 0x80;
			}
			else if (team == NONE)
			{
				definition.value5c = NONE;
				definition.flags |= 4;
				definition.value5f = true;
				definition.flags |= 0x80;
			}
		}
build:
		function_22f042(&definition, item, 1);
		function_251977(player_index, item);
		return;
	}
empty:
	show_empty_slot(item);
}

// @retail 0x2516b3
void c_matchmaking_list::v20(c_user_interface_widget *item, long unused)
{
	long datum_index = widget_item(item)->value70;

	if (datum_index != NONE)
	{
		long player = ((s_matchmaking_datum *)data->data)[datum_index & 0xffff].player;

		if (player >= 0 && player < 16)
		{
			show_player(item, player);
		}
		else if (player == 16)
		{
			show_open_slot(item);
		}
		else if (player == NONE)
		{
			show_empty_slot(item);
		}
	}
}

/* ---- the matchmaking screen (0x251afa..0x25217b) ---- */

long network_time_since(long time); /* 0x75890, network_observer.cpp */
long function_199f6d(void);
void network_session_manager_request_mode_acknowledge(void);
void function_19a942(void);

/* the title of the matchmaking state */
// @retail 0x251bff
long c_matchmaking_screen::get_title()
{
	switch (value610)
	{
	case 0:
		return 0x1400043a;
	case 1:
		return 0x1200043b;
	case 2:
		return 0x1300043c;
	case 3:
		return 0x1300043d;
	case 4:
		return 0xe00043e;
	case 5:
		return 0xc00043f;
	case 6:
		return 0xd000440;
	case 7:
		return 0xd000440;
	case 8:
		return 0x16000441;
	}
	return 0;
}

// @retail 0x251c68
void function_251c68(c_matchmaking_screen *screen)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)screen->find_child(6, 2, false);

	if (text)
	{
		text->set_string(screen->get_title());
	}
}

/* the dialog's first choice leaves the matchmaking */
// @retail 0x252150
bool __stdcall function_252150(long controller)
{
	g_51ec99 = true;
	network_session_manager_request_mode_acknowledge();
	function_19a942();
	return true;
}

/* the dialog closes by itself once the matchmaking is past its search */
// @retail 0x252166
bool __stdcall function_252166(c_screen_widget *screen, long dialog_id)
{
	bool result = false;

	if (function_25142f() >= 2)
	{
		result = true;
	}
	return result;
}

/* asks whether to leave the matchmaking (not for the first minute once it
   has found a game) */
// @retail 0x25217b
void function_25217b(c_matchmaking_screen *screen, s_widget_event *event)
{
	if (function_25142f() < 2 || network_time_since(screen->value614) >= 60000)
	{
		long dialog_id = (function_199ebc() <= function_199f6d()) + 0x8c;
		long user_flags = event ? 1 << event->controller_index : (short)function_1901fc();

		dialog_choice_show(3, dialog_id, 4, (word)user_flags, function_252150, 0, function_252166);
	}
	else
	{
		function_236299(2);
	}
}

// @retail 0x251afa
bool c_matchmaking_screen::v10(s_widget_event *event)
{
	if (event->type == 5)
	{
		switch (event->param)
		{
		case 1:
		case 13:
			function_25217b(this, event);
			return true;
		case 3:
			if (function_25142f() >= 2)
			{
				return true;
			}
			break;
		}
	}
	return c_screen_widget::v10(event);
}
