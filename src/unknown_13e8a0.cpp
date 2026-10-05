// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_13E8A0.CPP: the text drawing state: font, colours, shadow,
   justification and tab stops */

#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include "globals.h"
#include "font_loading.h"
#include "unknown_030290.h"
#include <string.h>


s_draw_string_globals g_4e73a0;

struct s_13e8e0_bounds
{
	short_rectangle2d bounds;
	short field_8;
	short field_a;
};

s_13e8e0_bounds g_4e7394;

typedef bool (__stdcall *f_13e8e0_vertices)(real *vertices, long parameter);

// @retail 0x13e8e0
void __stdcall function_13e8e0(void const *glyph, long font, long character,
	dword color, dword shadow, real x, real y, real u, real v,
	real width, real height, real scale,
	f_13e8e0_vertices vertex_proc, long parameter)
{
	s_font_header *header = font_get(g_4e28f4[font]);
	real right = width * scale + x + 1.0f;
	real bottom = height * scale + y + 1.0f;
	if (g_4e7394.bounds.left > x)
		g_4e7394.bounds.left = (short)x;
	if (g_4e7394.bounds.top > y)
		g_4e7394.bounds.top = (short)y;
	if (right > g_4e7394.bounds.right)
		g_4e7394.bounds.right = (short)right;
	if (bottom > g_4e7394.bounds.bottom)
		g_4e7394.bounds.bottom = (short)bottom;
	if (header)
	{
		g_4e7394.field_8 = header->ascending_height;
		g_4e7394.field_a = header->descending_height;
	}
}

void function_13eb60(color4f const *color);

/* the brightest colour text is drawn in: a colour with every channel above
   this is scaled down to it */
#define k_maximum_text_brightness 0.68f

// @retail 0x13ec70
void function_13ec70(color4f const *color)
{
	color4f c = *color;
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
	g_4e73a0.tab_stop_count = count > 16 ? 16 : count;
	if (g_4e73a0.tab_stop_count > 0)
	{
		memcpy(g_4e73a0.tab_stops, tab_stops, g_4e73a0.tab_stop_count * sizeof(short));
	}
}

// @retail 0x13eb60
void function_13eb60(color4f const *color)
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
void function_13ed50(color4f const *field_24)
{
	if (field_24)
	{
		g_4e73a0.shadow = true;
		g_4e73a0.field_24 = *field_24;
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
void function_13edb0(long font, long style, long justification, dword flags, color4f const *color, color4f const *field_24)
{
	font_get(g_4e28f4[font]);
	g_4e73a0.font = font;
	function_13ec70(color);
	if (field_24)
	{
		g_4e73a0.shadow = true;
		g_4e73a0.field_24 = *field_24;
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
	s_font_header *field_4_3;
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
	dword field_24;
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
bool function_13f470(s_text_iterator *iterator, long font, short justification, dword const *string, short style, color4f const *color, bool const *shadow, color4f const *field_24)
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
		real alpha = field_24->alpha > color->alpha ? color->alpha : field_24->alpha;
		iterator->field_24 = alpha_rgb_to_pixel32(alpha, &field_24->red);
	}
	else
	{
		iterator->field_24 = 0;
	}
	iterator->field_4_3 = font_get(g_4e28f4[font]);
	return iterator->field_4_3 != NULL;
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

	if (function_13f470(&iterator, font, (short)g_4e73a0.justification, string, (short)g_4e73a0.style, &g_4e73a0.color, &g_4e73a0.shadow, &g_4e73a0.field_24))
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
/* whether a private use character is drawn as a glyph (rather than being a
   formatting code) */
// @retail 0x13f660
bool function_13f660(long character)
{
	bool result = false;

	if (character >= 0xe112 && character <= 0xe12b)
	{
		return result;
	}

	if (character >= 0xe000 && character <= 0xe3ff)
	{
		switch (character)
		{
		case 0xe000:
		case 0xe001:
		case 0xe002:
		case 0xe004:
		case 0xe008:
		case 0xe106:
		case 0xe107:
		case 0xe108:
		case 0xe109:
		case 0xe10a:
		case 0xe10b:
		case 0xe10c:
		case 0xe10d:
		case 0xe10e:
		case 0xe10f:
		case 0xe110:
		case 0xe111:
		case 0xe12c:
		case 0xe12d:
		case 0xe12e:
		case 0xe12f:
		case 0xe130:
		case 0xe131:
			break;
		default:
			result = true;
			break;
		}
	}

	return result;
}

struct s_13ef40_header
{
	short field_0;
	word field_2;
	short width;
	short height;
	short field_8;
	short field_a;
	dword field_c;
};

struct s_13ef40_entry
{
	byte field_0[0x10];
	long state;
	byte field_14[8];
	s_13ef40_header header;
	byte field_2c[0xc];
};

struct s_13ef40_result
{
	real width;
	long index;
};

extern s_record_pool *g_54d574;
long font_cache_get_character(long font_index, long character, dword flags);
bool font_cache_character_load_pixels(long datum_index, dword flags);
short function_122570(s_font_header const *header, dword first_character, dword second_character);

// @retail 0x13ef40
s_13ef40_result function_13ef40(s_text_iterator *iterator, point2f const *origin, real const *bounds, real scale)
{
	real local_1 = 0.0f;
	// Retail keeps this value in a stack slot across calls.
	volatile long local_2 = 0;
	long count = 0;
	long break_index = 0;
	real break_width;
	bool stop = false;
	do
	{
		bool keep_break = false;
		function_13f5a0(iterator);
		if (iterator->token == _text_token_character)
		{
			long datum_index = font_cache_get_character(iterator->font, iterator->character, 3);
			if (datum_index != NONE && font_cache_character_load_pixels(datum_index, 3))
			{
				s_13ef40_entry *entry = &((s_13ef40_entry *)g_54d574->data)[datum_index & 0xffff];
				s_13ef40_header const *header = entry->state == 4 ? &entry->header : NULL;
				if (header)
				{
					short spacing = function_122570(iterator->field_4_3, iterator->previous_character, iterator->character);
					real advance = header->field_0 * scale;
					real offset = header->field_8 * scale;
					real kerning = spacing * scale;
					if (iterator->can_break)
					{
						break_index = local_2;
						break_width = local_1;
					}
					if (bounds[1] > origin->x + advance + offset + kerning + local_1 || !count)
					{
						local_1 = advance + offset + kerning + local_1;
						count++;
					}
					else if (g_4e73a0.flags & 1)
					{
						if (break_index > 0)
						{
							local_1 = break_width;
							local_2 = break_index;
							keep_break = true;
						}
						stop = true;
					}
				}
			}
		}
		else
		{
			stop = true;
		}
		if (keep_break)
			break;
		local_2 = iterator->index;
	} while (!stop);
	s_13ef40_result result = { local_1, local_2 };
	return result;
}

typedef void (__stdcall *f_13f700_draw)(void const *, long, long, dword, dword,
	real, real, real, real, real, real, real, f_13e8e0_vertices, long);

struct s_font_character_header;
s_font_character_header *font_cache_get_character_header(long font_index, long character, dword flags);

// @retail 0x13f700
void function_13f700(real const *bounds, f_13f700_draw draw, point2f *origin,
	dword color, dword shadow, dword const *string, short end, real scale,
	short begin, real const *clip)
{
	real right, left, bottom, top;
	top = left = -32768.0f;
	bottom = right = 32767.0f;
	if (bounds)
	{
		if (bounds[0] > left) left = bounds[0];
		if (right > bounds[1]) right = bounds[1];
		if (bounds[2] > top) top = bounds[2];
		if (bottom > bounds[3]) bottom = bounds[3];
	}
	if (clip)
	{
		if (clip[0] > left) left = clip[0];
		if (right > clip[1]) right = clip[1];
		if (clip[2] > top) top = clip[2];
		if (bottom > clip[3]) bottom = clip[3];
	}
	if (right > left && bottom > top)
	{
		s_text_iterator iterator;
		if (function_13f470(&iterator, g_4e73a0.font, (short)g_4e73a0.justification, string,
			(short)g_4e73a0.style, &g_4e73a0.color, &g_4e73a0.shadow, &g_4e73a0.field_24))
		{
			iterator.index = begin;
			while (iterator.index < end)
			{
				function_13f5a0(&iterator);
				if (!iterator.character)
					break;
				if (iterator.token == _text_token_character && ((long)iterator.character < 0 || (long)iterator.character > 31))
				{
					s_13ef40_header const *header = (s_13ef40_header const *)font_cache_get_character_header(iterator.font, iterator.character, 3);
					if (header)
					{
						real width = header->width;
						real height = header->height;
						real u = 0.0f;
						real v = 0.0f;
						short spacing = function_122570(iterator.field_4_3, iterator.previous_character, iterator.character);
						real offset = header->field_8 * scale;
						real kerning = spacing * scale;
						real x = origin->x + offset + kerning;
						real y = origin->y - header->field_a * scale;
						if (x + width > right) width = right - x;
						if (left > x)
						{
							u = left - x;
							x = left;
							width -= u;
						}
						if (y + height > bottom) height = bottom - y;
						if (top > y)
						{
							v = top - y;
							y = top;
							height -= v;
						}
						if (width > 0.0f && height > 0.0f)
						{
							dword glyph_color = color;
							if (function_13f660(iterator.character)) glyph_color |= 0xffffff;
							draw(&iterator, iterator.font, iterator.character, glyph_color, shadow,
								x, y, u, v, width, height, scale, g_4e73a0.vertex_proc, g_4e73a0.vertex_proc_parameter);
						}
						origin->x = header->field_0 * scale + origin->x + offset + kerning;
					}
				}
			}
		}
	}
}

struct s_13f0e0_point
{
	short x, y;
};

// @retail 0x13f0e0
void function_13f0e0(f_13f700_draw draw, short_rectangle2d const *bounds, s_13f0e0_point *cursor,
	short_rectangle2d const *clip, short spacing, real scale, dword const *string)
{
	short row = 0;
	short column = 0;
	short maximum_line = 0;
	short line = 0;
	point2f position = { (real)bounds->left, (real)bounds->top };
	s_text_iterator iterator;
	if (function_13f470(&iterator, g_4e73a0.font, (short)g_4e73a0.justification, string,
		(short)g_4e73a0.style, &g_4e73a0.color, &g_4e73a0.shadow, &g_4e73a0.field_24))
	{
		for (;;)
		{
			long begin = iterator.index;
			long justification = iterator.justification;
			s_font_header *font = iterator.field_4_3;
			real line_bounds[4] = { (real)bounds->left, (real)bounds->right, (real)bounds->top, (real)bounds->bottom };
			if (g_4e73a0.tab_stop_count > 0)
			{
				if (column)
					line_bounds[0] = g_4e73a0.tab_stops[column - 1] * scale;
				else
					line_bounds[0] += (row ? (long)g_4e73a0.unknown60 : (long)g_4e73a0.unknown5e) * scale;
				if (column < g_4e73a0.tab_stop_count)
					line_bounds[1] = g_4e73a0.tab_stops[column] * scale;
			}
			else
				line_bounds[0] += (row ? (long)g_4e73a0.unknown60 : (long)g_4e73a0.unknown5e) * scale;
			position.x = *(short const *)font->unknown0a * scale + line_bounds[0];
			position.y = (font->ascending_height + (row + line) * (font->ascending_height + font->descending_height + font->leading_height + spacing)) * scale + line_bounds[2];
			s_13ef40_result measured = function_13ef40(&iterator, &position, line_bounds, scale);
			font = iterator.field_4_3;
			if (justification == 1)
				position.x = line_bounds[1] - line_bounds[0] + line_bounds[0] - measured.width - *(short const *)font->unknown0a;
			else if (justification == 2)
				position.x = (line_bounds[1] - line_bounds[0] - measured.width) * 0.5f + line_bounds[0];
			if (line_bounds[3] > position.y)
			{
				real clip_bounds[4];
				real const *clip_pointer = NULL;
				if (clip)
				{
					clip_bounds[0] = clip->left;
					clip_bounds[1] = clip->right;
					clip_bounds[2] = clip->top;
					clip_bounds[3] = clip->bottom;
					clip_pointer = clip_bounds;
				}
				function_13f700(line_bounds, draw, &position, iterator.color, iterator.field_24,
					string, (short)measured.index, scale, begin, clip_pointer);
			}
			iterator.index = (short)measured.index;
			switch (iterator.token)
			{
			case _text_token_end: goto done;
			case _text_token_newline:
				row += maximum_line + 1;
				column = 0;
				line = 0;
				maximum_line = 0;
				break;
			case _text_token_tab:
				if (column >= g_4e73a0.tab_stop_count) break;
				column++;
			case _text_token_justification:
				line = 0;
				break;
			case _text_token_character:
				line++;
				if (line > maximum_line) maximum_line = line;
				break;
			}
		}
	}
done:
	if (cursor)
	{
		cursor->x = (short)position.x;
		cursor->y = (short)position.y;
	}
}

// @retail 0x13ea60
void function_13ea60(short_rectangle2d const *bounds, dword const *string, real scale,
	short_rectangle2d *area, short_rectangle2d *cursor)
{
	s_font_header *font = font_get(g_4e28f4[g_4e73a0.font]);
	g_4e7394.bounds.top = 0x7fff;
	g_4e7394.bounds.left = 0x7fff;
	g_4e7394.bounds.bottom = (short)0x8000;
	g_4e7394.bounds.right = (short)0x8000;
	if (font)
	{
		g_4e7394.field_8 = font->ascending_height;
		g_4e7394.field_a = font->descending_height;
	}
	s_13f0e0_point point;
	function_13f0e0(function_13e8e0, bounds, &point, NULL, 0, scale, string);
	cursor->left = point.x;
	cursor->right = point.x + 1;
	cursor->top = point.y - g_4e7394.field_8;
	cursor->bottom = point.y + g_4e7394.field_a;
	*area = g_4e7394.bounds;
}

// @retail 0x13e9c0
void function_13e9c0(word const *text, short_rectangle2d const *bounds,
	short_rectangle2d *area, short_rectangle2d *cursor, real scale)
{
	dword characters[0x800];
	unicode_string_to_characters(0x800, text, characters);
	function_13ea60(bounds, characters, scale, area, cursor);
}
