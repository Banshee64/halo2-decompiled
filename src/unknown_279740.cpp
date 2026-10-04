#include "unknown_11c920.h"
#include "unknown_122870.h"

// @flags /O2 /Gr


#define PIN(value, lower, upper) ((value) < (lower) ? (lower) : (value) > (upper) ? (upper) : (value))

// @retail 0x279740
bool function_279740(long value)
{
	return value == PIN(value, (long)0x80061000, (long)0x80061000 + (cache_file_globals.loaded ? cache_file_globals.header.unknown1c : 0));
}
