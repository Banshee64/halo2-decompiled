// @flags /O2 /Gr
/* UNKNOWN_08CA00.CPP: the message of the day, kept in
   n:\message_of_the_day.dat (lane D) */

#include "unknown_11c920.h"
#include "files.h"
#include <string.h>

/* the message of the day's state: its flags (bit 1: there is a message) and
   its length */
struct s_message_of_the_day_globals
{
	dword flags;
	short length;
};

s_message_of_the_day_globals g_479784;
char const *g_4672b8 = "message_of_the_day.dat";

static inline void function_x454397(s_type_acf665 *reference)
{
	memset(reference, 0, sizeof(*reference));
	reference->signature = FILE_REFERENCE_SIGNATURE;
	reference->location = NONE;
}

static inline void function_x73bce5(s_type_acf665 *reference, char const *name)
{
	if (reference->flags & 1)
		function_1373c0(reference->path);
	function_137320(reference->path, name);
	reference->flags |= 1;
}

// @retail 0x8ca00
void message_of_the_day_get_file(s_type_acf665 *file)
{
	function_x454397(file);
	function_137320(file->path, "n:\\");
	function_x73bce5(file, g_4672b8);
}

// @retail 0x8ca50
bool message_of_the_day_available(void)
{
	if (g_479784.length != 0 && (g_479784.flags & 2))
		return true;
	return false;
}

#include "language.h"

// @retail 0x8c900
bool __stdcall function_8c900(const byte *messages)
{
	bool result = false;
	s_type_acf665 file;
	message_of_the_day_get_file(&file);
	if (function_1367d0(&file))
	{
		dword error;
		if (function_136970(&file, 2, &error))
		{
			long language = g_47ff38;
			if (language == NONE)
			{
				g_47ff38 = function_11ca80(XGetLanguage());
				language = g_47ff38;
			}
			if (function_136d00(&file, messages + language * 0x200, 0x200) &&
				function_136d00(&file, messages + 0x1200 + language * 0x4004, 0x4004))
				result = true;
			if (CloseHandle(file.handle))
			{
				file.handle = 0;
				file.position = 0;
			}
			else
			{
				GetLastError();
				SetLastError(0);
			}
		}
		if (!result)
			function_136860(&file);
	}
	return result;
}

#include "globals.h"

struct s_name_buffer
{
	wchar_t name[256];
};

struct s_name_list
{
	long count;
	s_name_buffer names[32];
};

void name_list_clear(s_name_list *list);
void function_08cc20(s_name_buffer *buffer, const wchar_t *name);
bool function_1368f0(s_type_acf665 *file);

// @retail 0x8ca70
bool __stdcall function_8ca70(long mode, s_name_buffer *output)
{
	bool result = false;
	s_type_acf665 file;
	message_of_the_day_get_file(&file);
	if (function_1368f0(&file) && message_of_the_day_available())
	{
		dword error;
		if (function_136970(&file, 1, &error))
		{
			bool local_c793c4 = true;
			if (file.position != 0)
			{
				file.position = SetFilePointer(file.handle, 0, 0, FILE_BEGIN);
				local_c793c4 = file.position != INVALID_SET_FILE_POINTER;
			}
			if (local_c793c4)
			{
				if (mode == 1)
				{
					s_name_list list;
					name_list_clear(&list);
					function_136bf0(&file, 0x200, true);
					if (function_136ca0(&file, &list, sizeof(list), true) && list.count <= 32)
					{
						g_4e7408->seed = g_4e7408->seed * 0x19660d + 0x3c6ef35f;
						short index = (short)(((g_4e7408->seed >> 16) * (short)list.count) >> 16);
						function_08cc20(output, list.names[index].name);
						result = true;
					}
				}
				else
				{
					wchar_t message[256] = { 0 };
					if (function_136ca0(&file, message, sizeof(message), true))
					{
						function_08cc20(output, message);
						result = true;
					}
				}
			}
			function_136bb0(&file);
		}
	}
	return result;
}
