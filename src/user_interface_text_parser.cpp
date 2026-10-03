// @flags /O1 /Oi /arch:SSE /Gr
/* USER_INTERFACE_TEXT_PARSER.CPP: the user interface's text parser. A
   string's private use characters (0xe000..0xf8ff) are either glyphs (the
   controller buttons) or stand for a value the parser fills in: the map
   name, the game type, a player's gamertag, the time and so on. Emoticons
   become their glyphs too. */

#include "cseries.h"
#include <string.h>
#include <wchar.h>
#include "globals.h"
#include "data_array.h"
#include "screen_widgets.h"
#include "unknown_234c64.h"

#define NUMBEROF(array) (sizeof(array) / sizeof((array)[0]))

enum
{
	k_parse_buffer_length = 0x100
};

/* ---- callees ---- */

struct s_name_buffer;
void function_08cc20(s_name_buffer *buffer, const wchar_t *name);
void unicode_string_copy(word *destination, const word *source, long maximum_count);
void ascii_string_to_unicode(long maximum_count, const char *source, word *destination);
word *function_1630e0(word *buffer, const word *format, ...);
void function_23620d(long string_id, word *buffer);
void unicode_string_list_get_string(long tag_index, long string_id, word *buffer);
bool function_140420(word c);

byte *network_session_interface_get_data_4db0(void);
long network_session_interface_get_value_49c8(void);
long network_session_interface_find_player_49b8(void);
short network_session_interface_get_value_5dd0(void);
bool network_session_interface_get_unknown64(byte *data, long *unknown84, long *unknown88, long *unknown8c);
long network_session_manager_get_value49ac(void);

bool game_variant_get_name(long index, word *name);
bool game_variant_get_description(long index, word *description);

bool function_19a84e(long *a, long *b);
long function_19989d(void);
long function_199ebc(void);
long function_199ef8(void);
byte *function_19aaa5(long player_index);
bool function_19a902(void);
long function_19a8d0(void);
short function_19ac53(void);

struct s_entry_a;
struct s_entry_b;
struct s_entry_c;
s_entry_a *function_148d61();
s_entry_b *function_19c1f0(long key);
s_entry_c *function_19c5f0(long key);

struct s_localized_name;
struct s_localized_description;
struct s_localized_short_name;
wchar_t *localized_name_get(s_localized_name *definition);
wchar_t *localized_description_get(s_localized_description *definition);
wchar_t *localized_short_name_get(s_localized_short_name *definition);

struct s_player_profile
{
	dword data[0x78];
};

struct s_player_slot_blockb82;
void player_slot_get_profile(long index, s_player_profile *profile, long *profile_index);
void function_18ff47(long player, dword *out);
bool function_18ffc3(long index, s_player_slot_blockb82 *block);

struct s_window_manager_text;
void function_14887e(s_screen_settings_54dc6c *settings);
const char *function_148956(s_window_manager_text *text);
void function_14896e(s_window_manager_754 *a, s_window_manager_df6 *b);
long function_14de70(long local_player_index);

long saved_film_size_in_blocks();
long minimal_storage_size_in_blocks();
long saved_game_file_type_size_in_blocks(long type);
real function_122dd0(byte *map_name, long unknown);
void function_13934d(long string_id, word *buffer);
void function_15ea80(long string_id, long maximum_count, word *buffer);
struct s_friend_request;
bool friend_request_get(s_friend_request *request);
void function_1a33c4(dword *xuid, bool *a, bool *b, long *c, long *d, bool *e, long f);
void title_name_get(wchar_t *name, long name_length, dword title_id);

/* ---- data ---- */

s_game_variant g_54e4a0;

struct s_510c4c;
extern s_510c4c *g_510c4c;

/* the game engine globals' views the hud strings read */
struct s_game_engine_globals_view
{
	byte unknown000[0x1b0];
	long hud_quantity;
	byte unknown1b4[4];
	long betraying_player;
	void *scoreboard;
};

/* a controller's button layout (0x1c bytes, by input user) */
struct s_button_layout
{
	byte unknown00[8];
	byte buttons[0x10];
	short stick_layout;
	byte unknown1a[2];
};

/* the input users' button layouts (unknown_217c20.cpp) */
extern byte g_51ea18[];
extern long g_4b9ed8;

/* a player (0x21c bytes) */
struct s_player_view
{
	byte unknown000[0x24];
	long input_user;
	byte unknown028[0x21c - 0x28];
};

/* the session's last map and the strings the procedures keep */
long g_54e7c0;
long g_54e7c4;
wchar_t g_55c2e4[k_parse_buffer_length];
wchar_t g_55c4e4[k_parse_buffer_length];
wchar_t g_55c6e4[k_parse_buffer_length];
wchar_t g_55c8e4[k_parse_buffer_length];
long g_55c2e0;
long g_55cae4;
long g_55cae8;
extern bool g_51055d;

/* the system time the clock strings show */
struct s_system_time
{
	word year;
	word month;
	word day_of_week;
	word day;
	word hour;
	word minute;
	word second;
	word milliseconds;
};

s_system_time g_54e7ce;

extern dword g_54d5b8;
extern long g_46e7b8;

typedef void (__stdcall *text_parse_proc)(long string_id, word *buffer);

/* a special character: its name, the character and what it stands for (no
   procedure for a glyph) */
struct s_text_parse_entry
{
	const wchar_t *name;
	long character;
	text_parse_proc proc;
};

/* an emoticon and its glyph */
struct s_emoticon
{
	const wchar_t *text;
	long character;
};

static inline s_game_variant *get_game_variant()
{
	s_game_variant *variant = (s_game_variant *)network_session_interface_get_data_4db0();

	if (!variant)
	{
		variant = &g_54e4a0;
	}
	return variant;
}

static inline void parse_copy(word *buffer, const wchar_t *string)
{
	function_08cc20((s_name_buffer *)buffer, string);
}

/* ---- the procedures ---- */

/* the characters the font can't show become a box */
// @retail 0x22d28d
void parse_replace_missing_characters(word *string, long count)
{
	while (count > 0 && *string)
	{
		if (function_140420(*string))
		{
			*string = 0x25a1;
		}
		string++;
		count--;
	}
}

// @retail 0x22d5a7
void __stdcall parse_mapname(long string_id, word *buffer)
{
	long campaign_id;
	long map_id;
	const wchar_t *name;

	if (function_19a84e(&campaign_id, &map_id))
	{
		if (campaign_id != NONE)
		{
			s_entry_b *level = function_19c1f0(map_id);

			if (!level)
			{
				goto fallback;
			}
			name = localized_name_get((s_localized_name *)level);
		}
		else
		{
			s_entry_c *map = function_19c5f0(map_id);

			if (!map)
			{
				goto fallback;
			}
			name = localized_short_name_get((s_localized_short_name *)map);
		}
		if (name)
		{
			goto copy;
		}
	}
fallback:
	if (g_54e7c0 == NONE || g_54e7c4 == NONE)
	{
		goto done;
	}
	{
		s_entry_b *level = function_19c1f0(g_54e7c4);

		if (!level)
		{
			goto done;
		}
		name = localized_name_get((s_localized_name *)level);
		if (!name)
		{
			goto done;
		}
	}
copy:
	parse_copy((word *)g_55c2e4, name);
done:
	parse_copy(buffer, g_55c2e4);
}

// @retail 0x22d456
void __stdcall parse_gametype(long string_id, word *buffer)
{
	s_user_interface_globals *globals = function_148350();

	if (globals)
	{
		s_game_variant *variant = get_game_variant();
		long tag_index = *(long *)((byte *)globals + 0x14c);
		long name_id;

		switch (variant->game_engine_index)
		{
		case 0:
			name_id = 0x800010b;
			break;
		case 1:
			name_id = 0x1000010d;
			break;
		case 9:
			name_id = 0x700010e;
			break;
		case 2:
			name_id = 0x600010f;
			break;
		case 3:
			name_id = 0x7000110;
			break;
		case 4:
			name_id = 0x10000111;
			break;
		case 7:
			name_id = 0xa000113;
			break;
		case 8:
			name_id = 0xb000115;
			break;
		default:
			goto done;
		}
		unicode_string_list_get_string(tag_index, name_id, (word *)g_55c6e4);
	}
done:
	parse_copy(buffer, g_55c6e4);
}
// @retail 0x22d503
void __stdcall parse_variant(long string_id, word *buffer)
{
	parse_copy((word *)g_55c4e4, get_game_variant()->name);
	parse_copy(buffer, g_55c4e4);
}

// @retail 0x22d52e
void __stdcall parse_hopper_name(long string_id, word *buffer)
{
	word name[0x10];
	long hopper = network_session_interface_get_value_49c8();

	if (hopper != NONE)
	{
		g_55c2e0 = hopper;
	}
	if (game_variant_get_name(g_55c2e0, name))
	{
		parse_copy(buffer, name);
	}
}

// @retail 0x22d566
void __stdcall parse_hopper_description(long string_id, word *buffer)
{
	word description[0x80];
	long hopper = network_session_interface_get_value_49c8();

	if (hopper != NONE)
	{
		g_55c2e0 = hopper;
	}
	if (game_variant_get_description(g_55c2e0, description))
	{
		parse_copy(buffer, description);
	}
}

// @retail 0x22d62b
void __stdcall parse_motion_sensor_enabled(long string_id, word *buffer)
{
	s_game_variant *variant = (s_game_variant *)network_session_interface_get_data_4db0();

	if (variant && TEST_FIELD_BIT(variant->motion_sensor_enabled))
	{
		function_23620d(0x2000181, buffer);
	}
	else
	{
		function_23620d(0x3000182, buffer);
	}
}

// @retail 0x22d655
void __stdcall parse_teams_enabled(long string_id, word *buffer)
{
	s_game_variant *variant = (s_game_variant *)network_session_interface_get_data_4db0();

	if (variant && TEST_FIELD_BIT(variant->teams_enabled))
	{
		function_23620d(0x7000183, buffer);
	}
	else
	{
		function_23620d(0x8000184, buffer);
	}
}

/* the vehicle set and the build number show nothing */
// @retail 0x22d82c
void __stdcall parse_nothing(long string_id, word *buffer)
{
}

// @retail 0x22d67c
void __stdcall parse_weapon_set(long string_id, word *buffer)
{
	function_23620d(0x60000b8, buffer);
}

// @retail 0x22d68f
void __stdcall parse_player_profile_name(long string_id, word *buffer)
{
	s_player_profile profile;
	long player;

	switch (string_id)
	{
	default:
		parse_copy(buffer, (wchar_t *)&g_54e5d0.settings.unknown000[8]);
		return;
	case 0xe40f:
		player = 0;
		break;
	case 0xe410:
		player = 1;
		break;
	case 0xe411:
		player = 2;
		break;
	case 0xe412:
		player = 3;
		break;
	}
	player_slot_get_profile(player, &profile, &string_id);
	parse_copy(buffer, (wchar_t *)&profile.data[2]);
}
// @retail 0x22d6ee
void __stdcall parse_player_gamertag(long string_id, word *buffer)
{
	dword identity[0x1c];
	long player;

	switch (string_id)
	{
	case 0xe429:
		player = 0;
		break;
	case 0xe42a:
		player = 1;
		break;
	case 0xe42b:
		player = 2;
		break;
	default:
		player = 3;
		break;
	}
	function_18ff47(player, identity);
	ascii_string_to_unicode(k_parse_buffer_length, (const char *)&identity[3], buffer);
}

// @retail 0x22d730
void __stdcall parse_countdown(long string_id, word *buffer)
{
	if (function_19a902())
	{
		long seconds = function_19a8d0();
		long minutes = seconds / 60;

		function_1630e0(buffer, (const word *)L"%d:%02d", minutes, seconds - minutes * 60);
	}
	else
	{
		parse_copy(buffer, L"-:--");
	}
}

// @retail 0x22d775
void __stdcall parse_matchmaking_countdown(long string_id, word *buffer)
{
	long seconds = network_session_manager_get_value49ac();

	if (seconds != NONE)
	{
		function_1630e0(buffer, (const word *)L":%02d", seconds);
	}
	else
	{
		parse_copy(buffer, L":--");
	}
}

// @retail 0x22d7a1
void __stdcall parse_new_content(long string_id, word *buffer)
{
	if (g_51055d)
	{
		function_1630e0(buffer, (const word *)L"%c", 0xe007);
	}
	else
	{
		buffer[0] = 0;
	}
}

// @retail 0x22d7ce
void __stdcall parse_map_load_percent(long string_id, word *buffer)
{
	real progress = 0.0f;
	byte *entry = (byte *)function_148d61();

	if (entry)
	{
		progress = function_122dd0(entry + 8, 0);
	}
	function_1630e0(buffer, (const word *)L"%d", (long)(progress * 100.0f));
}

// @retail 0x22d80b
void __stdcall parse_name_of_last_person_to_delay_countdown(long string_id, word *buffer)
{
	long player_index = network_session_interface_find_player_49b8();

	if (player_index != NONE)
	{
		byte *player = function_19aaa5(player_index);

		if (player)
		{
			parse_copy(buffer, (wchar_t *)player);
		}
	}
}

// @retail 0x22d82f
void __stdcall parse_leader(long string_id, word *buffer)
{
	long player_index = function_19ac53();

	if (player_index != NONE)
	{
		byte *player = function_19aaa5(player_index);

		if (player)
		{
			parse_copy((word *)g_55c8e4, (wchar_t *)player);
		}
	}
	parse_copy(buffer, g_55c8e4);
}

/* the quality of service values */
// @retail 0x22d85f
void __stdcall parse_qos(long string_id, word *buffer)
{
	struct
	{
		long probes_sent;
		long probes_received;
		long rtt_minimum;
		long rtt_median;
		long bandwidth_up;
		long bandwidth_down;
		byte unknown18[8];
	} qos;
	long estimated_bandwidth;
	long maximum_machines;
	long nat_type;
	long value = 0;
	real kilobits;

	if (network_session_interface_get_unknown64((byte *)&qos, &nat_type, &estimated_bandwidth, &maximum_machines))
	{
		switch (string_id)
		{
		case 0xe417:
			value = qos.probes_sent;
			break;
		case 0xe418:
			value = qos.probes_received;
			break;
		case 0xe419:
			value = qos.rtt_minimum;
			break;
		case 0xe41a:
			value = qos.rtt_median;
			break;
		case 0xe41b:
			kilobits = (real)qos.bandwidth_down * (1.0f / 1024.0f);
			goto print_real;
		case 0xe41c:
			kilobits = (real)qos.bandwidth_up * (1.0f / 1024.0f);
			goto print_real;
		case 0xe41d:
			value = maximum_machines;
			break;
		case 0xe41e:
			value = nat_type;
			break;
		case 0xe432:
			value = (long)((real)estimated_bandwidth * (1.0f / 1024.0f));
			break;
		}
	}
	function_1630e0(buffer, (const word *)L"%d", value);
	return;

print_real:
	if (kilobits != 0.0f)
	{
		function_1630e0(buffer, (const word *)L"%.1f", (double)kilobits);
		return;
	}
	function_1630e0(buffer, (const word *)L"%d", value);
}

// @retail 0x22d93b
void __stdcall parse_active_multiplayer_protocol(long string_id, word *buffer)
{
	switch (function_19989d())
	{
	case 0:
		g_55cae8 = 0x100005be;
		break;
	case 1:
		g_55cae8 = 0x120005c0;
		break;
	case 2:
		g_55cae8 = 0x100005c1;
		break;
	case 3:
		g_55cae8 = 0x120005c3;
		break;
	case 4:
		g_55cae8 = 0x90005c4;
		break;
	case 5:
		g_55cae8 = 0xb0005c6;
		break;
	case 6:
		g_55cae8 = 0xe0005c8;
		break;
	}
	function_23620d(g_55cae8, buffer);
}
// @retail 0x22d99e
void __stdcall parse_live_ui_driver_gamertag(long string_id, word *buffer)
{
	ascii_string_to_unicode(k_parse_buffer_length, (const char *)0x54e438, buffer);
}

// @retail 0x22d9b4
void __stdcall parse_live_ui_driver_clan_name(long string_id, word *buffer)
{
	byte clan[0x6a4];

	if (friend_request_get((s_friend_request *)clan))
	{
		parse_copy(buffer, (wchar_t *)&clan[0xc]);
	}
}

// @retail 0x22d9e0
void __stdcall parse_hud_quantity(long string_id, word *buffer)
{
	long quantity = 0;

	if (string_id == 0xe422)
	{
		quantity = ((s_game_engine_globals_view *)g_510c4c)->hud_quantity;
	}
	function_1630e0(buffer, (const word *)L"%d", quantity);
}

// @retail 0x22da0d
void __stdcall parse_hud_betraying_player(long string_id, word *buffer)
{
	long player_index = NONE;

	if (string_id == 0xe458)
	{
		player_index = ((s_game_engine_globals_view *)g_510c4c)->betraying_player;
	}
	function_1630e0(buffer, (const word *)L"??");
	if (player_index != NONE)
	{
		byte *player = datum_get(g_4e8c24, player_index);

		if (player)
		{
			function_1630e0(buffer, (const word *)L"%s", player + 0x44);
		}
	}
}

// @retail 0x22da65
void __stdcall parse_hud_scoreboard(long string_id, word *buffer)
{
	function_13934d(string_id, buffer);
}

// @retail 0x22da75
void __stdcall parse_ge_round_time_left(long string_id, word *buffer)
{
	word time[0x14];

	if (g_4e6948->state == 2)
	{
		function_15ea80(string_id, NUMBEROF(time), time);
		parse_copy(buffer, time);
	}
}

// @retail 0x22daa5
void __stdcall parse_target_player_gamertag(long string_id, word *buffer)
{
	s_screen_settings_54dc6c settings;
	const char *gamertag;

	function_14887e(&settings);
	gamertag = function_148956((s_window_manager_text *)&settings);
	if (gamertag)
	{
		ascii_string_to_unicode(k_parse_buffer_length, gamertag, buffer);
	}
}

// @retail 0x22dad1
void __stdcall parse_game_player_count(long string_id, word *buffer)
{
	long names[0x11];
	long count;

	names[0] = 0x100030a;
	names[1] = 0x100030b;
	names[2] = 0x100030c;
	names[3] = 0x100030d;
	names[4] = 0x100030e;
	names[5] = 0x100030f;
	names[6] = 0x1000310;
	names[7] = 0x1000311;
	names[8] = 0x1000312;
	names[9] = 0x1000313;
	names[10] = 0x2000314;
	names[11] = 0x2000315;
	names[12] = 0x2000316;
	names[13] = 0x2000317;
	names[14] = 0x2000318;
	names[15] = 0x2000319;
	names[16] = 0x200031a;
	count = function_199ebc() > function_199ef8() ? function_199ebc() : function_199ef8();
	if (count >= 0)
	{
		if (count > 0x10)
		{
			count = 0x10;
		}
		if (count)
		{
			g_55cae4 = count;
		}
	}
	function_23620d(names[g_55cae4], buffer);
}

// @retail 0x22db96
void __stdcall parse_coop_level_name(long string_id, word *buffer)
{
	long campaign_id = NONE;
	long map_id = NONE;

	buffer[0] = 0;
	function_19a84e(&campaign_id, &map_id);
	if (campaign_id != NONE && map_id != NONE)
	{
		s_entry_b *level = function_19c1f0(map_id);

		if (level)
		{
			parse_copy(buffer, localized_name_get((s_localized_name *)level));
		}
	}
}

// @retail 0x22dbe0
void __stdcall parse_coop_level_description(long string_id, word *buffer)
{
	long campaign_id = NONE;
	long map_id = NONE;

	buffer[0] = 0;
	function_19a84e(&campaign_id, &map_id);
	if (campaign_id != NONE && map_id != NONE)
	{
		s_entry_b *level = function_19c1f0(map_id);

		if (level)
		{
			parse_copy(buffer, localized_description_get((s_localized_description *)level));
		}
	}
}

// @retail 0x22dc2a
void __stdcall parse_coop_difficulty(long string_id, word *buffer)
{
	long name_id;

	switch (network_session_interface_get_value_5dd0())
	{
	case 0:
		name_id = 0x400028e;
		break;
	case 1:
		name_id = 0x60000b8;
		break;
	case 2:
		name_id = 0x600028f;
		break;
	case 3:
		name_id = 0x9000290;
		break;
	default:
		name_id = 0x60000b8;
		break;
	}
	function_23620d(name_id, buffer);
}

/* sizes in blocks */
// @retail 0x22dc68
void __stdcall parse_blocks(long string_id, word *buffer)
{
	long blocks = 0;

	switch (string_id)
	{
	case 0xe433:
		blocks = saved_game_file_type_size_in_blocks(0);
		break;
	case 0xe434:
		blocks = saved_game_file_type_size_in_blocks(1);
		break;
	case 0xe436:
		blocks = 0x4a2b1e2;
		break;
	case 0xe444:
		blocks = saved_film_size_in_blocks();
		break;
	case 0xe445:
		blocks = minimal_storage_size_in_blocks();
		break;
	}
	function_1630e0(buffer, (const word *)L"%d", blocks);
}

// @retail 0x22e1ed
void __stdcall parse_animating_thumbstick(long string_id, word *buffer)
{
	word character[2];
	dword phase = g_54d5b8 % 500;

	if (phase < 0xa6)
	{
		switch (string_id)
		{
		case 0xe459:
			character[0] = 0xe12c;
			break;
		case 0xe45a:
			character[0] = 0xe12f;
			break;
		default:
			character[0] = '?';
			break;
		}
	}
	else if (phase < 0x14d)
	{
		switch (string_id)
		{
		case 0xe459:
			character[0] = 0xe12d;
			break;
		case 0xe45a:
			character[0] = 0xe130;
			break;
		default:
			character[0] = '?';
			break;
		}
	}
	else
	{
		switch (string_id)
		{
		case 0xe459:
			character[0] = 0xe12e;
			break;
		case 0xe45a:
			character[0] = 0xe131;
			break;
		default:
			character[0] = '?';
			break;
		}
	}
	character[1] = 0;
	parse_copy(buffer, character);
}

// @retail 0x22dcbc
void __stdcall parse_button(long string_id, word *buffer)
{
	long player_index = function_14de70(g_4b9ed8);
	char index;
	word character[2];

	switch (string_id)
	{
	case 0xe446:
		index = 0;
		break;
	case 0xe447:
		index = 1;
		break;
	case 0xe448:
		index = 2;
		break;
	case 0xe449:
		index = 3;
		break;
	case 0xe44a:
		index = 4;
		break;
	case 0xe44b:
		index = 5;
		break;
	case 0xe44c:
		index = 6;
		break;
	case 0xe44d:
		index = 7;
		break;
	case 0xe44e:
		index = 8;
		break;
	case 0xe44f:
		index = 9;
		break;
	case 0xe450:
		index = 10;
		break;
	case 0xe451:
		index = 11;
		break;
	case 0xe452:
		index = 12;
		break;
	case 0xe453:
		index = 13;
		break;
	case 0xe454:
		index = 14;
		break;
	case 0xe455:
		index = 15;
		break;
	case 0xe45e:
		index = NONE;
		break;
	default:
		__assume(0);
	}
	if (player_index != NONE)
	{
		s_player_view *player = &((s_player_view *)g_4e8c24->data)[player_index & 0xffff];
		long input_user = player->input_user;

		if (input_user != NONE)
		{
			s_button_layout layout;
			long button;
			word thumbstick[k_parse_buffer_length];

			thumbstick[0] = 0;
			character[1] = 0;
			layout = ((s_button_layout *)g_51ea18)[input_user];
			character[0] = '?';
			if (index == NONE)
			{
				button = layout.buttons[7] == 7 ? 6 : 7;
			}
			else
			{
				button = layout.buttons[index];
			}
			switch (button)
			{
			case 0:
				character[0] = 0xe100;
				break;
			case 1:
				character[0] = 0xe101;
				break;
			case 2:
				character[0] = 0xe102;
				break;
			case 3:
				character[0] = 0xe103;
				break;
			case 4:
				character[0] = 0xe104;
				break;
			case 5:
				character[0] = 0xe105;
				break;
			case 6:
				character[0] = 0xe106;
				break;
			case 7:
				character[0] = 0xe107;
				break;
			case 8:
				character[0] = 0xe108;
				break;
			case 9:
				character[0] = 0xe109;
				break;
			case 10:
				character[0] = 0xe10a;
				break;
			case 11:
				character[0] = 0xe10b;
				break;
			case 12:
				character[0] = 0xe10c;
				break;
			case 13:
				character[0] = 0xe10d;
				break;
			case 14:
				parse_animating_thumbstick(0xe459, thumbstick);
				character[0] = thumbstick[0];
				break;
			case 15:
				parse_animating_thumbstick(0xe45a, thumbstick);
				character[0] = thumbstick[0];
				break;
			}
			parse_copy(buffer, character);
		}
	}
	else
	{
		function_1630e0(buffer, (const word *)L"?");
	}
}

// @retail 0x22df04
void __stdcall parse_stick(long string_id, word *buffer)
{
	long player_index = function_14de70(g_4b9ed8);
	word character[4];

	memset(character, 0, sizeof(character));

	if (player_index != NONE)
	{
		s_player_view *player = &((s_player_view *)g_4e8c24->data)[player_index & 0xffff];
		long input_user = player->input_user;

		if (input_user != NONE)
		{
			s_button_layout layout = ((s_button_layout *)g_51ea18)[input_user];

			if (string_id == 0xe456)
			{
				switch (layout.stick_layout)
				{
				case 0:
					character[0] = 0xe110;
					break;
				case 1:
					character[0] = 0xe111;
					break;
				case 2:
					character[0] = 0xe110;
					break;
				case 3:
					character[0] = 0xe111;
					break;
				}
			}
			else if (string_id == 0xe457)
			{
				switch (layout.stick_layout)
				{
				case 0:
					character[0] = 0xe111;
					break;
				case 1:
					character[0] = 0xe110;
					break;
				case 2:
					character[0] = 0xe111;
					break;
				case 3:
					character[0] = 0xe110;
					break;
				}
			}
		}
	}
	parse_copy(buffer, character);
}

#pragma pack(push, 4)
/* the player a screen targets, as the window manager keeps it (0x78
   bytes) */
struct s_target_player
{
	long type;
	unsigned __int64 xuid;
	byte unknown0c[0x78 - 0xc];
};

/* a player's identity (0x6a2 bytes) and clan (0x92 bytes) */
struct s_player_identity_view
{
	unsigned __int64 xuid;
	long unknown08;
	wchar_t clan_name[0x349];
};

struct s_clan_view
{
	unsigned __int64 clan_id;
	byte unknown08[0x1c - 0x08];
	long level;
	byte unknown20[0x92 - 0x20];
};
#pragma pack(pop)

/* whether the window manager targets a player */
static inline bool target_player_valid(s_target_player *target)
{
	function_14887e((s_screen_settings_54dc6c *)target);
	switch (target->type)
	{
	case 1:
	case 2:
		return target->xuid != 0;
	}
	return false;
}

// @retail 0x22dfa9
void __stdcall parse_target_player_clan_name(long string_id, word *buffer)
{
	s_target_player target;

	if (target_player_valid(&target))
	{
		s_player_identity_view identity;
		s_clan_view clan;

		function_14896e((s_window_manager_754 *)&identity, (s_window_manager_df6 *)&clan);
		if (identity.xuid)
		{
			parse_copy(buffer, identity.clan_name);
		}
	}
}

// @retail 0x22e001
void __stdcall parse_target_player_game_title_name(long string_id, word *buffer)
{
	s_target_player target;

	if (target_player_valid(&target))
	{
		bool online;
		bool joinable;
		bool playing;
		long title_id;
		long state;
		word title_name[0x40];

		function_1a33c4((dword *)&target.xuid, &online, &joinable, &state, &title_id, &playing, 0);
		if (title_id)
		{
			title_name_get((wchar_t *)title_name, NUMBEROF(title_name), title_id);
			parse_copy(buffer, title_name);
		}
	}
}

// @retail 0x22e06c
void __stdcall parse_target_player_clan_level(long string_id, word *buffer)
{
	s_player_identity_view identity;
	s_clan_view clan;
	long name_id = 0;

	function_14896e((s_window_manager_754 *)&identity, (s_window_manager_df6 *)&clan);
	if (identity.xuid && clan.clan_id)
	{
		switch (clan.level)
		{
		case 0:
			name_id = 0x40002c6;
			break;
		case 1:
			name_id = 0x60002c7;
			break;
		case 2:
			name_id = 0xd0002c8;
			break;
		case 3:
			name_id = 0x90002c9;
			break;
		}
	}
	function_23620d(name_id, buffer);
}

// @retail 0x22e0df
void __stdcall parse_live_ui_driver_clan_level(long string_id, word *buffer)
{
	s_clan_view clan;
	long name_id = 0;

	if (function_18ffc3(g_46e7b8, (s_player_slot_blockb82 *)&clan) && clan.clan_id)
	{
		switch (clan.level)
		{
		case 0:
			name_id = 0x40002c6;
			break;
		case 1:
			name_id = 0x60002c7;
			break;
		case 2:
			name_id = 0xd0002c8;
			break;
		case 3:
			name_id = 0x90002c9;
			break;
		}
	}
	function_23620d(name_id, buffer);
}

// @retail 0x22e148
void __stdcall parse_hour(long string_id, word *buffer)
{
	word hour = g_54e7ce.hour;

	if (string_id == 0xe43e && hour > 12)
	{
		hour = hour - 12;
	}
	function_1630e0(buffer, (const word *)L"%d", hour);
}

// @retail 0x22e179
void __stdcall parse_minute(long string_id, word *buffer)
{
	function_1630e0(buffer, (const word *)L"%02d", g_54e7ce.minute);
}

// @retail 0x22e196
void __stdcall parse_day(long string_id, word *buffer)
{
	function_1630e0(buffer, (const word *)L"%d", g_54e7ce.day);
}

// @retail 0x22e1b3
void __stdcall parse_month(long string_id, word *buffer)
{
	function_1630e0(buffer, (const word *)L"%d", g_54e7ce.month);
}

// @retail 0x22e1d0
void __stdcall parse_year(long string_id, word *buffer)
{
	function_1630e0(buffer, (const word *)L"%d", g_54e7ce.year);
}

/* ---- the tables ---- */

const s_emoticon g_44a3f0[] =
{
	{ L":)", 0x263b },
	{ L":-)", 0x263b },
	{ L":o)", 0x263b },
	{ L":P", 0xe004 },
	{ L":-P", 0xe004 },
	{ L":oP", 0xe004 },
	{ L":(", 0x2639 },
	{ L":-(", 0x2639 },
	{ L":o(", 0x2639 },
	{ L":|", 0xe002 },
	{ L":-|", 0xe002 },
	{ L":o|", 0xe002 },
	{ L":D", 0xe001 },
	{ L":-D", 0xe001 },
	{ L":oD", 0xe001 },
	{ L";)", 0xe000 },
	{ L";-)", 0xe000 },
	{ L";o)", 0xe000 }
};

s_text_parse_entry g_470200[] =
{
	{ L"&", 0x26, 0 },
	{ L"a-button", 0xe100, 0 },
	{ L"b-button", 0xe101, 0 },
	{ L"x-button", 0xe102, 0 },
	{ L"y-button", 0xe103, 0 },
	{ L"black-button", 0xe104, 0 },
	{ L"white-button", 0xe105, 0 },
	{ L"left-trigger", 0xe106, 0 },
	{ L"right-trigger", 0xe107, 0 },
	{ L"dpad-up", 0xe108, 0 },
	{ L"dpad-down", 0xe109, 0 },
	{ L"dpad-left", 0xe10a, 0 },
	{ L"dpad-right", 0xe10b, 0 },
	{ L"start-button", 0xe10c, 0 },
	{ L"back-button", 0xe10d, 0 },
	{ L"left-thumb", 0xe10e, 0 },
	{ L"right-thumb", 0xe10f, 0 },
	{ L"left-stick", 0xe110, 0 },
	{ L"right-stick", 0xe111, 0 },
	{ L"mapname", 0xe408, parse_mapname },
	{ L"gametype", 0xe409, parse_gametype },
	{ L"variant", 0xe40a, parse_variant },
	{ L"hopper_name", 0xe43a, parse_hopper_name },
	{ L"hopper_description", 0xe43b, parse_hopper_description },
	{ L"motion_sensor_enabled", 0xe40b, parse_motion_sensor_enabled },
	{ L"teams_enabled", 0xe40c, parse_teams_enabled },
	{ L"vehicle_set", 0xe40d, parse_nothing },
	{ L"weapon_set", 0xe40e, parse_weapon_set },
	{ L"player0_profile_name", 0xe40f, parse_player_profile_name },
	{ L"player1_profile_name", 0xe410, parse_player_profile_name },
	{ L"player2_profile_name", 0xe411, parse_player_profile_name },
	{ L"player3_profile_name", 0xe412, parse_player_profile_name },
	{ L"player0_gamertag", 0xe429, parse_player_gamertag },
	{ L"player1_gamertag", 0xe42a, parse_player_gamertag },
	{ L"player2_gamertag", 0xe42b, parse_player_gamertag },
	{ L"player3_gamertag", 0xe42c, parse_player_gamertag },
	{ L"countdown", 0xe413, parse_countdown },
	{ L"name_of_last_person_to_delay_countdown", 0xe414, parse_name_of_last_person_to_delay_countdown },
	{ L"build_number", 0xe415, parse_nothing },
	{ L"leader", 0xe416, parse_leader },
	{ L"qprs", 0xe417, parse_qos },
	{ L"qprr", 0xe418, parse_qos },
	{ L"qpimi", 0xe419, parse_qos },
	{ L"qpime", 0xe41a, parse_qos },
	{ L"qbwd", 0xe41b, parse_qos },
	{ L"qbwu", 0xe41c, parse_qos },
	{ L"qmaxm", 0xe41d, parse_qos },
	{ L"qebw", 0xe432, parse_qos },
	{ L"qnat", 0xe41e, parse_qos },
	{ L"active_multiplayer_protocol", 0xe41f, parse_active_multiplayer_protocol },
	{ L"live_ui_driver_gamertag", 0xe420, parse_live_ui_driver_gamertag },
	{ L"live_ui_driver_clan_name", 0xe421, parse_live_ui_driver_clan_name },
	{ L"battle_rifle", 0xe112, 0 },
	{ L"bomb", 0xe113, 0 },
	{ L"brute_shot", 0xe114, 0 },
	{ L"carbine", 0xe115, 0 },
	{ L"flag", 0xe116, 0 },
	{ L"flak_cannon", 0xe117, 0 },
	{ L"flamethrower", 0xe118, 0 },
	{ L"fuel_rod", 0xe119, 0 },
	{ L"magnum", 0xe11a, 0 },
	{ L"needler", 0xe11b, 0 },
	{ L"oddball", 0xe11c, 0 },
	{ L"plasma_pistol", 0xe11d, 0 },
	{ L"plasma_rifle", 0xe11e, 0 },
	{ L"plasma_sword", 0xe11f, 0 },
	{ L"rocket_launcher", 0xe120, 0 },
	{ L"shotgun", 0xe121, 0 },
	{ L"smg", 0xe122, 0 },
	{ L"silenced_smg", 0xe12b, 0 },
	{ L"sniper", 0xe123, 0 },
	{ L"beam_rifle", 0xe124, 0 },
	{ L"disintegrator", 0xe125, 0 },
	{ L"sentinel_beam_weapon", 0xe126, 0 },
	{ L"sentinel_needle_weapon", 0xe127, 0 },
	{ L"sentinel_grenade_weapon", 0xe128, 0 },
	{ L"sentinel_charge_weapon", 0xe129, 0 },
	{ L"brute_plasma_rifle", 0xe12a, 0 },
	{ L"hud_quantity", 0xe422, parse_hud_quantity },
	{ L"hud_betraying_player", 0xe458, parse_hud_betraying_player },
	{ L"ge_round_time_left", 0xe423, parse_ge_round_time_left },
	{ L"target_player_gamertag", 0xe424, parse_target_player_gamertag },
	{ L"game_player_count", 0xe425, parse_game_player_count },
	{ L"coop_level_name", 0xe426, parse_coop_level_name },
	{ L"coop_level_description", 0xe427, parse_coop_level_description },
	{ L"coop_difficulty", 0xe428, parse_coop_difficulty },
	{ L"hud_scoreboard_player_score", 0xe42d, parse_hud_scoreboard },
	{ L"hud_scoreboard_other_player_score", 0xe42e, parse_hud_scoreboard },
	{ L"hud_scoreboard_variant_name", 0xe42f, parse_hud_scoreboard },
	{ L"hud_scoreboard_time_left", 0xe430, parse_hud_scoreboard },
	{ L"edit_player_profile_name", 0xe431, parse_player_profile_name },
	{ L"player_profile_blocks", 0xe433, parse_blocks },
	{ L"game_variant_blocks", 0xe434, parse_blocks },
	{ L"playlist_blocks", 0xe435, parse_blocks },
	{ L"saved_film_blocks", 0xe436, parse_blocks },
	{ L"target_player_clan_name", 0xe437, parse_target_player_clan_name },
	{ L"target_player_game_title_name", 0xe438, parse_target_player_game_title_name },
	{ L"target_player_clan_level", 0xe43c, parse_target_player_clan_level },
	{ L"live_ui_driver_clan_level", 0xe43d, parse_live_ui_driver_clan_level },
	{ L"hour_12", 0xe43e, parse_hour },
	{ L"hour_24", 0xe43f, parse_hour },
	{ L"minute", 0xe440, parse_minute },
	{ L"day", 0xe441, parse_day },
	{ L"month", 0xe442, parse_month },
	{ L"year", 0xe443, parse_year },
	{ L"saved_game_size", 0xe444, parse_blocks },
	{ L"total_minimal_storage_blocks", 0xe445, parse_blocks },
	{ L"button_jump", 0xe446, parse_button },
	{ L"button_switch_grenade", 0xe447, parse_button },
	{ L"button_action_reload", 0xe448, parse_button },
	{ L"button_switch_weapon", 0xe449, parse_button },
	{ L"button_melee_attack", 0xe44a, parse_button },
	{ L"button_flashlight", 0xe44b, parse_button },
	{ L"button_throw_grenade", 0xe44c, parse_button },
	{ L"button_fire", 0xe44d, parse_button },
	{ L"button_start", 0xe44e, parse_button },
	{ L"button_back", 0xe44f, parse_button },
	{ L"button_crouch", 0xe450, parse_button },
	{ L"button_scope_zoom", 0xe451, parse_button },
	{ L"button_lean_left", 0xe452, parse_button },
	{ L"button_lean_right", 0xe453, parse_button },
	{ L"button_accept", 0xe454, parse_button },
	{ L"button_cancel", 0xe455, parse_button },
	{ L"stick_move", 0xe456, parse_stick },
	{ L"stick_look", 0xe457, parse_stick },
	{ L"animating_left_thumbstick", 0xe459, parse_animating_thumbstick },
	{ L"animating_right_thumbstick", 0xe45a, parse_animating_thumbstick },
	{ L"matchmaking_countdown", 0xe45b, parse_matchmaking_countdown },
	{ L"new-content", 0xe45c, parse_new_content },
	{ L"map_load_percent", 0xe45d, parse_map_load_percent },
	{ L"button_dual_fire", 0xe45e, parse_button }
};

/* fills in the string's special characters and emoticons, as long as the
   string stays shorter than the maximum length */
// @retail 0x22d2ee
void parse_string(word *string, long maximum_length)
{
	word *character = string;
	long length = wcslen(string);

	while (*character)
	{
		word replacement[k_parse_buffer_length];
		long replaced_length;
		long c = *character;
		long i;

		replacement[0] = 0;
		replaced_length = 0;
		if (c >= 0xe000 && c <= 0xf8ff)
		{
			for (i = 0; i < NUMBEROF(g_470200); i++)
			{
				if (c == g_470200[i].character)
				{
					if (g_470200[i].proc)
					{
						replacement[0] = 0;
						g_470200[i].proc(c, replacement);
						replaced_length = 1;
						goto replace;
					}
					break;
				}
			}
		}
		for (i = 0; i < NUMBEROF(g_44a3f0); i++)
		{
			long text_length = wcslen(g_44a3f0[i].text);

			if (!wcsncmp(g_44a3f0[i].text, character, text_length))
			{
				word glyph[2];

				glyph[0] = (word)g_44a3f0[i].character;
				glyph[1] = 0;
				wcsncpy(replacement, glyph, k_parse_buffer_length - 1);
				replacement[k_parse_buffer_length - 1] = 0;
				replaced_length = text_length;
			}
		}
replace:
		{
			long replacement_length = wcslen(replacement);
			long new_length = replacement_length - replaced_length + length;

			if (new_length < maximum_length)
			{
				memcpy(character + replacement_length, character + replaced_length, (length - (character - string)) * sizeof(word));
				memcpy(character, replacement, replacement_length * sizeof(word));
				string[new_length] = 0;
				length = new_length;
			}
		}
		character++;
	}
}

/* parses a string in place */
// @retail 0x22d2b7
void parse_text(word *string)
{
	word buffer[k_parse_buffer_length];

	unicode_string_copy(buffer, string, k_parse_buffer_length);
	parse_string(buffer, k_parse_buffer_length);
	parse_copy(string, buffer);
}
