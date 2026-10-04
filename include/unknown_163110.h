/* UNKNOWN_163110.H: the text widgets of the game engine's score rows
   (src/unknown_163110.cpp) and the formatted text buffer */

#ifndef UNKNOWN_163110_H
#define UNKNOWN_163110_H

#include "cseries.h"
#include "real_math.h"

struct s_short_rectangle
{
	short top;
	short left;
	short bottom;
	short right;
};

#define TEXT_WIDGET(name, count) \
struct name \
{ \
	s_short_rectangle bounds; \
	byte flag; \
	byte unknown09; \
	s_short_rectangle text_bounds; \
	color4f color_a; \
	color4f color_b; \
	byte unknown34[4]; \
	word text[count]; \
	byte valid; \
\
	void initialize(const s_short_rectangle *rectangle, const color4f *color_a, const color4f *color_b, const word *text, long text_length, bool flag); \
}

TEXT_WIDGET(s_text_widget_a, 6);
TEXT_WIDGET(s_text_widget_b, 20);
TEXT_WIDGET(s_text_widget_c, 2);
TEXT_WIDGET(s_text_widget_d, 80);

struct s_text_buffer
{
	word text[0x50];
};

s_text_buffer *text_buffer_format(s_text_buffer *buffer, const word *format, ...);

#endif
