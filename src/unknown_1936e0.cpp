// @flags /O2 /Gr
/* UNKNOWN_1936E0.CPP: building the built-in game variants: a variant's
   names and descriptions from the user interface's string list, its
   place-to-points table and its settings (lane H) */

#include "unknown_11c920.h"
#include <xtl.h>
#include <wchar.h>
#include "globals.h"

#define PIN(x, lo, hi) ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

/* a game variant (0x614 bytes); the checks are in unknown_193560.cpp */
struct s_built_variant
{
	long type;
	long field_4;
	dword field_8;
	char names[9][0x10];
	char descriptions[9][0x80];
	char field_51c[128];
	long field_59c;
	long field_5a0;
	long points[16];
	byte unknown5e4[0x614 - 0x5e4];
};

/* the shared user interface globals: the string list of the variants'
   names at +0x1c */
struct s_variant_strings_view
{
	byte unknown00[0x1c];
	long string_list_index;
};

/* a language's string table and a string list's range of its strings
   (unknown_1a0120.cpp) */
struct s_string_reference;

struct s_string_table
{
	s_string_reference *references;
	char *data;
	long count;
	long data_size;
	long references_offset;
	long data_offset;
	bool loaded;
	byte unknown19[3];
};

struct s_variant_string_tables_view
{
	byte unknown000[0x188];
	s_string_table tables[1];
};

struct s_variant_string_list_view
{
	byte unknown00[0x10];
	struct
	{
		short first;
		short count;
	} languages[1];
};

void *function_1482e8(void);
long function_11ca80(long value);
const char *string_table_find(s_string_table *table, long first, long count, long string_handle);
void utf8_string_to_utf16_string(const char *source, word *destination, long destination_count);
void utf16_string_to_utf8_string(const word *source, char *destination, long destination_size);

/* a string of a string list in the current language (as 0x1a0180, which
   retail inlines here) */
__forceinline void variant_string_get(long tag_index, long string_handle, word *buffer)
{
	long language;
	s_variant_string_list_view *list;
	s_string_table *table;

	if (g_47ff38 == NONE)
	{
		g_47ff38 = function_11ca80(XGetLanguage());
	}
	language = g_47ff38;
	list = (s_variant_string_list_view *)g_4e3b44[tag_index & 0xffff].bytes;
	table = &((s_variant_string_tables_view *)g_4e034c)->tables[language];
	if (table->loaded)
	{
		word string[0x100];

		utf8_string_to_utf16_string(string_table_find(table, list->languages[language].first, list->languages[language].count, string_handle), string, 0x100);
		wcsncpy((wchar_t *)buffer, (wchar_t *)string, 0xff);
		buffer[0xff] = 0;
	}
}

/* fills a variant: its type and settings, its nine names and descriptions
   and its tables */
// @retail 0x1936e0
void function_1936e0(long type, s_built_variant *variant, dword field_8, long description_string, long field_5a0, long const *points, long field_59c, long name_string)
{
	word text[0x100];
	s_variant_strings_view *strings;
	long i;

	text[0] = 0;
	variant->type = type;
	variant->field_4 = 0;
	variant->field_8 = field_8;
	strings = (s_variant_strings_view *)function_1482e8();
	if (strings && strings->string_list_index != NONE)
	{
		long string_list_index = strings->string_list_index;

		for (i = 0; i < 9; i++)
		{
			variant_string_get(string_list_index, name_string, text);
			utf16_string_to_utf8_string(text, variant->names[i], sizeof(variant->names[i]));
		}
		for (i = 0; i < 9; i++)
		{
			variant_string_get(string_list_index, description_string, text);
			utf16_string_to_utf8_string(text, variant->descriptions[i], sizeof(variant->descriptions[i]));
		}
	}
	for (i = 0; i <= 5; i++)
		variant->field_51c[i] = PIN(i + 5, i, 127);
	for (i = 6; i <= 15; i++)
		variant->field_51c[i] = PIN(i + 6, i, 127);
	for (i = 16; i <= 25; i++)
		variant->field_51c[i] = PIN(i + 7, i, 127);
	for (i = 26; i <= 30; i++)
		variant->field_51c[i] = PIN(i + 8, i, 127);
	for (i = 31; i <= 35; i++)
		variant->field_51c[i] = PIN(i + 9, i, 127);
	for (i = 36; i <= 127; i++)
		variant->field_51c[i] = PIN(i + 10, i, 127);
	variant->field_59c = field_59c;
	variant->field_5a0 = field_5a0;
	for (i = 0; i < 16; i++)
		variant->points[i] = points[i];
}
