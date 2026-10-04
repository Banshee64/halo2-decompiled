// @flags /O1 /Gr
/* UNKNOWN_23690B.CPP: the dialog callbacks that leave the game for the
   dashboard (0x23690b..0x23699f) */

#include "cseries.h"

/* marketing_and_strategic_business_development.cpp: never returns */
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
