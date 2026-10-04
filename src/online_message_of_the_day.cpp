// @flags /O2 /Gr
/* ONLINE_MESSAGE_OF_THE_DAY.CPP: the message of the day, kept in
   n:\message_of_the_day.dat (lane D) */

#include "cseries.h"
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

static inline void file_reference_create(file_reference *reference)
{
	memset(reference, 0, sizeof(*reference));
	reference->signature = FILE_REFERENCE_SIGNATURE;
	reference->location = NONE;
}

static inline void file_reference_set_name(file_reference *reference, char const *name)
{
	if (reference->flags & 1)
		file_path_remove_name(reference->path);
	file_path_add_name(reference->path, name);
	reference->flags |= 1;
}

// @retail 0x8ca00
void message_of_the_day_get_file(file_reference *file)
{
	file_reference_create(file);
	file_path_add_name(file->path, "n:\\");
	file_reference_set_name(file, g_4672b8);
}

// @retail 0x8ca50
bool message_of_the_day_available(void)
{
	if (g_479784.length != 0 && (g_479784.flags & 2))
		return true;
	return false;
}
