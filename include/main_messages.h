/* MAIN_MESSAGES.H: the messages the main loop and the caches show the local
   players (loading, saving, switching structure bsps), strings of the hud
   globals' string list. Retail inlines both helpers everywhere. */
#ifndef MAIN_MESSAGES_H
#define MAIN_MESSAGES_H

#include "cseries.h"
#include "globals.h"

void unicode_string_list_get_string(long tag_index, long string_id, word *buffer);
void function_24cbee(long player_index, word const *text);
void __fastcall scripted_hud_messages_clear(void);

/* the first local player in use, or NONE */
static __forceinline long local_player_first_index(void)
{
	long result = NONE;

	for (long i = 0; i < 4; i++)
	{
		if (g_4e8c20->entries[i] != NONE)
		{
			result = i;
			break;
		}
	}
	return result;
}

/* shows a local player a message of the hud globals' string list */
static __forceinline void main_print_message(long local_player_index, long string_id)
{
	word text[0x100];

	text[0] = 0;
	if (g_510c94 && g_510c94->string_list != NONE)
		unicode_string_list_get_string(g_510c94->string_list, string_id, text);
	function_24cbee(local_player_index, text);
}

#endif
