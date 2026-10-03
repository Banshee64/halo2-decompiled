// @flags /O2 /Gr
/* UNKNOWN_2AFD10.CPP: the text of a definition in the current language */

#include "cseries.h"
#include <xtl.h>

enum
{
	k_language_count = 8
};

extern long g_47ff38;
long function_11ca80(long value);

/* the language (0x47ff38 caches it) */
inline long get_current_language()
{
	long language = g_47ff38;
	if (language == NONE)
	{
		language = function_11ca80(XGetLanguage());
		g_47ff38 = language;
	}
	return language;
}

struct s_localized_name
{
	byte unknown00[0x10];
	wchar_t names[k_language_count][0x20];
};

struct s_localized_description
{
	byte unknown000[0x250];
	wchar_t descriptions[k_language_count][0x80];
};

struct s_localized_short_name
{
	byte unknown00[0xc];
	wchar_t names[k_language_count][0x20];
};

// @retail 0x2afd10
wchar_t *localized_name_get(s_localized_name *definition)
{
	return definition->names[get_current_language()];
}

// @retail 0x2afd40
wchar_t *localized_description_get(s_localized_description *definition)
{
	return definition->descriptions[get_current_language()];
}

// @retail 0x2afd70
wchar_t *localized_short_name_get(s_localized_short_name *definition)
{
	return definition->names[get_current_language()];
}
