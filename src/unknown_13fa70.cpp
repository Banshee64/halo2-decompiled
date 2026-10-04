// @flags /O2 /Gr
/* UNKNOWN_13FA70.CPP: unicode string helpers */

#include "cseries.h"
#include "unknown_13fd90.h"
#include <stdarg.h>
#include <string.h>
#include <wchar.h>
#include <xtl.h>

struct unicode_range
{
	word first;
	word last;
};

struct unicode_escape
{
	word character;
	long value;
};

dword g_55e730;

// @retail 0x13fa70
void unicode_string_append(word *destination, const word *source, long maximum_count)
{
	while (*destination)
	{
		destination++;
		maximum_count--;
	}

	while (*source && maximum_count > 1)
	{
		*destination++ = *source++;
		maximum_count--;
	}

	*destination = 0;
}

// @retail 0x13fac0
void unicode_string_copy(word *destination, const word *source, long maximum_count)
{
	wcsncpy((wchar_t *)destination, (const wchar_t *)source, maximum_count - 1);
	destination[maximum_count - 1] = 0;
}

// @retail 0x13fae0
void unicode_string_upper(word *string, long maximum_count)
{
	while (*string && maximum_count >= 0)
	{
		word c = *string;

		if (c >= 'a' && c <= 'z')
		{
			c -= 0x20;
		}
		else if (c > 0x7f)
		{
			switch (c)
			{
				case 0xe9: c = 0xc9; break;
				case 0xf3: c = 0xd3; break;
			}
		}

		*string = (word)c;
		string++;
		maximum_count--;
	}
}

// @retail 0x13fb50
int unicode_string_vsnprintf(word *buffer, long maximum_count, const word *format, ...)
{
	va_list arguments;
	va_start(arguments, format);
	return _vsnwprintf((wchar_t *)buffer, maximum_count, (const wchar_t *)format, arguments);
}

// @retail 0x13fb70
void unicode_string_snprintf(word *buffer, long maximum_count, const word *format, ...)
{
	va_list arguments;
	va_start(arguments, format);
	_vsnwprintf((wchar_t *)buffer, maximum_count - 1, (const wchar_t *)format, arguments);
	buffer[maximum_count - 1] = 0;
}

// @retail 0x13fb90
void unicode_string_to_ascii(const word *source, char *destination, long maximum_count)
{
	while (maximum_count > 0)
	{
		word c = *source;

		if (maximum_count == 1)
		{
			*destination = 0;
		}
		else if (c <= 0x7f)
		{
			*destination = (char)c;
		}
		else
		{
			*destination = '?';
		}

		source++;
		destination++;
		maximum_count--;

		if (!c || maximum_count <= 0)
		{
			break;
		}
	}
}

// @retail 0x13fbd0
long unicode_escape_character_lookup(word character, bool *found)
{
	unicode_escape table[] =
	{
		{ '|', '|' },
		{ 'l', 0xe405 },
		{ 'r', 0xe406 },
		{ 'c', 0xe407 },
		{ 'n', 0xd },
		{ 't', 9 },
		{ 0, 0 }
	};
	dword result;
	bool valid = false;
	result = '|';

	for (long i = 0; table[i].character; ++i)
	{
		if (character == table[i].character)
		{
			valid = true;
			result = table[i].value;
			break;
		}
	}

	if (!valid && character)
	{
		unsigned long time = GetTickCount();
		if (time > g_55e730)
		{
			g_55e730 = time + 60000;
		}
	}

	*found = valid;
	return result;
}

// @retail 0x13fc90
void ascii_string_to_unicode(const char *source, word *destination, long maximum_count)
{
	word *d = destination;
	const char *s = source;
	long count = maximum_count;

	while (count > 0)
	{
		char c = *s;

		if (count == 1)
		{
			*d = 0;
		}
		else if ((byte)c <= 0x7f)
		{
			*d = (short)c;
		}
		else
		{
			*d = 0x25a1;
		}

		if (!*s)
		{
			break;
		}

		d++;
		s++;
		count--;
	}
}

// @retail 0x13fcd0
bool unicode_ranges_contain(long range_count, word character, const unicode_range *ranges)
{
	bool result = false;
	long low = 0;
	long high = range_count - 1;

	while (low <= high)
	{
		long mid = (low + high) / 2;

		if (character < ranges[mid].first)
		{
			high = mid - 1;
		}
		else if (character > ranges[mid].last)
		{
			low = mid + 1;
		}
		else
		{
			result = true;
			break;
		}
	}

	return result;
}
