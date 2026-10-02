#include "cseries.h"

// @flags /O2 /Gr

byte g_5476f8;
long g_54771c;

#define PIN(value, lower, upper) ((value) < (lower) ? (lower) : (value) > (upper) ? (upper) : (value))

// @retail 0x279740
bool function_279740(long value)
{
	return value == PIN(value, (long)0x80061000, (long)0x80061000 + (g_5476f8 ? g_54771c : 0));
}
