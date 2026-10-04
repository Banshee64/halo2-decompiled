#pragma once
/* the game variant as the variant menus and the session pass it, its
   defaults and its checks (unknown_19d220.cpp, lane H) */

#include "unknown_11c920.h"
#include <stddef.h>

/* the settings of capture the flag (1) and assault (9) */
struct s_game_variant_flag_settings
{
	dword flags;          // 8 bits
	long unknownf4;       // 0..0x7fff
	long unknownf8;       // 0..2
	long unknownfc;       // 0..1
	long unknown100;      // 0..3
	long unknown104;      // 0..2
	short unknown108;     // 0..15 (assault)
	short unknown10a;     // 0..15 (assault)
};

/* the settings of oddball (3) */
struct s_game_variant_ball_settings
{
	dword flags;          // 3 bits
	short unknownf4;      // 0..3
	short unknownf6;      // 0..1
	short unknownf8;      // 0..2
	short unknownfa;      // 0..3
};

/* the settings of king of the hill (4) and juggernaut (7) */
struct s_game_variant_king_settings
{
	dword flags;          // 5 bits (king), 7 bits (juggernaut)
	short unknownf4;
};

/* the settings of territories (8) */
struct s_game_variant_territories_settings
{
	short unknownf0;      // 1..8
	short unknownf2;      // 1..0x7fff
	short unknownf4;      // 1..0x7fff
};

/* a game variant (0x130 bytes): its name, its game engine and flags, as the
   variant menus, the session and the user interface pass it */
struct s_game_variant
{
	word flags;
	byte unknown02;
	char unknown03;
	wchar_t name[0x20];
	long field_xcb8724; // 1..9
	union
	{
		dword flags48;    // 15 bits
		struct
		{
			dword teams_enabled : 1;
			dword motion_sensor_enabled : 1;
			dword flags_bits2 : 30;
		};
	};
	long unknown4c;
	long unknown50;
	long unknown54;
	long unknown58;
	byte unknown5c[0x74 - 0x5c];
	long unknown74;
	long unknown78;
	long unknown7c;
	long unknown80;
	long unknown84;
	long unknown88;
	byte unknown8c[0xa4 - 0x8c];
	long unknowna4;
	long unknowna8;
	long unknownac;
	long unknownb0;
	long unknownb4;
	byte unknownb8[0xcc - 0xb8];
	char unknowncc;
	char unknowncd;
	char unknownce;
	char unknowncf;
	char unknownd0;
	char unknownd1;
	char unknownd2;
	char unknownd3;
	char unknownd4;
	char unknownd5;
	char unknownd6;
	char unknownd7;
	byte unknownd8[0xf0 - 0xd8];
	union
	{
		dword engine_flags;
		s_game_variant_flag_settings flag;
		s_game_variant_ball_settings ball;
		s_game_variant_king_settings king;
		s_game_variant_territories_settings territories;
		byte engine_settings[0x130 - 0xf0];
	};
};

s_game_variant *__stdcall function_19d220(s_game_variant *variant, long type);
bool function_19d620(s_game_variant *variant);
