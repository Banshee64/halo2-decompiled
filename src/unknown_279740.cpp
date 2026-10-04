// @flags /O2 /Gr
#include "cseries.h"
#include "cache_files.h"


#define PIN(value, lower, upper) ((value) < (lower) ? (lower) : (value) > (upper) ? (upper) : (value))

// @retail 0x279740
bool __cdecl function_279740(long value)
{
	return PIN(value, (long)0x80061000, (long)0x80061000 + (cache_file_globals.loaded ? cache_file_globals.header.unknown1c : 0)) == value;
}
