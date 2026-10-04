/* MAIN_MESSAGES.H: the messages the main loop and the caches show the local
   players (loading, saving, switching structure bsps), strings of the hud
   globals' string list. Retail inlines both helpers everywhere. */
#ifndef MAIN_MESSAGES_H
#define MAIN_MESSAGES_H

#include "cseries.h"
#include "globals.h"

void function_1a0180(long tag_index, long string_handle, word *buffer);
void function_24cbee(long player_index, word const *text);
void __fastcall function_24cdaf(void);

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
static __forceinline void main_print_message(long local_player_index, long string_handle)
{
	word text[0x100];

	text[0] = 0;
	if (g_510c94 && g_510c94->string_list != NONE)
		function_1a0180(g_510c94->string_list, string_handle, text);
	function_24cbee(local_player_index, text);
}

#endif
