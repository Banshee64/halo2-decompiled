// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_138800.CPP: queries and timers on the game options (g_4e6948) */

#include "cseries.h"
#include "globals.h"

enum
{
	_game_state_campaign = 1,
	_game_state_multiplayer = 2
};

bool g_4f55e7;
extern byte g_547f6f; // hs_library_external.cpp

void function_123ed0();

// @retail 0x138800
bool function_138800()
{
	bool result = false;
	if (g_4e6948 && g_4e6948->flag1120)
	{
		result = true;
	}
	return result;
}

// @retail 0x138820
bool function_138820()
{
	return g_4e6948->state == _game_state_campaign && g_4e6948->flag134;
}

// @retail 0x138840
bool function_138840()
{
	bool result = false;
	long mode = g_4e6948->mode;
	if (mode >= 2 && mode <= 5)
	{
		result = true;
	}
	return result;
}

// @retail 0x138880
bool function_138880()
{
	bool result = false;
	if (g_4e6948->mode == 4)
	{
		result = true;
	}
	return result;
}

// @retail 0x138860
bool function_138860()
{
	return !function_138880();
}

static inline bool game_is_campaign()
{
	return g_4e6948->state == _game_state_campaign;
}

static inline short game_difficulty_get()
{
	short difficulty = 0;
	if (game_is_campaign())
	{
		difficulty = g_4e6948->difficulty;
	}
	return difficulty;
}

// @retail 0x1388a0
bool function_1388a0()
{
	return !(game_is_campaign() && (g_4f55e7 || game_difficulty_get() == 3));
}

// @retail 0x138960
void function_138960(bool start)
{
	if (start)
	{
		if (!g_4e6948->flag1121)
		{
			g_4e6948->flag1121 = true;
			real seconds = g_510c54->ticks_per_second * 5.0f;
			long ticks;
			__asm
			{
				fld seconds
				fistp ticks
			}
			g_4e6948->ticks1124 = ticks;
			function_123ed0();
		}
	}
	else if (g_4e6948->flag1121)
	{
		g_4e6948->flag1121 = false;
	}
}

// @retail 0x1389c0
void function_1389c0()
{
	s_game_options_view *options = g_4e6948;
	if (!options->flag1128)
	{
		options->flag1128 = true;
		real seconds = g_510c54->ticks_per_second * 7.0f;
		long ticks;
		__asm
		{
			fld seconds
			fistp ticks
		}
		options->ticks112c = ticks;
		options->flag1129 = false;
	}
}

// @retail 0x138a10
bool function_138a10()
{
	return g_4e6948->flag1128 && g_4e6948->ticks112c == 0;
}

void function_188cd0(void);

// @retail 0x138eb0
void function_138eb0()
{
	s_game_options_view *options = g_4e6948;
	if (options->flag1128 && options->ticks112c > 0)
	{
		options->ticks112c--;
		if (!options->flag1129 && 2.0f >= options->ticks112c * g_510c54->rate)
		{
			function_188cd0();
			g_4e6948->flag1129 = true;
		}
	}
}

// @retail 0x138e40
void function_138e40()
{
	s_game_options_view *options = g_4e6948;
	if (options->state == _game_state_campaign && options->mode != 4)
	{
		bool start;
		if (options->flag134 && !function_1388a0())
		{
			start = ((bool *)g_4e8c20)[5];
		}
		else
		{
			start = ((bool *)g_4e8c20)[4];
		}
		function_138960(start);

		if (options->flag1121)
		{
			if (options->ticks1124 > 0)
			{
				options->ticks1124--;
			}
			if (options->ticks1124 == 0)
			{
				g_547f6f = true;
			}
		}
	}
}
