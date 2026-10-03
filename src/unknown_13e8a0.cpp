// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_13E8A0.CPP: the text drawing state: font, colours, shadow,
   justification and tab stops */

#include "cseries.h"
#include "real_math.h"
#include "font_loading.h"
#include <string.h>


struct s_draw_string_globals
{
	long font;
	dword flags; /* bit 0: wrap lines only where breaking is allowed */
	long style;
	long justification;
	real_argb_color color;
	bool shadow;
	byte unknown21[3];
	real_argb_color shadow_color;
	byte unknown34[8];
	short tab_stop_count;
	short tab_stops[16];
	short unknown5e;
	short unknown60;
	short unknown62;
};

s_draw_string_globals g_4e73a0;

extern real_argb_color const *g_4686cc;

void function_13eb60(real_argb_color const *color);

/* the brightest colour text is drawn in: a colour with every channel above
   this is scaled down to it */
#define k_maximum_text_brightness 0.68f

// @retail 0x13ec70
void function_13ec70(real_argb_color const *color)
{
	real_argb_color c = *color;
	if ((c.red > k_maximum_text_brightness) & (c.green > k_maximum_text_brightness) & (c.blue > k_maximum_text_brightness))
	{
		real minimum = c.green > c.blue ? c.blue : c.green;
		minimum = c.red > minimum ? (c.green > c.blue ? c.blue : c.green) : c.red;
		if (k_maximum_text_brightness > minimum)
			minimum = k_maximum_text_brightness;
		else if (minimum > 1.0f)
			minimum = 1.0f;
		real scale = k_maximum_text_brightness / minimum;
		c.red *= scale;
		c.green *= scale;
		c.blue *= scale;
	}
	function_13eb60(&c);
}

// @retail 0x13e8a0
void function_13e8a0()
{
	memset(&g_4e73a0, 0, sizeof(g_4e73a0));
	g_4e73a0.font = 0;
	g_4e73a0.tab_stop_count = 0;
	g_4e73a0.flags = 0;
	g_4e73a0.justification = 0;
	g_4e73a0.unknown5e = 0;
	g_4e73a0.unknown60 = 0;
	function_13ec70(g_4686cc);
}

// @retail 0x13eb20
void function_13eb20(short count, short const *tab_stops)
{
	short tab_stop_count = count;
	if (tab_stop_count > 16)
	{
		tab_stop_count = 16;
	}
	g_4e73a0.tab_stop_count = tab_stop_count;
	if (tab_stop_count > 0)
	{
		memcpy(g_4e73a0.tab_stops, tab_stops, tab_stop_count * sizeof(short));
	}
}

// @retail 0x13eb60
void function_13eb60(real_argb_color const *color)
{
	bool valid =
		color->alpha >= 0.0f && 1.0f >= color->alpha &&
		color->red >= 0.0f && 1.0f >= color->red &&
		color->green >= 0.0f && 1.0f >= color->green &&
		color->blue >= 0.0f && 1.0f >= color->blue;

	g_4e73a0.color = *color;
	if (!valid)
	{
		if (0.0f > g_4e73a0.color.alpha)
			g_4e73a0.color.alpha = 0.0f;
		else if (g_4e73a0.color.alpha > 1.0f)
			g_4e73a0.color.alpha = 1.0f;
		if (0.0f > g_4e73a0.color.red)
			g_4e73a0.color.red = 0.0f;
		else if (g_4e73a0.color.red > 1.0f)
			g_4e73a0.color.red = 1.0f;
		if (0.0f > g_4e73a0.color.green)
			g_4e73a0.color.green = 0.0f;
		else if (g_4e73a0.color.green > 1.0f)
			g_4e73a0.color.green = 1.0f;
		if (0.0f > g_4e73a0.color.blue)
			g_4e73a0.color.blue = 0.0f;
		else if (g_4e73a0.color.blue > 1.0f)
			g_4e73a0.color.blue = 1.0f;
	}
}

// @retail 0x13ed50
void function_13ed50(real_argb_color const *shadow_color)
{
	if (shadow_color)
	{
		g_4e73a0.shadow = true;
		g_4e73a0.shadow_color = *shadow_color;
	}
	else
	{
		g_4e73a0.shadow = false;
	}
}

// @retail 0x13ed90
void function_13ed90(long font)
{
	font_get(g_4e28f4[font]);
	g_4e73a0.font = font;
}

// @retail 0x13edb0
void function_13edb0(long font, long style, long justification, dword flags, real_argb_color const *color, real_argb_color const *shadow_color)
{
	font_get(g_4e28f4[font]);
	g_4e73a0.font = font;
	function_13ec70(color);
	if (shadow_color)
	{
		g_4e73a0.shadow = true;
		g_4e73a0.shadow_color = *shadow_color;
	}
	else
	{
		g_4e73a0.shadow = false;
	}
	g_4e73a0.style = style;
	g_4e73a0.justification = justification;
	g_4e73a0.flags = flags;
}

/* ---- string iteration ---- */

#include "unknown_13fd90.h"

bool font_cache_predict_character(long font_index, long character);
long unicode_escape_character_lookup(word character, bool *found);
bool function_13fd20(utf32 previous, utf32 character);

enum
{
	_text_token_end = 0,
	_text_token_newline,
	_text_token_tab,
	_text_token_justification,
	_text_token_character,
};

enum
{
	_text_character_left = 0xe405,
	_text_character_right,
	_text_character_center
};

#pragma pack(push, 2)
/* walks a string of utf32 characters token by token */
struct s_text_iterator
{
	long font;
	s_font_header *font_header;
	dword const *string;
	short index;
	short style;
	short justification;
	dword character;
	dword previous_character;
	bool can_break;
	bool check_breaks;
	long token;
	long previous_token;
	dword color;
	dword shadow_color;
};
#pragma pack(pop)

static __forceinline dword alpha_rgb_to_pixel32(real alpha, real const *rgb)
{
	dword pixel = (long)(alpha * 255.0f);
	pixel = (pixel << 8) | (long)(rgb[0] * 255.0f);
	pixel = (pixel << 8) | (long)(rgb[1] * 255.0f);
	pixel = (pixel << 8) | (long)(rgb[2] * 255.0f);
	return pixel;
}

// @retail 0x13f470
bool function_13f470(s_text_iterator *iterator, long font, short justification, dword const *string, short style, real_argb_color const *color, bool const *shadow, real_argb_color const *shadow_color)
{
	memset(iterator, 0, sizeof(*iterator));
	iterator->string = string;
	iterator->style = style;
	iterator->justification = justification;
	iterator->font = font;
	iterator->index = 0;
	iterator->can_break = false;
	iterator->check_breaks = (bool)(g_4e73a0.flags & 1);
	iterator->color = alpha_rgb_to_pixel32(color->alpha, &color->red);
	if (*shadow)
	{
		real alpha = shadow_color->alpha > color->alpha ? color->alpha : shadow_color->alpha;
		iterator->shadow_color = alpha_rgb_to_pixel32(alpha, &shadow_color->red);
	}
	else
	{
		iterator->shadow_color = 0;
	}
	iterator->font_header = font_get(g_4e28f4[font]);
	return iterator->font_header != NULL;
}

// @retail 0x13f5a0
void function_13f5a0(s_text_iterator *iterator)
{
	long character;
	long token;

	do
	{
		character = iterator->string[iterator->index++];
		switch (character)
		{
		case 0:
			token = _text_token_end;
			break;
		case '\t':
			token = _text_token_tab;
			break;
		case '\n':
			token = NONE;
			break;
		case '\r':
			token = _text_token_newline;
			break;
		case _text_character_left:
			iterator->justification = 0;
			token = _text_token_justification;
			break;
		case _text_character_right:
			iterator->justification = 1;
			token = _text_token_justification;
			break;
		case _text_character_center:
			iterator->justification = 2;
			token = _text_token_justification;
			break;
		default:
			if (iterator->check_breaks)
			{
				utf32 previous = { iterator->character };
				utf32 next = { character };
				iterator->can_break = function_13fd20(previous, next);
			}
			else
			{
				iterator->can_break = false;
			}
			token = _text_token_character;
			break;
		}
	} while (token == NONE);

	if (iterator->previous_token != _text_token_character)
	{
		iterator->previous_character = 0;
	}
	else
	{
		iterator->previous_character = iterator->character;
	}
	iterator->character = character;
	iterator->previous_token = iterator->token;
	iterator->token = token;
}

// @retail 0x13eeb0
bool function_13eeb0(dword const *string, long font)
{
	bool result = true;
	s_text_iterator iterator;

	if (function_13f470(&iterator, font, (short)g_4e73a0.justification, string, (short)g_4e73a0.style, &g_4e73a0.color, &g_4e73a0.shadow, &g_4e73a0.shadow_color))
	{
		for (;;)
		{
			function_13f5a0(&iterator);
			if (iterator.token == _text_token_character)
			{
				if (!font_cache_predict_character(font, iterator.character))
				{
					result = false;
				}
			}
			else if (iterator.token == _text_token_end)
			{
				break;
			}
		}
	}

	return result;
}

/* copies a string into utf32 characters, decoding the '|' escapes */
static __forceinline void unicode_string_to_characters(long maximum_count, word const *source, dword *destination)
{
	while (maximum_count > 0)
	{
		if (maximum_count == 1)
		{
			*destination = 0;
			break;
		}

		dword character;
		word const *next = source + 1;
		word c = *source;
		if (c == '|')
		{
			bool found = false;
			character = unicode_escape_character_lookup(*next, &found);
			if (found)
			{
				next++;
			}
		}
		else
		{
			character = c;
		}

		source = next;
		*destination = character;
		if (!character)
		{
			break;
		}
		destination++;
		maximum_count--;
	}
}
// @retail 0x13ee20
bool function_13ee20(word const *string, long font)
{
	dword characters[0x800];
	unicode_string_to_characters(0x800, string, characters);
	return function_13eeb0(characters, font);
}

// @retail 0x13ef30
bool function_13ef30(word const *string)
{
	return function_13ee20(string, g_4e73a0.font);
}