// @flags /O2 /Gr
/* UNKNOWN_1A0120.CPP: the strings of unicode string list tags. A string list
   gives, per language, a range of the strings in that language's string
   table; the tables (UTF-8, one per language) live in the globals tag. */

#include "cseries.h"
#include <xtl.h>
#include <wchar.h>
#include "globals.h"

#define PIN(x, lo, hi) ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

long function_11ca80(long value);
void utf8_string_to_utf16_string(const char *source, word *destination, long destination_count);

/* a string of a table: its id and the offset of its text */
struct s_string_reference
{
	long string_id;
	long offset;
};

/* one language's strings (0x1c bytes) */
struct s_string_table
{
	s_string_reference *references;
	char *data;
	long count;
	byte unknown0c[0x18 - 0x0c];
	bool loaded;
	byte unknown19[3];
};

/* the globals tag's string tables, one per language */
struct s_string_table_globals
{
	byte unknown000[0x188];
	s_string_table tables[1];
};

/* a unicode string list tag: the range of its strings in each language */
struct s_unicode_string_list
{
	byte unknown00[0x10];
	struct
	{
		short first;
		short count;
	} languages[1];
};

/* the current language (NONE until it is first asked for) */
long g_47ff38 = NONE;

static inline long current_language()
{
	if (g_47ff38 == NONE)
	{
		g_47ff38 = function_11ca80(XGetLanguage());
	}
	return g_47ff38;
}

/* the text of a string in a table, or an empty string */
// @retail 0x1a02d0
const char *string_table_find(s_string_table *table, long first, long count, long string_id)
{
	for (long i = first; i < first + count; i++)
	{
		if (PIN(i, 0, table->count - 1) == i && table->references[i].string_id == string_id)
		{
			return table->data + table->references[i].offset;
		}
	}
	return "";
}

/* whether a table has a string */
// @retail 0x1a0310
bool string_table_has_string(s_string_table *table, long first, long count, long string_id)
{
	bool result = false;

	if (table->loaded)
	{
		for (long i = first; i < first + count; i++)
		{
			if (PIN(i, 0, table->count - 1) == i && table->references[i].string_id == string_id)
			{
				result = true;
				break;
			}
		}
	}
	return result;
}

/* copies a string of a table into a buffer of 0x100 characters */
// @retail 0x1a0350
void string_table_get_string(s_string_table *table, long first, long count, long string_id, word *buffer)
{
	if (table->loaded)
	{
		word string[0x100];

		utf8_string_to_utf16_string(string_table_find(table, first, count, string_id), string, 0x100);
		wcsncpy((wchar_t *)buffer, (wchar_t *)string, 0xff);
		buffer[0xff] = 0;
	}
}

/* whether a string list has a string in the current language */
// @retail 0x1a0120
bool unicode_string_list_has_string(long tag_index, long string_id)
{
	bool result = false;

	if (tag_index != NONE)
	{
		long language = current_language();
		s_unicode_string_list *list = (s_unicode_string_list *)g_4e3b44[tag_index & 0xffff].bytes;
		s_string_table_globals *globals = (s_string_table_globals *)g_4e034c;

		result = string_table_has_string(&globals->tables[language], list->languages[language].first,
			list->languages[language].count, string_id);
	}
	return result;
}

/* copies a string of a string list, in the current language, into a buffer
   of 0x100 characters */
// @retail 0x1a0180
void unicode_string_list_get_string(long tag_index, long string_id, word *buffer)
{
	if (tag_index != NONE)
	{
		long language = current_language();
		s_unicode_string_list *list = (s_unicode_string_list *)g_4e3b44[tag_index & 0xffff].bytes;
		s_string_table_globals *globals = (s_string_table_globals *)g_4e034c;

		string_table_get_string(&globals->tables[language], list->languages[language].first,
			list->languages[language].count, string_id, buffer);
	}
}
