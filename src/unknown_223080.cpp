// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_223080.CPP: the loading screen (the progress text in the
   language of the console) */

#include "unknown_11c920.h"
#include <xtl.h>
#include "language.h"
#include <stdio.h>
#include <stdarg.h>

/* the length of a wide string, at most maximum_count characters */
PRIVATE inline long ustrnlen(wchar_t const *string, long maximum_count)
{
	long length;

	for (length = 0; length < maximum_count && string[length]; length++)
	{
	}
	return length;
}

// @retail 0x223320
wchar_t const *loading_text_get(void)
{
	switch (get_current_language())
	{
	case 1:
		return L"\x8aad\x307f\x8fbc\x3093\x3067\x3044\x307e\x3059";
	case 2:
		return L"LADEN";
	case 3:
		return L"CHARGEMENT EN COURS";
	case 4:
		return L"CARGANDO";
	case 5:
		return L"CARICAMENTO";
	case 6:
		return L"\xbd88\xb7ec\xc624\xb294 \xc911";
	case 7:
		return L"\x8f09\x5165\x4e2d";
	default:
		return L"LOADING";
	}
}

// @retail 0x223720
wchar_t *ustrnzcatf(
	wchar_t *buffer,
	long size,
	wchar_t const *format,
	...)
{
	long length = ustrnlen(buffer, size - 1);
	long remaining = size - length;
	va_list arguments;

	va_start(arguments, format);
	_vsnwprintf(buffer + length, remaining - 1, format, arguments);
	buffer[length + remaining - 1] = 0;
	va_end(arguments);
	return buffer;
}
