// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_13E8A0.CPP: the text drawing state: font, colours, shadow,
   justification and tab stops */

#include "cseries.h"
#include "real_math.h"
#include "globals.h"
#include <string.h>

struct s_font_header;
s_font_header *font_get(long font_index);
extern long g_4e28f4[11];

s_draw_string_globals g_4e73a0;

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
	g_4e73a0.justification = 0;
	g_4e73a0.style = 0;
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
void function_13edb0(long font, long flags, long style, long justification, real_argb_color const *color, real_argb_color const *shadow_color)
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
	g_4e73a0.flags = flags;
	g_4e73a0.style = style;
	g_4e73a0.justification = justification;
}
