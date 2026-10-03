// @flags /O2 /arch:SSE /Gr
/* FONT_LOADING.CPP: the fonts: the font table (font_table[_<language>].txt)
   names up to 11 font files, which are copied from the DVD (d:\maps\fonts\)
   to the utility drive (z:\fonts\) when the language or the files change, and
   whose headers (with the kerning pairs) are read asynchronously into a cache
   of 10 entries. */

#include "cseries.h"
#include "async.h"
#include "font_loading.h"
#include "language.h"
#include "global_preferences.h"
#include <xtl.h>
#include <string.h>

#define k_maximum_font_count 10
#define k_font_header_version 0xf0000001

struct file_reference
{
	dword signature;
	word flags;
	short location;
	char path[256];
	byte unknown108[8];
};

struct s_font_cache_entry
{
	s_font_header header;
	s_file_handle file;
	bool volatile done;
	bool pending;
	long task;
};

char *csnprintf(char *buffer, long maximum_count, const char *format, ...);
char const *function_11cb00(long language);
void file_path_add_name(char *path, const char *name);
void file_path_remove_name(char *path);
bool function_1368f0(file_reference *file);
char *function_122810(char *string, const char *suffix);
bool function_120ce0(long job, long priority);
void global_preferences_flush(void);


char const *g_4687f0 = "z:\\fonts\\";
char const *g_4687f4 = "d:\\maps\\fonts\\";
long g_4687f8 = NONE;
char const *g_55e718;

long g_4e28f4[11];
s_font_cache_entry g_4e2920[k_maximum_font_count];
bool g_4e3b40;

static inline void csstrncpy(char *destination, char const *source, long size)
{
	strncpy(destination, source, size);
	destination[size - 1] = 0;
}

static inline void file_reference_create(file_reference *reference)
{
	memset(reference, 0, sizeof(*reference));
	reference->signature = 'filo';
	reference->location = NONE;
}

static inline void file_reference_set_name(file_reference *reference, char const *name)
{
	if (reference->flags & 1)
		file_path_remove_name(reference->path);
	file_path_add_name(reference->path, name);
	reference->flags |= 1;
}

// @retail 0x121570
char *font_table_get_name(char *buffer, long buffer_size)
{
	char name[256];
	file_reference reference;
	char path[256];
	char const *language;

	name[0] = 0;
	language = function_11cb00(get_current_language());
	if (g_4687f8 != 0)
	{
		csstrncpy(name, "font_table", sizeof(name));
		if (*language)
		{
			function_122810(name, "_");
			function_122810(name, language);
		}
		function_122810(name, ".txt");
		if (g_4687f8 == NONE)
		{
			path[0] = 0;
			csstrncpy(path, g_4687f4, sizeof(path));
			function_122810(path, name);
			file_reference_create(&reference);
			file_reference_set_name(&reference, path);
			if (function_1368f0(&reference))
				g_4687f8 = get_current_language();
			else
				g_4687f8 = 0;
		}
	}
	if (g_4687f8 == 0)
	{
		csstrncpy(name, "font_table", sizeof(name));
		function_122810(name, ".txt");
	}
	csstrncpy(buffer, name, buffer_size);
	return buffer;
}

// @retail 0x121730
void fonts_get_source_directory(file_reference *reference)
{
	char directory[256];

	g_55e718 = "d:\\maps\\";
	csnprintf(directory, sizeof(directory), "%sfonts\\", "d:\\maps\\");
	file_reference_create(reference);
	file_path_add_name(reference->path, directory);
}

// @retail 0x1222d0
long __stdcall font_load_callback(s_async_task *task)
{
	s_font_cache_entry *entry = &g_4e2920[task->font_load.font_index];
	bool finished = false;

	if (entry->file.handle == INVALID_HANDLE_VALUE)
	{
		char path[256];

		csnprintf(path, sizeof(path), "%s%s", g_4687f0, task->font_load.name);
		entry->file.handle = CreateFileA(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
		if (entry->file.handle == INVALID_HANDLE_VALUE)
		{
			entry->pending = finished;
			finished = true;
		}
	}
	else
	{
		DWORD bytes_read;

		SetFilePointer(entry->file.handle, 0x200, NULL, FILE_BEGIN);
		ReadFile(entry->file.handle, &entry->header, sizeof(entry->header), &bytes_read, NULL);
		finished = true;
	}
	return finished ? 1 : 0;
}

// @retail 0x1223a0
void font_load(long font_index, char const *name, bool wait)
{
	s_font_cache_entry *entry = &g_4e2920[font_index];
	s_async_task task;

	entry->pending = true;
	memset(&task, 0, sizeof(task));
	csstrncpy(task.font_load.name, name, sizeof(task.font_load.name));
	task.font_load.font_index = font_index;
	entry->file.handle = INVALID_HANDLE_VALUE;
	entry->task = async_task_add(wait ? 6 : 2, &task, 7, font_load_callback, &entry->done);
	if (wait)
	{
		async_yield_until_done(&entry->done, false);
		if (entry->header.version != k_font_header_version && global_preferences_globals.current.unknown1c != NONE)
		{
			global_preferences_globals.current.unknown1c = NONE;
			global_preferences_globals.dirty = true;
			global_preferences_flush();
		}
	}
}

// @retail 0x1224c0
s_font_header *font_get(long font_index)
{
	s_font_cache_entry *entry = &g_4e2920[font_index];
	s_font_header *result = NULL;

	if (font_index >= 0 && font_index < k_maximum_font_count && (entry->done || entry->pending))
	{
		if (!entry->done)
		{
			function_120ce0(entry->task, 6);
			async_yield_until_done(&entry->done, false);
		}
		result = &entry->header;
	}
	return result;
}

// @retail 0x122540
long font_get_line_height(long font)
{
	s_font_header *header = font_get(g_4e28f4[font]);
	long result = 10;

	if (header)
		result = header->leading_height + header->descending_height + header->ascending_height;
	return result;
}

// @retail 0x122570
short font_get_kerning_pair_offset(s_font_header const *header, dword first_character, dword second_character)
{
	short result = 0;

	if (header && first_character && second_character && first_character <= 0xff && second_character <= 0xff &&
		(header->kerning_characters[first_character >> 5] & (1 << (first_character & 31))))
	{
		long index = 0;

		do
		{
			if (header->kerning_pairs[index].first_character >= first_character)
				break;
			index++;
		}
		while (index < header->kerning_pair_count);
		if (index < header->kerning_pair_count && header->kerning_pairs[index].first_character == first_character)
		{
			do
			{
				if (header->kerning_pairs[index].second_character >= second_character)
					break;
				index++;
			}
			while (index < header->kerning_pair_count && header->kerning_pairs[index].first_character == first_character);
			if (index < header->kerning_pair_count && header->kerning_pairs[index].first_character == first_character &&
				header->kerning_pairs[index].second_character == second_character)
			{
				result = header->kerning_pairs[index].offset;
			}
		}
	}
	return result;
}
