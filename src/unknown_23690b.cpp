// @flags /O1 /Gr
/* UNKNOWN_23690B.CPP: the dialog callbacks of 0x236877..0x23699f: the
   four way sign in, and leaving the game for the dashboard */

#include "unknown_11c920.h"
#include "screen_widgets.h"
#include "unknown_19b516.h"

/* unknown_22376b.cpp: never returns */
void function_2238f4(long page, dword context, dword parameter1, dword parameter2);
long saved_game_file_type_size_in_blocks(long type);
long minimal_storage_size_in_blocks();
void __stdcall function_18f1c0(long a);

// @retail 0x23690b
bool __stdcall function_23690b(long controller)
{
	function_18f1c0(0);
	return true;
}

/* the dashboard's online menu */
// @retail 0x236937
bool __stdcall function_236937(long controller)
{
	function_2238f4(5, 0, 0, 0);
	return true;
}

/* the dashboard's memory page */
// @retail 0x236964
bool __stdcall function_236964(long controller)
{
	function_2238f4(1, 0, 0, 0);
	return true;
}

/* the dashboard's memory page, asking for room for a saved game */
// @retail 0x236973
bool __stdcall function_236973(long controller)
{
	function_2238f4(1, 0, 0x75, saved_game_file_type_size_in_blocks(0));
	return true;
}

/* the dashboard's memory page, asking for room for a saved profile */
// @retail 0x236989
bool __stdcall function_236989(long controller)
{
	function_2238f4(1, 0, 0x75, saved_game_file_type_size_in_blocks(1));
	return true;
}

/* the dashboard's memory page, asking for the room the game needs */
// @retail 0x23699f
bool __stdcall function_23699f(long controller)
{
	function_2238f4(1, 0, 0x75, minimal_storage_size_in_blocks());
	return true;
}

void function_6cb60(void);
short player_slot_count_active(void);
word function_1901fc(void);
void __stdcall function_1483c3(long reason);
c_class_1473c9 *__stdcall function_25240c(s_screen_parameters *parameters);
c_class_1473c9 *__stdcall function_252433(s_screen_parameters *parameters);

/* the four way sign in, or the main screen when no one is signed in */
// @retail 0x236877
bool __stdcall function_236877(long controller)
{
	function_6cb60();
	if (player_slot_count_active() > 0)
	{
		s_screen_parameters parameters;

		parameters.field_c = 0;
		function_149f49((s_message *)&parameters, 0, 0, function_1901fc(), 5, 4, (long)function_25240c);
		parameters.load(&parameters);
	}
	else
	{
		function_1483c3(0);
	}
	return true;
}

// @retail 0x2368c1
bool __stdcall function_2368c1(long controller)
{
	function_6cb60();
	if (player_slot_count_active() > 0)
	{
		s_screen_parameters parameters;

		parameters.field_c = 0;
		function_149f49((s_message *)&parameters, 0, 0, function_1901fc(), 5, 4, (long)function_252433);
		parameters.load(&parameters);
	}
	else
	{
		function_1483c3(0);
	}
	return true;
}
